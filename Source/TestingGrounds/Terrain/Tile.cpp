// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATile::PlaceActors(const TSubclassOf<AActor>& ToSpawn, int32 MaxSpawnNumber, int32 MinSpawnNumber)
{
	FBox Bounds{ MinSpawnBound,MaxSpawnBound };
	int32 NumToSpawn{ FMath::RandRange(MinSpawnNumber,MaxSpawnNumber) };
	for (size_t i = 0; i < NumToSpawn; i++)
	{
		AActor* SpawnedProp{ GetWorld()->SpawnActor(ToSpawn) };

		FVector SpawnPoint{ FMath::RandPointInBox(Bounds) };
		SpawnedProp->SetActorRelativeLocation(SpawnPoint);

		SpawnedProp->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	}
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

