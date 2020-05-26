// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "FrameRateSyn.h"
#include "VehicleInformation.h"
#include "FrameRateSyn.h"
#include "WheeledVehiclePawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;
class UInputComponent;
class UBoxComponent;
class Client;
class USumoGameInstance;

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

	

private:
	// The socket on client side (Unreal side) that inherited from TraCIAPI class. Used to send ego wheeled vehicle speed, world position and rotator to SUMO server in the timestep to update.
	Client* client;

	// Identity of ego wheeled vehicle in unreal engine and SUMO added vehicle. 
	FString  EgoWheeledVehicleId;

	/// Transform of the vehicle. Location is shifted so it matches center of the vehicle bounds rather than the actor's location.
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
	UFUNCTION(Category = "Unreal Ego Wheeled Vehicle", BlueprintCallable)
		float GetMaximumSteerAngle() const;

	/// A box used for simple collision. Bounds are rendered as lines in the editor.
	UPROPERTY(Category = "Unreal Ego Wheeled Vehicle", EditAnywhere)
		UBoxComponent *VehicleBounds;


	/// Variables modified in UpdateEgoWheeledVehicleToSUMO()
	// Ego wheeled vehicle position in the world
	FVector VehiclePositionInWorld;
	// Yaw of the ego wheeled vehicle relative to the world. Yaw is rotation around the up axis (around Z axis), Running in circles 0=East, +North, -South.
	float VehicleAngle;

	// Shared custom GameInstance class. Variables in SumoGameInstance are modified in SumoGameMode.
	USumoGameInstance* SumoGameInstance;

	/**
	 * Update SUMO from Unreal based on Tick counter
	 */
	void UpdateToSUMOByTickCount();

	/**
	 * Retrieve vehicle speed, position and yaw of ego wheeled vehicle and update to SUMO server
	 */
	void UpdateEgoWheeledVehicleToSUMO();


public:
	// TODO
	static const FName LookUpBinding;
	static const FName LookRightBinding;


	// Set up custom input bindings with class
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

	/** Handle vehicle throttle set by user input */
	void MoveForward(float Val);

	/** Handle vehicle steering set by the user input */
	void MoveRight(float Val);

	/** Handle handbrake pressed */
	void OnHandbrakePressed();

	/** Handle handbrake released */
	void OnHandbrakeReleased();

	/** Switch between cameras */
	void OnToggleCamera();

	/**
	 * Activate In-Car camera. Enable camera and sets visibility of incar hud display
	 *
	 * @param	bState true will enable in car view and set visibility of various if its doesnt match new state
	 * @param	bForce true will force to always change state
	 */
	void EnableIncarView(const bool bState, const bool bForce = false);

	/**
	 * NOT USED 
	 * Handle reset VR device
	 * 
	*/
	void OnResetVR();

	/** Update the gear and speed strings */
	void UpdateHUDStrings();

	/** Setup the strings used on the hud */
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
	
	/* Constuctor class to set springarm, camera, internal camera, vehicle wheels and car speed display */
	AWheeledVehiclePawn();

	/**
	 * Override default BeginPlay function. Called when ego wheeled vehicle is spawned.
	 */
	virtual void BeginPlay() override;

	/**
	 * Override default Tick function. This function is called every frame.
	 */
	virtual void Tick(float Delta) override;



	// NOT USED
	std::string EndEdge;
	// Return false if ego wheeled vehicle finish the route in the SUMO
	bool ArrivedFlag;

	
	/**
	 * NOT USED
	 *
	 */
	void EgoWheeledVehicleArrivedInSUMO();
};