// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrameRateSyn.h"
#include "VehicleInformation.h"
#include "GameFramework/Pawn.h"
#include "CustomVehicle.generated.h"
class Client;
class UVehiclePositionUpdateComponent;
UCLASS()
class UNREALSUMO_API ACustomVehicle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACustomVehicle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Attach Actor component to the Vehicle blueprint class
	UVehiclePositionUpdateComponent* VehiclePositionUpdateComponent = nullptr;

	// Instantiate VehicleInformation Structure class to pass these value to VehiclePositionUpdateComponent Class
	FVehicleInformation UnrealVehicleInformation;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	/**
	 * TODO
	 * @param
	 * @return
	 */
	bool InitializeVehicle(FVehicleInformation &VehicleToSet, Client* ClientToSet, FrameRateSyn &SUMOToUnrealFrameRate);


};
