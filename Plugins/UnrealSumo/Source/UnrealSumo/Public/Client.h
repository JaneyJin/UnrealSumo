// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "src/TraCIAPI.h"

/**
 * 
 */
class UNREALSUMO_API Client : public TraCIAPI{
public:
	// Constructor for Client class. Client class inherited from TraCI API.
	Client();
	~Client();
};
