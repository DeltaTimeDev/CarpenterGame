// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/CarpenterPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Gameplay/Machines/ShapingMachine.h"
#include "Gameplay/Subsystems/MoneySubsystem.h"
#include "Widgets/MainWidget.h"


void ACarpenterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		if (UUserWidget* MainWidget =  CreateWidget(this,MainWidgetClass))
		{
			MainWidget->AddToViewport();
			MainWidgetRef = Cast<UMainWidget>(MainWidget);
		}
		if (UMoneySubsystem* MoneySubsystem = Cast<UMoneySubsystem>(GetGameInstance()->GetSubsystem<UMoneySubsystem>()))
		{
			MoneySubsystem->OnMoneyChanged.AddDynamic(this, &ACarpenterPlayerController::OnMoneyChanged);
			if (IsValid(MainWidgetRef))
			{
				MainWidgetRef->SetMoney(MoneySubsystem->MoneyCount);
			}
		}
	}
}

void ACarpenterPlayerController::OnMoneyChanged(int32 NewMoneyValue)
{
	MainWidgetRef->SetMoney(NewMoneyValue);

	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		AController* Controller = It->Get();
		if (Controller)
		{
			if (ACarpenterPlayerController* PlayerController = Cast<ACarpenterPlayerController>(Controller))
			{
				PlayerController->ClientSetMoney(NewMoneyValue);
			}
		}
	}
}

void ACarpenterPlayerController::ClientSetMoney_Implementation(int32 NewMoney)
{
	MainWidgetRef->SetMoney(NewMoney);
}

void ACarpenterPlayerController::ServerSetProductShapingMachine_Implementation(AShapingMachine* ShapingMachine, FGuid SelectedProductID)
{
	ShapingMachine->SetSelectedProductOnServer(SelectedProductID);
}

void ACarpenterPlayerController::ServerSpawnProduct_Implementation(AShapingMachine* ShapingMachine)
{
	ShapingMachine->SpawnProduct();
}



