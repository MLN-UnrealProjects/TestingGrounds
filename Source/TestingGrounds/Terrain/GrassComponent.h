// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GrassComponent.generated.h"

/**
 * Component that spawns several instances at beginplay
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TESTINGGROUNDS_API UGrassComponent : public UHierarchicalInstancedStaticMeshComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	int32 MinSpawnNumber = 1000;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	int32 MaxSpawnNumber = 1000;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	FBox SpawnBounds = FBox{ FVector{ 0.0f, -2000.0f, 0.0f }, FVector{ 4000.0f, 2000.0f, 0.0f } };

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
