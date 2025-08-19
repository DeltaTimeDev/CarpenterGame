// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ProductWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/GenericStructs.h"
#include "Gameplay/Machines/ShapingMachine.h"

void UProductWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UProduct* Product = Cast<UProduct>(ListItemObject);
	if(Product)
	{
		ProductName->SetText(FText::FromString(Product->Data.Name));
		ProductImage->SetBrushFromTexture(Product->Data.Image);
		FText MoneyTextValue = FText::Format(NSLOCTEXT("UI", "MoneyFormat", "${0}"), FText::AsNumber(Product->Data.RawMaterialCost));
		ProductPrice->SetText(MoneyTextValue);
	}
	SetSelected(false);
}

void UProductWidget::SetSelected(bool bIsSelected)
{
	const FLinearColor BorderColor = bIsSelected ? SelectedColor : UnselectedColor;
	SelectionBorder->SetBrushColor(BorderColor);
}
