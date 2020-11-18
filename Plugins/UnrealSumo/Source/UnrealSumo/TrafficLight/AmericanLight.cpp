// Fill out your copyright notice in the Description page of Project Settings.


#include "AmericanLight.h"
#include "Materials/MaterialInstanceDynamic.h"
AAmericanLight::AAmericanLight(const FObjectInitializer &ObjectInitializer)
        : Super(ObjectInitializer)
{
}


void AAmericanLight::BeginPlay() {
    Super::BeginPlay();

    if(MaterialIsNotValid(LeftLightPiece) && MaterialIsNotValid(CenterLightPiece) && MaterialIsNotValid(RightLightPiece)){
        
    }
}

bool AAmericanLight::MaterialIsNotValid(FAmericanLightPiece LightPiece){
    if(LightPiece.GreenLightsMaterial.Num() < 1 || LightPiece.YellowLightsMaterial.Num() < 1 || LightPiece.RedLightsMaterial.Num() < 1){
        return true;
    }
    return false;
}


void AAmericanLight::Tick(float DeltaSeconds)
{
    count++;
    if(count == 10){
        TickByCount();
        count = 0;
    }
}

void AAmericanLight::TickByCount(){
    LeftLightPiece.ElapsedTick = LeftLightPiece.ElapsedTick + 1;
    CenterLightPiece.ElapsedTick = CenterLightPiece.ElapsedTick + 1;
    RightLightPiece.ElapsedTick = RightLightPiece.ElapsedTick + 1;

    SwitchAmericanLightPiece(LeftLightPiece);
    SwitchAmericanLightPiece(CenterLightPiece);
   SwitchAmericanLightPiece(RightLightPiece);
}

void AAmericanLight::SwitchAmericanLightPiece(FAmericanLightPiece& LightPiece){
    float ChangeTick;
    switch (LightPiece.State) {
        case ETrafficLightState::Red:
            ChangeTick = LightPiece.RedTick;
            break;
        case ETrafficLightState::Yellow:
            ChangeTick = LightPiece.YellowTick;
            break;
        case ETrafficLightState::Green:
            ChangeTick = LightPiece.GreenTick;
            break;
        default:
            UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
            SetAmericanLightState(LightPiece, ETrafficLightState::Red);
            return;
    }

    if(LightPiece.ElapsedTick > ChangeTick){
        SwitchAmericanLightState(LightPiece);
        LightPiece.ElapsedTick = 0.0f;
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
    UE_LOG(LogTemp, Warning, TEXT("%s -> Tick: %f; Current State: %s; Next State: %s"), *LightPiece.LightPieceDescription,LightPiece.ElapsedTick, *CurrentState, *NextState);

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