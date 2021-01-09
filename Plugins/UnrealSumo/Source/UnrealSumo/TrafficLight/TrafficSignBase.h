// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrafficSignBase.generated.h"

UENUM(BlueprintType)
enum class ETrafficSignState : uint8 {
  Null = 0,
  UNKNOWN            = 0u   UMETA(DisplayName = "UNKNOWN"),
  TrafficLightRed    = 1u   UMETA(DisplayName = "Traffic Light - Red"),
  TrafficLightYellow = 2u   UMETA(DisplayName = "Traffic Light - Yellow"),
  TrafficLightGreen  = 3u   UMETA(DisplayName = "Traffic Light - Green")

};


UCLASS()
class UNREALSUMO_API ATrafficSignBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	// ATrafficSignBase();
    ATrafficSignBase(const FObjectInitializer &ObjectInitializer);


//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;

private:
    UPROPERTY(Category = "Traffic Sign", EditAnywhere)
    ETrafficSignState TrafficSignState = ETrafficSignState::UNKNOWN;
	
};
