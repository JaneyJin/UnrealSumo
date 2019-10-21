// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <string>
#include <vector>
#include "FrameRateSyn.h"
#include "VehicleInformation.h"
#include "Components/ActorComponent.h"
#include "VehiclePositionUpdateComponent.generated.h"

class Client;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALSUMO_API UVehiclePositionUpdateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Instantiate VehicleInformation Structure class to store Vehicle information get from SUMO and update these data in Unreal
	FVehicleInformation UnrealVehicleInformation;

	FrameRateSyn UnrealFRS;

	/**
	 * TODO
	 * @param
	 * @return
	 */
	bool SetVehicleIdAndClient(FVehicleInformation &VehicleToSet, Client* ClientToSet, FrameRateSyn &SUMOToUnrealFrameRate);

	/**
	 * Update Unreal from SUMO based on Tick counter
	 * @param
	 * @return
	 */
	void UpdateSUMOByTickCount();

	/**
	 * Update Unreal from SUMO based on relative unreal project time (TimeInWorld)
	 * @param
	 * @return
	 */
	void UpdateSUMOByMachineTime();

	/**
	 * TODO
	 * @param
	 * @return
	 */
	void UpdateFromSUMO();

	/**
	 * TODO
	 * @param
	 * @return
	 */
	bool DestroyVehicle();


	// Sets default values for this component's properties
	UVehiclePositionUpdateComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// BeginPlay()
	AActor* Owner = nullptr;

	//// SetVehicleIdAndClient()
	// TODO
	Client* client;
	// FrameRateSyn UnrealFRS;
	// TODO
	std::string VehicleId = "";


	// UpdateSUMOByMachineTime()
	// TODO
	float TimeInWorld = 0;

	// UpdateFromSUMO()
	// TODO
	FVector VehicleNewPosition = FVector(0, 0, 0);
	int32 MeterUnitConversion = 100;
	int ArrivedNumber = 0;
	std::vector<std::string> ArrivedList;

	FRotator VehicleNewRotator = FRotator(0, 0, 0);

};
