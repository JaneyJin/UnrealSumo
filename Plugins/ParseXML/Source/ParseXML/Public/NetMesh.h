// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NetMesh.generated.h"

class UProceduralMeshComponent;

UCLASS()
class PARSEXML_API ANetMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANetMesh();
	~ANetMesh();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent * mesh;

	// PostActorCreated/PostLoad -> OnConstruction -> PostInitializeComponent -> BeginPlay
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;
	virtual void OnConstruction(const FTransform & Transform) override;
	virtual void PostInitializeComponents() override;


	void CreateSection(int32 indexa);
};
