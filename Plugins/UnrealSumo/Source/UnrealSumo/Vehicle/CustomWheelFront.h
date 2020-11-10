// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehicleWheel.h"
#include "CustomWheelFront.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSUMO_API UCustomWheelFront : public UVehicleWheel
{
	GENERATED_BODY()

public:
    // Parameter for front wheels. This is custom UVehicleWheeled class and it is used by WheeledVehiclePawn when setup ego wheeled vehicle.
    UCustomWheelFront();
	
	
};
