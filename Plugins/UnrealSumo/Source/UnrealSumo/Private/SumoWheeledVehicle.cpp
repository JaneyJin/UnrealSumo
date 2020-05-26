// Fill out your copyright notice in the Description page of Project Settings.


#include "SumoWheeledVehicle.h"
#include "Client.h"
#include "Components/InputComponent.h"
#include "Engine/SkeletalMesh.h"
#include "SumoGameInstance.h"
#include "WheeledVehicleMovementComponent4W.h"

ASumoWheeledVehicle::ASumoWheeledVehicle() {
	client = nullptr;
	VehicleId = "";
}

bool ASumoWheeledVehicle::InitializeWheeledVehicle(FVehicleInformation &VehicleToSet) {

	this->VehicleId = VehicleToSet.VehicleId;
	
	if (client && VehicleId.IsEmpty()) {
		/* Fail to initialize */
		return false; 
	}
	return true;
}

void ASumoWheeledVehicle::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultController();

	if (!GetController()) {
		Destroy();
	}
	
	SumoGameInstance = Cast<USumoGameInstance>(GetGameInstance());
	if (SumoGameInstance) {
		this->client = SumoGameInstance->client;
	}
	
	// UE_LOG(LogTemp, Log, TEXT("GetVehicleMovement: %s; GetVehicleMovementComponent: %s"), *GetVehicleMovement()->GetName(), *GetVehicleMovementComponent()->GetName());
}

void ASumoWheeledVehicle::Tick(float Delta)
{
	Super::Tick(Delta);
	/* Unreal Engine frame rate must be higher than SUMO frame rate. */
	/* Check whether the socket connection close */
	if (GetController() && SumoGameInstance && SumoGameInstance->client && !SumoGameInstance->SUMOToUnrealFrameRate.UnrealTickSlower) {
		UpdateFromSUMOByTickCount();
	}
	
}

void ASumoWheeledVehicle::UpdateFromSUMOByTickCount() {
	if (SumoGameInstance->SUMOToUnrealFrameRate.TickCount == SumoGameInstance->SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates) {
		// UE_LOG(LogTemp, Warning, TEXT("%f :Update from SUMO. NextTimeToUpdate %f"), TimeInWorld, NextTimeToUpdate)
		UE_LOG(LogTemp, Warning, TEXT("SumoWheeledVehicle -> WheeledVehicle Tick() %d. Update from SUMo."), SumoGameInstance->SUMOToUnrealFrameRate.TickCount)
		UpdateFromSUMO();
	}

}

void ASumoWheeledVehicle::UpdateFromSUMO() {

	std::string VID(TCHAR_TO_UTF8(*VehicleId));
	// UE_LOG(LogTemp, Display, TEXT("GetOwner %s :: VehicleId %s "), *ObjectName, *VID);

	
	if (SumoGameInstance->ArrivedNumber > 0) {
		std::vector<std::string> ArrivedList = SumoGameInstance->ArrivedList;
		std::vector<std::string>::iterator it = std::find(ArrivedList.begin(), ArrivedList.end(), VID);
		if ( it != ArrivedList.end()) {
			/* Destroy this SumoWheeledVehicle class. */
			DestroyVehicle();
			/* Deleted the destroyed vehicle in the arrived list. */
			ArrivedList.erase(it);
			SumoGameInstance->ArrivedList = ArrivedList;
			SumoGameInstance->ArrivedNumber = SumoGameInstance->ArrivedNumber - 1;
			return;
		}

	}
	
	// Update location
	auto vehiclepos = client->vehicle.getPosition(VID);
	VehicleNewPosition.X = vehiclepos.x * MeterUnitConversion;
	VehicleNewPosition.Y = vehiclepos.y * MeterUnitConversion * -1;
	VehicleNewPosition.Z = vehiclepos.z * MeterUnitConversion;

	auto vehicleangle = client->vehicle.getAngle(VID) - 90;
	if (fabs(vehicleangle - VehicleNewRotator.Yaw) > 0.00001) {
		VehicleNewRotator.Yaw = vehicleangle;
	}
	// UE_LOG(LogTemp, Display, TEXT("New Location: %s ; New Rotator: %s"), *VehicleNewPosition.ToString(), *VehicleNewRotator.ToString())

	FHitResult HitResult;
	if (K2_SetActorLocationAndRotation(VehicleNewPosition, VehicleNewRotator, false, HitResult, true) == false) {
		// If the set function returned false something is blocking at that location. We can interrogate this result to determine details of this  
		// @See FHitResult for more information  
		if (HitResult.GetActor() != nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Cannot move object to location, blocked by %s"), *HitResult.GetActor()->GetName());
		}
	}
	
	// TODO: Vehicle dynamics controller
	// MoveForward(speed);

}

bool ASumoWheeledVehicle::DestroyVehicle() {

	if (this->Destroy()) {
		//FString VId = VehicleId.c_str();
		UE_LOG(LogTemp, Display, TEXT("Destroy %s success."), *VehicleId)
			return true;
	}

	UE_LOG(LogTemp, Error, TEXT("Fail to Destroy vehicle %s."), *VehicleId);
	return false;
}



void ASumoWheeledVehicle::MoveForward(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void ASumoWheeledVehicle::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void ASumoWheeledVehicle::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ASumoWheeledVehicle::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

