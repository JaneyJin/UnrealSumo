// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehicleWheel.h"
#include "CustomWheelRear.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSUMO_API UCustomWheelRear : public UVehicleWheel
{
	GENERATED_BODY()

public:
	// Parameter set for rear wheels. This is custom UVehicleWheeled class and it is used by WheeledVehiclePawn when setup ego wheeled vehicle. 
	UCustomWheelRear();
};
