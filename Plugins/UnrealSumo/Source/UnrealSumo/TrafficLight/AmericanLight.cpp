// Fill out your copyright notice in the Description page of Project Settings.


#include "AmericanLight.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UnrealSumo/World/SumoGameMode.h"
#include "UnrealSumo/World/SumoGameInstance.h"

AAmericanLight::AAmericanLight(const FObjectInitializer &ObjectInitializer)
        : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;
    LeftLightPiece.LightPieceDescription = "LeftPiece";
    CenterLightPiece.LightPieceDescription = "CenterPiece";
    RightLightPiece.LightPieceDescription = "RightPiece";
}


void AAmericanLight::BeginPlay() {
    Super::BeginPlay();

    if(MaterialIsNotValid(LeftLightPiece) && MaterialIsNotValid(CenterLightPiece) && MaterialIsNotValid(RightLightPiece)){
        UE_LOG(LogTemp, Error, TEXT("Failed to load light materials"))
    }

    if(CrossWalk.CrossWalkMaterial == nullptr){
        UE_LOG(LogTemp, Error, TEXT("Failed to load cross walk light materials"))
    }

    auto GameMode = GetWorld()->GetAuthGameMode();
    if(!GameMode->GetName().Contains("SumoGameMode")){
        SynBySUMO = false;
        return;
    }

    ASumoGameMode* SumoGameMode = Cast<ASumoGameMode>(GameMode);

    if(!GameMode->HasActorBegunPlay()){
        GameMode->DispatchBeginPlay();
    }

    if(!SumoGameMode->SynBySUMOTrafficLight()){
        SynBySUMO = false;
        return;
    }
    SumoGameInstance = Cast<USumoGameInstance>(GetGameInstance());
}

bool AAmericanLight::MaterialIsNotValid(FAmericanLightPiece LightPiece){
    if(LightPiece.GreenLightsMaterial.Num() < 1 || LightPiece.YellowLightsMaterial.Num() < 1 || LightPiece.RedLightsMaterial.Num() < 1){
        return true;
    }
    return false;
}


void AAmericanLight::Tick(float DeltaSeconds)
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

void AAmericanLight::TickByMachineTime(float DeltaSeconds) {
    SwitchAmericanLightPieceByMachineTime(DeltaSeconds, LeftLightPiece);
    SwitchAmericanLightPieceByMachineTime(DeltaSeconds, CenterLightPiece);
    SwitchAmericanLightPieceByMachineTime(DeltaSeconds, RightLightPiece);

    if(CrossWalk.ContainsCrossWalk){
        SwitchCrossWalkPieceByMachineTime(CrossWalk, DeltaSeconds);
    }
}

void AAmericanLight::SwitchAmericanLightPieceByMachineTime(float DeltaSeconds, FAmericanLightPiece& LightPiece){
    LightPiece.ElapsedTime += DeltaSeconds;

    double ChangeTime;
    switch (LightPiece.State)
    {
        case ETrafficLightState::Red:
            ChangeTime = LightPiece.RedTime[LightPiece.RedIndex];
            break;
        case ETrafficLightState::Yellow:
            ChangeTime = LightPiece.YellowTime[LightPiece.YellowIndex];
            break;
        case ETrafficLightState::Green:
            ChangeTime = LightPiece.GreenTime[LightPiece.GreenIndex];
            break;
        default:
        UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
            SetAmericanLightState(LightPiece, ETrafficLightState::Red);
            return;
    }

    if (LightPiece.ElapsedTime > ChangeTime)
    {
        SwitchAmericanLightState(LightPiece);
        LightPiece.ElapsedTime = 0.0f;
    }


}


void AAmericanLight::TickByCount(){

    SwitchAmericanLightPieceByTickCount(LeftLightPiece);
    SwitchAmericanLightPieceByTickCount(CenterLightPiece);
    SwitchAmericanLightPieceByTickCount(RightLightPiece);

    if(CrossWalk.ContainsCrossWalk){
        SwitchCrossWalkPieceByTickCount(CrossWalk);
    }
}


void AAmericanLight::SwitchCrossWalkPieceByMachineTime(FCrossWalkPiece& CrossWalkPiece, float DeltaSeconds){
    CrossWalkPiece.ElapsedTime += DeltaSeconds;
    double ChangeTime;
    switch (CrossWalk.State) {
        case ECrossWalkState::Red:
            ChangeTime = CrossWalkPiece.RedTime;
            break;
        case ECrossWalkState::White:
            ChangeTime = CrossWalkPiece.WhiteTime;
            break;
        default:
        UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
            SetCrossWalkLightState(CrossWalkPiece, ECrossWalkState::Red);
            return;
    }

    if(CrossWalk.ElapsedTime > ChangeTime){
        SwitchCrossWalkLightState(CrossWalk);
        CrossWalk.ElapsedTime = 0.0f;
    }
}


void AAmericanLight::SwitchAmericanLightPieceByTickCount(FAmericanLightPiece& LightPiece){

    LightPiece.ElapsedTime = LightPiece.ElapsedTime + 1;

    float ChangeTick;
    switch (LightPiece.State) {
        case ETrafficLightState::Red:
            ChangeTick = LightPiece.RedTime[LightPiece.RedIndex];
            break;
        case ETrafficLightState::Yellow:
            ChangeTick = LightPiece.YellowTime[LightPiece.YellowIndex];
            break;
        case ETrafficLightState::Green:
            ChangeTick = LightPiece.GreenTime[LightPiece.GreenIndex];
            break;
        default:
            UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
            SetAmericanLightState(LightPiece, ETrafficLightState::Red);
            return;
    }

    if(LightPiece.ElapsedTime > ChangeTick){
        SwitchAmericanLightState(LightPiece);
        LightPiece.ElapsedTime = 0.0f;
    }

}

void AAmericanLight::SwitchCrossWalkPieceByTickCount(FCrossWalkPiece& CrossWalkPiece){
    CrossWalkPiece.ElapsedTime = CrossWalkPiece.ElapsedTime + 1;
    float ChangeTick;
    switch (CrossWalk.State) {
        case ECrossWalkState::Red:
            ChangeTick = CrossWalkPiece.RedTime;
            break;
        case ECrossWalkState::White:
            ChangeTick = CrossWalkPiece.WhiteTime;
            break;
        default:
            UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
            SetCrossWalkLightState(CrossWalkPiece, ECrossWalkState::Red);
            return;
    }



    if(CrossWalk.ElapsedTime > ChangeTick){
        SwitchCrossWalkLightState(CrossWalk);
        CrossWalk.ElapsedTime = 0.0f;
    }

}

void AAmericanLight::SetCrossWalkLightState(FCrossWalkPiece& CrossWalkPiece, const ECrossWalkState InState){
    FString NextState = UEnum::GetValueAsString(InState);
    FString CurrentState = UEnum::GetValueAsString(CrossWalkPiece.State);
    UE_LOG(LogTemp, Error, TEXT("CrossWalk -> Tick: %f; Current State: %s; Next State: %s"),CrossWalkPiece.ElapsedTime, *CurrentState, *NextState);

    CrossWalkPiece.State = InState;

    // OnCrossWalkLightStateChange
    switch(CrossWalkPiece.State)
    {
        case ECrossWalkState::Red:
            if(CrossWalkPiece.CrossWalkMaterial){
                CrossWalkPiece.CrossWalkMaterial->SetScalarParameterValue("Emissive_White", 0);
                CrossWalkPiece.CrossWalkMaterial->SetScalarParameterValue("Emissive_Red", 1);
            }
            break;
        case ECrossWalkState::White:
            if(CrossWalkPiece.CrossWalkMaterial){
                CrossWalkPiece.CrossWalkMaterial->SetScalarParameterValue("Emissive_White", 1);
                CrossWalkPiece.CrossWalkMaterial->SetScalarParameterValue("Emissive_Red", 0);
            }
            break;
        default:
            UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
            break;
    }

}

void AAmericanLight::SwitchCrossWalkLightState(FCrossWalkPiece& CrossWalkPiece){
    switch (CrossWalkPiece.State)
    {
        case ECrossWalkState::Red:
            SetCrossWalkLightState(CrossWalkPiece, ECrossWalkState::White);
            break;
        case ECrossWalkState::White:
            SetCrossWalkLightState(CrossWalkPiece, ECrossWalkState::Red);
            break;
        default:
            UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
            SetCrossWalkLightState(CrossWalkPiece, ECrossWalkState::Red);
            break;
    }
}


void AAmericanLight::SwitchAmericanLightState(FAmericanLightPiece& LightPiece)
{
    switch (LightPiece.State)
    {
        case ETrafficLightState::Red:
            SetAmericanLightState(LightPiece, ETrafficLightState::Green);
            break;
        case ETrafficLightState::Yellow:
            SetAmericanLightState(LightPiece, ETrafficLightState::Red);
            break;
        case ETrafficLightState::Green:
            SetAmericanLightState(LightPiece, ETrafficLightState::Yellow);
            break;
        default:
            UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
            SetAmericanLightState(LightPiece, ETrafficLightState::Red);
            break;
    }
}

void AAmericanLight::SetAmericanLightState(FAmericanLightPiece& LightPiece, const ETrafficLightState InState){

    FString NextState = UEnum::GetValueAsString(InState);
    FString CurrentState = UEnum::GetValueAsString(LightPiece.State);
    UE_LOG(LogTemp, Warning, TEXT("%s -> Tick: %f; Current State: %s; Next State: %s"), *LightPiece.LightPieceDescription,LightPiece.ElapsedTime, *CurrentState, *NextState);

    // Switch light index to next stage so that traffic light switch time is different for each loop
    switch (LightPiece.State)
    {
        case ETrafficLightState::Red:
            if(LightPiece.RedTime.Num() > 1){
                if(LightPiece.RedIndex == LightPiece.RedTime.Num() - 1){
                    LightPiece.RedIndex = 0;
                }else{
                    LightPiece.RedIndex += 1;
                }
            }
            break;
        case ETrafficLightState::Yellow:
            if(LightPiece.YellowTime.Num() > 1){
                if(LightPiece.YellowIndex == LightPiece.YellowTime.Num() - 1){
                    LightPiece.YellowIndex = 0;
                }else{
                    LightPiece.YellowIndex += 1;
                }
            }
            break;
        case ETrafficLightState::Green:
            if(LightPiece.GreenTime.Num() > 1){
                if(LightPiece.GreenIndex == LightPiece.GreenTime.Num() - 1){
                    LightPiece.GreenIndex = 0;
                }else{
                    LightPiece.GreenIndex += 1;
                }
            }
            break;
        default:
            UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
            break;
    }

    LightPiece.State = InState;
    OnAmericanLightStateChanged(LightPiece);
}


void AAmericanLight::OnAmericanLightStateChanged(FAmericanLightPiece& LightPiece){
    switch(LightPiece.State)
    {
        case ETrafficLightState::Red:
            SetLightsMaterialOnAndOff(LightPiece.RedLightsMaterial, 1);
            SetLightsMaterialOnAndOff(LightPiece.YellowLightsMaterial, 0);
            SetLightsMaterialOnAndOff(LightPiece.GreenLightsMaterial, 0);
            break;
        case ETrafficLightState::Yellow:
            SetLightsMaterialOnAndOff(LightPiece.RedLightsMaterial, 0);
            SetLightsMaterialOnAndOff(LightPiece.YellowLightsMaterial, 1);
            SetLightsMaterialOnAndOff(LightPiece.GreenLightsMaterial, 0);
            break;
        case ETrafficLightState::Green:
            SetLightsMaterialOnAndOff(LightPiece.RedLightsMaterial, 0);
            SetLightsMaterialOnAndOff(LightPiece.YellowLightsMaterial, 0);
            SetLightsMaterialOnAndOff(LightPiece.GreenLightsMaterial, 1);
            break;
        default:
            UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
            break;
    }
}


void AAmericanLight::SetLightsMaterialOnAndOff(TArray<UMaterialInstanceDynamic*>& LightsMaterial,float Value){
    for(int i = 0; i < LightsMaterial.Num(); i++){
        if( LightsMaterial[i]){
            // float OutValue;
            // LightsMaterial[i]->GetScalarParameterValue(FName(TEXT("On/Off")),OutValue);
            LightsMaterial[i]->SetScalarParameterValue("On/Off",Value);
        }
    }
}

void AAmericanLight::SetLightMaterial(UMaterialInstanceDynamic* CurrentMaterial, ETrafficLightState InState, FString LightDescription){
    // UE_LOG(LogTemp, Error, TEXT("%s; %s; %s"), CurrentMaterial, *(UEnum::GetValueAsString(InState)), *LightDescription);

    switch(InState){
        case ETrafficLightState::Red:
            if(LightDescription.Contains("Left")){
                LeftLightPiece.RedLightsMaterial.Add(CurrentMaterial);
            }else if(LightDescription.Contains("Center")){
                CenterLightPiece.RedLightsMaterial.Add(CurrentMaterial);
            }else if(LightDescription.Contains("Right")){
                RightLightPiece.RedLightsMaterial.Add(CurrentMaterial);
            }
            break;
        case ETrafficLightState::Yellow:
            if(LightDescription.Contains("Left")){
                LeftLightPiece.YellowLightsMaterial.Add(CurrentMaterial);
            }else if(LightDescription.Contains("Center")){
                CenterLightPiece.YellowLightsMaterial.Add(CurrentMaterial);
            }else if(LightDescription.Contains("Right")){
                RightLightPiece.YellowLightsMaterial.Add(CurrentMaterial);
            }
            break;
        case ETrafficLightState::Green:
            if(LightDescription.Contains("Left")){
                LeftLightPiece.GreenLightsMaterial.Add(CurrentMaterial);
            }else if(LightDescription.Contains("Center")){
                CenterLightPiece.GreenLightsMaterial.Add(CurrentMaterial);
            }else if(LightDescription.Contains("Right")){
                RightLightPiece.GreenLightsMaterial.Add(CurrentMaterial);
            }
            break;
        default:
            UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
            break;
    }

}

void AAmericanLight::SetCrossWalkMaterial(UMaterialInstanceDynamic* CurrentMaterial){
    CrossWalk.CrossWalkMaterial = CurrentMaterial;
}

void AAmericanLight::AmericanLightInitialization(){
    SetAmericanLightState(LeftLightPiece,ETrafficLightState::Red);
    SetAmericanLightState(CenterLightPiece,ETrafficLightState::Red);
    SetAmericanLightState(RightLightPiece,ETrafficLightState::Red);
    LeftLightPiece.RedTime={330.0, 240.0};
    LeftLightPiece.GreenTime = {300.0};
    LeftLightPiece.YellowTime = {30.0};


    RightLightPiece.RedTime = CenterLightPiece.RedTime = {330.0};
    RightLightPiece.GreenTime = CenterLightPiece.GreenTime = {210.0};
    RightLightPiece.YellowTime = CenterLightPiece.YellowTime = {30.0};
    UE_LOG(LogTemp, Error, TEXT("AmericanLightInitialization"));
}


// NOT USE
void AAmericanLight::ConstructorScript(){
    UMaterialInstanceDynamic* NewMaterial;
    UMaterialInterface* DynMaterial;


    FTrafficLightStruct TLStruct;
    TLStruct.Description = "CenterPiece";
    TLStruct.Position.SetLocation(FVector(-800,0,640));
    Heads.Add(TLStruct);

    TLStruct.Description = "RightPiece";
    TLStruct.Position.SetLocation(FVector(-500,0,640));
    Heads.Add(TLStruct);

    TLStruct.Description = "LeftPiece";
    TLStruct.Position.SetLocation(FVector(-1150,0,640));
    for(int i = 0; i < TLStruct.Lights.Num(); i++){
        TLStruct.Lights[i].Masked = true;
        TLStruct.Lights[i].Rotation = 180.f;
    }
    Heads.Add(TLStruct);


    for(int i = 0; i < Heads.Num(); i++){
        FTrafficLightStruct head = Heads[i];
        UStaticMeshComponent* BotCoverMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>( *(Heads[i].Description.Left(Heads[i].Description.Find("Piece")) + "RoadSign_BotCover"));
        UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/UnrealSumo/TrafficLight/Static/SM_TL_BotCover.SM_TL_BotCover'"));;

        BotCoverMeshComponent->SetupAttachment(RootComponent);
        BotCoverMeshComponent->SetRelativeTransform(head.Position);
        // UE_LOG(LogTemp, Warning, TEXT("Position %s"), *head.Position.GetLocation().ToString())
        BotCoverMeshComponent->SetStaticMesh(Mesh);

        TArray<FTrafficLightPieces> lights = head.Lights;
        for(int j = 0; j < lights.Num(); j++){
            FTransform transform = lights[j].RelativeTransform;
            UStaticMesh* mesh = lights[j].Mesh;

            FString lightcolor = UEnum::GetValueAsString(lights[j].LightColor);

            int32 CharacterIndex = lightcolor.FindLastChar(':',CharacterIndex);
            UStaticMeshComponent* LightMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*(Heads[i].Description.Left(Heads[i].Description.Find("Piece")) + lightcolor.RightChop(CharacterIndex + 1) + "Light"));
            LightMeshComponent->SetupAttachment(BotCoverMeshComponent);
            LightMeshComponent->SetStaticMesh(mesh);
            LightMeshComponent->SetRelativeTransform(lights[j].RelativeTransform);

            if(lights[j].Masked){
                DynMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("MaterialInstanceConstant'/UnrealSumo/TrafficLight/Static/M_StreetlightsMasked.M_StreetlightsMasked'"));
            }else{
                DynMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/UnrealSumo/TrafficLight/Static/M_StreetlightsMaster.M_StreetlightsMaster'"));
            }

            // UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
            NewMaterial = UMaterialInstanceDynamic::Create(DynMaterial, NULL);

            NewMaterial->SetTextureParameterValue(FName(TEXT("LightMask")),lights[j].Mask);
            NewMaterial->SetScalarParameterValue("Angle",lights[j].Rotation);

            switch(lights[j].LightColor){
                case ETrafficLightState::Red:
                    NewMaterial->SetVectorParameterValue("LightColor",FLinearColor::Red);
                    if(head.Description.Contains("Left")){
                        LeftLightPiece.RedLightsMaterial.Add(NewMaterial);
                    }else if(head.Description.Contains("Center")){
                        CenterLightPiece.RedLightsMaterial.Add(NewMaterial);
                    }else if(head.Description.Contains("Right")){
                        RightLightPiece.RedLightsMaterial.Add(NewMaterial);
                    }
                    break;
                case ETrafficLightState::Yellow:
                    NewMaterial->SetVectorParameterValue("LightColor",FLinearColor::Yellow);
                    if(head.Description.Contains("Left")){
                        LeftLightPiece.YellowLightsMaterial.Add(NewMaterial);
                    }else if(head.Description.Contains("Center")){
                        CenterLightPiece.YellowLightsMaterial.Add(NewMaterial);
                    }else if(head.Description.Contains("Right")){
                        RightLightPiece.YellowLightsMaterial.Add(NewMaterial);
                    }
                    break;
                case ETrafficLightState::Green:
                    NewMaterial->SetVectorParameterValue("LightColor",FLinearColor::Green);
                    if(head.Description.Contains("Left")){
                        LeftLightPiece.GreenLightsMaterial.Add(NewMaterial);
                    }else if(head.Description.Contains("Center")){
                        CenterLightPiece.GreenLightsMaterial.Add(NewMaterial);
                    }else if(head.Description.Contains("Right")){
                        RightLightPiece.GreenLightsMaterial.Add(NewMaterial);
                    }
                    break;
                default:
                UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
                    break;
            }

            LightMeshComponent->SetMaterial(1,NewMaterial);

        }

    }

}
