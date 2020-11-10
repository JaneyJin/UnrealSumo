#pragma once
#include "CoreMinimal.h"
#include <iostream>
#include "VehicleInformation.generated.h"

USTRUCT(BlueprintType)
struct FVehicleInformation
{
	GENERATED_BODY()

public:
	/// VehicleInformation store vehicle information retreive from SUMO for each vehicle departed in SUMO scenario. Units and data format changed to Unreal defined format.

	// Wheeled vehicle identity set up in SUMO. Format changed from 'std::string' to 'FString'.
	FString VehicleId = ""; 

	// Vehicle position in Unreal after convert units. Format changed from 'libsumo::TraCIPosition' to 'FVector'.
	FVector VehiclePosition = FVector(0, 0, 0); 

	// Speed for wheeled vehicle. Unit is cm/s.
	double VehicleSpeed = 0; 

	// FRotator(Pitch, Yaw, Roll). Only modified the Yaw. Yaw is rotation around the up axis (around Z axis), Running in circles 0=East, +North, -South.
	FRotator VehicleAngle = FRotator(0,0,0);
	
	/* Logout vehicle information. */
	void print() {
		UE_LOG(LogTemp, Warning, TEXT("VehicleID: %s; VehiclePosition: %s; VehicleSpeed: %f; VehicleAngle: %s"), *VehicleId, *VehiclePosition.ToString(), VehicleSpeed, *VehicleAngle.ToString());
	}

};
