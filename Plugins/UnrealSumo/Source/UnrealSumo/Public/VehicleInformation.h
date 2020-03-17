#pragma once
#include "CoreMinimal.h"
#include <iostream>
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

	void print() {
		UE_LOG(LogTemp,Warning,TEXT("VehicleID: %s; VehiclePosition: %s"), TCHAR_TO_UTF8(*VehicleId), TCHAR_TO_UTF8(*VehiclePosition.ToString()), VehicleSpeed, TCHAR_TO_UTF8(*VehicleAngle.ToString()));
	}
};

//std::ostream& operator<<(std::ostream& os, const FVehicleInformation& vehicle) {
//	return os << "VehicleID: " << TCHAR_TO_UTF8(*vehicle.VehicleId) << std::endl
//		<< "VehiclePosition: " << TCHAR_TO_UTF8(*vehicle.VehiclePosition.ToString()) << std::endl
//		<< "VehicleSpeed: " << vehicle.VehicleSpeed << std::endl
//		<< "VehicleAngle:" << TCHAR_TO_UTF8(*vehicle.VehicleAngle.ToString()) << std::endl;
//}