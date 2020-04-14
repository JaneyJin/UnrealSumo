// Fill out your copyright notice in the Description page of Project Settings.


#include "SumoGameInstance.h"
#include "Client.h"

USumoGameInstance::USumoGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{   
	client = nullptr;
	EgoWheeledVehicleId = "";
}

void USumoGameInstance::SetSumoGameInstance(Client* client, FString EgoWheeledVehicleId, FrameRateSyn SUMOToUnrealFrameRate) {
	this->client = client;
	this->EgoWheeledVehicleId = EgoWheeledVehicleId;
	this->SUMOToUnrealFrameRate = SUMOToUnrealFrameRate;
	UE_LOG(LogTemp, Warning, TEXT("# of tick between SUMOUpdate: %d"), SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates)
}

