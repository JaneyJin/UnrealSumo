// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePositionUpdateComponent.h"
#include "Client.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UVehiclePositionUpdateComponent::UVehiclePositionUpdateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVehiclePositionUpdateComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
}


// Called every frame
void UVehiclePositionUpdateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (UnrealFRS.TickCount < UnrealFRS.FPS) {
		UnrealFRS.TickCount++;
	}
	else {
		UnrealFRS.TickCount = 1;
	}

	// TODO: Check the socket is close or not
	//if (SocketIsNotClosed) {
		// UpdateSUMoByTickCount();
		UpdateSUMOByMachineTime();
	//}
	/*else {
		UE_LOG(LogTemp, Warning, TEXT("Tick. Socket Close."))
	}*/

	
}

bool UVehiclePositionUpdateComponent::SetVehicleIdAndClient(FVehicleInformation &VehicleToSet, Client* ClientToSet, FrameRateSyn &SUMOToUnrealFrameRate) {
	if ((VehicleToSet.VehicleId).IsEmpty()) {
		return false;
	}

	this->VehicleId = std::string(TCHAR_TO_UTF8(*VehicleToSet.VehicleId));
	this->client = ClientToSet;
	// this->UnrealFRS.UpdateDeltaT = SUMOToUnrealFrameRate.UpdateDeltaT;
	// this->UnrealFRS.NextTimeToUpdate = SUMOToUnrealFrameRate.NextTimeToUpdate;
	this->UnrealFRS = SUMOToUnrealFrameRate;
	return true;
}

void UVehiclePositionUpdateComponent::UpdateSUMOByTickCount() {
	if (UnrealFRS.TickCount < UnrealFRS.FPS) {
		UE_LOG(LogTemp, Display, TEXT("GameMode Tick() %d"), UnrealFRS.TickCount)
	}
	else if (UnrealFRS.TickCount == UnrealFRS.FPS) {
		// UE_LOG(LogTemp, Warning, TEXT("%f :Update from SUMO. NextTimeToUpdate %f"), TimeInWorld, NextTimeToUpdate)
		UE_LOG(LogTemp, Display, TEXT("GameMode Tick() %d. Update from SUMo."), UnrealFRS.TickCount)
			UpdateFromSUMO();
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Tick calculation is wrong."))
	}

}

void UVehiclePositionUpdateComponent::UpdateSUMOByMachineTime() {
	TimeInWorld = GetWorld()->GetTimeSeconds();

	if (UnrealFRS.NextTimeToUpdate - TimeInWorld < 0.0001) {
		UnrealFRS.NextTimeToUpdate += UnrealFRS.UpdateDeltaT;
		UpdateFromSUMO();
		// UE_LOG(LogTemp, Warning, TEXT("%f :Update from SUMO. NextTimeToUpdate %f"), TimeInWorld, NextTimeToUpdate)

	}
	//else {
		// UE_LOG(LogTemp, Display, TEXT("%f GameMode Tick()"), TimeInWorld)
	//}


}

void UVehiclePositionUpdateComponent::UpdateFromSUMO() {
	FString ObjectName = Owner->GetName();
	FString VID = VehicleId.c_str();
	// UE_LOG(LogTemp, Display, TEXT("GetOwner %s :: VehicleId %s "), *ObjectName, *VID);

	ArrivedNumber = client->simulation.getArrivedNumber();
	if (ArrivedNumber != 0) {
		ArrivedList = client->simulation.getArrivedIDList();
		for (std::vector<std::string>::iterator it = ArrivedList.begin(); it != ArrivedList.end(); ++it) {
			if (*it == VehicleId) {
				DestroyVehicle();
				return;
			}
		}

	}

	if (!Owner) { return; }

	// Update location
	auto vehiclepos = client->vehicle.getPosition(VehicleId);
	VehicleNewPosition.X = vehiclepos.x * MeterUnitConversion;
	VehicleNewPosition.Y = vehiclepos.y * MeterUnitConversion;
	VehicleNewPosition.Z = vehiclepos.z * MeterUnitConversion;

	auto vehicleangle = client->vehicle.getAngle(VehicleId);
	if (vehicleangle != VehicleNewRotator.Yaw) {
		VehicleNewRotator.Yaw = vehicleangle;
	}

	FHitResult HitResult;
	if (Owner->SetActorLocationAndRotation(VehicleNewPosition, VehicleNewRotator, false, &HitResult) == false) {

		// If the set function returned false something is blocking at that location. We can interrogate this result to determine details of this  
		// @See FHitResult for more information  
		if (HitResult.GetActor() != nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Cannot move object to location, blocked by %s"), *HitResult.GetActor()->GetName());
		}

	}

	// auto MyVehicle = Owner->GetName();
	// auto Time = GetWorld()->GetTimeSeconds();
	// UE_LOG(LogTemp, Warning, TEXT("%f: %s is at %s"), Time, *MyVehicle, *VehicleNewPosition.ToString());
}

bool UVehiclePositionUpdateComponent::DestroyVehicle() {

	if (Owner->Destroy()) {
		// FString VId = VehicleId.c_str();
		// UE_LOG(LogTemp, Warning, TEXT("Destroy %s success."), *VId)
		return true;
	}

	// UE_LOG(LogTemp, Warning, TEXT("Fail to Destroy."));
	return false;

}