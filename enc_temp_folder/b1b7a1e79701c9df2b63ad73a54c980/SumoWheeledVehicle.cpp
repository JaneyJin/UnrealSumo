// Fill out your copyright notice in the Description page of Project Settings.


#include "SumoWheeledVehicle.h"
#include "Client.h"
#include "Components/InputComponent.h"
#include "Engine/SkeletalMesh.h"
#include "WheeledVehicleMovementComponent4W.h"

ASumoWheeledVehicle::ASumoWheeledVehicle() {
	client = nullptr;
	VehicleId = "";

}

void ASumoWheeledVehicle::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultController();

	if (!GetController()) {
		Destroy();
	}
	/*else {
		MoveForward(ThrottleVal);
	}*/
	// UE_LOG(LogTemp, Warning, TEXT("SumoWheeledVehicle beginplay"));
	// UE_LOG(LogTemp, Log, TEXT("GetVehicleMovement: %s; GetVehicleMovementComponent: %s"), *GetVehicleMovement()->GetName(), *GetVehicleMovementComponent()->GetName());
}

void ASumoWheeledVehicle::Tick(float Delta)
{
	Super::Tick(Delta);
	// UE_LOG(LogTemp, Warning, TEXT("SumoWheeledVehicle tick"))

	if (GetController()) {
		//GetVehicleMovementComponent()->SetSteeringInput(10);
		UpdateSUMOByTickCount(Delta);

		// UE_LOG(LogTemp, Warning, TEXT("Current controller is %s"), GetController());
		// UE_LOG(LogTemp, Warning, TEXT("%s set speed is not set. Forward Vector: %s ; Current forward speed is %f; Current Gear: %d"), *GetName(), *GetActorForwardVector().ToString(), GetVehicleMovement()->GetForwardSpeed(),GetVehicleMovement()->GetCurrentGear())
	}

}

void ASumoWheeledVehicle::UpdateSUMOByTickCount(float Delta) {
	if (UnrealFRS.TickCount < UnrealFRS.UETickBetweenSUMOUpdates) {
		// UE_LOG(LogTemp, Display, TEXT("SumoWheeledVehicle -> Tick() %d"), UnrealFRS.TickCount)
		UnrealFRS.TickCount++;
	}
	else if (UnrealFRS.TickCount == UnrealFRS.UETickBetweenSUMOUpdates) {
		
		// UE_LOG(LogTemp, Warning, TEXT("SumoWheeledVehicle -> Tick() %d. Update from SUMo."), UnrealFRS.TickCount)

		UpdateFromSUMO(Delta);
		UnrealFRS.TickCount = 1;
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Tick calculation is wrong."))
	}

}

void ASumoWheeledVehicle::UpdateFromSUMO(float Delta) {


	

	std::string VID(TCHAR_TO_UTF8(*VehicleId));
	// UE_LOG(LogTemp, Display, TEXT("GetOwner %s :: VehicleId %s "), *ObjectName, *VID);

	int ArrivedNumber = client->simulation.getArrivedNumber();
	if (ArrivedNumber != 0) {
		std::vector<std::string> ArrivedList = client->simulation.getArrivedIDList();
		for (std::vector<std::string>::iterator it = ArrivedList.begin(); it != ArrivedList.end(); ++it) {
			if (*it == VID) {
				DestroyVehicle();
				return;
			}
		}

	}
	
	// Update location
	auto vehiclepos = client->vehicle.getPosition(VID);
	VehicleNewPosition.X = vehiclepos.x * MeterUnitConversion;
	VehicleNewPosition.Y = vehiclepos.y * MeterUnitConversion * -1;
	VehicleNewPosition.Z = vehiclepos.z * MeterUnitConversion;

	auto vehicleangle = client->vehicle.getAngle(VID) - 90;
	if (fabs(vehicleangle - VehicleNewRotator.Yaw) > 0.00001) {
		VehicleNewRotator.Yaw = vehicleangle;
	}
	// UE_LOG(LogTemp, Display, TEXT("New Location: %s ; New Rotator: %s"), *VehicleNewPosition.ToString(), *VehicleNewRotator.ToString())

	FHitResult HitResult;
	if (K2_SetActorLocationAndRotation(VehicleNewPosition, VehicleNewRotator, false, HitResult, true) == false) {
		// If the set function returned false something is blocking at that location. We can interrogate this result to determine details of this  
		// @See FHitResult for more information  
		if (HitResult.GetActor() != nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Cannot move object to location, blocked by %s"), *HitResult.GetActor()->GetName());
		}
	}
	

	// auto VehicleSpeed = client->vehicle.getSpeed(VID);
	//auto VehicleAngle = client->vehicle.getAngle(VID) - 90;  // 90 degrees off between UE and SUMO
	// auto MyVehicle = Owner->GetName();
	// auto Time = GetWorld()->GetTimeSeconds();
	// UE_LOG(LogTemp, Warning, TEXT("%f: %s is at %s"), Time, *MyVehicle, *VehicleNewPosition.ToString());
}

bool ASumoWheeledVehicle::DestroyVehicle() {

	if (this->Destroy()) {
		//FString VId = VehicleId.c_str();
		UE_LOG(LogTemp, Display, TEXT("Destroy %s success."), *VehicleId)
			return true;
	}

	UE_LOG(LogTemp, Error, TEXT("Fail to Destroy vehicle %s."), *VehicleId);
	return false;
}

bool ASumoWheeledVehicle::InitializeWheeledVehicle(FVehicleInformation &VehicleToSet, Client* ClientToSet, FrameRateSyn &SUMOToUnrealFrameRate) {

	this->VehicleId = VehicleToSet.VehicleId;
	this->client = ClientToSet;
	this->UnrealFRS = SUMOToUnrealFrameRate;
	if (client && VehicleId.IsEmpty()) {
		return false; // Fail to initialize
	}
	return true;
}

void ASumoWheeledVehicle::MoveForward(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void ASumoWheeledVehicle::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void ASumoWheeledVehicle::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ASumoWheeledVehicle::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void ASumoWheeledVehicle::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASumoWheeledVehicle::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASumoWheeledVehicle::MoveRight);
	PlayerInputComponent->BindAxis("LookUp");
	PlayerInputComponent->BindAxis("LookRight");

	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &ASumoWheeledVehicle::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &ASumoWheeledVehicle::OnHandbrakeReleased);
	// PlayerInputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ASumoWheeledVehicle::OnToggleCamera);

	// PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASumoWheeledVehicle::OnResetVR);
}