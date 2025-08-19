// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class CARPENTER_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MoneyText;
		
	UPROPERTY(meta = (BindWidget))
	class UImage* Crosshair;

public:
	void SetMoney(int32 NewMoney);
	
};
