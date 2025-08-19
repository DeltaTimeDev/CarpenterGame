// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Subsystems/MoneySubsystem.h"

void UMoneySubsystem::Spend(int32 Value)
{
	MoneyCount -= Value;
	OnMoneyChanged.Broadcast(MoneyCount);
}

void UMoneySubsystem::Earn(int32 Value)
{
	MoneyCount += Value;
	OnMoneyChanged.Broadcast(MoneyCount);
}
