// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/D1CharacterStatWidget.h"
#include "GameData/D1CharacterStat.h"
#include "Components/TextBlock.h"

UD1CharacterStatWidget::UD1CharacterStatWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UD1CharacterStatWidget::UpdateStat(const FD1CharacterStat& BaseStat , const FD1CharacterStat& ModifierStat)
{
	MaxHpBase->SetText(FText::FromString(FString::SanitizeFloat(BaseStat.MaxHp)));
	AttackBase->SetText(FText::FromString(FString::SanitizeFloat(BaseStat.Attack)));
	AttackRangeBase->SetText(FText::FromString(FString::SanitizeFloat(BaseStat.AttackRange)));
	AttackRadiusBase->SetText(FText::FromString(FString::SanitizeFloat(BaseStat.AttackRadius)));
	AttackSpeedBase->SetText(FText::FromString(FString::SanitizeFloat(BaseStat.AttackSpeed)));
	MovementSpeedBase->SetText(FText::FromString(FString::SanitizeFloat(BaseStat.MovementSpeed)));

	MaxHpModifier->SetText(FText::FromString(FString::SanitizeFloat(ModifierStat.MaxHp)));
	AttackModifier->SetText(FText::FromString(FString::SanitizeFloat(ModifierStat.Attack)));
	AttackRangeModifier->SetText(FText::FromString(FString::SanitizeFloat(ModifierStat.AttackRange)));
	AttackRadiusModifier->SetText(FText::FromString(FString::SanitizeFloat(ModifierStat.AttackRadius)));
	AttackSpeedModifier->SetText(FText::FromString(FString::SanitizeFloat(ModifierStat.AttackSpeed)));
	MovementSpeedModifier->SetText(FText::FromString(FString::SanitizeFloat(ModifierStat.MovementSpeed)));
}
