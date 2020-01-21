// Fill out your copyright notice in the Description page of Project Settings.


#include "WheeledVehicleUpdateComponent.h"

// Sets default values for this component's properties
UWheeledVehicleUpdateComponent::UWheeledVehicleUpdateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWheeledVehicleUpdateComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Error, TEXT("Component Begin"))
}


// Called every frame
void UWheeledVehicleUpdateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Error, TEXT("Component Tick"))
	SetThrottleInput(25);
}