// Fill out your copyright notice in the Description page of Project Settings.


#include "NetMesh.h"
#include "ProceduralMeshComponent.h"

// Sets default values
ANetMesh::ANetMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;
	// New in UE 4.17, multi-threaded PhysX cooking.
	mesh->bUseAsyncCooking = true;
	UE_LOG(LogTemp, Warning, TEXT("*** Constructor of ANetMesh ***"));

}

ANetMesh::~ANetMesh()
{}

// This is called when actor is spawned (at runtime or when you drop it into the world in editor)
void ANetMesh::PostActorCreated() {
	UE_LOG(LogTemp, Error, TEXT(">>>   PostActorCreated   <<<"));


}

// This is called when actor is already in level and map is opened
void ANetMesh::PostLoad() {
	UE_LOG(LogTemp, Error, TEXT(">>>   PostLoad   <<<"));




}

void ANetMesh::OnConstruction(const FTransform & Transform) {
	UE_LOG(LogTemp, Error, TEXT(">>>   OnConstruction   <<<"));




}

void ANetMesh::PostInitializeComponents() {
	UE_LOG(LogTemp, Error, TEXT(">>>   PostInitializeComponents   <<<"));




}


// Called when the game starts or when spawned
void ANetMesh::BeginPlay()
{
	// Super::BeginPlay();
	UE_LOG(LogTemp, Error, TEXT(">>>   BeginPlay   <<<"));
	CreateSection(0);
}

void ANetMesh::CreateSection(int32 indexa) {

}


