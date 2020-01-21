// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicleMovementComponent.h"
#include "WheeledVehicleUpdateComponent.generated.h"

class ACustomWheeledVehicle;

UCLASS()
class UNREALSUMO_API UWheeledVehicleUpdateComponent : public UWheeledVehicleMovementComponent
{
	GENERATED_BODY()

private:
	// BeginPlay()
	ACustomWheeledVehicle* Owner = nullptr;

public:
	UWheeledVehicleUpdateComponent();


	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
