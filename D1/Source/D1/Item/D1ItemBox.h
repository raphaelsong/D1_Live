// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D1ItemBox.generated.h"

UCLASS()
class D1_API AD1ItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AD1ItemBox();

public:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent , AActor*  OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEffectFinished(class UParticleSystemComponent* PSystem);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> TriggerBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UParticleSystemComponent> Effect;
};
