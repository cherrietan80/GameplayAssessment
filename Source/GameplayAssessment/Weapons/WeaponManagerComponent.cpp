// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponManagerComponent.h"
#include "Weapons/BaseWeapon.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UWeaponManagerComponent::UWeaponManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
}


// Called every frame
void UWeaponManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponManagerComponent::ToggleEquipWeapon(TSubclassOf<class ABaseWeapon> WeaponClass)
{	
	if (OwnerCharacter)
	{
		if (EquippedWeapon)
		{
			if (EquippedWeapon->GetClass() == WeaponClass)
			{
				UnequipWeapon(WeaponClass);
			}
			else
			{
				UnequipWeapon(EquippedWeapon->GetClass());
				EquipWeapon(WeaponClass);
			}
		}
		else
		{
			EquipWeapon(WeaponClass);
		}

	}
}

void UWeaponManagerComponent::EquipWeapon(TSubclassOf<class ABaseWeapon> WeaponClass)
{
	EquippedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass);
	EquippedWeapon->AttachToComponent(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		EquippedWeapon->GetWeaponConfig().SocketName
	);
	OwnerCharacter->GetMesh()->SetAnimInstanceClass(EquippedWeapon->GetWeaponConfig().AnimBlueprint);
}

void UWeaponManagerComponent::UnequipWeapon(TSubclassOf<class ABaseWeapon> WeaponClass)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
		EquippedWeapon = nullptr;

		OwnerCharacter->GetMesh()->SetAnimInstanceClass(DefaultAnimBlueprint);

	}
}


