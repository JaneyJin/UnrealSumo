#pragma once
#include "CoreMinimal.h"
#include "VehicleInformation.generated.h"

USTRUCT(BlueprintType)
struct FVehicleInformation
{
	GENERATED_BODY()
public:
	FString VehicleId; // std::string 
	double VehicleSpeed; // double
	FVector VehiclePosition; // libsumo::TraCIPosition
	FColor VehicleColor; // libsumo::TraCIColor


	/*UPROPERTY(BlueprintReadWrite, Category = "Student")
	int32 test;*/
};