// Fill out your copyright notice in the Description page of Project Settings.


#include "TrafficLightGroup.h"
#include "TrafficLightBase.h"
#include "UnrealSumo/World/SumoGameMode.h"
#include "UnrealSumo/World/SumoGameInstance.h"
#include "TrafficLightSubGroup.h"
#include "AmericanLight.h"
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

    auto GameMode = GetWorld()->GetAuthGameMode();
    if(!GameMode->GetName().Contains("SumoGameMode")){
        return;
    }

    // Require SumoGameMode to setup SumoGameInstance and then get SumoGameInstance in TrafficLightGroup BeginPlay
     ASumoGameMode* SumoGameMode = Cast<ASumoGameMode>(GetWorld()->GetAuthGameMode());
    if(!SumoGameMode->HasActorBegunPlay()){
        SumoGameMode->DispatchBeginPlay();
    }

    if(!SumoGameMode->SynBySUMOTrafficLight()){
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("SYN SUMO Traffic Light"))
    SumoGameInstance = Cast<USumoGameInstance>(GetGameInstance());
    if(SumoGameInstance && SumoGameInstance->client){
//
//        ValidateJunctionID(JunctionID);
//
//		    if(TrafficLightReference.Num() > 0){
//		        std::vector<libsumo::TraCILogic> SumoTL_Logic = SumoGameInstance->client->trafficlights.getAllProgramLogics(TCHAR_TO_UTF8(*JunctionID));
//		        std::vector<int> NumbersofLanes = GetLaneNumsforGroup(std::string(TCHAR_TO_UTF8(*JunctionID)));
//
//                if(NumbersofLanes.size() != TrafficLightReference.Num()){
//                        UE_LOG(LogTemp, Warning, TEXT("The numbers of traffic light in Junction %s is not correct. Stop traffic light synchronization for this junction"),*JunctionID)
//                        return;
//                }
//
//                int indexInState = 0;
//                std::vector<std::vector<libsumo::TraCILink>> ControlledLink =  SumoGameInstance->client->trafficlights.getControlledLinks(TCHAR_TO_UTF8(*JunctionID));
//                for (int Group = 0;Group < NumbersofLanes.size();Group++){
//
//                    // TODO: if it is Single traffic light
//	                if(TrafficLightReference[Group]->IsA(ATrafficLightBase::StaticClass())){
//	                    UE_LOG(LogTemp, Warning, TEXT("ATrafficLightBase"))
//	                    continue;
//	                }
//
//                    // If the traffic light is American Light class, it is a compound traffic light with 3 subgroup.
//	                if(TrafficLightReference[Group]->IsA(AAmericanLight::StaticClass())){
//	                    AAmericanLight* AmericanLight = Cast<AAmericanLight>(TrafficLightReference[Group]);
//	                    if(AmericanLight){
//
//                          // std::tuple<right, straight, left>
//                            std::tuple<int, int, int> subgroupIndex = GetSubGroupIndex(ControlledLink, indexInState, NumbersofLanes[Group]);
//                            std::vector<FSubGroup> LightTime = GetAmericanLightTime(SumoTL_Logic[0].phases, subgroupIndex, Group);
//	                        AmericanLight->AmericanLightInitialization(LightTime);
//	                        // UE_LOG(LogTemp, Warning, TEXT("AAmericanLight"))
//	                    }
//	                }
//
//                        indexInState += NumbersofLanes[Group];
//                }
//        }
    }

}


void ATrafficLightGroup::ValidateJunctionID(FString InputJunctionID){
    std::vector<std::string> TrafficLightList= SumoGameInstance->client->trafficlights.getIDList();
    if (std::find(TrafficLightList.begin(), TrafficLightList.end(), std::string(TCHAR_TO_UTF8(*InputJunctionID))) != TrafficLightList.end()){
         return;
    }
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Traffic light Junction ID %s is invalid. Assign a valid ID %s for it."), *JunctionID, *FString(TrafficLightList[0].c_str())));
    this->JunctionID = FString(TrafficLightList[0].c_str());
}



std::vector<int> ATrafficLightGroup::GetLaneNumsforGroup(std::string ControlledLanesID){
    char delimiter = '_';
    std::vector<int> LaneNumsforGroup;
    std::vector<std::string> ControlledLanes= SumoGameInstance->client->trafficlights.getControlledLanes(ControlledLanesID);
    std::string ComparedLane = ControlledLanes[0].substr(0, ControlledLanes[0].find(delimiter));
    int count = 0;
    for(int i = 0; i < ControlledLanes.size();i++){
        std::string LaneID = ControlledLanes[i].substr(0, ControlledLanes[i].find(delimiter));

        if(ComparedLane == LaneID){
            count++;
            continue;
        }
        LaneNumsforGroup.push_back(count);
        ComparedLane = LaneID;
        count = 1;
    }
    if(count > 0){
        LaneNumsforGroup.push_back(count);
    }

    return LaneNumsforGroup;
}



std::tuple<int, int, int> ATrafficLightGroup::GetSubGroupIndex(const std::vector<std::vector<libsumo::TraCILink>>& ControlledLink, const int position, const int length){
    FSubGroup subgroup;
    int LeftIndex = -1;
    int StraightIndex = -1;
    int RightIndex = -1;

    for(int i = position; i < position + length; i++){
        std::string LaneId = ControlledLink[i][0].fromLane;

        auto LaneLinks = SumoGameInstance->client->lane.getLinks(LaneId);

        for(int j = 0; j < LaneLinks.size(); j++){
            std::string direction = LaneLinks[j].direction;

            std::transform(direction.begin(), direction.end(), direction.begin(), ::tolower);
            if(RightIndex < 0 && direction == "r"){
                RightIndex = i + j;
                // i = i + j;
                continue;
            }

            if(StraightIndex < 0 && direction == "s"){
                StraightIndex = i + j;
                // i = i + j;
                continue;
            }

            if(LeftIndex < 0 && direction == "l"){
                LeftIndex = i + j;
                // i = i + j;
                continue;
            }

        }
    }

    return std::make_tuple(RightIndex, StraightIndex, LeftIndex);
}


// std::tuple<RightIndex, StraightIndex, LeftIndex>
std::vector<FSubGroup> ATrafficLightGroup::GetAmericanLightTime(const std::vector<libsumo::TraCIPhase*>& Phases, const std::tuple<int, int, int> subgroupIndex, int GroupNumber){

    std::vector<FSubGroup> TrafficLightSubGroupTime;
    // Right Group
    if(std::get<0>(subgroupIndex) >= 0){
        TrafficLightSubGroupTime.push_back(GetSubGroupTime(Phases, GroupNumber, std::get<0>(subgroupIndex), 'r'));
    }

    // Straight Group
    if(std::get<1>(subgroupIndex) >= 0){
        TrafficLightSubGroupTime.push_back(GetSubGroupTime(Phases, GroupNumber, std::get<1>(subgroupIndex), 's'));
    }

    // Left Group
    if(std::get<2>(subgroupIndex) >= 0){
        TrafficLightSubGroupTime.push_back(GetSubGroupTime(Phases, GroupNumber, std::get<2>(subgroupIndex), 'l'));
    }

		return TrafficLightSubGroupTime;

}


FSubGroup ATrafficLightGroup::GetSubGroupTime(const std::vector<libsumo::TraCIPhase*>& Phases, int GroupNumber, int index, char direction){
    FSubGroup subgroup;
    subgroup.SubGroup = direction;
    subgroup.GroupIndex = GroupNumber;

    char lastState;
    double countTime = 0;
    double SUMOFPS = 1 / SumoGameInstance->client->simulation.getDeltaT();
    for(int i = 0; i < Phases.size();i++){
        if(i == 0){
            lastState = subgroup.InitialState = std::tolower(Phases[i]->state[index]);
        }

        if(std::tolower(Phases[i]->state[index]) != lastState && countTime > 0){
            // Record the counted time
            switch(lastState){
                case 'r':
                    subgroup.RedTime.push_back(countTime);
                    break;
                case 'y':
                    subgroup.YellowTime.push_back(countTime);
                    break;
                case 'g':
                    subgroup.GreenTime.push_back(countTime);
                    break;
            }

            countTime = 0;
            lastState = std::tolower(Phases[i]->state[index]);
        }

        countTime += Phases[i]->duration;

    }

    if(countTime > 0){

        if(lastState == subgroup.InitialState){
            subgroup.NeedErase = true;

            if(lastState == 'r') {
                countTime += subgroup.RedTime[0];
            }else if(lastState == 'y') {
                countTime += subgroup.YellowTime[0];
            }else{
                countTime += subgroup.GreenTime[0];
            }
        }

        switch(lastState){
            case 'r':
                subgroup.RedTime.push_back(countTime);
                break;
            case 'y':
                subgroup.YellowTime.push_back(countTime);
                break;
            case 'g':
                subgroup.GreenTime.push_back(countTime);
                break;
        }
    }


    return subgroup;
}
