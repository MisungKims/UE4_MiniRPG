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
			Enemy = Cast<AEnemy>(Pawn);		// �ʱ�ȭ ���� Enemy�� ���� ������ ����
		}
	}
}

void UEnemyAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)	// Pawn�� ��ȿ�ϴٸ�
		{
			Enemy = Cast<AEnemy>(Pawn);
		}
	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();	// �ӵ��� ���ϰ�
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);	// Z�� 0���� ���� ����
		MovementSpeed = LateralSpeed.Size();	//�ش� ������ ũ�⸦ ���ϰ� �̵��ӵ��� ����
	}
}