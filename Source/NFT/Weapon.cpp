// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "NFTCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "Engine/EngineTypes.h"
#include "Components/SceneComponent.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;


}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, false);

	//MeshComp->AttachTo(RootComponent);
 //   UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMesh()->GetSocketByName(FName("pelvis")); // FONCTIONNE
	//MeshComp->AttachToComponent(RootComponent, rules, "hand_r"); // fonctionne pas 
//	MeshComp->Attach
	
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Fire()
{
	AActor* MyOwner = GetOwner(); // récupère qui tire
	if (MyOwner) // verifie qui tire (pour un eventuelle multijoueur)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		//MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		EyeLocation = RootComponent->GetComponentLocation();

		
		
		FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 10000); // 10 000 car il fallait un grand nombre

		FCollisionQueryParams QueryParms;
		QueryParms.AddIgnoredActor(MyOwner); // ignore le joueur qui a tiré
		QueryParms.AddIgnoredActor(this); // ignore l'arme
		QueryParms.bTraceComplex = true; // pour savoir exactement où on touche


		FHitResult Hit;
		/*if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParms)) // si ça touche quelqu'un
		{
			// T'as touché ! Faisons les dégats


		}*/

	//	DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
		GetWorld()->SpawnActor<AProjectile>(ProjectileClass, EyeLocation, EyeRotation);
	
		
		
		for (TObjectIterator<ANFTCharacter> Itr; Itr; ++Itr)
		{
			if (Itr->IsA(ANFTCharacter::StaticClass()))
			{
				ANFTCharacter* actorClass = *Itr;
				actorClass->Rotation(EyeRotation);
			}
		}
		


	}
}

void AWeapon::OnClick()
{
	

}

