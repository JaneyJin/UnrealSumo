// Fill out your copyright notice in the Description page of Project Settings.


#include "SumoDefaultPawn.h" // Must at the line for #include
#include "Misc/App.h"
#include "GameFramework/Actor.h"
#include "Client.h"
#include "Vehicle.h"

ASumoDefaultPawn::ASumoDefaultPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default player class to customed MyPlayerController
	// PlayerControllerClass = ASumoPlayerController::StaticClass();

}

// Called when the game starts or when spawned
void ASumoDefaultPawn::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("SumoDefaultPawn BeginPlay."))
	

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
void ASumoDefaultPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/// Test Machine time vs Unreal Tick time
	/* auto DateTime = FDateTime::Now().GetMillisecond();
	 auto DateTime = FDateTime::Now();
	 UE_LOG(LogTemp, Warning, TEXT("Machine Time: %s"), *DateTime.ToString())*/


	// Fix Tick() rate to update vehicle from SUMO
	if (TickCount < FPS) {
		TickCount += 1;
	}
	else {
		TickCount = 1;
	}

	if (SocketIsNotClosed) {
		TimeInWorld = GetWorld()->GetTimeSeconds();

		// Not allow UE Tick() slower than SUMo FPS.  
		if (SetUpdateDeltaTFlag) {
			// UpdateSUMOByTickCount();
			UpdateSUMOByMachineTime();
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Tick. Socket Close."))
	}

}

void ASumoDefaultPawn::MatchFrameRatePerSecond() {
	SUMODeltaT = client.simulation.getDeltaT();
	auto UEDeltaT = FApp::GetDeltaTime();

	// GetFPS and set TickCounter
	FPS = FMath::RoundHalfFromZero(1.0f / UEDeltaT);

	UE_LOG(LogTemp, Warning, TEXT("SUMO DeltaT is %f. UE DeltaT is %f.FPS is %d"), SUMODeltaT, UEDeltaT, FPS)

		if (SUMODeltaT > 0) {

			if (SUMODeltaT < UEDeltaT) {
				UE_LOG(LogTemp, Error, TEXT("Unreal Engine frame rate is lower than SUMO. Cannot change it within code. Please change in setting."));
			}
			else {
				NextTimeToUpdate = UpdateDeltaT = SUMODeltaT;
				SetUpdateDeltaTFlag = true;
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

	UE_LOG(LogTemp, Warning, TEXT("Update at %f per second. Next update at %f"), UpdateDeltaT, NextTimeToUpdate)
}

void ASumoDefaultPawn::UpdateSUMOByTickCount() {
	if (TickCount < FPS) {
		UE_LOG(LogTemp, Display, TEXT("GameMode Tick() %d"), TickCount)
	}
	else if (TickCount == FPS) {
		// UE_LOG(LogTemp, Warning, TEXT("%f :Update from SUMO. NextTimeToUpdate %f"), TimeInWorld, NextTimeToUpdate)
		UE_LOG(LogTemp, Display, TEXT("GameMode Tick() %d. Update from SUMo."), TickCount)
			UpdateFromSUMO();
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Tick calculation is wrong."))
	}

}

void ASumoDefaultPawn::UpdateSUMOByMachineTime() {
	TimeInWorld = GetWorld()->GetTimeSeconds();

	if (NextTimeToUpdate - TimeInWorld < 0.0001) {
		NextTimeToUpdate += UpdateDeltaT;
		UpdateFromSUMO();
		// UE_LOG(LogTemp, Warning, TEXT("%f :Update from SUMO. NextTimeToUpdate %f"), TimeInWorld, NextTimeToUpdate)
	}
	// else {
		// UE_LOG(LogTemp, Display, TEXT("%f GameMode Tick()"), TimeInWorld)
	// }
}

void ASumoDefaultPawn::UpdateFromSUMO() {
		if (client.simulation.getMinExpectedNumber() > 0) {
			client.simulationStep();
			// SUMO time increment 
			SUMOStep++;
			SUMOTime += SUMODeltaT;
			// UE_LOG(LogTemp, Warning, TEXT("MachineTime;;SUMOTime;%f;SUMOStep;%d"),  SUMOTime, SUMOStep)


			DepartedNumber = client.simulation.getDepartedNumber();
			ArrivedNumber = client.simulation.getArrivedNumber();


			// TODO: Think how to destroy veicle from here
			//if (ArrivedNumber != 0) {
			//	std::vector<std::string> arrivedList = client.simulation.getArrivedIDList();
			//	for (int i = 0; i < ArrivedNumber; i++) {
			//		// Remove from the simulatedVehicle
			//		if (!RemoveVehicleFromList(arrivedList[i].c_str(), VehicleList)) {
			//			// removeVehicleFlag = true;
			//			break;
			//		}

			//	}
			//}


			if (DepartedNumber != 0) {

				std::vector<std::string> departedList = client.simulation.getDepartedIDList();

				// for (unsigned int i = 0; i < departedList.size(); i++) {
				for (int i = 0; i < DepartedNumber; i++) {

					/// Retrieve vehicle id, speed, positon and color from SUMO 
					// Covert std::string into FString
					DepartedVehicleId = departedList[i];
					SUMOVehicleInformation.VehicleId = DepartedVehicleId.c_str();

					// Assign speed
					SUMOVehicleInformation.VehicleSpeed = client.vehicle.getSpeed(DepartedVehicleId);

					// Convert libsumo::TraCIPosition into FVector 
					DepartedVehiclePos = client.vehicle.getPosition(DepartedVehicleId);
					SUMOVehicleInformation.VehiclePosition.X = DepartedVehiclePos.x * MeterUnitConversion;
					SUMOVehicleInformation.VehiclePosition.Y = DepartedVehiclePos.y * MeterUnitConversion;
					SUMOVehicleInformation.VehiclePosition.Z = DepartedVehiclePos.z * MeterUnitConversion;

					// Convert libsumo:: TraCIColor into FColor
					DepartedVehicleColor = client.vehicle.getColor(DepartedVehicleId);
					SUMOVehicleInformation.VehicleColor.R = DepartedVehicleColor.r;
					SUMOVehicleInformation.VehicleColor.G = DepartedVehicleColor.g;
					SUMOVehicleInformation.VehicleColor.B = DepartedVehicleColor.b;
					SUMOVehicleInformation.VehicleColor.A = DepartedVehicleColor.a;

					//Add to the simulatedVehicle
					//if (!AddVehicleToList(Vehicle, VehicleList)) {
					//	UE_LOG(LogTemp, Error, TEXT("Fail to add vehicle to list."))
					//		// addVehicleFlag = true;
					//		break;
					//}

					/*if (SpawnRandomVehicle(SUMOVehicleInformation)) {

					}*/

				}

			}




		}
		else {
			// Close socket 
			client.close();
			SocketIsNotClosed = false;

			// Exit game in UE
			//FGenericPlatformMisc::RequestExit(false);
			//GetWorld()->Exec(GetWorld(), TEXT("Exit"));
		}

}

//bool ASumoDefaultPawn::RemoveVehicleFromList(FString VehicleId, TMap<FString, SUMOVehicleInformation> &VehicleList) {
//	int eraseFlag = -1;
//	if (VehicleList.Contains(VehicleId)) {
//		eraseFlag = VehicleList.Remove(VehicleId);
//		//If erase success, eraseFlag is 1; otherwise, eraseFlag is 0
//		if (eraseFlag != 1) {
//			return false;
//		}
//		//UE_LOG(LogTemp, Display, TEXT("Step %d: Remove vehicle %s from list."), step, *vehicleId);
//		UE_LOG(LogTemp, Display, TEXT("Destroy %s"), *VehicleId);
//
//
//		// TODO Find Vehicle that need to be removed here instead of let the component find whether it is need to be removed
//		if (RandomVehicle) {
//			RandomVehicle->VehicleToDestroy(VehicleId);
//		}
//
//	}
//	else {
//		UE_LOG(LogTemp, Warning, TEXT("Fail to find vehicle %s from list."), *VehicleId);
//		return false;
//	}
//
//	return true;
//}

bool ASumoDefaultPawn::SpawnRandomVehicle(FVehicleInformation& DepartedVehicle) {
	FRotator Rotator;
	// FVector spawnLocation = vehicle.vehiclePosition;
	// Spawn a vehicle at its start location from SUMO
	UWorld* world = GetWorld();
	if (world) {


		SpawnPoint = SUMOVehicleInformation.VehiclePosition;

		// TODO modify the FRotator
		if (SpawnPoint.X > 0) {
			Rotator = FRotator(0, 90, 0);
		}
		else {
			Rotator = FRotator(0, -90, 0);
		}


		if (VehicleBPList.Num() > 0) {

			selectedClass = *VehicleBPList[FMath::RandRange(0, VehicleBPList.Num() - 1)];
			RandomVehicle = Cast<AVehicle>(world->SpawnActor(selectedClass, &SpawnPoint, &Rotator));
			if (RandomVehicle->InitializeVehicle(SUMOVehicleInformation.VehicleId, SUMOVehicleInformation.VehicleSpeed, SUMOVehicleInformation.VehiclePosition, SUMOVehicleInformation.VehicleColor, &client, UpdateDeltaT, NextTimeToUpdate)) {
				// UE_LOG(LogTemp, Warning, TEXT("SpawnVehicle %s."), *RandomVehicle->GetName())
				return true;
			}
			
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Fail to spawn vehcile because none blueprint class is selected"))
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Can't get world."))
	}

	return false;
}