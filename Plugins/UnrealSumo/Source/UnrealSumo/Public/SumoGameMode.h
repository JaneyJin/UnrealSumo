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


UCLASS()
class ASumoGameMode : public AGameMode
{
	GENERATED_BODY()


public:
	/// Variables modified in BeginPlay()
	UPROPERTY(EditAnywhere, Category = "SUMO Setup", meta = (ToolTip = "Port number for connection with SUMO server. \nIf the port number is empty or invalid, the UE will set to the default port number 24000"))
		int32 PortNumber;

	UPROPERTY(EditAnywhere, Category = "SUMO Setup", meta = (ToolTip = "Match the Unreal FPS with SUMO server."))
		bool UpdateByMachineTime;
	// Creation of an Client instance
	// Instantiate a socket and connect to SUMO server
	Client client;
	// Flag to check whether Unreal is connecting to SUMO server
	// If Unreal fail to connect to SUMO server or SUMO has all of vehicles finish the trip, UE will not get crash 
	bool SocketIsNotClosed = false;

	/// Variables modified in MatchFrameRatePerSecond()
	FrameRateSyn SUMOToUnrealFrameRate;

	// SUMO server FPS
	double SUMODeltaT = 0;


	/// Variables modified in Tick()
	// Unreal time after start play the game
	float TimeInWorld = 0;


	/// Variables modified in UpdateFromSUMO()
	// Record SUMo server current step and time in Unreal project 
	int32 SUMOStep = 0;
	double SUMOTime = 0;
	int DepartedNumber = 0;
	std::vector<std::string> DepartedList;


	// Information retrieval from SUMO server
	FVehicleInformation SUMOVehicleInformation;
	// Turn SUMO meters into 
	int32 MeterUnitConversion = 100;
	// Current Departed Vehicle Id. Used to retrieval information
	std::string DepartedVehicleId = "";
	libsumo::TraCIPosition DepartedVehiclePos;
	libsumo::TraCIColor DepartedVehicleColor;


	/// Variables modified in SpawnRandomVehicle()
	FVector SpawnPoint;
	FRotator SpawnRotator;
	// Spawn random vehicle from a list of BP class
	UClass* selectedClass;
	/// UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SUMo Setup") 
	/*UPROPERTY(EditAnywhere, Category = "SUMO Setup")
		TArray<TSubclassOf<ACustomVehicle>> VehicleBPList;

	ACustomVehicle* RandomVehicle = nullptr;*/


	UPROPERTY(EditAnywhere, Category = "SUMO Setup", meta = (ToolTip = "Wheeled vehicle with custom SumoWheeledVehicle class"))
		TArray<TSubclassOf<ASumoWheeledVehicle>> WheeledVehicleBPList;

	ASumoWheeledVehicle* RandomWheeledVehicle = nullptr;

	AWheeledVehiclePawn* EgoWheeledVehicle = nullptr;

	UPROPERTY(EditAnywhere, Category = "SUMO Setup| EgoVehicle Setup", meta = (ToolTip = "Match the route id specify in *.rou.xml. \nExample: <route id=\"route0\" ...> "))
		FString RouteId;
	
	FVehicleInformation EgoWheeledVehicleInformation;

	// Note CanEditChange is only available when compiling with the editor. Must add this or your builds might not work!
#if WITH_EDITOR
	virtual bool CanEditChange(const UProperty* InProperty) const override;
#endif


	/**
	 * Unreal get the frame rate from SUMO and re-align Unreal time to update from SUMO
	 * @param
	 * @return
	 */
	void MatchFrameRatePerSecond();


	bool SetupEgoVehicle();

	/**
	 * Update Unreal from SUMO based on Tick counter
	 * @param
	 * @return
	 */
	void UpdateSUMOByTickCount();

	/**
	 * Update Unreal from SUMO based on relative unreal project time (TimeInWorld)
	 * @param
	 * @return
	 */
	void UpdateSUMOByMachineTime();

	/**
	 * TODO
	 * @param
	 * @return
	 */
	void UpdateVehicleFromSUMO();

	void UpdateEgoWheeledVehicleToSUMO();

	/**
	 * Spawn random vehicle from the VehicleBPList in SumoDefaultPawn_BP in the UE
	 * @param  vehicle Vehicle to spawn
	 * @return    1 on success, -1 on fail
	 */
	bool SpawnRandomVehicle(FVehicleInformation& DepartedVehicle);


	/**
	 * Spawn random vehicle from the WheeledVehicleBPList in SumoDefaultPawn_BP in the UE
	 * @param  vehicle Vehicle to spawn
	 * @return    1 on success, -1 on fail
	 */
	bool SpawnRandomWheeledVehicle(FVehicleInformation& DepartedVehicle);

public:
	// Override original function defined by Unreal Engine
	ASumoGameMode(const FObjectInitializer& ObjectInitializer);
	/// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
