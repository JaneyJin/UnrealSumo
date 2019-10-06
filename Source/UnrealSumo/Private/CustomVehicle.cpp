// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomVehicle.h"
#include "Client.h"
#include "VehicleInformation.h"
#include "VehiclePositionUpdateComponent.h"
// Sets default values
ACustomVehicle::ACustomVehicle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//// Add actor component to the vehicle blueprint class
	VehiclePositionUpdateComponent = CreateDefaultSubobject<UVehiclePositionUpdateComponent>(FName("Update Vehicle Position Component"));
}

// Called when the game starts or when spawned
void ACustomVehicle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACustomVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//bool AVehicle::InitializeVehicle(FVehicleInformation VehicleToSet, Client* ClientToSet, &FrameRateSyn SUMOToUnrealFrameRate) {
bool ACustomVehicle::InitializeVehicle(FVehicleInformation &VehicleToSet, Client* ClientToSet, FrameRateSyn &SUMOToUnrealFrameRate) {
		// UE_LOG(LogTemp, Warning, TEXT("Set Vehicle: ID: %s,  Speed: %f,  Position: %s, Color: %s"), *(*VehicleToSet.VehicleId), *VehicleToSet.VehicleSpeed, *(*VehicleToSet.VehiclePosition.ToString()), *(*VehicleToSet.VehicleColor.ToString()));
		UnrealVehicleInformation = VehicleToSet;

		
		//  FVehicleInformation VehicleToSet;
		if (VehiclePositionUpdateComponent->SetVehicleIdAndClient(VehicleToSet, ClientToSet, SUMOToUnrealFrameRate)) {
			return true;
		}

		UE_LOG(LogTemp, Warning, TEXT("Fail to Set"))
		return false;

}