// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	// Create Destroy well
	USphereDestroy = CreateDefaultSubobject<USphereComponent>(TEXT("SphereDestroy"));
	USphereDestroy->SetSphereRadius(300);
	USphereDestroy->SetupAttachment(MeshComp);

	// Bind to event
	USphereDestroy->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapDestroySphere);
	
	// Create Gravity well and set the Channel for the collision 
	USphereAddForce = CreateDefaultSubobject<USphereComponent>(TEXT("SphereAddForce"));
	USphereAddForce->SetSphereRadius(3000);
	USphereAddForce->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
}

void AFPSBlackHole::OverlapDestroySphere(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TArray<UPrimitiveComponent*> OverlappingComponents;
	USphereAddForce->GetOverlappingComponents(OverlappingComponents);

	for (int32 i = 0; i < OverlappingComponents.Num(); i++)
	{
		UPrimitiveComponent* OverlappComponent = OverlappingComponents[i];
		if (OverlappComponent && OverlappComponent->IsSimulatingPhysics())
		{
			float fRadius = USphereAddForce->GetScaledSphereRadius();
			float fStrength = -2000;

			OverlappComponent->AddRadialForce(GetActorLocation(), fRadius, fStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}



