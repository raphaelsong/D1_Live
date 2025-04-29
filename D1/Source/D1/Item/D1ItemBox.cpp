// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/D1ItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AD1ItemBox::AD1ItemBox()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	TriggerBox->SetCollisionProfileName(FName("D1Trigger"));
	TriggerBox->SetBoxExtent(FVector(40.0f , 42.0f , 30.0f));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this , &AD1ItemBox::OnBoxBeginOverlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->SetCollisionProfileName(FName("NoCollision"));
	Mesh->SetRelativeLocation(FVector(0.0f , -3.5f , -30.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/_Art/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (MeshRef.Succeeded())
	{
		Mesh->SetStaticMesh(MeshRef.Object);
	}

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));
	Effect->SetupAttachment(GetRootComponent());
	Effect->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/_Art/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (EffectRef.Succeeded())
	{
		Effect->SetTemplate(EffectRef.Object);
	}
}

void AD1ItemBox::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	Effect->Activate(true);
	Effect->OnSystemFinished.AddDynamic(this , &AD1ItemBox::OnBoxEffectFinished);
}

void AD1ItemBox::OnBoxEffectFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}

