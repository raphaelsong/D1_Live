// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/D1CharacterStatComponent.h"
#include "Singleton/D1GameSingleton.h"

// Sets default values for this component's properties
UD1CharacterStatComponent::UD1CharacterStatComponent()
{
	CurrentLevel = 1;
	bWantsInitializeComponent = true;
}


// Called when the game starts
void UD1CharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UD1CharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevel(CurrentLevel);
	SetHp(BaseStat.MaxHp);
}

float UD1CharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PreHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage , 0.0f , InDamage);

	SetHp(PreHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UD1CharacterStatComponent::SetLevel(int32 NewLevel)
{
	CurrentLevel = FMath::Clamp(NewLevel , 1 , UD1GameSingleton::Get().GetCharacterMaxLevel());
	SetBaseStat(UD1GameSingleton::Get().GetCharacterStat(CurrentLevel));
}

void UD1CharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp(NewHp , 0.0f , GetTotalStat().MaxHp);
	OnHpChanged.Broadcast(CurrentHp);
}
