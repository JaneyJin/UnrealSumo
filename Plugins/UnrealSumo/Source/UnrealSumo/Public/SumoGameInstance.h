// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FrameRateSyn.h"
#include "SumoGameInstance.generated.h"

class Client;

UCLASS()
class UNREALSUMO_API USumoGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// UPROPERTY(BlueprintReadOnly, Category = "SUMO Shared Data") 
	Client* client;
	FString EgoWheeledVehicleId;
	FrameRateSyn SUMOToUnrealFrameRate;

	USumoGameInstance(const FObjectInitializer& ObjectInitializer);   /** Increment this value in any map, change map, and notice it persists! */
	void SetSumoGameInstance(Client* client, FString EgoWheeledVehicleId, FrameRateSyn SUMOToUnrealFrameRate);
	
};
