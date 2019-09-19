// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePoisitonUpdateComponent.h"
#include "Client.h"


// Sets default values for this component's properties
UVehiclePoisitonUpdateComponent::UVehiclePoisitonUpdateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVehiclePoisitonUpdateComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	
}


// Called every frame
void UVehiclePoisitonUpdateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);




}


bool UVehiclePoisitonUpdateComponent::SetVehicleIdAndClient(FString VehicleIdToSet, Client* ClientToSet, float UpdateDeltaT, float NextTimeToUpdate) {
	if (VehicleIdToSet.IsEmpty()) {
		return false;
	}

	this->VehicleId = std::string(TCHAR_TO_UTF8(*VehicleIdToSet));
	this->client = ClientToSet;
	this->UpdateDeltaT = UpdateDeltaT;
	this->NextTimeToUpdate = NextTimeToUpdate;
	return true;
}
