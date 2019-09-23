// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"
#include "Client.h"
#include "VehicleInformation.h"
#include "VehiclePoisitonUpdateComponent.h"


// Sets default values
AVehicle::AVehicle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Add actor component to the vehicle blueprint class
	VehiclePoisitonUpdateComponent = CreateDefaultSubobject<UVehiclePoisitonUpdateComponent>(FName("Update Vehicle Position Component"));
}

// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//bool AVehicle::InitializeVehicle(FVehicleInformation VehicleToSet, Client* ClientToSet, &FrameRateSyn SUMOToUnrealFrameRate) {
bool AVehicle::InitializeVehicle(FVehicleInformation VehicleToSet,Client* ClientToSet, FrameRateSyn SUMOToUnrealFrameRate) {
	// UE_LOG(LogTemp, Warning, TEXT("ID: %s,  Speed: %f,  Position: %s, Color: %s"), *VehicleIdToSet, VehicleSpeed, *VehiclePosition.ToString(), *VehicleColor.ToString());
	TESTUnrealVehicleInformation = VehicleToSet;
	
	UE_LOG(LogTemp, Warning, TEXT("Fail to Set"))

		UE_LOG(LogTemp, Warning, TEXT("Fail to Set"))
	// TODO: Think whether to keep it
	// UnrealVehicleInformation.VehicleColor = VehicleToSet.VehicleColor;
	 
	//  FVehicleInformation VehicleToSet;
	/* if (VehiclePoisitonUpdateComponent->SetVehicleIdAndClient(VehicleToSet, ClientToSet, SUMOToUnrealFrameRate)) {

		return true;
	}*/
	
	UE_LOG(LogTemp, Warning, TEXT("Fail to Set"))
	return false;
	
}


