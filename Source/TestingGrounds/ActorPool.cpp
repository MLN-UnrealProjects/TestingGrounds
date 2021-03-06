// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"


// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	// ...
}

AActor * UActorPool::Checkout()
{
	return StoredElements.Num() == 0 ? nullptr : StoredElements.Pop();
}

void UActorPool::Return(AActor* ToReturn)
{
	StoredElements.Push(ToReturn);
}

void UActorPool::Clear()
{
	while (Checkout())
	{
	}
}