// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomWheeledVehicle.h"
#include "Client.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "WheeledVehicleUpdateComponent.h"
// Sets default values
ACustomWheeledVehicle::ACustomWheeledVehicle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	//// Add actor component to the vehicle blueprint class
	WheeledVehicleUpdateComponent = CreateDefaultSubobject<UWheeledVehicleUpdateComponent>(FName("Update Vehicle Position Component"));
	// WheeledVehicleUpdateComponent = Super(ObjectInitializer.SetDefaultSubobjectClass<UWheeledVehicleUpdateComponent>("Update Vehicle Position Component"));
}

void ACustomWheeledVehicle::BeginPlay() {
	UE_LOG(LogTemp, Warning, TEXT("CustomWheeledVehicle begin play"))
}

void ACustomWheeledVehicle::Tick(float Delta)
{
	Super::Tick(Delta);

	
	UE_LOG(LogTemp, Warning, TEXT("CustomWheeledVehicle Tick"))
}

//bool AVehicle::InitializeVehicle(FVehicleInformation VehicleToSet, Client* ClientToSet, &FrameRateSyn SUMOToUnrealFrameRate) {
bool ACustomWheeledVehicle::InitializeWheeledVehicle(FVehicleInformation &VehicleToSet, Client* ClientToSet, FrameRateSyn &SUMOToUnrealFrameRate) {
	// UE_LOG(LogTemp, Warning, TEXT("Set Vehicle: ID: %s,  Speed: %f,  Position: %s, Color: %s"), *(*VehicleToSet.VehicleId), *VehicleToSet.VehicleSpeed, *(*VehicleToSet.VehiclePosition.ToString()), *(*VehicleToSet.VehicleColor.ToString()));
	UnrealVehicleInformation = VehicleToSet;
	UE_LOG(LogTemp, Warning, TEXT("CustomWheeledVehicle initialize wheeled vehicle"))
	return true;

	/*UE_LOG(LogTemp, Warning, TEXT("Fail to Set"))
	return false;*/

}