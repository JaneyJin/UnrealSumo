// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define LargestPortNumber 65535
#define DefaultPortNumber 24000


#include "CoreMinimal.h"
#include "Client.h" // TraCI object class
#include "VehicleInformation.h"
#include "FrameRateSyn.h"
#include "GameFramework/GameMode.h"

#include "WheeledVehiclePawn.h"
#include "SumoGameMode.generated.h" // Need include at last line

// Predefine class
class ASumoWheeledVehicle;
class AWheeledVehiclePawn;
class USumoGameInstance;

UCLASS()
class ASumoGameMode : public AGameMode
{
	GENERATED_BODY()


private:
	/// Variables modified in BeginPlay()
	// Creation of an Client instance
	// Instantiate a socket and connect to SUMO server
	Client client;
	

	/// Variables modified in SetupEgoVehicle()
	// Identity of ego wheeled vehicle in unreal engine and SUMO added vehicle. 
	FString EgoWheeledVehicleId;
	// Reference to the ego wheeled vehicle in Unreal Engine.
	AWheeledVehiclePawn* EgoWheeledVehicle = nullptr;


	/// Variables modified in UpdateFromSUMO()
	// Record SUMo server current step and time in Unreal project. 
	int DepartedNumber = 0;
	std::vector<std::string> DepartedList;

	// Information retrieval from SUMO server.
	FVehicleInformation SUMOVehicleInformation;
	// Turn SUMO meters into centermeters
	int32 MeterUnitConversion = 100;
	// Current Departed Vehicle Id. Used to retrieval information
	std::string DepartedVehicleId;
	libsumo::TraCIPosition DepartedVehiclePos;


	/// Variables modified in SpawnRandomVehicle()
	FVector SpawnPoint;
	FRotator SpawnRotator;
	// Spawn random vehicle from a list of BP class
	UClass* selectedClass;


	// Variables used by SumoGameMode, SumoWheeledVehicle and WheeledVehiclePawn class. Variables are modified in SumoGameMode.
	USumoGameInstance* SumoGameInstance;

	/**
	 * Setup ego wheeled vehicle in Unreal Engine, share connected socket and add an ego wheeled vehicle in SUMO
	 *
	 * @return   True on success set up above information, vice versa.
	 */
	bool SetupEgoWheeledVehicle();


	/**
	 * Unreal get the frame rate from SUMO and re-align Unreal time to update from SUMO. Related variables are saved to the shared variable class SumoGameInstance.
	 *
	 */
	void MatchFrameRatePerSecond();



	/**
	 * Setup the customized GameInstance shared between different object and
	 *
	 * @return   True on success set SumoGameInstance and initialize connected socket, vice versa.
	 */
	bool SetupGameInstance();

	/**
	 * Update Unreal from SUMO based on Tick counter
	 */
	void UpdateFromSUMOByTickCount();



	/**
	 * Get arrived (left scenario) and departed (get into scenario) vehicle in SUMO within one step
	 *
	 */
	void UpdateSimulationFromSUMO();

	/**
	 * Spawn random vehicle from the WheeledVehicleBPList
	 * Can be set in UnrealSumo Content -> Blueprint folder -> SumoGameMode_BP -> SUMO Setup section
	 *
	 * @param  Wheeled Vehicle to spawn
	 * @return   True on success spawn a wheeled vehicle and vice versa.
	 */
	bool SpawnRandomWheeledVehicle(FVehicleInformation& DepartedVehicle);

public:
	/// Variables modified in BeginPlay()
	UPROPERTY(EditAnywhere, Category = "SUMO Setup", meta = (ToolTip = "Port number for connection with SUMO server. \nIf the port number is empty or invalid, the UE will set to the default port number 24000"))
		int32 PortNumber;


	// Wheeled vehicles controlled by SUMO vehicle. All of the vehicles selected are inherited from ASumoWheeledVehicle.
	UPROPERTY(EditAnywhere, Category = "SUMO Setup", meta = (ToolTip = "Wheeled vehicle with custom SumoWheeledVehicle class"))
		TArray<TSubclassOf<ASumoWheeledVehicle>> WheeledVehicleBPList;


	/*UPROPERTY(EditAnywhere, Category = "SUMO Setup")
		bool AllowSpawnEgoVehicle;

	UPROPERTY(EditAnywhere, Category = "SUMO Setup", meta = (editcondition = "AllowSpawnEgoVehicle"))
		TSubclassOf<AWheeledVehiclePawn> EgoWheeledVehicle;*/


	// Route Id for ego wheeled vehicle to start.
	UPROPERTY(EditAnywhere, Category = "SUMO Setup| EgoVehicle Setup", meta = (ToolTip = "Match the route id specify in *.rou.xml. \nExample: <route id=\"route0\" ...> "))
		FString RouteId;
	
	
	/**
	 * Constructor for SumoGameMode.
	 */
	ASumoGameMode(const FObjectInitializer& ObjectInitializer);
	

	/**
	 * Override default BeginPlay function. Called when the game starts or when objects are spawned
	 */
	virtual void BeginPlay() override;

	/**
	 * Override default Tick function. This function is called every frame.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Override default Tick function. 
	 * Note CanEditChange is only available when compiling with the editor. Must add this or your builds might not work!
	 *
	 */
#if WITH_EDITOR
	virtual bool CanEditChange(const UProperty* InProperty) const override;
#endif
	

	// NOT USED
	/// Variables modified in UpdateSUMOByMachineTime()
	// Unreal time after start play the game
	float TimeInWorld = 0;
	/**
	 * NOT USED
	 * Update Unreal from SUMO based on relative unreal project time (TimeInWorld)
	 * @param
	 * @return
	 */
	void UpdateFromSUMOByMachineTime();

};
