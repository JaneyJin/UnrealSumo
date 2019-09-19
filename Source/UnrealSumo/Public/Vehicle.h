// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "VehicleInformation.h"
#include "GameFramework/Pawn.h"
#include "Vehicle.generated.h"

class Client;
class UVehiclePoisitonUpdateComponent;


UCLASS()
class UNREALSUMO_API AVehicle : public APawn
{
	GENERATED_BODY()




public:	
	// Sets default values for this pawn's properties
	AVehicle();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	/**
	 * TODO
	 * @param
	 * @return
	 */
	bool InitializeVehicle(FString VehicleIdToSet, double VehicleSpeed, FVector VehiclePosition, FColor VehicleColor, Client* ClientToSet, float UpdateDeltaT, float NextTimeToUpdate);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	// Instantiate VehicleInformation Structure class to pass these value to VehiclePositionUpdateComponent Class
	FVehicleInformation UnrealVehicleInformation;

	// Attach Actor component to the Vehicle blueprint class
	UVehiclePoisitonUpdateComponent* VehiclePoisitonUpdateComponent = nullptr;
};
