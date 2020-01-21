#pragma once
#include "CoreMinimal.h"
#include "VehicleInformation.generated.h"

USTRUCT(BlueprintType)
struct FVehicleInformation
{
	GENERATED_BODY()
public:
	FString VehicleId = ""; // std::string 
	FVector VehiclePosition = FVector(0, 0, 0); // libsumo::TraCIPosition  
	double VehicleSpeed = 0; // double 
	FColor VehicleColor = FColor(0, 0, 0, 1); // libsumo::TraCIColor //NOT USE
	FRotator VehicleAngle = FRotator(0,0,0);

};