// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "src/TraCIAPI.h"

/**
 *
 */
class UNREALSUMO_API Client : public TraCIAPI
{
public:
	Client();
	~Client();

public:
	// Record update SUMODeltaT setup in Unreal. 
	// If fail to setup SUMO FPS in Unreal, return false. Or SUMO FPS is lower than Unreal FPS, return false.
	bool SetUpdateDeltaTFlag = false;
};
