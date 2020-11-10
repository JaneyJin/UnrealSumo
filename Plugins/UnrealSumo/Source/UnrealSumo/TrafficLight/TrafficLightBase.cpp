// Fill out your copyright notice in the Description page of Project Settings.


#include "TrafficLightBase.h"
#include "UnrealSumo/World/SumoGameMode.h"
#include "UnrealSumo/World/SumoGameInstance.h"
#include "TrafficLightGroup.h"
#include "Engine/World.h"
#include "UnrealSumo/src/TraCIDefs.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceConstant.h"


ATrafficLightBase::ATrafficLightBase(const FObjectInitializer &ObjectInitializer)
        : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;
}

void ATrafficLightBase::BeginPlay() {
    Super::BeginPlay();

    auto GameMode = GetWorld()->GetAuthGameMode();
    if(!GameMode->GetName().Contains("SumoGameMode")){
        SynBySUMO = false;
        return;
    }

    ASumoGameMode* SumoGameMode = Cast<ASumoGameMode>(GameMode);
    if(!GameMode->HasActorBegunPlay()){
        GameMode->DispatchBeginPlay();
    }

    SetTrafficLightMeshComponent();

    if(!SumoGameMode->SynBySUMOTrafficLight()){
        SynBySUMO = false;
        return;
    }
    SumoGameInstance = Cast<USumoGameInstance>(GetGameInstance());

}

//void ATrafficLightBase::OnConstruction(const FTransform &Transform)
//{
//    Super::OnConstruction(Transform);
//    SetTrafficLightMeshComponent();
//    SetTrafficLightState(State);
//}

void ATrafficLightBase::Tick(float DeltaSeconds)
{

    if (!SynBySUMO) {
        TickByMachineTime(DeltaSeconds);
        return;
    }

    if (SumoGameInstance == nullptr || SumoGameInstance->SUMOToUnrealFrameRate.UnrealTickSlower) {
        return;
    }

    if(SumoGameInstance->SUMOToUnrealFrameRate.TickCount == SumoGameInstance->SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates){
        TickByCount();
    }
}


void ATrafficLightBase::TickByCount(){
    ElapsedTick = ElapsedTick + 1;

    float ChangeTick;

    switch (State)
    {
        case ETrafficLightState::Red:
            ChangeTick = RedTick;
            break;
        case ETrafficLightState::Yellow:
            ChangeTick = YellowTick;
            break;
        case ETrafficLightState::Green:
            ChangeTick = GreenTick;
            break;
        default:
            UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
            SetTrafficLightState(ETrafficLightState::Red);
            return;
    }

    if(ElapsedTick > ChangeTick){
        SwitchTrafficLightState();
        ElapsedTick = 0;
    }
}

void ATrafficLightBase::TickByMachineTime(float DeltaSeconds) {
    // if(TimeIsFrozen){  return;  }

    ElapsedTime += DeltaSeconds;

    float ChangeTime;

    switch (State)
    {
        case ETrafficLightState::Red:
            ChangeTime = RedTime;
            break;
        case ETrafficLightState::Yellow:
            ChangeTime = YellowTime;
            break;
        case ETrafficLightState::Green:
            ChangeTime = GreenTime;
            break;
        default:
            UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
            SetTrafficLightState(ETrafficLightState::Red);
            return;
    }

    if (ElapsedTime > ChangeTime)
    {
        SwitchTrafficLightState();
        ElapsedTime = 0.0f;
    }

}
void ATrafficLightBase::SwitchTrafficLightState()
{
    switch (State)
    {
        case ETrafficLightState::Red:
            SetTrafficLightState(ETrafficLightState::Green);
            break;
        case ETrafficLightState::Yellow:
            SetTrafficLightState(ETrafficLightState::Red);
            break;
        case ETrafficLightState::Green:
            SetTrafficLightState(ETrafficLightState::Yellow);
            break;
        default:
        UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
            SetTrafficLightState(ETrafficLightState::Red);
            break;
    }
}

void ATrafficLightBase::SetTrafficLightState(const ETrafficLightState InState) {
    FString NextState = UEnum::GetValueAsString(InState);
    FString CurrentState = UEnum::GetValueAsString(State);
    UE_LOG(LogTemp, Warning, TEXT("%s -> Time: %f; Current State: %s; Next State: %s"), *GetName(),ElapsedTime, *CurrentState, *NextState);
    State = InState;

    OnTrafficLightStateChanged(State);
    //OnAmericanLightStateChanged(State);
    //TODO
    // SetTrafficSignState(ToTrafficSignState(State));
    // VehicleController
}

// Change TrafficSignBase
//static ETrafficSignState ToTrafficSignState(ETrafficLightState State)
//{
//    switch (State)
//    {
//        case ETrafficLightState::Green:
//            return ETrafficSignState::TrafficLightGreen;
//        case ETrafficLightState::Yellow:
//            return ETrafficSignState::TrafficLightYellow;
//        default:
//        case ETrafficLightState::Red:
//            return ETrafficSignState::TrafficLightRed;
//    }
//}

void ATrafficLightBase::SetTrafficLightMeshComponent(){
    TArray<UStaticMeshComponent*> Components;
    this->GetComponents<UStaticMeshComponent>(Components);

    for(int32 i=0; i < Components.Num(); i++) //Count is zero
    {
        UStaticMeshComponent* StaticMeshComponent = Components[i];
        FString StaticMeshName = StaticMeshComponent->GetName();
        if(StaticMeshName.Contains("Red")){
            RedLight = StaticMeshComponent;
        }else if(StaticMeshName.Contains("Yellow")){
            YellowLight = StaticMeshComponent;
        }else if(StaticMeshName.Contains("Green")){
            GreenLight = StaticMeshComponent;
        }
    }
}

void ATrafficLightBase::OnTrafficLightStateChanged(ETrafficLightState TrafficLightState) {
    TArray<UStaticMeshComponent*> OffStreetLights;
    FString OnMaterial;
    FString OffMaterial = TEXT("MaterialInstanceConstant'/UnrealSumo/TrafficLight/Static/MI_StreetLightsOff.MI_StreetLightsOff'");
    switch (TrafficLightState)
    {
        case ETrafficLightState::Red:
            OffStreetLights.Add(YellowLight);
            OffStreetLights.Add(GreenLight);
            OnMaterial = TEXT("MaterialInstanceConstant'/UnrealSumo/TrafficLight/Static/MI_Streetlights_Red.MI_Streetlights_Red'");
            SetTrafficLightMaterials(RedLight, *OnMaterial, OffStreetLights, *OffMaterial);
            break;
        case ETrafficLightState::Yellow:
            OffStreetLights.Add(RedLight);
            OffStreetLights.Add(GreenLight);
            OnMaterial = TEXT("MaterialInstanceConstant'/UnrealSumo/TrafficLight/Static/MI_Streetlights_Yellow.MI_Streetlights_Yellow'");
            SetTrafficLightMaterials(YellowLight, *OnMaterial, OffStreetLights, *OffMaterial);
            break;
        case ETrafficLightState::Green:
            OffStreetLights.Add(RedLight);
            OffStreetLights.Add(YellowLight);
            OnMaterial = TEXT("MaterialInstanceConstant'/UnrealSumo/TrafficLight/Static/MI_Streetlights_Green.MI_Streetlights_Green'");
            SetTrafficLightMaterials(GreenLight, *OnMaterial, OffStreetLights, *OffMaterial);
            break;
        default:
        UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
            SetTrafficLightState(ETrafficLightState::Red);
            return;
    }
}

void ATrafficLightBase::SetTrafficLightMaterials(UStaticMeshComponent* OnTrafficLights, const TCHAR * OnMaterialPath,
        TArray<UStaticMeshComponent*> OffStreetLights, const TCHAR * OffMaterialPath){

    UMaterialInstanceConstant* FoundMaterial;
    if(OnTrafficLights){
        FoundMaterial = LoadObject<UMaterialInstanceConstant>(nullptr, OnMaterialPath);
        if (FoundMaterial){
            OnTrafficLights->SetMaterial(1,FoundMaterial);
        }
    }

    for(int32 i = 0; i < OffStreetLights.Num(); i++){
        FoundMaterial = LoadObject<UMaterialInstanceConstant>(nullptr, OffMaterialPath);
        if(OffStreetLights[i] && FoundMaterial) {
            OffStreetLights[i]->SetMaterial(1, FoundMaterial);
        }
    }
}


// Getter
float ATrafficLightBase::GetGreenTime() const
{
    return GreenTime;
}


float ATrafficLightBase::GetYellowTime() const
{
    return YellowTime;
}

float ATrafficLightBase::GetRedTime() const
{
    return RedTime;
}

float ATrafficLightBase::GetElapsedTime() const
{
    return ElapsedTime;
}

bool ATrafficLightBase::GetTimeIsFrozen() const
{
    return TimeIsFrozen;
}

// Setter
void ATrafficLightBase::SetGreenTime(float InGreenTime)
{
    GreenTime = InGreenTime;
}

void ATrafficLightBase::SetYellowTime(float InYellowTime)
{
    YellowTime = InYellowTime;
}

void ATrafficLightBase::SetRedTime(float InRedTime)
{
    RedTime = InRedTime;
}


void ATrafficLightBase::SetElapsedTime(float InElapsedTime)
{
    ElapsedTime = InElapsedTime;
}

void ATrafficLightBase::SetTimeIsFrozen(bool InTimeIsFrozen)
{
    TimeIsFrozen = InTimeIsFrozen;
    if (!TimeIsFrozen)
    {
        ElapsedTime = 0.0f;
    }
}


void ATrafficLightBase::TrafficLightInitialization(FString InTrafficLightName, char InState,double RTick, double YTick, double GTick){

    if(InState == 'r' ){
        SetTrafficLightState(ETrafficLightState::Red);
        // UE_LOG(LogTemp, Warning, TEXT("Red"))
    }else if(InState == 'y'){
        SetTrafficLightState(ETrafficLightState::Yellow);
        // UE_LOG(LogTemp, Warning, TEXT("Yellow"))
    }else if(InState == 'g'){
        SetTrafficLightState(ETrafficLightState::Green);
        // UE_LOG(LogTemp, Warning, TEXT("Green"))
    }
    this->TrafficLightID = InTrafficLightName;
    this->RedTick = RTick;
    this->YellowTick = YTick;
    this->GreenTick = GTick;

    UE_LOG(LogTemp, Display, TEXT("%s, %f, %f, %f"),*InTrafficLightName, RTick, YTick, GTick)
}