// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomWheeledVehicle.h"
#include "Client.h"
#include "VehiclePositionUpdateComponent.h"

void ACustomWheeledVehicle::BeginPlay() {
	UE_LOG(LogTemp, Warning, TEXT("CustomWheeledVehicle begin play"))
}

void ACustomWheeledVehicle::Tick(float Delta)
{
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