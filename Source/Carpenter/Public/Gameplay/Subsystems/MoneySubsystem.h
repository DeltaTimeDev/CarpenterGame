// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MoneySubsystem.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoneyValueDelegate, int32, Money);

UCLASS()
class CARPENTER_API UMoneySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void Spend(int32 Value);

	UFUNCTION()
	void Earn(int32 Value);
	
	UPROPERTY()
	int32 MoneyCount = 100;

	UPROPERTY(BlueprintAssignable)
	FMoneyValueDelegate OnMoneyChanged;
	
};
