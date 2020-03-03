#include "SumoGameMode.h"// Must at the line for #include
#include "Misc/App.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "SumoWheeledVehicle.h"
#include "WheeledVehiclePawn.h"
#include "GameFramework/PlayerStart.h"

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

	}
	catch (tcpip::SocketException& e) {
		UE_LOG(LogTemp, Error, TEXT("#Error while connecting: %s"), e.what())
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

	UE_LOG(LogTemp, Warning, TEXT("SUMO DeltaT is %f. UE DeltaT is %f.FPS is %d"), SUMODeltaT, UEDeltaT, SUMOToUnrealFrameRate.UnrealFPS)

		if (SUMODeltaT > 0) {

			if (SUMODeltaT < UEDeltaT) {
				UE_LOG(LogTemp, Error, TEXT("Unreal Engine frame rate is lower than SUMO. Cannot change it within code. Please change in setting."));
			}
			else {
				SUMOToUnrealFrameRate.NextTimeToUpdate = SUMOToUnrealFrameRate.UpdateDeltaT = SUMODeltaT;
				SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates = 1 / SUMODeltaT;
				client.SetUpdateDeltaTFlag = true;
			}

			/*if (GEngine != nullptr) {
				Settings = GEngine->GameUserSettings;
			}
			if (Settings != nullptr)
			{

				UEDeltaT = Settings->GetFrameRateLimit();
				UE_LOG(LogTemp, Warning, TEXT("GetFrameRateLimit %f."), UEDeltaT)
				Settings->SetFrameRateLimit(60);

				UEDeltaT = FApp::GetDeltaTime();
				UE_LOG(LogTemp, Warning, TEXT("New UE DeltaT is %f."), UEDeltaT)
			}*/

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("SUMO DeltaT is negative or zero. Fail to set frame rate match."))
		}

	//UE_LOG(LogTemp, Warning, TEXT("Update at %f per second. Next update at %f"), SUMOToUnrealFrameRate.UpdateDeltaT, SUMOToUnrealFrameRate.NextTimeToUpdate,)
	UE_LOG(LogTemp, Warning, TEXT("Current tick is %d. Tick between SUMO updates is %d"), SUMOToUnrealFrameRate.TickCount, SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates)
}

void ASumoGameMode::UpdateSUMOByTickCount() {
	//if (SUMOToUnrealFrameRate.TickCount < SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates) {
	//	// UE_LOG(LogTemp, Display, TEXT("GameMode Tick() %d"), SUMOToUnrealFrameRate.TickCount)
	//	SUMOToUnrealFrameRate.TickCount++;
	//}
	//else 
	// Fix Tick() rate to update vehicle from SUMO
	if (SUMOToUnrealFrameRate.TickCount < SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates) {
		SUMOToUnrealFrameRate.TickCount++;
		UE_LOG(LogTemp, Display, TEXT("#of ticks between SUMO updates is %d. GameMode Tick() %d. Update from SUMo."), SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates, SUMOToUnrealFrameRate.TickCount)
	}
	else if (SUMOToUnrealFrameRate.TickCount == SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates) {
		SUMOToUnrealFrameRate.TickCount = 1;
		// UE_LOG(LogTemp, Warning, TEXT("%f :Update from SUMO. NextTimeToUpdate %f"), TimeInWorld, NextTimeToUpdate)
		UE_LOG(LogTemp, Display, TEXT("#of ticks between SUMO updates is %d. GameMode Tick() %d. Update from SUMo."), SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates, SUMOToUnrealFrameRate.TickCount)
			UpdateFromSUMO();
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Tick calculation is wrong."))
	}

}

void ASumoGameMode::UpdateSUMOByMachineTime() {
	TimeInWorld = GetWorld()->GetTimeSeconds();

	if (SUMOToUnrealFrameRate.NextTimeToUpdate - TimeInWorld < 0.0001) {
		SUMOToUnrealFrameRate.NextTimeToUpdate += SUMOToUnrealFrameRate.UpdateDeltaT;
		UpdateFromSUMO();
		// UE_LOG(LogTemp, Warning, TEXT("SumoDefaultPawn:: SUMOSTEP: %d ; Update from SUMO: %f . NextTimeToUpdate %f"), SUMOStep, TimeInWorld, SUMOToUnrealFrameRate.NextTimeToUpdate)
	}
	/*else {
		UE_LOG(LogTemp, Display, TEXT("%f GameMode Tick()"), TimeInWorld)
	}*/
}

void ASumoGameMode::UpdateFromSUMO() {
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
				DepartedVehicleColor = client.vehicle.getColor(DepartedVehicleId);
				SUMOVehicleInformation.VehicleColor.R = DepartedVehicleColor.r;
				SUMOVehicleInformation.VehicleColor.G = DepartedVehicleColor.g;
				SUMOVehicleInformation.VehicleColor.B = DepartedVehicleColor.b;
				SUMOVehicleInformation.VehicleColor.A = DepartedVehicleColor.a;

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

					/* selectedClass = *VehicleBPList[FMath::RandRange(0, WheeledVehicleBPList.Num() - 1)];
					RandomWheeledVehicle = Cast<ASumoWheeledVehicle>(world->SpawnActor(selectedClass, &SpawnPoint, &SpawnRotator));
					if (RandomWheeledVehicle) {
						if (RandomWheeledVehicle->InitializeWheeledVehicle(SUMOVehicleInformation, &client, SUMOToUnrealFrameRate)) {
							UE_LOG(LogTemp, Warning, TEXT("SpawnVehicle %s."), *RandomWheeledVehicle->GetName())
							return true;
						}
					}*/

					selectedClass = *WheeledVehicleBPList[FMath::RandRange(0, WheeledVehicleBPList.Num() - 1)];
				RandomWheeledVehicle = Cast<ASumoWheeledVehicle>(world->SpawnActor(selectedClass, &SpawnPoint, &SpawnRotator));

				if (RandomWheeledVehicle) {

					if (RandomWheeledVehicle->InitializeWheeledVehicle(SUMOVehicleInformation, &client, SUMOToUnrealFrameRate)) {
						// UE_LOG(LogTemp, Warning, TEXT("Spawn wheeled vehicle %s."), *RandomWheeledVehicle->GetName())
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
