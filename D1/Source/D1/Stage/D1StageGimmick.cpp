// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/D1StageGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/OverlapResult.h"

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
}

void AD1StageGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetState(CurrentState);
}

void AD1StageGimmick::OnStageTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
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
		AD1StageGimmick* NewGimmick = GetWorld()->SpawnActor<AD1StageGimmick>(AD1StageGimmick::StaticClass() , NewTransform);
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
}

void AD1StageGimmick::SetFight()
{
}

void AD1StageGimmick::SetChooseReward()
{
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
