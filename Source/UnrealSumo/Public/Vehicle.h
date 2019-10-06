// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrameRateSyn.h"
#include "VehicleInformation.h"
#include "GameFramework/Pawn.h"
#include "Vehicle.generated.h"

class Client;
class UVehiclePoisitonUpdateComponent;

UCLASS()
class UNREALSUMO_API AVehicle : public APawn
{
	GENERATED_BODY()


	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Instantiate VehicleInformation Structure class to pass these value to VehiclePositionUpdateComponent Class
	FVehicleInformation UnrealVehicleInformation;
	
	// Sets default values for this pawn's properties
	AVehicle();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * TODO
	 * @param
	 * @return
	 */
	bool InitializeVehicle(FVehicleInformation VehicleToSet, Client* ClientToSet, FrameRateSyn SUMOToUnrealFrameRate);


};
