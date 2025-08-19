// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MachineBase.h"
#include "OrderTable.generated.h"


class AOrderActor;
class UProductDataAsset;

UCLASS()
class CARPENTER_API AOrderTable : public AMachineBase
{
	GENERATED_BODY()

public:
	AOrderTable();

protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, Category = "Orders")
	TSubclassOf<AOrderActor> OrderActorClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Orders")
	TArray<FName> SpawnPointSocketNames;
	
	UPROPERTY(EditDefaultsOnly, Category = "Orders")
	TArray<FLinearColor> AvailableColors;
	
	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	TArray<AOrderActor*> SpawnedOrderActors;
	
	FTimerHandle OrderSpawnTimerHandle;
	
	void SpawnOrderActorAtSocket(const FName& SocketName);
	
	void HandleOrderTimerTick();
	
	FLinearColor GetRandomColor() const;
	
	AOrderActor* GetAvailableOrderActor() const;
	
};
