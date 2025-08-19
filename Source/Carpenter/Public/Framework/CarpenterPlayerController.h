// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CarpenterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CARPENTER_API ACarpenterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> MainWidgetClass;

	UPROPERTY()
	class UMainWidget* MainWidgetRef;

	UFUNCTION()
	void OnMoneyChanged(int32 NewMoneyValue);
	
	UFUNCTION(Client,Reliable)
	void ClientSetMoney(int32 NewMoney);
	
	UFUNCTION(Server,Reliable)
	void ServerSetProductShapingMachine(class AShapingMachine* ShapingMachine, FGuid SelectedProductID);

	UFUNCTION(Server,Reliable)
	void ServerSpawnProduct(class AShapingMachine* ShapingMachine);
	
};
