// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSObjectiveActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"
#include "FPSCharacter.h"


// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
	// Instantiate a component
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	// Deactivating collision manually in code
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// Our mesh component is the root
	RootComponent = MeshComp;

	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// Set sphere component collision to be with player (pawn) and bullets (overlap)
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
	
	PlayEffects();
}

void AFPSObjectiveActor::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}

void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayEffects();

	AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);
	if(MyCharacter)
	{
		MyCharacter->bIsCarryingObjective= true;

		Destroy();
	}
}

