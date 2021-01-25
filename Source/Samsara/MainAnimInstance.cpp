// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
}

void UMainAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();

		//Боковая скорость, которая интересна нам когда персонаж не в водухе
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

		bIsinAir = Pawn->GetMovementComponent()->IsFalling();
	}
}