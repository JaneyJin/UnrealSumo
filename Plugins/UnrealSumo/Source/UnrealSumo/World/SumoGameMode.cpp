// Fill out your copyright notice in the Description page of Project Settings.


#include "SumoGameMode.h"
#include "Misc/App.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "UnrealSumo/Vehicle/SumoWheeledVehicle.h"
#include "UnrealSumo/Vehicle/WheeledVehiclePawn.h"
#include "GameFramework/PlayerStart.h"
#include <algorithm>    // std::find
#include <vector>
// #include "SumoPlayerController.h"
#include "SumoGameInstance.h"


//ASumoGameMode::ASumoGameMode(const FObjectInitializer& ObjectInitializer)
//        : Super(ObjectInitializer)
//{
//
//    // set default player class to customed MyPlayerController
//    // PlayerControllerClass = ASumoPlayerController::StaticClass();
//
//}

void ASumoGameMode::BeginPlay() {
    Super::BeginPlay();

    // Setup socket for UE and connect to SUMO
    try {

        // Validate the port number
        if ((PortNumber <= 0) || (PortNumber > LargestPortNumber)) {
            PortNumber = DefaultPortNumber;
            UE_LOG(LogTemp, Warning, TEXT("Missing PortNum. Set to 24000....."))
        }
        UE_LOG(LogTemp, Warning, TEXT("Connect....."))

        client.connect("localhost", PortNumber);

        if (!SetupEgoWheeledVehicle()) {
            // UE_LOG(LogTemp, Error, TEXT("Fail to set up EGOVehcile"))
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("EGO Wheeled Vehicle is not set.")));
        }

        if (!SetupGameInstance()) {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection fail. Please set SumoGameInstance in the project setting before use.")));
        }
        // Validate FPS between Unreal and SUMO.
        MatchFrameRatePerSecond();

    }
    catch (tcpip::SocketException& e) {
        TrafficLightSyn = ETrafficLightController::UnrealTrafficLight;
        UE_LOG(LogTemp, Error, TEXT("#Error while connecting: %s.\n"), e.what())
    }

    UE_LOG(LogTemp, Warning, TEXT("GameMode BeginPlay"))

}

void ASumoGameMode::MatchFrameRatePerSecond() {
    /* Seconds between 2 frame in SUMO */
    double SUMODeltaT = client.simulation.getDeltaT();

    /* Seconds between 2 frame in Unreal */
    double UEDeltaT = FApp::GetDeltaTime();

    /* Get FPS and set TickCounter */
    SumoGameInstance->SUMOToUnrealFrameRate.SUMOFPS = FMath::RoundHalfFromZero(1.0f / SUMODeltaT);
    SumoGameInstance->SUMOToUnrealFrameRate.UnrealFPS = FMath::RoundHalfFromZero(1.0f / UEDeltaT);

    UE_LOG(LogTemp, Warning, TEXT("SUMO DeltaT is %f. UE DeltaT is %f. Sumo fps is %d. Unreal FPS is %d"), SUMODeltaT, UEDeltaT, SumoGameInstance->SUMOToUnrealFrameRate.SUMOFPS, SumoGameInstance->SUMOToUnrealFrameRate.UnrealFPS)

    if (SUMODeltaT > 0) {

        if (SUMODeltaT < UEDeltaT) {
            SumoGameInstance->SUMOToUnrealFrameRate.UnrealTickSlower = true;
            UE_LOG(LogTemp, Error, TEXT("Unreal Engine frame rate is lower than SUMO. Cannot change it within code. Please change in setting."));
        }
        else {
            SumoGameInstance->SUMOToUnrealFrameRate.UnrealTickSlower = false;
            SumoGameInstance->SUMOToUnrealFrameRate.NextTimeToUpdate = SumoGameInstance->SUMOToUnrealFrameRate.UpdateDeltaT = SUMODeltaT;
            SumoGameInstance->SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates = FMath::RoundHalfFromZero(1 / SUMODeltaT);
        }

    }

    // UE_LOG(LogTemp, Warning, TEXT("Update at %f per second. Next update at %f"), SUMOToUnrealFrameRate.UpdateDeltaT, SUMOToUnrealFrameRate.NextTimeToUpdate)
    UE_LOG(LogTemp, Warning, TEXT("Current tick is %d. Tick between SUMO updates is %d"), SumoGameInstance->SUMOToUnrealFrameRate.TickCount, SumoGameInstance->SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates)
}



void ASumoGameMode::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    // auto DateTime = FDateTime::Now().GetMillisecond();
    // UE_LOG(LogTemp, Warning, TEXT("Machine Time: %s"), *DateTime.ToString())

    // Unreal Engine frame rate must be higher than SUMO frame rate.
    if (SumoGameInstance && SumoGameInstance->client && !SumoGameInstance->SUMOToUnrealFrameRate.UnrealTickSlower) {
        UpdateFromSUMOByTickCount();
        // UpdateSUMOByMachineTime();

    }
//    else {
//        UE_LOG(LogTemp, Warning, TEXT("Tick. Socket Close."))
//    }
}

bool ASumoGameMode::SetupEgoWheeledVehicle() {

    if (DefaultPawnClass) {
        auto CurrentDefaultPawnSuperClassName = DefaultPawnClass->GetSuperClass()->GetName();
        auto CurrentDefaultPawnOwnerName = DefaultPawnClass->GetOwnerClass()->GetName();

        /* Ego wheeled vehicle must be inherited from WheeledVehiclePawn class so that it can update vehicle information to SUMO server in real time. */
        if (CurrentDefaultPawnOwnerName != "DefaultPawn" && CurrentDefaultPawnSuperClassName == "WheeledVehiclePawn") {
            EgoWheeledVehicle = (AWheeledVehiclePawn*)DefaultPawnClass->GetClass();

            if (EgoWheeledVehicle) {

                EgoWheeledVehicleId = DefaultPawnClass->GetName();
                auto RouteIDList = client.route.getIDList();
                // EgoWheeledVehicleInformation.VehicleId = CurrentDefaultPawnOwnerName;

                /* Validate the route for ego wheeled vehicle to start the trip. */
                std::string StartRouteId = RouteId.IsEmpty() || std::find(RouteIDList.begin(), RouteIDList.end(), TCHAR_TO_UTF8(*RouteId)) == RouteIDList.end() ? RouteIDList[0] : TCHAR_TO_UTF8(*RouteId);

                client.vehicle.add(TCHAR_TO_UTF8(*EgoWheeledVehicleId), StartRouteId);
                auto EgoWheeledVehicleStartPosition = client.vehicle.getPosition(TCHAR_TO_UTF8(*EgoWheeledVehicleId));

                // EgoWheeledVehicle->SetupEgoWheeledVehicle(EgoWheeledVehicleId, &client, SUMOToUnrealFrameRate);
                return true;
            }

        }
    }
    return false;
}

bool ASumoGameMode::SetupGameInstance() {
    SumoGameInstance = NewObject<USumoGameInstance>(this, TEXT("MyGameInstance"));
    GetWorld()->SetGameInstance(SumoGameInstance);

    UE_LOG(LogTemp, Warning, TEXT("GameInstance class name: %s; GameInstance name: %s"), *GetGameInstance()->GetClass()->GetName(), *GetGameInstance()->GetName())
    if (GetGameInstance()->GetClass()->GetName() == "SumoGameInstance") {

        // FString EnumAsString = UEnum::GetValueAsString(TrafficLightSyn.GetValue());
        // UE_LOG(LogTemp, Error, TEXT("%s: %d"), *EnumAsString, TrafficLightSyn.GetValue())
        SumoGameInstance->SetSumoGameInstance(&client, DefaultPawnClass->GetName());
        return true;
    }

    return false;
}

bool ASumoGameMode::SynBySUMOTrafficLight(){
    if(TrafficLightSyn == ETrafficLightController::SumoTrafficLight) {
        return true;
    }
    return false;
}

void ASumoGameMode::UpdateFromSUMOByTickCount() {

    /* Fix Tick() rate to update vehicle from SUMO */
    if (SumoGameInstance->SUMOToUnrealFrameRate.TickCount < SumoGameInstance->SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates) {
        // UE_LOG(LogTemp, Display, TEXT("SUMOGameMode -> #of ticks between SUMO updates is %d. GameMode Tick() %d. Update from SUMo."), SumoGameInstance->SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates, SumoGameInstance->SUMOToUnrealFrameRate.TickCount)

        SumoGameInstance->SUMOToUnrealFrameRate.TickCount++;

    }
    else if (SumoGameInstance->SUMOToUnrealFrameRate.TickCount == SumoGameInstance->SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates) {
        // UE_LOG(LogTemp, Warning, TEXT("SUMOGameMode -> Tick %d : Update from SUMO."), SumoGameInstance->SUMOToUnrealFrameRate.TickCount)
        UpdateSimulationFromSUMO();

        SumoGameInstance->SUMOToUnrealFrameRate.TickCount = 1;
    }
}


void ASumoGameMode::UpdateSimulationFromSUMO() {
    if (client.simulation.getMinExpectedNumber() > 0) {
        client.simulationStep();

        SumoGameInstance->ArrivedNumber = client.simulation.getArrivedNumber();
        if (SumoGameInstance->ArrivedNumber != 0) {
            SumoGameInstance->ArrivedList = client.simulation.getArrivedIDList();
        }

        DepartedNumber = client.simulation.getDepartedNumber();
        if (DepartedNumber != 0) {

            DepartedList = client.simulation.getDepartedIDList();

            for (int i = 0; i < DepartedNumber; i++) {

                /* Retrieve vehicle id, speed, positon and color from SUMO */

                DepartedVehicleId = DepartedList[i];
                if (DepartedVehicleId == TCHAR_TO_UTF8(*EgoWheeledVehicleId)) {
                    continue;
                }

                /* Covert std::string into FString */
                SUMOVehicleInformation.VehicleId = DepartedVehicleId.c_str();


                // Assign speed
                SUMOVehicleInformation.VehicleSpeed = client.vehicle.getSpeed(DepartedVehicleId);

                /* Convert libsumo::TraCIPosition into FVector */
                DepartedVehiclePos = client.vehicle.getPosition(DepartedVehicleId);
                SUMOVehicleInformation.VehiclePosition.X = DepartedVehiclePos.x * MeterUnitConversion;
                SUMOVehicleInformation.VehiclePosition.Y = DepartedVehiclePos.y * MeterUnitConversion * -1;
                /* Avoid sumo wheeled vehicle collide with floor. */
                SUMOVehicleInformation.VehiclePosition.Z = DepartedVehiclePos.z * MeterUnitConversion + 10;


                SUMOVehicleInformation.VehicleAngle.Yaw = client.vehicle.getAngle(DepartedVehicleId);
                // Align with SUMO initial forward direction in Unreal
                SUMOVehicleInformation.VehicleAngle.Yaw = SUMOVehicleInformation.VehicleAngle.Yaw - 90;

                if (!SpawnRandomWheeledVehicle(SUMOVehicleInformation)) {
                    UE_LOG(LogTemp, Error, TEXT("Fail to spawn vehicle %s"), *SUMOVehicleInformation.VehicleId)

                    if (GEngine)
                    {
                        const int32 AlwaysAddKey = -1; // Add a new one instead of overwrite last message
                        static const FString ErrorMessage(TEXT("Fail to spawn vehicle "));
                        GEngine->AddOnScreenDebugMessage(AlwaysAddKey, 1.0f, FColor::Red, ErrorMessage + SUMOVehicleInformation.VehicleId + "!");
                    }

                }

            }

        }

    }
    else {
        // Close socket
        client.close();
        SumoGameInstance->CloseSocket();

        // Exit game in UE
        //FGenericPlatformMisc::RequestExit(false);
        //GetWorld()->Exec(GetWorld(), TEXT("Exit"));
    }
}


bool ASumoGameMode::SpawnRandomWheeledVehicle(FVehicleInformation& DepartedVehicle) {
    // Spawn a vehicle at its start location from SUMO
    UWorld* world = GetWorld();
    if (world) {
        SpawnPoint = SUMOVehicleInformation.VehiclePosition;
        SpawnRotator.Yaw = DepartedVehicle.VehicleAngle.Yaw;
        UE_LOG(LogTemp, Display, TEXT("Spawn location: %s ; SpawnVehicle rotator: %s"), *SpawnPoint.ToString(), *SpawnRotator.ToString())
        if (WheeledVehicleBPList.Num() > 0) {
            UE_LOG(LogTemp, Display, TEXT("WheeledVehicle number %f"), WheeledVehicleBPList.Num())

            selectedClass = *WheeledVehicleBPList[FMath::RandRange(0, WheeledVehicleBPList.Num() - 1)];
            ASumoWheeledVehicle* RandomWheeledVehicle = Cast<ASumoWheeledVehicle>(world->SpawnActor(selectedClass, &SpawnPoint, &SpawnRotator));

            if (RandomWheeledVehicle) {
                if (RandomWheeledVehicle->InitializeWheeledVehicle(SUMOVehicleInformation)) {
                    UE_LOG(LogTemp, Warning, TEXT("Spawn wheeled vehicle %s."), *RandomWheeledVehicle->GetName())
                    return true;
                }

            }
            UE_LOG(LogTemp, Warning, TEXT("Fail to spawn wheeled vehicle."))
            return false;

        }
        else {
            UE_LOG(LogTemp, Error, TEXT("Fail to spawn wheeled vehicle because none blueprint class is selected"))
            if (GEngine)
            {
                const int32 AlwaysAddKey = -1; // Add a new one instead of overwrite last message
                static const FString ErrorMessage(TEXT("Fail to spawn wheeld vehicle because none blueprint class is selected"));
                GEngine->AddOnScreenDebugMessage(AlwaysAddKey, 5.0f, FColor::Red, ErrorMessage);

            }
        }
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Can't get world."))
    }

    return false;
}


#if WITH_EDITOR
bool ASumoGameMode::CanEditChange(const UProperty* InProperty) const
{
    const bool ParentVal = Super::CanEditChange(InProperty);

    if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ASumoGameMode, RouteId))
    {
        auto CurrentDefaultPawnSuperClassName = DefaultPawnClass->GetSuperClass()->GetName();
        return  CurrentDefaultPawnSuperClassName == "WheeledVehiclePawn";
    }

    return ParentVal;
}
#endif


