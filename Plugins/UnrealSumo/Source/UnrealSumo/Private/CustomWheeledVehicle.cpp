// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomWheeledVehicle.h"
#include "Client.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "WheeledVehicleUpdateComponent.h"
#include "Components/InputComponent.h"
// Sets default values


//ACustomWheeledVehicle::ACustomWheeledVehicle(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(VehicleMeshComponentName);
//	Mesh->SetCollisionProfileName(UCollisionProfile::Vehicle_ProfileName);
//	Mesh->BodyInstance.bSimulatePhysics = true;
//	Mesh->BodyInstance.bNotifyRigidBodyCollision = true;
//	Mesh->BodyInstance.bUseCCD = true;
//	Mesh->bBlendPhysics = true;
//	Mesh->bGenerateOverlapEvents = true;
//	Mesh->SetCanEverAffectNavigation(false);
//	RootComponent = Mesh;
//
//	VehicleMovement = CreateDefaultSubobject<UNWheeledVehicleMovementComponent, Update>(VehicleMovementComponentName);
//	VehicleMovement->SetIsReplicated(true); // Enable replication by default
//	VehicleMovement->UpdatedComponent = Mesh;
//
//	PrimaryActorTick.bCanEverTick = true;
//	PrimaryActorTick.bStartWithTickEnabled = true;
//}

ACustomWheeledVehicle::ACustomWheeledVehicle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	//// Add actor component to the vehicle blueprint class
	//WheeledVehicleUpdateComponent = CreateDefaultSubobject<UWheeledVehicleUpdateComponent>(FName("Update Vehicle Position Component"));
	// WheeledVehicleUpdateComponent = Super(ObjectInitializer.SetDefaultSubobjectClass<UWheeledVehicleUpdateComponent>("Update Vehicle Position Component"));
	//GetVehicleMovementComponent()->SetThrottleInput(10);



	//SetupPlayerInputComponent();
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

void ACustomWheeledVehicle::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACustomWheeledVehicle::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACustomWheeledVehicle::MoveRight);
	PlayerInputComponent->BindAxis("LookUp");
	PlayerInputComponent->BindAxis("LookRight");

	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &ACustomWheeledVehicle::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &ACustomWheeledVehicle::OnHandbrakeReleased);
	
}

void ACustomWheeledVehicle::MoveForward(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void ACustomWheeledVehicle::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void ACustomWheeledVehicle::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ACustomWheeledVehicle::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}


/******************* Keyboard bindings*******************/
//This method must be in pawn because Unreal doesn't allow key bindings to non UObject pointers
//void ACustomWheeledVehicle::setupInputBindings()
//{
//	UAirBlueprintLib::EnableInput(this);
//
//	UAirBlueprintLib::BindAxisToKey(FInputAxisKeyMapping("MoveForward", EKeys::Up, 1), this,
//		this, &ACustomWheeledVehicle::onMoveForward);
//
//	UAirBlueprintLib::BindAxisToKey(FInputAxisKeyMapping("MoveForward", EKeys::Down, -1), this,
//		this, &ACustomWheeledVehicle::onMoveForward);
//
//	UAirBlueprintLib::BindAxisToKey(FInputAxisKeyMapping("MoveRight", EKeys::Right, 0.5), this,
//		this, &ACustomWheeledVehicle::onMoveRight);
//
//	UAirBlueprintLib::BindAxisToKey(FInputAxisKeyMapping("MoveRight", EKeys::Left, -0.5), this,
//		this, &ACustomWheeledVehicle::onMoveRight);
//
//	UAirBlueprintLib::BindActionToKey("Handbrake", EKeys::End, this, &ACustomWheeledVehicle::onHandbrakePressed, true);
//	UAirBlueprintLib::BindActionToKey("Handbrake", EKeys::End, this, &ACustomWheeledVehicle::onHandbrakeReleased, false);
//
//	UAirBlueprintLib::BindAxisToKey(FInputAxisKeyMapping("Footbrake", EKeys::SpaceBar, 1), this,
//		this, &ACustomWheeledVehicle::onFootBrake);
//
//	UAirBlueprintLib::BindAxisToKey(FInputAxisKeyMapping("MoveRight", EKeys::Gamepad_LeftX, 1), this,
//		this, &ACustomWheeledVehicle::onMoveRight);
//
//	UAirBlueprintLib::BindAxisToKey(FInputAxisKeyMapping("MoveForward", EKeys::Gamepad_RightTriggerAxis, 1), this,
//		this, &ACustomWheeledVehicle::onMoveForward);
//
//	UAirBlueprintLib::BindAxisToKey(FInputAxisKeyMapping("Footbrake", EKeys::Gamepad_LeftTriggerAxis, 1), this,
//		this, &ACustomWheeledVehicle::onFootBrake);
//
//	//below is not needed
//	//UAirBlueprintLib::BindActionToKey("Reverse", EKeys::Down, this, &ACustomWheeledVehicle::onReversePressed, true);
//	//UAirBlueprintLib::BindActionToKey("Reverse", EKeys::Down, this, &ACustomWheeledVehicle::onReverseReleased, false);
//}

//void ACustomWheeledVehicle::onMoveForward(float Val)
//{
//	if (Val < 0)
//		onReversePressed();
//	else
//		onReverseReleased();
//
//	keyboard_controls_.throttle = Val;
//}
//
//void ACustomWheeledVehicle::onMoveRight(float Val)
//{
//	keyboard_controls_.steering = Val;
//}
//
//void ACustomWheeledVehicle::onHandbrakePressed()
//{
//	keyboard_controls_.handbrake = true;
//}
//
//
//
//void ACustomWheeledVehicle::onReversePressed()
//{
//	if (keyboard_controls_.manual_gear >= 0) {
//		keyboard_controls_.is_manual_gear = true;
//		keyboard_controls_.manual_gear = -1;
//		keyboard_controls_.gear_immediate = true;
//	}
//}
//
//void ACustomWheeledVehicle::onReverseReleased()
//{
//	if (keyboard_controls_.manual_gear < 0) {
//		keyboard_controls_.is_manual_gear = false;
//		keyboard_controls_.manual_gear = 0;
//		keyboard_controls_.gear_immediate = true;
//	}
//}