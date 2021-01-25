// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AHero::AHero()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->bUsePawnControlRotation = true;

	GetCapsuleComponent()->SetCapsuleSize(29.17f, 99.56f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	BaseTurnRate = 65.0f;
	BaseLookUpRate = 65.0f;

	//Запрещаем вращаться персонажу с камерой, теперь мышкой
	//вращаемся вокруг персонажа
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//Персонаж движется в направлении ввода
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//Поворачивается в сторону движения по оси рыскания со скоростью поворота 540
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);
	//Скорость прыжка по Z
	GetCharacterMovement()->JumpZVelocity = 650.f;
	//Управление в воздухе, макс 1, слабое поставим 0.25
	GetCharacterMovement()->AirControl = 0.25f;
}

// Called when the game starts or when spawned
void AHero::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHero::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHero::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("TurnRate", this, &AHero::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AHero::LookUpAtRate);
}

void AHero::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AHero::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AHero::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AHero::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
