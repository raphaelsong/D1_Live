// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/D1PlayerController.h"

AD1PlayerController::AD1PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AD1PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}

void AD1PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AD1PlayerController::GameScoreChanged(int32 NewScore)
{
	K2_OnScoreChanged(NewScore);
}

void AD1PlayerController::GameClear()
{
	K2_GameClear();
}

void AD1PlayerController::GameOver()
{
	K2_GameOver();
}
