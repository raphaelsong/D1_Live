// Fill out your copyright notice in the Description page of Project Settings.


#include "MainActor.h"
#include "D1Actor.h"

// Sets default values
AMainActor::AMainActor()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<AD1Actor> FindClssRef(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_D1Actor.BP_D1Actor_C'"));
	if (FindClssRef.Succeeded())
	{
		BPActorClass = FindClssRef.Class;
	}
}

// Called when the game starts or when spawned
void AMainActor::BeginPlay()
{
	Super::BeginPlay();
	
	FVector Location(0.0f , 0.0f , 0.0f);
	FRotator Rotation(0.0f , 0.0f , 0.0f);
	//D1Actor = GetWorld()->SpawnActor<AD1Actor>(Location , Rotation);
	D1Actor = Cast<AD1Actor>(GetWorld()->SpawnActor(BPActorClass));

	//GetWorld()->DestroyActor(D1Actor);

	//D1Actor->SetLifeSpan(5.0f);
}

// Called every frame
void AMainActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

