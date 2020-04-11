// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Bonus/Door.h"
#include "Engine/World.h"
#include <Runtime\Engine\Public\DrawDebugHelpers.h>
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *(GetOwner()->GetName()))
	}
}

void UGrabber::SetupInputComponent()	// Look for attached Input Component
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{	
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);		// Bind the input axis
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Released);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Input component"), *(GetOwner()->GetName()))
	}
}

void UGrabber::Grab()
{
	// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResults = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResults.GetComponent(); //Gets the mesh in our case
	auto ActorHit = HitResults.GetActor();
	
	// If we hit something then attach a physics handle
	if (ActorHit)
	{
		// attach physics hanle
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, GetOwner()->GetActorLocation(), true /*allows rotation*/);
	}
}

void UGrabber::Released()
{
	// release physics handle
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
		// If the Physics Handle is attached
		if (PhysicsHandle->GrabbedComponent)
		{
			// then move the object that we're holding
			PhysicsHandle->SetTargetLocation(GetReachLineEnd());
		}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	// Line Trace (AKA Ray-cast) out to reach distance
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return HitResult;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return  PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	return LineTraceEnd;
}