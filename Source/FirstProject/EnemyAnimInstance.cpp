// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Enemy = Cast<AEnemy>(Pawn);		// 초기화 직후 Enemy에 대한 참조를 가짐
		}
	}
}

void UEnemyAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)	// Pawn이 유효하다면
		{
			Enemy = Cast<AEnemy>(Pawn);
		}
	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();	// 속도를 구하고
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);	// Z를 0으로 만든 다음
		MovementSpeed = LateralSpeed.Size();	//해당 벡터의 크기를 구하고 이동속도로 설정
	}
}