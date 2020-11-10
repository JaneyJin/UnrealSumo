// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "UnrealSumo/World/FrameRateSyn.h"
#include "VehicleInformation.h"
#include <string>
#include <vector>
#include "SumoWheeledVehicle.generated.h"

class Client;
class UInputComponent;
class USumoGameInstance;

UCLASS()
class UNREALSUMO_API ASumoWheeledVehicle : public AWheeledVehicle
{
	GENERATED_BODY()

private:
    // The socket on client side (Unreal side) that inherited from TraCIAPI class. Used to update vehicles' speed, world position and rotator from SUMO to Unreal.
    Client* client;

    // Identity of wheeled vehicle in unreal engine and SUMO vehicle.
    FString VehicleId;

    // Shared custom GameInstance class. Variables in SumoGameInstance are modified in SumoGameMode.
    USumoGameInstance* SumoGameInstance;

    /// Variables modified in UpdateFromSUMO()
    FVector VehicleNewPosition = FVector(0, 0, 0);
    // Convert from meters(SUMO) to centimeter(Unreal)
    int32 MeterUnitConversion = 100;
    std::vector<std::string> ArrivedList;
    FRotator VehicleNewRotator = FRotator(0, 0, 0);

    /**
     * Update Unreal from SUMO based on Tick counter
     */
    void UpdateFromSUMOByTickCount();

    /**
     * Get arrived (left scenario) vehicle list from SUMO in current time. Destroy wheeled vehicle if identity of sumo wheeled vehicle is in the arrived list and vice versa.
     * Update vehicle speed, and turning angle form SUMO.
     *
     */
    void UpdateFromSUMO();

    /**
     * Destroy current sumo wheeled vehicle in Unreal Engine since it arrived the destination in SUMO.
     *
     * @return  True on success destory a wheeled vehicle and vice versa.
     */
    bool DestroyVehicle();

public:
    /**
     * Constructor for SumoWheeledVehicle.
     */
    ASumoWheeledVehicle();


    /**
	 * Assign the vehicle identity from SUMO server to sumo wheeled vehicle in Unreal Engine
	 */
	bool InitializeWheeledVehicle(FVehicleInformation &VehicleToSet);

	/**
	 * Override default BeginPlay function. Called when sumo wheeled vehicle is spawned.
	 */
	virtual void BeginPlay() override;

	/**
	 * Override default Tick function. This function is called every frame.
	 */
	virtual void Tick(float Delta) override;


	// NOT USED NOW
	void MoveForward(float Val);
	void MoveRight(float Val);
	void OnHandbrakePressed();
	void OnHandbrakeReleased();
};
