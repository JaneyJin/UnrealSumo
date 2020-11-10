// Fill out your copyright notice in the Description page of Project Settings.


#include "TrafficLightGroup.h"
#include "TrafficLightBase.h"
#include "UnrealSumo/World/SumoGameMode.h"
#include "UnrealSumo/World/SumoGameInstance.h"
#include "Engine.h"
#include <algorithm>

// Sets default values
ATrafficLightGroup::ATrafficLightGroup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATrafficLightGroup::BeginPlay()
{
	Super::BeginPlay();
    // Require SumoGameMode to setup SumoGameInstance and then get SumoGameInstance in TrafficLightGroup BeginPlay
     ASumoGameMode* GameMode = Cast<ASumoGameMode>(GetWorld()->GetAuthGameMode());
    if(!GameMode->HasActorBegunPlay()){
        GameMode->DispatchBeginPlay();
    }

    if(!GameMode->SynBySUMOTrafficLight()){
//        if(TrafficLightReference.Num() > 0) {
//            for (int Group = 0; Group < TrafficLightReference.Num(); Group++) {
//                ATrafficLightBase* tl = TrafficLightReference[Group];
//
//                // TODO: Setup traffic light duration through user interface
//                tl->SetTrafficLightState(ETrafficLightState::Red);
//            }
//        }
        return;
    }

    SumoGameInstance = Cast<USumoGameInstance>(GetGameInstance());
    if(SumoGameInstance && SumoGameInstance->client){

        ValidateJunctionID(JunctionID);
        std::vector<libsumo::TraCILogic> SumoTL_Logic = SumoGameInstance->client->trafficlights.getAllProgramLogics(TCHAR_TO_UTF8(*JunctionID));
        if(TrafficLightReference.Num() > 0){
            for (int Group = 0;Group < TrafficLightReference.Num();Group++)
            {

                std::vector<std::pair<char,double>> TrafficLightLogic;
                double GreenTick = 0, YellowTick = 0, RedTick = 0;

                ATrafficLightBase* trafficlight = TrafficLightReference[Group];

                FString TrafficLightID = trafficlight->GetName();
                std::vector<libsumo::TraCIPhase*>  TL_Phases = SumoTL_Logic[0].phases;

                for(int i = 0; i < TL_Phases.size(); i++){
                    TrafficLightLogic.push_back({ ExtractLightState(TL_Phases[i]->state, Group), TL_Phases[i]->duration });
                }
                char InState = TrafficLightLogic.at(0).first;

                for(int j = 0; j < TrafficLightLogic.size(); j++){

                    if(TrafficLightLogic.at(j).first == 'r' ){
                        RedTick += TrafficLightLogic.at(j).second * SumoGameInstance->SUMOToUnrealFrameRate.SUMOFPS;
                    }else if(TrafficLightLogic.at(j).first == 'y'){
                        YellowTick += TrafficLightLogic.at(j).second * SumoGameInstance->SUMOToUnrealFrameRate.SUMOFPS;
                    }else if(TrafficLightLogic.at(j).first == 'g'){
                        GreenTick += TrafficLightLogic.at(j).second * SumoGameInstance->SUMOToUnrealFrameRate.SUMOFPS;
                    }

                }
                trafficlight->TrafficLightInitialization(TrafficLightID, InState, RedTick, YellowTick, GreenTick);

            }
        }
    }

}

//// Called every frame
//void ATrafficLightGroup::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//    UE_LOG(LogTemp, Warning, TEXT("TrafficLightGroupTick."))
//}

char ATrafficLightGroup::ExtractLightState(std::string TL_State, int TL_Group){
    int group = 0;
    for(int i = 0; i < TL_State.length(); i++){
        int j = i + 1;

        char left = tolower(TL_State.at(i));
        if(i == TL_State.length() - 1){
            return left;
        }

        char right = tolower(TL_State.at(j));
        if(left != right ){
            if(group == TL_Group){
                return left;
            }
            group++;
        }

    }

    return '\0';
}

void ATrafficLightGroup::ValidateJunctionID(FString InputJunctionID){
    std::vector<std::string> TrafficLightList= SumoGameInstance->client->trafficlights.getIDList();
    if (std::find(TrafficLightList.begin(), TrafficLightList.end(), std::string(TCHAR_TO_UTF8(*InputJunctionID))) != TrafficLightList.end()){
         return;
    }
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Traffic light Junction ID %s is invalid. Assign a valid ID %s for it."), *JunctionID, *FString(TrafficLightList[0].c_str())));

    this->JunctionID = FString(TrafficLightList[0].c_str());

}