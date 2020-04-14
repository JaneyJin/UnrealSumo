// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SumoPlayerController.generated.h"

class Client;

/**
 * 
 */
UCLASS()
class UNREALSUMO_API ASumoPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	ASumoPlayerController(const FObjectInitializer& ObjectInitializer);

	// virtual void Possess(APawn* InPawn) override;
	void SetSUMOSocket(Client* client);

	Client* client;
};
