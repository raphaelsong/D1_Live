// Fill out your copyright notice in the Description page of Project Settings.


#include "D1Actor.h"
#include "D1Object.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AD1Actor::AD1Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetRelativeScale3D(FVector(4.0f , 1.0f , 0.5f));

	Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(Box);
	Head->SetRelativeLocation(FVector(50.0f , 0.0f , 0.0f));
	Head->SetRelativeScale3D(FVector(0.1875f , 0.5f , 1.5f));

	Wing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wing"));
	Wing->SetupAttachment(Box);
	Wing->SetRelativeLocation(FVector(17.5f , 0.0f , 0.0f));
	Wing->SetRelativeScale3D(FVector(0.25f , 3.5f , 0.5f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> FindMeshRef(TEXT("/Script/Engine.StaticMesh'/Engine/EngineMeshes/Cube.Cube'"));
	if (FindMeshRef.Succeeded())
	{
		Box->SetStaticMesh(FindMeshRef.Object);
		Head->SetStaticMesh(FindMeshRef.Object);
		Wing->SetStaticMesh(FindMeshRef.Object);
	}
}

// Called when the game starts or when spawned
void AD1Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AD1Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//float Speed = 50.0f;
	//float Distance = DeltaTime * Speed;

	//FVector Location = GetActorLocation();
	//FVector NewLocation = Location + FVector::ForwardVector * Distance;

	////SetActorLocation(NewLocation);

	//AddActorWorldOffset(FVector::ForwardVector * Distance);

	float RotationRate = 45.0f;
	FRotator Rotation = GetActorRotation();
	FRotator NewRotation = FRotator(Rotation.Pitch , Rotation.Yaw + RotationRate * DeltaTime , Rotation.Roll);

	//SetActorRotation(NewRotation);

	AddActorWorldRotation(FRotator(0.0f, RotationRate * DeltaTime, 0.0f));
}

