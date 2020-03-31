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
class UBoxComponent;
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



	
	
	FString  VehicleId;
	FrameRateSyn UnrealFRS;

	UVehiclePositionUpdateComponent* VehiclePositionUpdateComponent = nullptr;

private:

	/// Transform of the vehicle. Location is shifted so it matches center of the
	/// vehicle bounds rather than the actor's location.
	UFUNCTION(Category = "Unreal Ego Wheeled Vehicle", BlueprintCallable)
		FTransform GetVehicleTransform() const
	{
		return GetActorTransform();
	}

	/// Forward speed in cm/s. Might be negative if goes backwards.
	UFUNCTION(Category = "Unreal Ego Wheeled Vehicle", BlueprintCallable)
		float GetVehicleForwardSpeed() const;

	/// Orientation vector of the vehicle, pointing forward.
	UFUNCTION(Category = "Unreal Ego Wheeled Vehicle", BlueprintCallable)
		FVector GetVehicleOrientation() const;

	/// Active gear of the vehicle.
	UFUNCTION(Category = "Unreal Ego Wheeled Vehicle", BlueprintCallable)
		int32 GetVehicleCurrentGear() const;

	/// Transform of the vehicle's bounding box relative to the vehicle.
	UFUNCTION(Category = "Unreal Ego Wheeled Vehicle", BlueprintCallable)
		FTransform GetVehicleBoundingBoxTransform() const;

	/// Extent of the vehicle's bounding box.
	UFUNCTION(Category = "Unreal Ego Wheeled Vehicle", BlueprintCallable)
		FVector GetVehicleBoundingBoxExtent() const;

	/// Get vehicle's bounding box component.
	UFUNCTION(Category = "Unreal Ego Wheeled Vehicle", BlueprintCallable)
	UBoxComponent *GetVehicleBoundingBox() const
	{
		return VehicleBounds;
	}

	/// Get the maximum angle at which the front wheel can steer.
	UFUNCTION(Category = "CARLA Wheeled Vehicle", BlueprintCallable)
		float GetMaximumSteerAngle() const;


	

	UPROPERTY(Category = "Unreal Ego Wheeled Vehicle", EditAnywhere)
		UBoxComponent *VehicleBounds;

public:
	// Instantiate VehicleInformation Structure class to pass these value to VehiclePositionUpdateComponent Class
	FVehicleInformation EgoWheeledVehicleInformation;
	Client* client;
	float GetEgoWheeledVehicleSpeed() { return EgoWheeledVehicleInformation.VehicleSpeed; }
	// void GetEgoWheeledVehicleInformation(FVehicleInformation &v) { v = EgoWheeledVehicleInformation; }

	void UpdateSpeedToSumo();

	void SetupSocketForEgoWheeledVehicle(FString VehicleId, Client* ClientToSet);
	
};