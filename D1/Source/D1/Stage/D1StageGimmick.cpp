// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/D1StageGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/OverlapResult.h"
#include "Character/D1Monster.h"
#include "Item/D1ItemBox.h"

// Sets default values
AD1StageGimmick::AD1StageGimmick()
{
	// Stage Section
	StageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StageMesh"));
	SetRootComponent(StageMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/_Art/Environment/Stages/SM_SQUARE.SM_SQUARE'"));
	if (StageMeshRef.Succeeded())
	{
		StageMesh->SetStaticMesh(StageMeshRef.Object);
	}

	StageTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTriggerBox"));
	StageTriggerBox->SetupAttachment(GetRootComponent());
	StageTriggerBox->SetBoxExtent(FVector(775.0f , 775.0f , 300.0f));
	StageTriggerBox->SetRelativeLocation(FVector(0.0f , 0.0f , 250.0f));
	StageTriggerBox->SetCollisionProfileName(FName("D1Trigger"));
	StageTriggerBox->OnComponentBeginOverlap.AddDynamic(this , &AD1StageGimmick::OnStageTriggerBoxBeginOverlap);

	// Gate Section
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/_Art/Environment/Props/SM_GATE.SM_GATE'"));

	if (GateMeshRef.Succeeded() == false)
		UE_LOG(LogTemp , Log , TEXT("SM_GATE Not Find!"));

	static FName GateSockets[] = { TEXT("+XGate"), TEXT("-XGate"), TEXT("+YGate"), TEXT("-YGate") };
	for (FName GateSocket : GateSockets)
	{
		UStaticMeshComponent* GateMesh = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);
		GateMesh->SetStaticMesh(GateMeshRef.Object);
		GateMesh->SetupAttachment(StageMesh , GateSocket);
		GateMesh->SetRelativeLocation(FVector(0.0f , -80.0f , 0.0f));
		GateMesh->SetRelativeRotation(FRotator(0.0f , -90.0f , 0.0f));
		GateMeshes.Add(GateSocket , GateMesh);

		FName TriggerBoxName = *GateSocket.ToString().Append(TEXT("TriggerBox"));
		UBoxComponent* GateTriggerBox = CreateDefaultSubobject<UBoxComponent>(TriggerBoxName);
		GateTriggerBox->SetupAttachment(StageMesh , GateSocket);
		GateTriggerBox->SetBoxExtent(FVector(100.0f , 100.0f , 300.0f));
		GateTriggerBox->SetRelativeLocation(FVector(70.0f , 0.0f , 250.0f));
		GateTriggerBox->SetCollisionProfileName(FName("D1Trigger"));
		GateTriggerBox->ComponentTags.Add(GateSocket);
		GateTriggerBox->OnComponentBeginOverlap.AddDynamic(this , &AD1StageGimmick::OnGateTriggerBoxBeginOverlap);
		GateTriggerBoxes.Add(GateTriggerBox);
	}

	// GameState
	CurrentState = EStageState::READY;

	StateChangeActions.Add(EStageState::READY , FOnStateChangedDelegate::CreateUObject(this , &AD1StageGimmick::SetReady));
	StateChangeActions.Add(EStageState::FIGHT , FOnStateChangedDelegate::CreateUObject(this , &AD1StageGimmick::SetFight));
	StateChangeActions.Add(EStageState::REWARD , FOnStateChangedDelegate::CreateUObject(this , &AD1StageGimmick::SetChooseReward));
	StateChangeActions.Add(EStageState::NEXT , FOnStateChangedDelegate::CreateUObject(this , &AD1StageGimmick::SetChooseNext));

	// FightState 
	static ConstructorHelpers::FClassFinder<AD1Monster> MonsterClassRef(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_D1Monster.BP_D1Monster_C'"));
	if (MonsterClassRef.Succeeded())
	{
		MonsterClass = MonsterClassRef.Class;
	}

	// RewardState
	static FName RewardSockets[] = {TEXT("+XReward"), TEXT("-XReward"), TEXT("+YReward"), TEXT("-YReward")};

	for (FName RewardSocket : RewardSockets)
	{
		FVector BoxLocation = StageMesh->GetSocketLocation(RewardSocket);
		RewardBoxLocations.Add(RewardSocket , BoxLocation);
	}
}

void AD1StageGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetState(CurrentState);
}

void AD1StageGimmick::OnStageTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	SetState(EStageState::FIGHT);
}

void AD1StageGimmick::OnGateTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	check(OverlappedComponent->ComponentTags.Num() == 1);
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	check(StageMesh->DoesSocketExist(SocketName));

	FVector NewLocation = StageMesh->GetSocketLocation(SocketName);
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam;
	CollisionQueryParam.AddIgnoredActor(this);

	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults ,
		NewLocation ,
		FQuat::Identity ,
		FCollisionObjectQueryParams::InitType::AllObjects ,
		FCollisionShape::MakeSphere(775.0f) ,
		CollisionQueryParam
	);

	if (!bResult)
	{
		FTransform  NewTransform(NewLocation);
		AD1StageGimmick* NewGimmick = GetWorld()->SpawnActorDeferred<AD1StageGimmick>(AD1StageGimmick::StaticClass() , NewTransform);
		if (NewGimmick)
		{
			NewGimmick->SetStageLevel(CurrentStageLevel + 1);
			NewGimmick->FinishSpawning(NewTransform);
		}
	}
}

void AD1StageGimmick::OpenAllGates()
{
	for (const auto GateMesh : GateMeshes)
	{
		(GateMesh.Value)->SetRelativeRotation(FRotator(0.0f , -90.0f , 0.0f));
	}
}

void AD1StageGimmick::CloseAllGates()
{
	for (const auto GateMesh : GateMeshes)
	{
		(GateMesh.Value)->SetRelativeRotation(FRotator(0.0f , 0.0f , 0.0f));
	}
}

void AD1StageGimmick::SetState(EStageState InNewState)
{
	CurrentState = InNewState;

	if (StateChangeActions.Contains(CurrentState))
	{
		StateChangeActions[CurrentState].ExecuteIfBound();
	}
}

void AD1StageGimmick::SetReady()
{
	StageTriggerBox->SetCollisionProfileName(FName("D1Trigger"));
	for (auto GateTriggerBox : GateTriggerBoxes)
	{
		GateTriggerBox->SetCollisionProfileName(FName("NoCollision"));
	}

	OpenAllGates();
}

void AD1StageGimmick::SetFight()
{
	StageTriggerBox->SetCollisionProfileName(FName("NoCollision"));
	for (auto GateTriggerBox : GateTriggerBoxes)
	{
		GateTriggerBox->SetCollisionProfileName(FName("NoCollision"));
	}

	CloseAllGates();

	// 몬스터 스폰
	GetWorld()->GetTimerManager().SetTimer(MonsterSpawnTimerHandle , this , &AD1StageGimmick::OnMonsterSpawn , MonsterSpawnTime , false);
}

void AD1StageGimmick::SetChooseReward()
{
	StageTriggerBox->SetCollisionProfileName(FName("NoCollision"));
	for (auto GateTriggerBox : GateTriggerBoxes)
	{
		GateTriggerBox->SetCollisionProfileName(FName("NoCollision"));
	}

	CloseAllGates();

	SpawnRewardBoxes();
}

void AD1StageGimmick::SetChooseNext()
{
	StageTriggerBox->SetCollisionProfileName(FName("NoCollision"));
	for (auto GateTriggerBox : GateTriggerBoxes)
	{
		GateTriggerBox->SetCollisionProfileName(FName("D1Trigger"));
	}

	OpenAllGates();
}

void AD1StageGimmick::OnMonsterSpawn()
{
	const FTransform SpawnTransform(GetActorLocation() + FVector::UpVector * 88.0f);
	AD1Monster* NewMonster = GetWorld()->SpawnActorDeferred<AD1Monster>(MonsterClass , SpawnTransform);
	if (NewMonster)
	{
		NewMonster->OnDestroyed.AddDynamic(this , &AD1StageGimmick::OnMonsterDestroyed);
		NewMonster->SetLevel(CurrentStageLevel);
		NewMonster->FinishSpawning(SpawnTransform);
	}
}

void AD1StageGimmick::OnMonsterDestroyed(AActor* DestroyedActor)
{
	SetState(EStageState::REWARD);
}

void AD1StageGimmick::SpawnRewardBoxes()
{
	for (const auto& RewardBoxLocation : RewardBoxLocations)
	{
		FTransform SpawnTransform(GetActorLocation() + RewardBoxLocation.Value + FVector(0.0f , 0.0f , 30.0f));
		AD1ItemBox* RewardBoxActor = GetWorld()->SpawnActorDeferred<AD1ItemBox>(AD1ItemBox::StaticClass() , SpawnTransform);
		
		if (RewardBoxActor)
		{
			RewardBoxActor->Tags.Add(RewardBoxLocation.Key);
			RewardBoxActor->GetTriggerBox()->OnComponentBeginOverlap.AddDynamic(this , &AD1StageGimmick::OnRewardBoxBeginOverlap);
			RewardBoxes.Add(RewardBoxActor);
		}
	}

	for (const auto& RewardBox : RewardBoxes)
	{
		if (RewardBox.IsValid())
		{
			RewardBox.Get()->FinishSpawning(RewardBox.Get()->GetActorTransform());
		}
	}
}

void AD1StageGimmick::OnRewardBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	for (const auto& RewardBox : RewardBoxes)
	{
		if (RewardBox.IsValid())
		{
			AD1ItemBox* ValidItemBox = RewardBox.Get();
			AActor* OverlapBox = OverlappedComponent->GetOwner();
			if (OverlapBox != ValidItemBox)
			{
				ValidItemBox->Destroy();
			}
		}
	}

	SetState(EStageState::NEXT);
}
