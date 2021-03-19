// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

#include "GameFramework/ProjectileMovementComponent.h"


AMovingPlatform::AMovingPlatform() 
{
    PrimaryActorTick.bCanEverTick = true;

    AStaticMeshActor::SetMobility(EComponentMobility::Movable);
    
    //ProjectileMesh = AStaticMeshActor::GetStaticMeshComponent();
    //ProjectileMesh->SetCollisionProfileName(TEXT("NoCollision"));
}


void AMovingPlatform::BeginPlay()
{
    Super::BeginPlay();

    GlobalStartLocation = AActor::GetActorLocation();
    // Get global location of local transform location
    GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);

    AActor::SetReplicates(true);
    AActor::SetReplicateMovement(true);
}


void AMovingPlatform::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);

    if (ActiveTriggers > 0)
    {
        //Move the Box
        if (AActor::HasAuthority())
        {
            FVector Location = AActor::GetActorLocation();
            float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
            float JourneyTravelled = (Location - GlobalStartLocation).Size();

            // Swap start location and target location for loop back and forth
            if (JourneyTravelled >= JourneyLength)
            {
                FVector Swap = GlobalStartLocation;
                GlobalStartLocation = GlobalTargetLocation;
                GlobalTargetLocation = Swap;
            }
            // Calculate the vector length from start to target location vectors
            FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
            Location += MovingSpeed * DeltaTime * Direction;
            AActor::SetActorLocation(Location);
        }
    }
    
}


void AMovingPlatform::AddActiveTrigger() 
{
    ActiveTriggers++;
}


void AMovingPlatform::RemoveActiveTrigger() 
{
    if (ActiveTriggers > 0)
    {
        ActiveTriggers--;
    }
}





