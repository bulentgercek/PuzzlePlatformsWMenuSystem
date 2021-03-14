// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

class UProjectileMovementComponent;

UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MovingPlatform", meta = (AllowPrivateAccess = "true"))
	int MovingSpeed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MovingPlatform", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector TargetLocation;

	FVector GlobalTargetLocation;
	FVector GlobalStartLocation;

	UPROPERTY(EditAnywhere)
	int ActiveTriggers = 1;


public:
	AMovingPlatform();

	virtual void Tick(float DeltaTime) override;

	void AddActiveTrigger();
	void RemoveActiveTrigger();

protected:
	virtual void BeginPlay() override;

};
