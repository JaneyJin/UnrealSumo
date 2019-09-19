// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <string>
#include "VehicleInformation.h"
#include "Components/ActorComponent.h"
#include "VehiclePoisitonUpdateComponent.generated.h"

class Client;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALSUMO_API UVehiclePoisitonUpdateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UVehiclePoisitonUpdateComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	/**
	 * TODO
	 * @param
	 * @return
	 */
	bool SetVehicleIdAndClient(FString VehicleIdToSet, Client* ClientToSet, float UpdateDeltaT, float NextTimeToUpdate);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	AActor* Owner = nullptr;
	
	// Instantiate VehicleInformation Structure class to store Vehicle information get from SUMO and update these data in Unreal
	FVehicleInformation UnrealVehicleInformation;

	// TODO
	Client* client;

	// TODO
	float UpdateDeltaT = 0;
	float NextTimeToUpdate = 0;

	// TODO
	std::string VehicleId = "";
};
