// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "FrameRateSyn.h"
#include "VehicleInformation.h"
#include "WheeledVehiclePawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;
class UInputComponent;

class Client;

class UVehiclePositionUpdateComponent;

UCLASS()
class UNREALSUMO_API AWheeledVehiclePawn : public AWheeledVehicle
{
	GENERATED_BODY()

protected:
	// User defined wheeled vehicle
	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;

	/** SCene component for the In-Car view origin */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* InternalCameraBase;

	/** Camera component for the In-Car view */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* InternalCamera;

	/** Text component for the In-Car speed */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* InCarSpeed;

	/** Text component for the In-Car gear */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* InCarGear;

	/** Are we using incar camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
		bool bInCarCameraActive;

	/** Are we in reverse gear */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
		bool bInReverseGear;

	/** Initial offset of incar camera */
	FVector InternalCameraOrigin;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		/** The color of the incar gear text in forward gears */
		FColor	GearDisplayColor;

	/** The color of the incar gear text when in reverse */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FColor	GearDisplayReverseColor;

	/** The current speed as a string eg 10 km/h */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FText SpeedDisplayString;

	/** The current gear as a string (R,N, 1,2 etc) */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FText GearDisplayString;

	int count = 0;
	int ThrottleVal = 10;

public:

	static const FName LookUpBinding;
	static const FName LookRightBinding;

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
	void MoveForward(float Val);
	void MoveRight(float Val);
	void OnHandbrakePressed();
	void OnHandbrakeReleased();


	void OnToggleCamera();
	void EnableIncarView(const bool bState, const bool bForce = false);
	void OnResetVR();
	void UpdateHUDStrings();
	void SetupInCarHUD();

	/** Returns SpringArm subobject **/
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	/** Returns InternalCamera subobject **/
	FORCEINLINE UCameraComponent* GetInternalCamera() const { return InternalCamera; }
	/** Returns InCarSpeed subobject **/
	FORCEINLINE UTextRenderComponent* GetInCarSpeed() const { return InCarSpeed; }
	/** Returns InCarGear subobject **/
	FORCEINLINE UTextRenderComponent* GetInCarGear() const { return InCarGear; }
	
	AWheeledVehiclePawn();

	virtual void BeginPlay() override;
	virtual void Tick(float Delta) override;



	// Update vehicle information
	bool InitializeWheeledVehicle(FVehicleInformation &VehicleToSet, Client* ClientToSet, FrameRateSyn &SUMOToUnrealFrameRate);
	bool DestroyVehicle();
	void UpdateSUMOByTickCount(float Delta);
	void UpdateFromSUMO(float Delta);
	// Instantiate VehicleInformation Structure class to pass these value to VehiclePositionUpdateComponent Class
	FVehicleInformation UnrealVehicleInformation;

	Client* client;
	FString  VehicleId;
	FrameRateSyn UnrealFRS;
	

	UVehiclePositionUpdateComponent* VehiclePositionUpdateComponent = nullptr;
};