// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FrameRateSyn.h"
#include <vector> 
#include "SumoGameInstance.generated.h"


class Client;

UCLASS()
class UNREALSUMO_API USumoGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// Connected socket for Unreal. It is internal endpoint for sending or receiving data within Unreal.
	Client* client;

	// Wheeled vehicle controlled by user or AI controller. 
	FString EgoWheeledVehicleId;

	// Frame rate of SUMO and Unreal. Shared between GameMode, SumoWheeledVehicle and WheeledVehiclePawn to algin timestep btw SUMO and Unreal.
	FrameRateSyn SUMOToUnrealFrameRate;
	
	// Record arrived vehicles (vehicles left the network) in SUMO. Used by SumoWheeledVehicle to destroy wheeld vehicle in Unreal.
	int ArrivedNumber;
	std::vector<std::string> ArrivedList;

	/**
	 * Initialize unreal socket and ego wheeled vehicle identity.
	 *
	 * @param Internal class to finalize [UObject] creation (initialize properties) after the real C++.
	 */
	USumoGameInstance(const FObjectInitializer& ObjectInitializer);   

	/**
	 * Update unreal socket, ego wheeled vehicle identity and arrived vehicle list in SumoGameMode. 
	 *
	 * @param  Reference to the endpoint for SUMO and Unreal commuinciation. 
	 * @param  Ego wheeled vehicle identity in Unreal and SUMO.
	 */
	void SetSumoGameInstance(Client* client, FString EgoWheeledVehicleId);

	
	/**
	 * Called when close the connection between Unreal and SUMO. Close the socket and 
	 */
	void CloseSocket();
};