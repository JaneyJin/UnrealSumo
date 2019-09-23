// Fill out your copyright notice in the Description page of Project Settings.


#include "SumoDefaultPawn.h" // Must at the line for #include
#include "Misc/App.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
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
	if (SUMOToUnrealFrameRate.TickCount < SUMOToUnrealFrameRate.FPS) {
		SUMOToUnrealFrameRate.TickCount++;
	}
	else {
		SUMOToUnrealFrameRate.TickCount = 1;
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
	SUMOToUnrealFrameRate.FPS = FMath::RoundHalfFromZero(1.0f / UEDeltaT);

	UE_LOG(LogTemp, Warning, TEXT("SUMO DeltaT is %f. UE DeltaT is %f.FPS is %d"), SUMODeltaT, UEDeltaT, SUMOToUnrealFrameRate.FPS)

		if (SUMODeltaT > 0) {

			if (SUMODeltaT < UEDeltaT) {
				UE_LOG(LogTemp, Error, TEXT("Unreal Engine frame rate is lower than SUMO. Cannot change it within code. Please change in setting."));
			}
			else {
				SUMOToUnrealFrameRate.NextTimeToUpdate = SUMOToUnrealFrameRate.UpdateDeltaT = SUMODeltaT;
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

	UE_LOG(LogTemp, Warning, TEXT("Update at %f per second. Next update at %f"), SUMOToUnrealFrameRate.UpdateDeltaT, SUMOToUnrealFrameRate.NextTimeToUpdate)
}

void ASumoDefaultPawn::UpdateSUMOByTickCount() {
	if (SUMOToUnrealFrameRate.TickCount < SUMOToUnrealFrameRate.FPS) {
		UE_LOG(LogTemp, Display, TEXT("GameMode Tick() %d"), SUMOToUnrealFrameRate.TickCount)
	}
	else if (SUMOToUnrealFrameRate.TickCount == SUMOToUnrealFrameRate.FPS) {
		// UE_LOG(LogTemp, Warning, TEXT("%f :Update from SUMO. NextTimeToUpdate %f"), TimeInWorld, NextTimeToUpdate)
		UE_LOG(LogTemp, Display, TEXT("GameMode Tick() %d. Update from SUMo."), SUMOToUnrealFrameRate.TickCount)
			UpdateFromSUMO();
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Tick calculation is wrong."))
	}

}

void ASumoDefaultPawn::UpdateSUMOByMachineTime() {
	TimeInWorld = GetWorld()->GetTimeSeconds();

	if (SUMOToUnrealFrameRate.NextTimeToUpdate - TimeInWorld < 0.0001) {
		SUMOToUnrealFrameRate.NextTimeToUpdate += SUMOToUnrealFrameRate.UpdateDeltaT;
		UpdateFromSUMO();
		 UE_LOG(LogTemp, Warning, TEXT("%f :Update from SUMO. NextTimeToUpdate %f"), TimeInWorld, SUMOToUnrealFrameRate.NextTimeToUpdate)
	}
	 else {
		 UE_LOG(LogTemp, Display, TEXT("%f GameMode Tick()"), TimeInWorld)
	 }
}

void ASumoDefaultPawn::UpdateFromSUMO() {
	if (client.simulation.getMinExpectedNumber() > 0) {
		client.simulationStep();
		// SUMO time increment 
		SUMOStep++;
		SUMOTime += SUMODeltaT;
		// UE_LOG(LogTemp, Warning, TEXT("MachineTime;;SUMOTime;%f;SUMOStep;%d"),  SUMOTime, SUMOStep)

		// TODO: Think how to destroy veicle from here
		// ArrivedNumber = client.simulation.getArrivedNumber();
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
				SUMOVehicleInformation.VehiclePosition.Y = DepartedVehiclePos.y * MeterUnitConversion;
				SUMOVehicleInformation.VehiclePosition.Z = DepartedVehiclePos.z * MeterUnitConversion;

				// Convert libsumo:: TraCIColor into FColor
				DepartedVehicleColor = client.vehicle.getColor(DepartedVehicleId);
				SUMOVehicleInformation.VehicleColor.R = DepartedVehicleColor.r;
				SUMOVehicleInformation.VehicleColor.G = DepartedVehicleColor.g;
				SUMOVehicleInformation.VehicleColor.B = DepartedVehicleColor.b;
				SUMOVehicleInformation.VehicleColor.A = DepartedVehicleColor.a;

				if (SpawnRandomVehicle(SUMOVehicleInformation)) {
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

bool ASumoDefaultPawn::SpawnRandomVehicle(FVehicleInformation& DepartedVehicle){
	FRotator Rotator;
	// FVector spawnLocation = vehicle.vehiclePosition;
	// Spawn a vehicle at its start location from SUMO
	UWorld* world = GetWorld();
	if (world) {


		SpawnPoint = SUMOVehicleInformation.VehiclePosition;

		// TODO: modify the FRotator
		/*if (SpawnPoint.X > 0) {
			Rotator = FRotator(0, 90, 0);
		}
		else {
			Rotator = FRotator(0, -90, 0);
		}*/

		Rotator = FRotator(0, 0, 0);
		if (VehicleBPList.Num() > 0) {

			selectedClass = *VehicleBPList[FMath::RandRange(0, VehicleBPList.Num() - 1)];
			RandomVehicle = Cast<AVehicle>(world->SpawnActor(selectedClass, &SpawnPoint, &Rotator));
			// ,  &SUMOVehicleInformation
			if (RandomVehicle->InitializeVehicle(SUMOVehicleInformation,&client, SUMOToUnrealFrameRate)) {
				// UE_LOG(LogTemp, Warning, TEXT("SpawnVehicle %s."), *RandomVehicle->GetName())
				return true;
			}
			
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Fail to spawn vehicle because none blueprint class is selected"))
			if (GEngine)
			{
				const int32 AlwaysAddKey = -1; // Add a new one instead of overwrite last message
				static const FString ErrorMessage(TEXT("Fail to spawn vehicle because none blueprint class is selected"));
				GEngine->AddOnScreenDebugMessage(AlwaysAddKey, 5.0f, FColor::Red, ErrorMessage);

			}
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Can't get world."))
	}

	return false;
}