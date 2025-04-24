// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/D1PlayerHUDWidget.h"
#include "UI/D1HpBarWidget.h"
#include "UI/D1CharacterStatWidget.h"

UD1PlayerHUDWidget::UD1PlayerHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UD1PlayerHUDWidget::UpdateStat(const FD1CharacterStat& BaseStat , const FD1CharacterStat& ModifierStat)
{
	WidgetHpBar->UpdateStat(BaseStat , ModifierStat);
	WidgetCharacterStat->UpdateStat(BaseStat , ModifierStat);
}

void UD1PlayerHUDWidget::UpdateHp(float NewCurrentHp)
{
	WidgetHpBar->UpdateHp(NewCurrentHp);
}
