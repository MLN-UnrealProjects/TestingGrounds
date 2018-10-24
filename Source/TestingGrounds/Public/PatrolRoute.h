// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolRoute.generated.h"

/*
* A Component that holds references to an array of actors that can be used as locations
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTINGGROUNDS_API UPatrolRoute : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPatrolRoute();

	TArray<AActor*> GetPatrolLocations() const { return PatrolLocations; };
private:
	UPROPERTY(EditAnywhere, Category = "Patrol Route", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> PatrolLocations;
};
