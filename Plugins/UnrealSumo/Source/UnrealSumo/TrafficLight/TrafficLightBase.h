// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrafficSignBase.h"
#include "TrafficLightState.h"
#include "TrafficLightBase.generated.h"

class USumoGameInstance;

UCLASS()
class UNREALSUMO_API ATrafficLightBase : public ATrafficSignBase
{
	GENERATED_BODY()


public:
    ATrafficLightBase(const FObjectInitializer &ObjectInitializer);


    /**
     * Override default BeginPlay function. Called when the game starts or when objects are spawned
     */
    virtual void BeginPlay() override;

   // void OnConstruction(const FTransform &Transform);

    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    void SwitchTrafficLightState();

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    void SetTrafficLightState(ETrafficLightState State);

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    ETrafficLightState GetTrafficLightState() const
    {
        return State;
    }

    // Getter
    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    float GetGreenTime() const;

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    float GetYellowTime() const;

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    float GetRedTime() const;

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    float GetElapsedTime() const;

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    bool GetTimeIsFrozen() const;


    // Setter
    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    void SetGreenTime(float InGreenTime);

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    void SetYellowTime(float InYellowTime);

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    void SetRedTime(float InRedTime);

    // used from replayer
    void SetElapsedTime(float InElapsedTime);

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    void SetTimeIsFrozen(bool InTimeIsFrozen);


    // Shared custom GameInstance class. Variables in SumoGameInstance are modified in SumoGameMode.
    USumoGameInstance* SumoGameInstance;

//    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
//    TArray<ATrafficLightBase *> GetGroupTrafficLights() const;
//
//    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
//    void SetGroupTrafficLights(TArray<ATrafficLightBase *> InGroupTrafficLights);

    void TrafficLightInitialization(FString InTrafficLightName, char InState,double RTick, double YTick, double GTick);


    // virtual void OnConstruction(const FTransform &Transform) override;
    // UFUNCTION(Category = "Traffic Light", BlueprintImplementableEvent)
    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    void OnTrafficLightStateChanged(ETrafficLightState TrafficLightState);


//    UFUNCTION(Category = "Traffic Light", BlueprintImplementableEvent)
//    void OnAmericanLightStateChanged(ETrafficLightState TrafficLightState);

private:
    // std::vector<std::pair<char,double>> FirstTrafficLightLogic;
    bool SynBySUMO = true;

    UPROPERTY(Category = "Traffic Light", EditAnywhere)
    ETrafficLightState State = ETrafficLightState::Red;


    UPROPERTY(Category = "Traffic Light", EditAnywhere)
    float GreenTime = 10.0f;

    UPROPERTY(Category = "Traffic Light", EditAnywhere)
    float YellowTime = 2.0f;


    UPROPERTY(Category = "Traffic Light", EditAnywhere)
    float RedTime = 7.0f;

    UPROPERTY(Category = "Traffic Light", VisibleAnywhere)
    float ElapsedTime = 0.0f;

    bool TimeIsFrozen = false;

    double GreenTick = 0;
    double YellowTick = 0;
    double RedTick = 0;

    FString TrafficLightID;
    double ElapsedTick = 0;

    UStaticMeshComponent* RedLight;
    UStaticMeshComponent* GreenLight;
    UStaticMeshComponent* YellowLight;

    void TickByMachineTime(float DeltaSeconds);

    void TickByCount();

    void SetTrafficLightMeshComponent();

    void SetTrafficLightMaterials(UStaticMeshComponent* OnTrafficLights,  const TCHAR * OnMaterialPath, TArray<UStaticMeshComponent*> OffStreetLights, const TCHAR * OffMaterialPath);

};
