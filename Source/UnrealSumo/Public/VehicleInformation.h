#pragma once
#include "CoreMinimal.h"
#include "VehicleInformation.generated.h"

USTRUCT(BlueprintType)
struct FVehicleInformation
{
	GENERATED_BODY()
public:
	FString VehicleId; // std::string 
	FVector VehiclePosition; // libsumo::TraCIPosition  
	double VehicleSpeed; // double // NOT USE
	FColor VehicleColor; // libsumo::TraCIColor //NOT USE

	/*UPROPERTY(BlueprintReadWrite, Category = "Student")
	int32 test;*/
};