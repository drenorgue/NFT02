 // Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFrameWork/ProjectileMovementComponent.h"
#include "Enemy02.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/Application/SlateApplication.h"

//#include "Enemy.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	CollisionSphere->InitSphereRadius(80.0f);
	CollisionSphere->SetSimulatePhysics(true);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = CollisionSphere;
/*	a->SetSimulatePhysics(true);
	a->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);*/

	TCHAR* SoundPath = L"Content'/LEJEU/Asset/Sound/Arrow/Hit/arrow-sound-effect.uasset'";
	auto AmmoPickupSoundAsset = ConstructorHelpers::FObjectFinder<USoundBase>(SoundPath);
	if (AmmoPickupSoundAsset.Object != nullptr)
	{
		ArrowHit = AmmoPickupSoundAsset.Object;
	}
	
 

	ProjectileMovement =
		CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	ProjectileMovement->UpdatedComponent = CollisionSphere;
	ProjectileMovement->InitialSpeed = 10000.0f;
	ProjectileMovement->MaxSpeed = 10000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	InitialLifeSpan = 3.0f;

//	CollisionSphere->OnComponentBeginOverlap.Ad
//	CollisionSphere->OnComponentBeginOverlap.__Internal_AddDynamic();
//	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);
	




}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
/*	OnCompo.AddDynamic(this, &AProjectile::touch);
	CollisionSphere->*
	
	*/
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnHit);
	CollisionSphere->OnComponentHit.AddDynamic(this, &AProjectile::OnCompHit);

	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	AEnemy02* Char = Cast<AEnemy02>(OtherActor);
	if (Char)
	{
		if (CanDamage) // pour qu'un projectile de fasse qu'un degat et pas plusieur (j'aurai du utilisier OnBeginOverlap plutot que OnHit)
		{
			CanDamage = false;
			Char->DealDamage(DamageValue);
			Sound = true;
			
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
				{
					CanDamage = true;
					Destroy();
				}, 0.1, false);

		}
	}

	
}

void AProjectile::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AEnemy02* Char = Cast<AEnemy02>(OtherActor);
	if (Char)
	{
		Char->DealDamage(DamageValue);
		Sound = true;
	}
	
}

void AProjectile::OnOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
}

void AProjectile::touch()
{
	Destroy();
}

