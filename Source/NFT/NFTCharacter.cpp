// Copyright Epic Games, Inc. All Rights Reserved.

#include "NFTCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Weapon.h"
#include "Engine/GameEngine.h"

//////////////////////////////////////////////////////////////////////////
// ANFTCharacter

ANFTCharacter::ANFTCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//create cam02

	Cam02 = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam02"));
	Cam02->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Cam02->bUsePawnControlRotation = true;
	Cam02->SetupAttachment(RootComponent);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereProjectile"));
	Sphere->SetupAttachment(FollowCamera);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ANFTCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ANFTCharacter::OnClick); // apelle la fonction de tire
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ANFTCharacter::ThirdPerson);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ANFTCharacter::FirtsPerson);
	PlayerInputComponent->BindAxis("MoveForward", this, &ANFTCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANFTCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ANFTCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ANFTCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ANFTCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ANFTCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ANFTCharacter::OnResetVR);
}

/*
FVector ANFTCharacter::GetPawnViewLocation() const
{
	if (!FollowCamera == NULL)
	{
		return FollowCamera->GetComponentLocation();
	}
	//return Super::GetPawnViewLocation();
	return FollowCamera->GetComponentLocation();
}*/

void ANFTCharacter::Rotation(FRotator NewRotation)
{
	NewRotation.Pitch = GetActorRotation().Pitch;
	SetActorRotation(NewRotation);
}

void ANFTCharacter::FirtsPerson() // met a la 3?me Personne
{
	Cam02->SetActive(false);
	FollowCamera->SetActive(true);

	FirtsPersonBool = true;

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ThirdPerson"));
}


void ANFTCharacter::ThirdPerson() // met a la 1 ?re personne 
{
	Cam02->SetActive(true);
	FollowCamera->SetActive(false);

	FirtsPersonBool = false;
}



void ANFTCharacter::OnTick(float DeltaTime) //        /!\ cette fonction ne fonctionne pas /!\ 
{
	Super::Tick(DeltaTime); 
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Tick")); 

}

void ANFTCharacter::OnBeginPlay()
{
	
	Super::BeginPlay();
	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	FViewTargetTransitionParams Params;
//	OurPlayerController->SetViewTarget(Cam02);

	ThirdPerson();
}

void ANFTCharacter::OnClick()
{
	if (FirtsPersonBool == true)
	{
		for (TObjectIterator<AWeapon> Itr; Itr; ++Itr) // apelle la fonction Fire() de Weapon.cpp
		{
			if (Itr->IsA(AWeapon::StaticClass()))
			{
				AWeapon* actorClass = *Itr;
				actorClass->Fire();
			}
		}
	}

	if (FirtsPersonBool == false) 
	{
		for (TObjectIterator<AWeapon> Itr; Itr; ++Itr) // apelle la fonction Fire02() de Weapon.cpp
		{
			if (Itr->IsA(AWeapon::StaticClass()))
			{
				AWeapon* actorClass = *Itr;

				FRotator relat = Cam02->GetRelativeRotation();

				actorClass->Fire02(relat);
			}
		}
	}


}

void ANFTCharacter::OnResetVR()
{
	// If NFT is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in NFT.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ANFTCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ANFTCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ANFTCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ANFTCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ANFTCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ANFTCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}