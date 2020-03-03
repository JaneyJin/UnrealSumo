// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "FrameRateSyn.h"
#include "VehicleInformation.h"
#include "SumoWheeledVehicle.generated.h"

class Client;

/**
 * 
 */
UCLASS()
class UNREALSUMO_API ASumoWheeledVehicle : public AWheeledVehicle
{
	GENERATED_BODY()

private:
	Client* client;
	FString  VehicleId;

public:
	ASumoWheeledVehicle();
	virtual void BeginPlay() override;
	virtual void Tick(float Delta) override;
	bool InitializeWheeledVehicle(FVehicleInformation &VehicleToSet, Client* ClientToSet, FrameRateSyn &SUMOToUnrealFrameRate);
	void MoveForward(float Val);
	void MoveRight(float Val);
	void OnHandbrakePressed();
	void OnHandbrakeReleased();
	bool DestroyVehicle();
	void UpdateSUMOByTickCount(float Delta);
	void UpdateFromSUMO(float Delta);
	

	// Instantiate VehicleInformation Structure class to pass these value to VehiclePositionUpdateComponent Class
	FVehicleInformation UnrealVehicleInformation;
	FrameRateSyn UnrealFRS;

};
