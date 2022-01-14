// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy02.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "NFTCharacter.h"
#include "Projectile.h"
#include "Engine/GameEngine.h"


// Sets default values
AEnemy02::AEnemy02()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(RootComponent);
	

	
}


// Called when the game starts or when spawned
void AEnemy02::BeginPlay()
{
	Super::BeginPlay();
	
	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy02::OnHit);
}

// Called every frame
void AEnemy02::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy02::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy02::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& Hit)
{
		
}

void AEnemy02::DealDamage(float DamageValue)
{
	health = health - 70.0f;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Damage"));

	if (health <= 0)
	{
		Destroy();
	}
		
}

