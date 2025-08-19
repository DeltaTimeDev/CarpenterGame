// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ProductCatalogWidget.h"

#include "Components/Button.h"
#include "Components/ListView.h"
#include "Gameplay/Machines/ShapingMachine.h"
#include "Widgets/ProductWidget.h"

void UProductCatalogWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(ProductList)
	{
		ProductList->OnItemClicked().AddUObject(this, &UProductCatalogWidget::OnItemClicked);
	}

	if(ActionButton)
	{
		ActionButton->OnPressed.AddDynamic(this, &UProductCatalogWidget::OnButtonClicked);
	}
}

void UProductCatalogWidget::SelectProductByID(FGuid ProductID)
{
	SetProductWidgetsSelectedByID(ProductID);
}

void UProductCatalogWidget::OnProductSelected(UObject* Item)
{
	if (UProduct* SelectedProduct = Cast<UProduct>(Item))
	{
		OnRequestForChangeSelection.Broadcast(SelectedProduct);
	}
}

void UProductCatalogWidget::OnItemClicked(UObject* Object)
{
	if (UProduct* SelectedProduct = Cast<UProduct>(Object))
	{
		OnRequestForChangeSelection.Broadcast(SelectedProduct);
	}
}

void UProductCatalogWidget::SetProductWidgetsSelectedByID(FGuid SelectedProductID)
{
	
	for(UObject* ListItem : ProductList->GetListItems())
	{
		if (UProduct* Product = Cast<UProduct>(ListItem))
		{
			if (UProductWidget* ProductWidget = Cast<UProductWidget>(ProductList->GetEntryWidgetFromItem(ListItem)))
			{
				if (Product->Data.TypeID == SelectedProductID)
				{
					ProductWidget->SetSelected(true);
					CurrentSelectedProduct = Product;
				}
				else
				{
					ProductWidget->SetSelected(false);
				}
			}
		}
	}
}

void UProductCatalogWidget::SetProductList(TArray<FProductData> Products)
{
	for (FProductData ProductData : Products)
	{
		UProduct* NewProductObject = NewObject<UProduct>(this);
		NewProductObject->Data = ProductData;
		AddProduct(NewProductObject);
	}
}

void UProductCatalogWidget::AddProduct(UProduct* Product)
{
	if(ProductList && Product)
	{
		ProductList->AddItem(Product);
	}
}

void UProductCatalogWidget::OnButtonClicked()
{
	if(CurrentSelectedProduct)
	{
		UE_LOG(LogTemp, Log, TEXT("Selected Product: %s"), *CurrentSelectedProduct->Data.Name);
		OnCreateCommand.Broadcast(CurrentSelectedProduct);
	}
}



