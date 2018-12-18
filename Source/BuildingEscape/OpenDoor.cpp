// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"

#define OUT

UOpenDoor::UOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	if (!DoorOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Door Owner not intialized!"))
		return;
	}

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pressure Plate not intialized!"))
	}
	DoorOwner = GetOwner();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOnPressurePlate() > TriggerMass)
	{
		OnOpenRequest.Broadcast();
	}
	else
	{ 
		OnCloseRequest.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOnPressurePlate()
{
	float TotalMass = 0.0f;

	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}