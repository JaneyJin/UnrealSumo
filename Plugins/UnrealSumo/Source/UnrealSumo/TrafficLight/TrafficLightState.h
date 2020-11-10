// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TrafficLightState.generated.h"

UENUM(BlueprintType)
enum class ETrafficLightState : uint8{
    Red     UMETA(DisplayName = "Red"),
    Yellow  UMETA(DisplayName = "Yellow"),
    Green   UMETA(DisplayName = "Green")
   //Off     UMETA(DisplayName = "Off")
};