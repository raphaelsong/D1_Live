// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "D1PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class D1_API AD1PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AD1PlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnScoreChangedCpp"))
	void K2_OnScoreChanged(int32 NewScore);

	UFUNCTION(BlueprintImplementableEvent , Meta = (DisplayName = "OnGameClearCpp"))
	void K2_GameClear();

	UFUNCTION(BlueprintImplementableEvent , Meta = (DisplayName = "OnGameOverCpp"))
	void K2_GameOver();

public:
	void GameScoreChanged(int32 NewScore);
	void GameClear();
	void GameOver();
};
