// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "MovingPlatform.h"


// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = UObject::CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	if (!ensure(TriggerVolume != nullptr)) return;
	RootComponent = TriggerVolume;

	MeshComponent = UObject::CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	if (!ensure(MeshComponent != nullptr)) return;
	MeshComponent->SetupAttachment(TriggerVolume);
	MeshComponent->SetGenerateOverlapEvents(false);

}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();

	TriggerVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &APlatformTrigger::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddUniqueDynamic(this, &APlatformTrigger::OnOverlapEnd);
}

void APlatformTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{	
	for (AMovingPlatform* Platform : PlatformsToTrigger)
	{
		Platform->AddActiveTrigger();
	}
}

void APlatformTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (AMovingPlatform* Platform : PlatformsToTrigger)
	{
		Platform->RemoveActiveTrigger();
	}
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TriggerVolume->IsOverlappingActor()
}

