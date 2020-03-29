#include "SumoGameMode.h"// Must at the line for #include
#include "Misc/App.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "SumoWheeledVehicle.h"
#include "WheeledVehiclePawn.h"
#include "GameFramework/PlayerStart.h"
#include <algorithm>    // std::find
#include <vector> 

ASumoGameMode::ASumoGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	// set default player class to customed MyPlayerController
	// PlayerControllerClass = ASumoPlayerController::StaticClass();

}

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
		// Validate all flags and FPS 
		SocketIsNotClosed = true;
		MatchFrameRatePerSecond();

		if (!SetupEgoVehicle()) {
			UE_LOG(LogTemp, Error, TEXT("Fail to set up EGOVehcile"))
		}
		// client.vehicle.moveToXY(TCHAR_TO_UTF8(*EgoWheeledVehicleInformation.VehicleId), "gneE7", 0,17.5, 83.13, 0, 0);

	}
	catch (tcpip::SocketException& e) {
		UE_LOG(LogTemp, Error, TEXT("#Error while connecting: %s.\n"), e.what())
	}

}

// Called every frame
void ASumoGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/// Test Machine time vs Unreal Tick time
	/* auto DateTime = FDateTime::Now().GetMillisecond();
	 auto DateTime = FDateTime::Now();
	 UE_LOG(LogTemp, Warning, TEXT("Machine Time: %s"), *DateTime.ToString())*/
	if (SocketIsNotClosed) {
		TimeInWorld = GetWorld()->GetTimeSeconds();

		// Not allow UE Tick() slower than SUMo FPS.  
		if (client.SetUpdateDeltaTFlag) {
			UpdateSUMOByTickCount();
			//UpdateSUMOByMachineTime();
		}

		
	}
	/*else {
		UE_LOG(LogTemp, Warning, TEXT("Tick. Socket Close."))
	}*/

}

void ASumoGameMode::MatchFrameRatePerSecond() {
	SUMODeltaT = client.simulation.getDeltaT();
	auto UEDeltaT = FApp::GetDeltaTime();

	// GetFPS and set TickCounter
	SUMOToUnrealFrameRate.UnrealFPS = FMath::RoundHalfFromZero(1.0f / UEDeltaT);
	SUMOToUnrealFrameRate.SUMOFPS = 1 / SUMODeltaT;
	UE_LOG(LogTemp, Warning, TEXT("SUMO DeltaT is %f. UE DeltaT is %f.FPS is %d"), SUMODeltaT, UEDeltaT, SUMOToUnrealFrameRate.UnrealFPS)

		if (SUMODeltaT > 0) {

			if (SUMODeltaT < UEDeltaT) {
				UE_LOG(LogTemp, Error, TEXT("Unreal Engine frame rate is lower than SUMO. Cannot change it within code. Please change in setting."));
			}
			else {
				SUMOToUnrealFrameRate.NextTimeToUpdate = SUMOToUnrealFrameRate.UpdateDeltaT = SUMODeltaT;
				SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates = SUMOToUnrealFrameRate.UnrealFPS / SUMOToUnrealFrameRate.SUMOFPS;
				client.SetUpdateDeltaTFlag = true;
			}

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("SUMO DeltaT is negative or zero. Fail to set frame rate match."))
		}

	//UE_LOG(LogTemp, Warning, TEXT("Update at %f per second. Next update at %f"), SUMOToUnrealFrameRate.UpdateDeltaT, SUMOToUnrealFrameRate.NextTimeToUpdate,)
	UE_LOG(LogTemp, Warning, TEXT("Current tick is %d. Tick between SUMO updates is %d"), SUMOToUnrealFrameRate.TickCount, SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates)
}

bool ASumoGameMode::SetupEgoVehicle() {
	auto CurrentDefaultPawnSuperClassName = DefaultPawnClass->GetSuperClass()->GetName();
	auto CurrentDefaultPawnOwnerName = DefaultPawnClass->GetOwnerClass()->GetName();
	if (CurrentDefaultPawnOwnerName != "DefaultPawn" && CurrentDefaultPawnSuperClassName == "WheeledVehiclePawn") {
		EgoWheeledVehicle = (AWheeledVehiclePawn*)DefaultPawnClass->GetClass();
	
		if (EgoWheeledVehicle) {
			EgoWheeledVehicle->SetWheeledVehicleID(DefaultPawnClass->GetName());

			EgoWheeledVehicleInformation = EgoWheeledVehicle->GetEgoWheeledVehicleInformation();
			EgoWheeledVehicleInformation.print();

			auto RouteIDList = client.route.getIDList();
			
			std::string StartRouteId = RouteId.IsEmpty() || std::find(RouteIDList.begin(), RouteIDList.end(), TCHAR_TO_UTF8(*RouteId)) == RouteIDList.end() ? RouteIDList[0] : TCHAR_TO_UTF8(*RouteId);
			client.vehicle.add(TCHAR_TO_UTF8(*EgoWheeledVehicleInformation.VehicleId), StartRouteId);
			auto EgoWheeledVehicleStartPosition = client.vehicle.getPosition(TCHAR_TO_UTF8(*EgoWheeledVehicleInformation.VehicleId));
			EgoWheeledVehicleInformation.VehiclePosition.X = EgoWheeledVehicleStartPosition.x;
			EgoWheeledVehicleInformation.VehiclePosition.Y = EgoWheeledVehicleStartPosition.y;
			EgoWheeledVehicleInformation.VehiclePosition.Z = EgoWheeledVehicleStartPosition.z;
			return true;
		}

	}

	return false;
}


void ASumoGameMode::UpdateSUMOByTickCount() {
	
	// Fix Tick() rate to update vehicle from SUMO
	if (SUMOToUnrealFrameRate.TickCount < SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates) {
		// UE_LOG(LogTemp, Display, TEXT("SUMOGameMode -> #of ticks between SUMO updates is %d. GameMode Tick() %d. Update from SUMo."), SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates, SUMOToUnrealFrameRate.TickCount)
		SUMOToUnrealFrameRate.TickCount++;
	}
	else if (SUMOToUnrealFrameRate.TickCount == SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates) {
		
		// UE_LOG(LogTemp, Warning, TEXT("SUMOGameMode -> Tick %d : Update from SUMO."), SUMOToUnrealFrameRate.TickCount)
		UpdateVehicleFromSUMO();
		UpdateEgoWheeledVehicleToSUMO();
		SUMOToUnrealFrameRate.TickCount = 1;

	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Tick calculation is wrong."))
	}

}

void ASumoGameMode::UpdateSUMOByMachineTime() {
	TimeInWorld = GetWorld()->GetTimeSeconds();

	if (SUMOToUnrealFrameRate.NextTimeToUpdate - TimeInWorld < 0.0001) {
		SUMOToUnrealFrameRate.NextTimeToUpdate += SUMOToUnrealFrameRate.UpdateDeltaT;
		UpdateVehicleFromSUMO();
		// UE_LOG(LogTemp, Warning, TEXT("SUMOGameMode ->  SUMOSTEP: %d ; Update from SUMO: %f . NextTimeToUpdate %f"), SUMOStep, TimeInWorld, SUMOToUnrealFrameRate.NextTimeToUpdate)
	}
	/*else {
		UE_LOG(LogTemp, Display, TEXT("SUMOGameMode -> %f GameMode Tick()"), TimeInWorld)
	}*/
}

void ASumoGameMode::UpdateVehicleFromSUMO() {
	if (client.simulation.getMinExpectedNumber() > 0) {
		client.simulationStep();

		// SUMO time increment 
		SUMOStep++;
		SUMOTime += SUMODeltaT;
		//UE_LOG(LogTemp, Warning, TEXT("MachineTime;;SUMOTime;%f;SUMOStep;%d"),  SUMOTime, SUMOStep)

		DepartedNumber = client.simulation.getDepartedNumber();
		if (DepartedNumber != 0) {

			DepartedList = client.simulation.getDepartedIDList();

			// for (unsigned int i = 0; i < departedList.size(); i++) {
			for (int i = 0; i < DepartedNumber; i++) {

				/// Retrieve vehicle id, speed, positon and color from SUMO 
				// Covert std::string into FString
				DepartedVehicleId = DepartedList[i];
				SUMOVehicleInformation.VehicleId = DepartedVehicleId.c_str();

				// Assign speed
				SUMOVehicleInformation.VehicleSpeed = client.vehicle.getSpeed(DepartedVehicleId);

				// Convert libsumo::TraCIPosition into FVector 
				DepartedVehiclePos = client.vehicle.getPosition(DepartedVehicleId);
				SUMOVehicleInformation.VehiclePosition.X = DepartedVehiclePos.x * MeterUnitConversion;
				SUMOVehicleInformation.VehiclePosition.Y = DepartedVehiclePos.y * MeterUnitConversion * -1;
				SUMOVehicleInformation.VehiclePosition.Z = DepartedVehiclePos.z * MeterUnitConversion + 30;

				// Convert libsumo:: TraCIColor into FColor
				/*DepartedVehicleColor = client.vehicle.getColor(DepartedVehicleId);
				SUMOVehicleInformation.VehicleColor.R = DepartedVehicleColor.r;
				SUMOVehicleInformation.VehicleColor.G = DepartedVehicleColor.g;
				SUMOVehicleInformation.VehicleColor.B = DepartedVehicleColor.b;
				SUMOVehicleInformation.VehicleColor.A = DepartedVehicleColor.a;*/

				SUMOVehicleInformation.VehicleAngle.Yaw = client.vehicle.getAngle(DepartedVehicleId);
				// Align with SUMO initial direction
				SUMOVehicleInformation.VehicleAngle.Yaw = SUMOVehicleInformation.VehicleAngle.Yaw - 90;

				// if (!SpawnRandomVehicle(SUMOVehicleInformation)) {
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
		SocketIsNotClosed = false;
		client.SetUpdateDeltaTFlag = false;
		// Exit game in UE
		//FGenericPlatformMisc::RequestExit(false);
		//GetWorld()->Exec(GetWorld(), TEXT("Exit"));
	}
}


void ASumoGameMode::UpdateEgoWheeledVehicleToSUMO() {
	if (EgoWheeledVehicle) {
		 EgoWheeledVehicleInformation = EgoWheeledVehicle->UpdateEgoVehicleToSUMO();
		 // EgoWheeledVehicleInformation.print();
	}
}

//bool ASumoGameMode::SpawnRandomVehicle(FVehicleInformation& DepartedVehicle) {
//
//
//	// Spawn a vehicle at its start location from SUMO
//	UWorld* world = GetWorld();
//	if (world) {
//		SpawnPoint = SUMOVehicleInformation.VehiclePosition;
//		SpawnRotator.Yaw = DepartedVehicle.VehicleAngle.Yaw;
//		UE_LOG(LogTemp, Display, TEXT("Spawn location: %s ; SpawnVehicle rotator: %s"), *SpawnPoint.ToString(), *SpawnRotator.ToString())
//			if (VehicleBPList.Num() > 0) {
//
//				selectedClass = *VehicleBPList[FMath::RandRange(0, VehicleBPList.Num() - 1)];
//				RandomVehicle = Cast<ACustomVehicle>(world->SpawnActor(selectedClass, &SpawnPoint, &SpawnRotator));
//				if (RandomVehicle) {
//					if (RandomVehicle->InitializeVehicle(SUMOVehicleInformation, &client, SUMOToUnrealFrameRate)) {
//						// UE_LOG(LogTemp, Warning, TEXT("SpawnVehicle %s."), *RandomVehicle->GetName())
//						return true;
//					}
//				}
//				return false;
//
//			}
//			else {
//				UE_LOG(LogTemp, Error, TEXT("Fail to spawn vehicle because none blueprint class is selected"))
//					if (GEngine)
//					{
//						const int32 AlwaysAddKey = -1; // Add a new one instead of overwrite last message
//						static const FString ErrorMessage(TEXT("Fail to spawn vehicle because none blueprint class is selected"));
//						GEngine->AddOnScreenDebugMessage(AlwaysAddKey, 5.0f, FColor::Red, ErrorMessage);
//
//					}
//			}
//	}
//	else {
//		UE_LOG(LogTemp, Error, TEXT("Can't get world."))
//	}
//
//	return false;
//}


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
				RandomWheeledVehicle = Cast<ASumoWheeledVehicle>(world->SpawnActor(selectedClass, &SpawnPoint, &SpawnRotator));

				if (RandomWheeledVehicle) {

					if (RandomWheeledVehicle->InitializeWheeledVehicle(SUMOVehicleInformation, &client, SUMOToUnrealFrameRate)) {
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

	// Can we edit flower color?
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ASumoGameMode, RouteId))
	{
		auto CurrentDefaultPawnSuperClassName = DefaultPawnClass->GetSuperClass()->GetName();
		return  CurrentDefaultPawnSuperClassName == "WheeledVehiclePawn";
	}

	return ParentVal;
}
#endif