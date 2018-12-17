// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

#define OUT

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponents();

}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handler detected"))
	}
}

void UGrabber::SetupInputComponents()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Input Controller Handler detected"))
	}
}

const FHitResult UGrabber::GetFirstPhysicsBody()
{
	//Calculate line trace
	FHitResult LineTraceHit;
	FCollisionQueryParams TraceParameters(FName(""), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT LineTraceHit, 
		GetReachLineStart(), 
		GetReachLineEnd(), 
		ECollisionChannel::ECC_PhysicsBody, 
		TraceParameters);

	return LineTraceHit;
}

//Grab object within reach
void UGrabber::Grab()
{
	auto HitResult = GetFirstPhysicsBody();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		//Attach physics handler
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation());
	}
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewpointRotation);

	return PlayerViewpointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewpointRotation);

	return PlayerViewpointLocation + PlayerViewpointRotation.Vector() * PlayerReach;
}
