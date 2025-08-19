// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Gameplay/Interfaces/InteractableInterface.h"
#include "PaintCan.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectPaintCanDelegate, FLinearColor, Color);


/**
 * 
 */
UCLASS()
class CARPENTER_API UPaintCan : public UStaticMeshComponent, public IInteractableInterface
{
	GENERATED_BODY()

public:
	void SetColor(FLinearColor NewColor);

	void SelectCan();

	//virtual void Interact(APlayerController* PlayerController) override;
	virtual void InteractStart(class ACarpenterCharacter* CarpenterCharacter) override;
	virtual void InteractEnd(class ACarpenterCharacter* CarpenterCharacter) override;

	FLinearColor Color;

	UPROPERTY(BlueprintAssignable)
	FSelectPaintCanDelegate OnPaintCanSelected;

};
