// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGameMode.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavMeshBoundsVolume.h"
#include "ActorPool.h"
AInfiniteTerrainGameMode::AInfiniteTerrainGameMode() :
	Super(),
	NavMeshBoundsPool{ CreateDefaultSubobject<UActorPool>(FName{"Nav Mesh Bounds Volume Pool" }) }
{
}
void AInfiniteTerrainGameMode::PopulateBoundsInPool()
{
	TActorIterator<ANavMeshBoundsVolume> Iterator{ GetWorld() };
	while (Iterator)
	{
		ANavMeshBoundsVolume* Volume{ *Iterator };
		AddToPool(Volume);
		++Iterator;
	}
}

void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume * ToAdd)
{
	NavMeshBoundsPool->Return(ToAdd);
}
