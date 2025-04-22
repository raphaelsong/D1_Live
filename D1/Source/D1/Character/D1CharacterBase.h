// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/D1AttackInterface.h"
#include "D1CharacterBase.generated.h"

struct FD1CharacterStat;

UCLASS()
class D1_API AD1CharacterBase : public ACharacter, public ID1AttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AD1CharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Damage , struct FDamageEvent const& DamageEvent , AController* EventInstigator , AActor* DamageCauser) override;

public:
	virtual void ProcessAttack();
	virtual void ProcessComboAttack();

	virtual void ComboAttackBegin();
	virtual void ComboAttackEnd(class UAnimMontage* TargetMontage , bool IsProperlyEnded);

	void SetComboCheckTimer();
	void ComboCheck();

	// Inherited via ID1AttackInterface
	void AttackHitCheck() override;

	virtual void SetDead();

	void ApplyStat(const FD1CharacterStat& BaseStat , const FD1CharacterStat& ModifierStat);

protected:
	UPROPERTY(EditAnywhere, Category = Attack)
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere , Category = Attack)
	TObjectPtr<class UAnimMontage> ComboAttackMontage;

	UPROPERTY(EditAnywhere , Category = Attack)
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, Category = Attack)
	TObjectPtr<class UD1ComboAttackData> ComboAttackData;

	int32 CurrentCombo = 0;

	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UD1CharacterStatComponent> StatComponent;
};
