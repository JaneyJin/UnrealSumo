#pragma once
#include <vector>
#include "TrafficLightSubGroup.generated.h"

USTRUCT(BlueprintType, Category="Traffic Light")
struct FSubGroup
{
    GENERATED_BODY()

public:
   int GroupIndex; // Group "0", Group "1", ..., Group "n"
   char SubGroup; // Left/ Straight/ Right Group
   char InitialState;
   bool NeedErase = false;
   std::vector<double> RedTime;
   std::vector<double> YellowTime;
   std::vector<double> GreenTime;
};
