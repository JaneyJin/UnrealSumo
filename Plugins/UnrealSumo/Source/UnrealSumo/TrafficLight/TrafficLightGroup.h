// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrafficLightSubGroup.h"
#include "src/TraCIDefs.h"
#include <string>
#include <vector>
#include <tuple>
#include "TrafficLightGroup.generated.h"

class ATrafficLightBase;
class USumoGameInstance;
class ATrafficSignBase;

UCLASS()
class UNREALSUMO_API ATrafficLightGroup : public AActor
{
	GENERATED_BODY()

public:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

public:
    // Sets default values for this actor's properties
    ATrafficLightGroup();

    UPROPERTY(EditAnywhere, Category = "Traffic Light", meta = (ToolTip = ""))
    TArray<ATrafficSignBase*> TrafficLightReference;

    UPROPERTY(EditAnywhere, Category = "Traffic Light", meta = (ToolTip = ""))
    FString JunctionID;

private:
    // Shared custom GameInstance class. Variables in SumoGameInstance are modified in SumoGameMode.
    USumoGameInstance* SumoGameInstance;

    void ValidateJunctionID(FString ID);

		// Return the numbers of lanes in Group0, Group1, ... to GroupN order
    std::vector<int> GetLaneNumsforGroup(std::string ControlledLanesID);

    std::tuple<int, int, int> GetSubGroupIndex(const std::vector<std::vector<libsumo::TraCILink>>& ControlledLink, const int position, const int length);

    std::vector<FSubGroup> GetAmericanLightTime(const std::vector<libsumo::TraCIPhase*>& Phases, const std::tuple<int, int, int> subgroupIndex, int GroupNumber);

    FSubGroup GetSubGroupTime(const std::vector<libsumo::TraCIPhase*>& Phases, int GroupNumber, int index, char direction);
};
