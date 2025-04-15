// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/D1Monster.h"
#include "Components/CapsuleComponent.h"
#include "AI/D1AIController.h"

AD1Monster::AD1Monster()
{
	AIControllerClass = AD1AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

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

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle , FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	) , DeadEventDelayTime , false);
}
