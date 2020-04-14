// Fill out your copyright notice in the Description page of Project Settings.


#include "SumoPlayerController.h"

ASumoPlayerController::ASumoPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{	
	client = nullptr;
};
//void ASumoPlayerController::Possess(APawn* InPawn)
//{
//	Super::Possess(InPawn);
//	UE_LOG(LogTemp, Warning, TEXT("HELP HELP I'VE BEEN POSSESSED!  THE CULPRIT IS %s"), *InPawn->GetName());
//}

void ASumoPlayerController::SetSUMOSocket(Client* client) {
	this->client = client;
	APawn* controller = GetPawn();
	// UE_LOG(LogTemp,Error, TEXT("%s"), *controller->GetName());
}