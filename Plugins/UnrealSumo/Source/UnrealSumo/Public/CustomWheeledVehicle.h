// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrameRateSyn.h"
#include "VehicleInformation.h"
#include "WheeledVehicle.h"
#include "CustomWheeledVehicle.generated.h"

class Client;
class UWheeledVehicleUpdateComponent;

UCLASS()
class UNREALSUMO_API ACustomWheeledVehicle : public AWheeledVehicle
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACustomWheeledVehicle();

	// Instantiate VehicleInformation Structure class to pass these value to VehiclePositionUpdateComponent Class
	FVehicleInformation UnrealVehicleInformation;


	virtual void BeginPlay() override;

	virtual void Tick(float Delta) override;

	/**
	 * TODO
	 * @param
	 * @return
	 */
	bool InitializeWheeledVehicle(FVehicleInformation &VehicleToSet, Client* ClientToSet, FrameRateSyn &SUMOToUnrealFrameRate);

	UWheeledVehicleUpdateComponent* WheeledVehicleUpdateComponent = nullptr;
};
