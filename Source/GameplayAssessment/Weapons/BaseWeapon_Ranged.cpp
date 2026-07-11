// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BaseWeapon_Ranged.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DrawDebugHelpers.h"
#include "GameplayAssessmentCharacter.h"

ABaseWeapon_Ranged::ABaseWeapon_Ranged()
{

}

void ABaseWeapon_Ranged::Fire()
{
	if (Ammo <= 0)
	{
		return;
	}

	Ammo--;

	OnAmmoChanged.Broadcast(Ammo);

	AActor* OwnerActor = GetInstigator();

	if (!OwnerActor)
	{
		return;
	}

	CalculateStartAndEndPoint(OwnerActor);

	FHitResult Hit;

	FCollisionQueryParams Params;

	Params.AddIgnoredActor(OwnerActor);

	bool bHit =
		GetWorld()->LineTraceSingleByChannel(
			Hit,
			StartPoint,
			EndPoint,
			ECC_Pawn,
			Params
		);

	if (bHit)
	{
		AActor* HitActor = Hit.GetActor();

		if (HitActor)
		{
			UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
			if (AbilitySystemComponent)
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(EffectSpec);
			}
		}
	}

	// Debug line
	if (bHit)
	{
		// Hit point
		DrawDebugSphere(
			GetWorld(),
			Hit.Location,
			10.f,
			12,
			FColor::Red,
			false,
			3.f
		);
	}
}

void ABaseWeapon_Ranged::FullFillAmmo()
{
	Ammo = AmmoPerMagazine;
	OnAmmoChanged.Broadcast(Ammo);
}

void ABaseWeapon_Ranged::CalculateStartAndEndPoint(AActor* OwnerActor)
{
	AGameplayAssessmentCharacter* Character = Cast<AGameplayAssessmentCharacter>(OwnerActor);

	if (Character->bIsAiming)
	{
		APlayerController* PlayerController = Cast<APlayerController>(
			OwnerActor->GetInstigatorController()
		);

		if (!PlayerController)
		{
			return;
		}


		FVector CameraLocation;
		FRotator CameraRotation;

		PlayerController->GetPlayerViewPoint(
			CameraLocation,
			CameraRotation
		);


		FVector CameraDirection = CameraRotation.Vector();

		StartPoint = CameraLocation;
		EndPoint = StartPoint + CameraDirection * TraceDistance;
	}
	else
	{
		StartPoint = TraceEnd->GetComponentLocation();
		EndPoint = StartPoint + OwnerActor->GetActorForwardVector() * TraceDistance;
	}
}
