// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CarpenterInteractableWidgetComp.h"

#include "Components/WidgetInteractionComponent.h"
#include "Framework/CarpenterCharacter.h"


UCarpenterInteractableWidgetComp::UCarpenterInteractableWidgetComp()
{
	bInteractLocally = true;
}

void UCarpenterInteractableWidgetComp::InteractStart(class ACarpenterCharacter* CarpenterCharacter)
{
	CarpenterCharacter->GetWidgetInteractionComponent()->PressPointerKey(EKeys::LeftMouseButton);
}

void UCarpenterInteractableWidgetComp::InteractEnd(class ACarpenterCharacter* CarpenterCharacter)
{
	CarpenterCharacter->GetWidgetInteractionComponent()->ReleasePointerKey(EKeys::LeftMouseButton);
}
