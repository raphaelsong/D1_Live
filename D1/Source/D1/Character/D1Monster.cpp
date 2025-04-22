// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/D1Monster.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/D1AIController.h"

AD1Monster::AD1Monster()
{
	AIControllerClass = AD1AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("D1Monster"));

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/_Art/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (MeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
}

void AD1Monster::BeginPlay()
{
	Super::BeginPlay();
}

void AD1Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AD1Monster::SetDead()
{
	Super::SetDead();

	AD1AIController* AIController = Cast<AD1AIController>(GetController());
	if (AIController)
	{
		AIController->StopAI();
	}

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle , FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	) , DeadEventDelayTime , false);
}

float AD1Monster::GetAttackRange()
{
	return 150.0f;
}

void AD1Monster::AttackByAI()
{
	ProcessComboAttack();
}

void AD1Monster::ComboAttackEnd(UAnimMontage* TargetMontage , bool IsProperlyEnded)
{
	Super::ComboAttackEnd(TargetMontage , IsProperlyEnded);

	// AttackTask한테 공격 끝남 알림
	OnAttackFinished.ExecuteIfBound();
}

void AD1Monster::SetAIAttackFinishedDelegate(const FAIAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}
