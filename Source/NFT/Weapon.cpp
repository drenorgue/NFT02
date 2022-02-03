// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "NFTCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"
#include "Engine/EngineTypes.h"
#include "Camera/CameraActor.h"
#include "Engine/GameEngine.h"
#include "NFTCharacter.h"
#include "Components/SceneComponent.h"
//#include "Blueprint/UserWidget.h"

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
	
//	FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, false);

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
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		EyeLocation = RootComponent->GetComponentLocation();
		
	//	EyeRotation.Add(0.0f, 0.0f, 100.0f); // met le personnage à l'envers

			
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
		FVector a = EyeLocation + (10.0f, 10.0f, 10.0f)  ; // pour pas spawn dans le corp du Mesh
		
		FActorSpawnParameters e ;
		FRotator b;
		b = EyeRotation;
		b.Add(1.0f, 5.0f, 0.0f); // éssaye de patch les balles

		for (TObjectIterator<ANFTCharacter> It; It; ++It)
		{
			It->FirtsPersonBool;
			
		}
	//	AActor* SpawnedActor = GetWorld()->SpawnActor<AProjectile>(a, EyeRotation);
	//	SpawnedActor->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AProjectile>(ProjectileClass, a, b, e); // fait spawn la balle
	
		
		for (TObjectIterator<ANFTCharacter> Itr; Itr; ++Itr)
		{
			if (Itr->IsA(ANFTCharacter::StaticClass()))
			{
				ANFTCharacter* actorClass = *Itr;
				actorClass->Rotation(EyeRotation); // change la rotation du personnage a chaque tire
			}
		}
	}
}

void AWeapon::OnClick()
{
}

void AWeapon::Fire02(FRotator ab)
{
	AActor* MyOwner = GetOwner(); // récupère qui tire
	if (MyOwner) // verifie qui tire (pour un eventuelle multijoueur)
	{


		FVector EyeLocation;
		FRotator EyeRotation;
		EyeLocation = RootComponent->GetComponentLocation();

		//	EyeRotation.Add(0.0f, 0.0f, 100.0f); // met le personnage à l'envers

		
		FVector TraceEnd = EyeLocation + (ab.Vector() * 10000); // 10 000 car il fallait un grand nombre

		FCollisionQueryParams QueryParms;
		QueryParms.AddIgnoredActor(MyOwner); // ignore le joueur qui a tiré
		QueryParms.AddIgnoredActor(this); // ignore l'arme
		QueryParms.bTraceComplex = true; // pour savoir exactement où on touche

		FHitResult Hit;
		FVector a = EyeLocation + (10.0f, 10.0f, 10.0f); // pour pas spawn dans le corp du Mesh

		FActorSpawnParameters e;
		FRotator Lo;
		FVector La;


		GetActorEyesViewPoint(La, Lo);

		FRotator Le = Lo;
		Le.Yaw = 0.0f;
	//	Le.Pitch = 0.0f;
		Le.Roll = 0.0f;


		for (TObjectIterator<ANFTCharacter> Itr; Itr; ++Itr)
		{
			if (Itr->IsA(ANFTCharacter::StaticClass()))
			{
				ANFTCharacter* actorClass = *Itr;
				actorClass->Rotation(Le); // change la rotation du personnage a chaque tire
			}
		}

		GetWorld()->SpawnActor<AProjectile>(ProjectileClass, La, Lo, e); // fait spawn la balle


		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("HALLO"));
	}

}

