// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_MeleeAttack_Omni.h"
#include "GameFramework/Character.h"
#include "GameAbilitiesGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyCharacter.h"

UGA_MeleeAttack_Omni::UGA_MeleeAttack_Omni()
{
	AbilityTags.AddTag(TAG_Ability_MeleeAttack_Omni);
	ActivationOwnedTags.AddTag(TAG_Ability_MeleeAttack_Omni_Active);
	ActiveUITag = TAG_Ability_MeleeAttack_Omni_Active;

	BlockAbilitiesWithTag.AddTag(TAG_Ability_Dash);
	BlockAbilitiesWithTag.AddTag(TAG_Ability_HitReaction);
	CancelAbilitiesWithTag.AddTag(TAG_Ability_HitReaction);
}

void UGA_MeleeAttack_Omni::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());

	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	UCharacterMovementComponent* Movement = Character->GetCharacterMovement();

	if (!Movement)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	MovetoNearestEnemy();

	APlayerController* PC = Cast<APlayerController>(Character->GetController());

	if (PC)
	{
		PC->SetIgnoreMoveInput(true);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_MeleeAttack_Omni::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, false, false);

	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		APlayerController* PC = Cast<APlayerController>(Character->GetController());

		if (PC)
		{
			PC->SetIgnoreMoveInput(false);
		}
	}
}

AEnemyCharacter* UGA_MeleeAttack_Omni::FindNearestEnemy()
{
	TArray<AActor*> FoundEnemies;

	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		TargetClass,
		FoundEnemies
	);

	AEnemyCharacter* ClosestEnemy = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();

	FVector PlayerLocation = GetAvatarActorFromActorInfo()->GetActorLocation();

	for (AActor* Actor : FoundEnemies)
	{
		if (!Actor)
			continue;

		float Distance = FVector::Dist(PlayerLocation, Actor->GetActorLocation());

		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestEnemy = Cast<AEnemyCharacter>(Actor);
		}
	}

	return ClosestEnemy;
}

void UGA_MeleeAttack_Omni::MovetoNearestEnemy()
{
	AEnemyCharacter* ClosestEnemy = FindNearestEnemy();
	if (ClosestEnemy)
	{
		FVector EnemyLocation = ClosestEnemy->GetActorLocation();
		FVector PlayerLocation = GetAvatarActorFromActorInfo()->GetActorLocation();

		FVector Direction = (EnemyLocation - PlayerLocation).GetSafeNormal();

		FVector TargetLocation = EnemyLocation - Direction * 100.f;

		GetAvatarActorFromActorInfo()->SetActorLocation(TargetLocation);
	}
}
