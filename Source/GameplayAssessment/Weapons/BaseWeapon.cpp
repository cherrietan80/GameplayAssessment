// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BaseWeapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(Root);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStart"));
	TraceStart->SetupAttachment(WeaponMesh);

	TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEnd"));
	TraceEnd->SetupAttachment(WeaponMesh);
}

FWeaponConfig ABaseWeapon::GetWeaponConfig() const
{
	return WeaponConfig;
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::HitActor()
{
	AActor* OwnerActor = GetInstigator();

	if (!OwnerActor)
	{
		return;
	}

	FVector Start = TraceStart->GetComponentLocation();
	FVector End = TraceEnd->GetComponentLocation();

	TArray<FHitResult> OutHits;


	// Object Types (Pawn)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(
		UEngineTypes::ConvertToObjectType(ECC_Pawn)
	);


	// Ignore Owner
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerActor);


	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		this,
		Start,
		End,
		HitScanRadius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		OutHits,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.0f
	);


	if (bHit)
	{
		for (const FHitResult& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();

			if (HitActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Weapon Location: %s"), *GetActorLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("TraceStart Location: %s"), *Start.ToString());
	UE_LOG(LogTemp, Warning, TEXT("TraceEnd Location: %s"), *End.ToString());

	//DrawDebugSphere(
	//	GetWorld(),
	//	Start,
	//	20.f,
	//	12,
	//	FColor::Red,
	//	false,
	//	5.f
	//);
	UE_LOG(LogTemp, Display, TEXT("diedei"));
}

void ABaseWeapon::StartHitActor(FGameplayEventData Payload)
{
	GetWorld()->GetTimerManager().SetTimer(
		HitScanTimer,
		this,
		&ABaseWeapon::HitActor,
		.03f,
		true
	);
}

void ABaseWeapon::EndHitActor(FGameplayEventData Payload)
{
	GetWorld()->GetTimerManager().ClearTimer(
		HitScanTimer
	);
}

