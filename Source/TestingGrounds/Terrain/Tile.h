// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATile();
	UFUNCTION(BlueprintCallable)
	void PlaceActors(const TSubclassOf<AActor>& ToSpawn, int32 MaxSpawnNumber, int32 MinSpawnNumber = 0);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	FVector MinSpawnBound = FVector{ 0.0f,-2000.0f,200.0f };
	UPROPERTY(EditDefaultsOnly, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	FVector MaxSpawnBound = FVector{ 4000.0f, 2000.0f,200.0f };

};
