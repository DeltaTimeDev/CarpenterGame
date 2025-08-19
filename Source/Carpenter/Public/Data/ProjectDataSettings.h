// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ProjectDataSettings.generated.h"

/**
 * 
 */
UCLASS(config = Game, defaultconfig)
class CARPENTER_API UProjectDataSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Products")
	 TSoftObjectPtr<class UProductDataAsset> Products;
};
