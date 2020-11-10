// Fill out your copyright notice in the Description page of Project Settings.


#include "TrafficSignBase.h"


// Sets default values
ATrafficSignBase::ATrafficSignBase(const FObjectInitializer &ObjectInitializer)  : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneRootComponent"));
    RootComponent->SetMobility(EComponentMobility::Static);
}

// Called when the game starts or when spawned
//void ATrafficSignBase::BeginPlay()
//{
//	Super::BeginPlay();
//
//}
