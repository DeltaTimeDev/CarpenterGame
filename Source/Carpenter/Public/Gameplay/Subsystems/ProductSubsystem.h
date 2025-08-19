// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ProductSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CARPENTER_API UProductSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	void LoadProductDataAsset();

	FProductData GetRandomProduct() const;

	UPROPERTY(EditAnywhere, Category = "Orders")
	class UProductDataAsset* ProductDataAsset;
};
