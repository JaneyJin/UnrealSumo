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
	//ACustomWheeledVehicle(const FObjectInitializer& ObjectInitializer);
	ACustomWheeledVehicle();
	
	// Instantiate VehicleInformation Structure class to pass these value to VehiclePositionUpdateComponent Class
	FVehicleInformation UnrealVehicleInformation;


	virtual void BeginPlay() override;
	virtual void Tick(float Delta) override;

	void setupVehicleMovementComponent();

	/**
	 * TODO
	 * @param
	 * @return
	 */
	bool InitializeWheeledVehicle(FVehicleInformation &VehicleToSet, Client* ClientToSet, FrameRateSyn &SUMOToUnrealFrameRate);
	//UWheeledVehicleMovementComponent* VehicleMovementComponent;
	UWheeledVehicleUpdateComponent* WheeledVehicleUpdateComponent = nullptr;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	void MoveForward(float Val);
	void MoveRight(float Val);
	void OnHandbrakePressed();
	void OnHandbrakeReleased();

	//void setupInputBindings();

	//void onMoveForward(float Val);

	//void onMoveRight(float Val);

	//void onHandbrakePressed();
	//void onHandbrakeReleased();
	//void onFootBrake(float Val);
	//void onFootBrake(float Val);
	

	//void onReversePressed();

	//void onReverseReleased();

	/** Returns Mesh subobject **/
	class USkeletalMeshComponent* GetMesh() const;
	/** Returns VehicleMovement subobject **/
	class UNWheeledVehicleMovementComponent* GetVehicleMovement() const;
};
