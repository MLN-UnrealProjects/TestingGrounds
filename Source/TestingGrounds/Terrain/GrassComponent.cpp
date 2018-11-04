// Fill out your copyright notice in the Description page of Project Settings.

#include "GrassComponent.h"

void UGrassComponent::BeginPlay()
{
	Super::BeginPlay();

	int32 SpawnTimes{ FMath::RandRange(MinSpawnNumber,MaxSpawnNumber) };
	for (size_t i = 0; i < SpawnTimes; i++)
	{
		AddInstance(FTransform{ FRotator::ZeroRotator ,FMath::RandPointInBox(SpawnBounds) ,FVector::OneVector });
	}
}

void UGrassComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ClearInstances();
}
