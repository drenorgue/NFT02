// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy02.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AEnemy02::AEnemy02()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	HitBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitBox"));

	HitBox->SetSimulatePhysics(true);
	HitBox->SetNotifyRigidBodyCollision(true);

	HitBox->BodyInstance.SetCollisionProfileName("BlockAllDynamic");

	
}

void AEnemy02::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

// Called when the game starts or when spawned
void AEnemy02::BeginPlay()
{
	Super::BeginPlay();
	
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

