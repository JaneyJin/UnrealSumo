// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrafficLightBase.h"
#include "Engine/StaticMesh.h"  // TODO: DELETE
#include "Engine/Texture2D.h"   // TODO: DELETE
#include "AmericanLight.generated.h"


class USumoGameInstance;

USTRUCT(BlueprintType, Category = "AmericanLights")
struct FTrafficLightPieces
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmericanLights")
    UStaticMesh* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmericanLights")
    FTransform RelativeTransform;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmericanLights")
    ETrafficLightState LightColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmericanLights")
    bool Masked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmericanLights")
    UTexture2D* Mask;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmericanLights")
    float Rotation;

    FTrafficLightPieces(){
        // static ConstructorHelpers::FObjectFinder<UStaticMesh> LightMesh(TEXT("StaticMesh'/UnrealSumo/TrafficLightSign/Static/SM_TLights_Leds.SM_TLights_Leds'"));
        // Mesh = LightMesh.Object;
        Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/UnrealSumo/TrafficLight/Static/SM_TLights_Leds.SM_TLights_Leds'"));
        LightColor = ETrafficLightState::Red;
        Masked = false;
        Mask = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/UnrealSumo/TrafficLight/Static/T_TrafficLightSign_op.T_TrafficLightSign_op'"));
        Rotation = 0.f;
        //UE_LOG(LogTemp, Warning, TEXT("%s, %s, %s"), *Mesh->GetName(), *UEnum::GetValueAsString(LightColor), *Mask->GetName())
    }

};

USTRUCT(blueprintable, Category = "AmericanLights")
struct FTrafficLightStruct
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmericanLights")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmericanLights")
    FTransform Position;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmericanLights")
    TArray<FTrafficLightPieces> Lights;

    FTrafficLightStruct(){
        FTrafficLightPieces TLElement;
        TLElement.LightColor = ETrafficLightState::Green;
        Lights.Add(TLElement);
        TLElement.LightColor = ETrafficLightState::Yellow;
        TLElement.RelativeTransform.SetLocation(FVector(0,0,28));
        Lights.Add(TLElement);
        TLElement.LightColor = ETrafficLightState::Red;
        TLElement.RelativeTransform.SetLocation(FVector(0,0,56));
        Lights.Add(TLElement);
    }
};


USTRUCT(blueprintable, Category = "AmericanLights")
struct FAmericanLightPiece
{
    GENERATED_BODY()

public:
    UPROPERTY(Category = "American Light", EditAnywhere)
    FString LightPieceDescription;

    UPROPERTY(Category = "American Light", EditAnywhere)
    TArray<double> GreenTime = {10.0f};
    UPROPERTY(Category = "American Light", EditAnywhere)
    TArray<double> YellowTime = {2.0f};
    UPROPERTY(Category = "American Light", EditAnywhere)
    TArray<double> RedTime = {7.0f};

    int32 GreenIndex = 0;
    int32 YellowIndex = 0;
    int32 RedIndex = 0;

    double ElapsedTime = 0.f;

    UPROPERTY(Category = "American Light", EditAnywhere)
    ETrafficLightState State = ETrafficLightState::Red;

    UPROPERTY(Category = "American Light", BlueprintReadOnly)
    TArray<UMaterialInstanceDynamic*> RedLightsMaterial;

    UPROPERTY(Category = "American Light", BlueprintReadOnly)
    TArray<UMaterialInstanceDynamic*> YellowLightsMaterial;

    UPROPERTY(Category = "American Light", BlueprintReadOnly)
    TArray<UMaterialInstanceDynamic*> GreenLightsMaterial;

};




USTRUCT(blueprintable, Category = "AmericanLights")
struct FCrossWalkPiece
{
    GENERATED_BODY()

public:
    UPROPERTY(Category = "American Light", EditAnywhere)
    bool ContainsCrossWalk = false;

    UPROPERTY(Category = "American Light", EditAnywhere)
    double WhiteTime = 12.0f;

    UPROPERTY(Category = "American Light", EditAnywhere)
    double RedTime = 7.0f;

    double ElapsedTime = 0.f;

    UPROPERTY(Category = "American Light", EditAnywhere)
    ECrossWalkState State = ECrossWalkState::Red;

    UPROPERTY(Category = "American Light", BlueprintReadOnly)
    UMaterialInstanceDynamic* CrossWalkMaterial;
};

UCLASS()
class UNREALSUMO_API AAmericanLight : public ATrafficSignBase
{
	GENERATED_BODY()

private:
    bool SynBySUMO = true;

	void SwitchAmericanLightPieceByMachineTime(float DeltaSeconds, FAmericanLightPiece& LightPiece);

    void SwitchAmericanLightPieceByTickCount(FAmericanLightPiece& LightPieceTick);

public:

    // Shared custom GameInstance class. Variables in SumoGameInstance are modified in SumoGameMode.
    USumoGameInstance* SumoGameInstance;

    virtual void BeginPlay() override;


    virtual void Tick(float DeltaSeconds) override;

    AAmericanLight(const FObjectInitializer &ObjectInitializer);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AmericanLightsDetails")
    TArray<FTrafficLightStruct> Heads;

    UPROPERTY(EditAnywhere, Category = "AmericanLightsDetails")
    FAmericanLightPiece LeftLightPiece;

    UPROPERTY(EditAnywhere, Category = "AmericanLightsDetails")
    FAmericanLightPiece CenterLightPiece;

    UPROPERTY(EditAnywhere, Category = "AmericanLightsDetails")
    FAmericanLightPiece RightLightPiece;

    UPROPERTY( EditAnywhere, Category = "AmericanLightsDetails")
    FCrossWalkPiece CrossWalk;


    void SetAmericanLightState(FAmericanLightPiece& LightPieceTick, const ETrafficLightState InState);


    void SetCrossWalkLightState(FCrossWalkPiece& CrossWalkPiece, const ECrossWalkState InState);

    void TickByCount();

    void SwitchAmericanLightState(FAmericanLightPiece& LightPieceTick);

    // UFUNCTION(Category = "American Light", BlueprintImplementableEvent)
    UFUNCTION(Category = "American Light", BlueprintCallable)
    void OnAmericanLightStateChanged(FAmericanLightPiece& LightPiece);

    UFUNCTION(Category = "American Light", BlueprintCallable)
    void SetLightMaterial(UMaterialInstanceDynamic* CurrentMaterial, ETrafficLightState InState, FString LightDescription);

    UFUNCTION(Category = "American Light", BlueprintCallable)
    void SetCrossWalkMaterial(UMaterialInstanceDynamic* CurrentMaterial);

    void AmericanLightInitialization();

private:
    void SwitchCrossWalkPieceByMachineTime(FCrossWalkPiece& CrossWalkPiece, float DeltaSeconds);

    void SwitchCrossWalkPieceByTickCount(FCrossWalkPiece& CrossWalkPiece);

    void SwitchCrossWalkLightState(FCrossWalkPiece& CrossWalk);

    void SetLightsMaterialOnAndOff(TArray<UMaterialInstanceDynamic*>& LightsMaterial,float Value);

    bool MaterialIsNotValid(FAmericanLightPiece LightPiece);

    void ConstructorScript();

    void TickByMachineTime(float DeltaSeconds);
};
