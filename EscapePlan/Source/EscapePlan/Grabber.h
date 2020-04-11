// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine\PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class ESCAPEPLAN_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// How far ahed of the player can we reach in cm
	float Reach = 100.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Returns current Start of reach line
	FVector GetReachLineStart();

	// Returns current End of reach line
	FVector GetReachLineEnd();

	// Ray-cast and grab what's in reach
	void Grab();

	// called when grab released
	void Released();

	// find attached physics handle
	void FindPhysicsHandleComponent();

	// setup input component
	void SetupInputComponent();

	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

};
