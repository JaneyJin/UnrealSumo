// Fill out your copyright notice in the Description page of Project Settings.


#include "WheeledVehiclePawn.h"
// Bind Input with wheeled vehicle motion
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Materials/Material.h"
#include "IXRTrackingSystem.h" 
#include "CustomWheelFront.h"
#include "CustomWheelRear.h"
#include "Client.h"
#include "VehiclePositionUpdateComponent.h"
#include "Components/BoxComponent.h"
#include "SumoGameInstance.h"
// For VR Headset
//#if HMD_MODULE_INCLUDED
//	#include "IHeadMountedDisplay.h"
//	#include "HeadMountedDisplayFunctionLibrary.h"
//#endif // HMD_MODULE_INCLUDED

const FName AWheeledVehiclePawn::LookUpBinding("LookUp");
const FName AWheeledVehiclePawn::LookRightBinding("LookRight");

#define LOCTEXT_NAMESPACE "YishenWheeledVehiclePawn"
#define MeterUnitConversion 100

AWheeledVehiclePawn::AWheeledVehiclePawn() {
	client = nullptr;
	VehicleId = "";
	
	// Car mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/UnrealSumo/WheeledVehicle/Sedan/Sedan_SkelMesh"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);
	// Car animation blueprint
	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/UnrealSumo/WheeledVehicle/Sedan/Sedan_AnimBP"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);

	// Simulation
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(Vehicle4W->WheelSetups.Num() == 4);

	Vehicle4W->WheelSetups[0].WheelClass = UCustomWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("Wheel_Front_Left");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = UCustomWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("Wheel_Front_Right");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = UCustomWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("Wheel_Rear_Left");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = UCustomWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("Wheel_Rear_Right");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	// Create In-Car camera component 
	InternalCameraOrigin = FVector(0.0f, -40.0f, 120.0f);

	InternalCameraBase = CreateDefaultSubobject<USceneComponent>(TEXT("InternalCameraBase"));
	InternalCameraBase->SetRelativeLocation(InternalCameraOrigin);
	InternalCameraBase->SetupAttachment(GetMesh());

	InternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InternalCamera"));
	InternalCamera->bUsePawnControlRotation = false;
	InternalCamera->FieldOfView = 90.f;
	InternalCamera->SetupAttachment(InternalCameraBase);

	//Setup TextRenderMaterial
	// static ConstructorHelpers::FObjectFinder<UMaterial> TextMaterial(TEXT("Material'/Engine/EngineMaterials/AntiAliasedTextMaterialTranslucent.AntiAliasedTextMaterialTranslucent'"));

	// UMaterialInterface* Material = TextMaterial.Object;

	// Create text render component for in car speed display
	InCarSpeed = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarSpeed"));
	// InCarSpeed->SetTextMaterial(Material);
	InCarSpeed->SetRelativeLocation(FVector(70.0f, -75.0f, 99.0f));
	InCarSpeed->SetRelativeRotation(FRotator(18.0f, 180.0f, 0.0f));
	InCarSpeed->SetupAttachment(GetMesh());
	InCarSpeed->SetRelativeScale3D(FVector(1.0f, 0.4f, 0.4f));

	// Create text render component for in car gear display
	InCarGear = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
	// InCarGear->SetTextMaterial(Material);
	InCarGear->SetRelativeLocation(FVector(66.0f, -9.0f, 95.0f));
	InCarGear->SetRelativeRotation(FRotator(25.0f, 180.0f, 0.0f));
	InCarGear->SetRelativeScale3D(FVector(1.0f, 0.4f, 0.4f));
	InCarGear->SetupAttachment(GetMesh());

	// Colors for the incar gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	// Colors for the in-car gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	bInReverseGear = false;

	// AddMovementInput(GetActorForwardVector(), 1.0f);
	// VehiclePositionUpdateComponent = CreateDefaultSubobject<UVehiclePositionUpdateComponent>(FName("Update Vehicle Position Component"));
}

void AWheeledVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	bool bEnableInCar = false;
//#if HMD_MODULE_INCLUDED
//	bEnableInCar = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
//#endif // HMD_MODULE_INCLUDED
	EnableIncarView(bEnableInCar, true);
	
	SumoData = Cast<USumoGameInstance>(GetGameInstance());
	this->client = SumoData->client;
	this->EgoWheeledVehicleInformation.VehicleId = SumoData->EgoWheeledVehicleId;
	this->UnrealFPS = SumoData->SUMOToUnrealFrameRate;

	if (EgoWheeledVehicleInformation.VehicleId != "") {
		EndEdge = client->vehicle.getRoute(TCHAR_TO_UTF8(*EgoWheeledVehicleInformation.VehicleId)).back();	
		ArrivedFlag = true;
	}
	
	
}

void AWheeledVehiclePawn::Tick(float Delta)
{
	Super::Tick(Delta);


	// Setup the flag to say we are in reverse gear
	bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;

	// Update the strings used in the hud (incar and onscreen)
	UpdateHUDStrings();

	// Set the string in the incar hud
	SetupInCarHUD();

	bool bHMDActive = false;
//#if HMD_MODULE_INCLUDED
//	/*if ((GEngine->HMDDevice.IsValid() == true) && ((GEngine->HMDDevice->IsHeadTrackingAllowed() == true) || (GEngine->IsStereoscopic3D() == true)))
//	{
//		bHMDActive = true;
//	}*/
//
//#endif // HMD_MODULE_INCLUDED
	if (bHMDActive == false)
	{
		if ((InputComponent) && (bInCarCameraActive == true))
		{
			FRotator HeadRotation = InternalCamera->RelativeRotation;
			HeadRotation.Pitch += InputComponent->GetAxisValue(LookUpBinding);
			HeadRotation.Yaw += InputComponent->GetAxisValue(LookRightBinding);
			InternalCamera->RelativeRotation = HeadRotation;
		}
	}
	// UE_LOG(LogTemp, Warning, TEXT("Current controller is %s"), GetController());
	// UE_LOG(LogTemp, Warning, TEXT("%s set speed is not set. Forward Vector: %s ; Current forward speed is %f; Current Gear: %d"), *GetName(), *GetActorForwardVector().ToString(), GetVehicleMovement()->GetForwardSpeed(), GetVehicleMovement()->GetCurrentGear())

	/*EgoWheeledVehicleInformation.VehicleSpeed = GetVehicleForwardSpeed();
	EgoWheeledVehicleInformation.VehiclePosition = GetTransform().GetLocation();
	
	UE_LOG(LogTemp, Error, TEXT("%s -> VehicleSpeed: %f; Vehicle Position: %s; Forward Vector: %s ; Get vehicle orientation: %s; Get Transform Rotation: %f"), *GetName(), GetVehicleForwardSpeed(), *GetTransform().GetLocation().ToString(), *GetActorForwardVector().ToString(), *GetVehicleOrientation().ToString(), *GetTransform().Rotator().ToString())
	*/
	if (SumoData->client && ArrivedFlag) {
		UpdateSUMOByTickCount(Delta);
	}
	
	

}
void AWheeledVehiclePawn::EgoWheeledVehicleArrivedInSUMO() {
	if (client->simulation.getArrivedNumber() > 0) {
		std::vector<std::string> ArrivedVehicle= client->simulation.getArrivedIDList();
		if (std::count(ArrivedVehicle.begin(), ArrivedVehicle.end(), TCHAR_TO_UTF8(*EgoWheeledVehicleInformation.VehicleId))) {
			ArrivedFlag = false;
		}
	}
	
}
void AWheeledVehiclePawn::UpdateSUMOByTickCount(float Delta) {
	if (UnrealFPS.TickCount < UnrealFPS.UETickBetweenSUMOUpdates) {
		//UE_LOG(LogTemp, Display, TEXT("AWheeledVehiclePawn -> Tick() %d; # of tick between SUMOUpdate: %d"), UnrealFPS.TickCount, UnrealFPS.UETickBetweenSUMOUpdates)
		UnrealFPS.TickCount++;
	}
	else if (UnrealFPS.TickCount == UnrealFPS.UETickBetweenSUMOUpdates) {
		// UE_LOG(LogTemp, Warning, TEXT("%f :Update from SUMO. NextTimeToUpdate %f"), TimeInWorld, NextTimeToUpdate)
		// UE_LOG(LogTemp, Warning, TEXT("AWheeledVehiclePawn -> WheeledVehicle Tick() %d. Update from SUMo. # of tick between SUMOUpdate: %d"), UnrealFPS.TickCount, UnrealFPS.UETickBetweenSUMOUpdates)
		EgoWheeledVehicleArrivedInSUMO();
		UpdateEgoWheeledVehicleToSUMO(Delta);
		UnrealFPS.TickCount = 1;
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Tick calculation is wrong."))
	}

}

void AWheeledVehiclePawn::UpdateEgoWheeledVehicleToSUMO(float Delta) {
	if (ArrivedFlag) {
		float VehicleSpeed = GetVehicleForwardSpeed() / 100; // Unreal speed unit cm/s -> Sumo speed unit m/s
		// How to solve negative speed
		if (VehicleSpeed > 0.0000001) {
			client->vehicle.setSpeed(TCHAR_TO_UTF8(*EgoWheeledVehicleInformation.VehicleId), VehicleSpeed);
		}

		auto VehicleAngle = GetActorRotation().Yaw;
		VehicleAngle_ = GetActorRotation().Yaw + 90;

		if (VehicleAngle_ < 0) {
			VehicleAngle_ += 360;
		}

		// Retrevie ego wheeled vehicle position, can also get by GetActorLocation() function;
		VehiclePositionInWorld = GetTransform().GetLocation();
		
		UE_LOG(LogTemp, Error, TEXT("Get location: %s ; VehicleAngle: %f ; VehicleAngle_: %f "), *VehiclePositionInWorld.ToString(), VehicleAngle, VehicleAngle_)
		std::string CurrentEdgeId = client->vehicle.getRoadID(TCHAR_TO_UTF8(*EgoWheeledVehicleInformation.VehicleId));
		int CurrentLaneId = client->vehicle.getLaneIndex(TCHAR_TO_UTF8(*EgoWheeledVehicleInformation.VehicleId));
		std::cout << TCHAR_TO_UTF8(*EgoWheeledVehicleInformation.VehicleId) << ";" << CurrentEdgeId << ";" << CurrentLaneId << ";" << VehiclePositionInWorld.X << "," << VehiclePositionInWorld.Y << "," << VehicleAngle_ << "\n";
		client->vehicle.moveToXY(TCHAR_TO_UTF8(*EgoWheeledVehicleInformation.VehicleId), CurrentEdgeId, CurrentLaneId, VehiclePositionInWorld.X /100, VehiclePositionInWorld.Y/-100, VehicleAngle_, 0);
		 UE_LOG(LogTemp, Error, TEXT("Update to SUMO.Speed: %f"), VehicleSpeed)
		// UE_LOG(LogTemp, Error, TEXT("Get speed from sumo: %f"), client->vehicle.getSpeed(TCHAR_TO_UTF8(*EgoWheeledVehicleInformation.VehicleId)))
	}
	
}

void AWheeledVehiclePawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AWheeledVehiclePawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWheeledVehiclePawn::MoveRight);
	PlayerInputComponent->BindAxis("LookUp");
	PlayerInputComponent->BindAxis("LookRight");

	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &AWheeledVehiclePawn::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AWheeledVehiclePawn::OnHandbrakeReleased);
	PlayerInputComponent->BindAction("SwitchCamera", IE_Pressed, this, &AWheeledVehiclePawn::OnToggleCamera);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AWheeledVehiclePawn::OnResetVR);
}

void AWheeledVehiclePawn::MoveForward(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void AWheeledVehiclePawn::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void AWheeledVehiclePawn::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AWheeledVehiclePawn::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void AWheeledVehiclePawn::OnToggleCamera()
{
	EnableIncarView(!bInCarCameraActive);
}

void AWheeledVehiclePawn::EnableIncarView(const bool bState, const bool bForce)
{
	if ((bState != bInCarCameraActive) || (bForce == true))
	{
		bInCarCameraActive = bState;

		if (bState == true)
		{
			OnResetVR();
			Camera->Deactivate();
			InternalCamera->Activate();
		}
		else
		{
			// InternalCamera->Deactivate();
			Camera->Activate();
		}

		// InCarSpeed->SetVisibility(bInCarCameraActive);
		// InCarGear->SetVisibility(bInCarCameraActive);
	}
}

void AWheeledVehiclePawn::OnResetVR()
{
//#if HMD_MODULE_INCLUDED
//	/*if (GEngine->HMDDevice.IsValid())
//	{
//		GEngine->HMDDevice->ResetOrientationAndPosition();
//		InternalCamera->SetRelativeLocation(InternalCameraOrigin);
//		GetController()->SetControlRotation(FRotator());
//	}*/
//
//	if (GEngine->XRSystem.IsValid())
//	{
//		GEngine->XRSystem->ResetOrientationAndPosition();
//		InternalCamera->SetRelativeLocation(InternalCameraOrigin);
//		GetController()->SetControlRotation(FRotator());
//	}
//#endif // HMD_MODULE_INCLUDED
}

void AWheeledVehiclePawn::UpdateHUDStrings()
{
	float KPH = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
	int32 KPH_int = FMath::FloorToInt(KPH);

	// Using FText because this is display text that should be localizable
	SpeedDisplayString = FText::Format(LOCTEXT("SpeedFormat", "{0} km/h"), FText::AsNumber(KPH_int));

	if (bInReverseGear == true)
	{
		GearDisplayString = FText(LOCTEXT("ReverseGear", "R"));
	}
	else
	{
		int32 Gear = GetVehicleMovement()->GetCurrentGear();
		GearDisplayString = (Gear == 0) ? LOCTEXT("N", "N") : FText::AsNumber(Gear);
	}
}

void AWheeledVehiclePawn::SetupInCarHUD()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if ((PlayerController != nullptr) && (InCarSpeed != nullptr) && (InCarGear != nullptr))
	{
		// Setup the text render component strings
		InCarSpeed->SetText(SpeedDisplayString);
		InCarGear->SetText(GearDisplayString);

		if (bInReverseGear == false)
		{
			InCarGear->SetTextRenderColor(GearDisplayColor);
		}
		else
		{
			InCarGear->SetTextRenderColor(GearDisplayReverseColor);
		}
	}
}
//
float AWheeledVehiclePawn::GetVehicleForwardSpeed() const
{
	return GetVehicleMovementComponent()->GetForwardSpeed();
}


FVector AWheeledVehiclePawn::GetVehicleOrientation() const
{
	return GetVehicleTransform().GetRotation().GetForwardVector();
}

int32 AWheeledVehiclePawn::GetVehicleCurrentGear() const
{
	return GetVehicleMovementComponent()->GetCurrentGear();
}

FTransform AWheeledVehiclePawn::GetVehicleBoundingBoxTransform() const
{
	return VehicleBounds->GetRelativeTransform();
}

FVector AWheeledVehiclePawn::GetVehicleBoundingBoxExtent() const
{
	return VehicleBounds->GetScaledBoxExtent();
}

float AWheeledVehiclePawn::GetMaximumSteerAngle() const
{
	const auto &Wheels = GetVehicleMovementComponent()->Wheels;
	check(Wheels.Num() > 0);
	const auto *FrontWheel = Wheels[0];
	check(FrontWheel != nullptr);
	return FrontWheel->SteerAngle;
}

