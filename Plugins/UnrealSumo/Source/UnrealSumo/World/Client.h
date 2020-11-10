// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "UnrealSumo/src/TraCIAPI.h"
/**
 *
 */
class UNREALSUMO_API Client : public TraCIAPI
{
public:
	Client();
	~Client();
};