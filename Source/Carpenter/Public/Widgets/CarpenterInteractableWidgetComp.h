// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Gameplay/Interfaces/InteractableInterface.h"
#include "CarpenterInteractableWidgetComp.generated.h"

/**
 * 
 */
UCLASS()
class CARPENTER_API UCarpenterInteractableWidgetComp : public UWidgetComponent, public IInteractableInterface
{
	GENERATED_BODY()

	public:
	UCarpenterInteractableWidgetComp();
	
	virtual void InteractStart(class ACarpenterCharacter* CarpenterCharacter) override;
	virtual void InteractEnd(class ACarpenterCharacter* CarpenterCharacter) override;
	
};
