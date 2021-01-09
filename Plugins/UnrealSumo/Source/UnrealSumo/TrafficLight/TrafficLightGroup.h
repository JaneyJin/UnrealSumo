// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <string>
#include "TrafficLightGroup.generated.h"

class ATrafficLightBase;
class USumoGameInstance;
class ATrafficSignBase;

UCLASS()
class UNREALSUMO_API ATrafficLightGroup : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
    // Sets default values for this actor's properties
    ATrafficLightGroup();

	// Called every frame
	// virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Traffic Light", meta = (ToolTip = ""))
    TArray<ATrafficSignBase*> TrafficLightReference;

    UPROPERTY(EditAnywhere, Category = "Traffic Light", meta = (ToolTip = ""))
    FString JunctionID;
		
private:
    char ExtractLightState(std::string TL_State, int TL_Group);

    std::vector<int> GetLaneNumsforGroup(std::string ControlledLanesID);

    // Shared custom GameInstance class. Variables in SumoGameInstance are modified in SumoGameMode.
    USumoGameInstance* SumoGameInstance;

    void ValidateJunctionID(FString ID);
};
