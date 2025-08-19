// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Machines/OrderTable.h"

#include "Gameplay/Actors/OrderActor.h"
#include "Gameplay/Libraries/ProductFunctionLibrary.h"


AOrderTable::AOrderTable()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AvailableColors = { FLinearColor::Red, FLinearColor::Green };
}

void AOrderTable::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		
		for (const FName& SocketName : SpawnPointSocketNames)
		{
			SpawnOrderActorAtSocket(SocketName);
		}
		
		GetWorld()->GetTimerManager().SetTimer(
			OrderSpawnTimerHandle,
			this,
			&AOrderTable::HandleOrderTimerTick,
			3.0f,
			true
		);
	}
}

void AOrderTable::SpawnOrderActorAtSocket(const FName& SocketName)
{
	if (!OrderActorClass || !MachineMeshComponent || !MachineMeshComponent->DoesSocketExist(SocketName))
	{
		return;
	}

	const FTransform SpawnTransform = MachineMeshComponent->GetSocketTransform(SocketName);

	if (AOrderActor* NewOrderActor = GetWorld()->SpawnActorDeferred<AOrderActor>(OrderActorClass, SpawnTransform))
	{
		FOrder EmptyOrder;
		EmptyOrder.IsActive = false;

		NewOrderActor->SetOrder(EmptyOrder);
		NewOrderActor->FinishSpawning(SpawnTransform);

		SpawnedOrderActors.Add(NewOrderActor);
	}
}

void AOrderTable::HandleOrderTimerTick()
{
	AOrderActor* TargetOrderActor = GetAvailableOrderActor();
	if (!TargetOrderActor)
	{
		return; 
	}
	
	FProductData RandomProduct = UProductFunctionLibrary::GetRandomProduct(this);
	FOrder NewOrder;
	NewOrder.Name = RandomProduct.Name;
	NewOrder.TypeID = RandomProduct.TypeID;
	NewOrder.Color = GetRandomColor();
	NewOrder.IsActive = true;

	TargetOrderActor->SetOrder(NewOrder);
}

FLinearColor AOrderTable::GetRandomColor() const
{
	if (AvailableColors.Num() == 0)
	{
		return FLinearColor::White;
	}

	const int32 RandomIndex = FMath::RandRange(0, AvailableColors.Num() - 1);
	return AvailableColors[RandomIndex];
}

AOrderActor* AOrderTable::GetAvailableOrderActor() const
{
	for (AOrderActor* OrderActor : SpawnedOrderActors)
	{
		if (OrderActor && !OrderActor->Order.IsActive)
		{
			return OrderActor;
		}
	}
	return nullptr;
}

