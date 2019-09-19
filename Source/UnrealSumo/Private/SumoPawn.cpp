// Fill out your copyright notice in the Description page of Project Settings.


#include "SumoPawn.h"

// Sets default values
ASumoPawn::ASumoPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASumoPawn::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp,Warning, TEXT("SumoPawn"))
}

// Called every frame
void ASumoPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASumoPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

