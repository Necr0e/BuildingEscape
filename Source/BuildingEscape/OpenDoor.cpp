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


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	DoorOwner = GetOwner();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOnPressurePlate() > 50.0f)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

void UOpenDoor::OpenDoor()
{
	DoorOwner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	DoorOwner->SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
}

float UOpenDoor::GetTotalMassOnPressurePlate()
{
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	float TotalMass = 0.0f;

	for (auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}



