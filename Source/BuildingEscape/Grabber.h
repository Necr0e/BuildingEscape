// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGrabber();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float PlayerReach = 100.0f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Raycast and grab item
	void Grab();

	// Release grabbed item
	void Release();

	// Find attached Physics Handle
	void FindPhysicsHandleComponent();

	// Setup attached input component
	void SetupInputComponents();

	// Return start and end of player's reach
	FVector GetReachLineStart();
	FVector GetReachLineEnd();

	// Return hit for first physics body hit
	const FHitResult GetFirstPhysicsBody();

};