// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&AEnemySpawner::SpawnEnemy,
		0.1f,
		false
	);
}

void AEnemySpawner::SpawnEnemy()
{
	if (!EnemyClass)
		return;

	FVector SpawnLocation;

	if (!GetRandomSpawnLocation(SpawnLocation))
	{
		SpawnLocation = GetActorLocation();
	}

	FActorSpawnParameters Params;

	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	CurrentEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(
		EnemyClass,
		SpawnLocation,
		FRotator::ZeroRotator,
		Params
	);

	if (CurrentEnemy)
	{
		CurrentEnemy->OnEnemyDeath.AddDynamic(
			this,
			&AEnemySpawner::OnEnemyDeath
		);
	}
}

void AEnemySpawner::OnEnemyDeath()
{
	CurrentEnemy = nullptr;

	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AEnemySpawner::SpawnEnemy,
		1.f,
		false
	);
}

bool AEnemySpawner::GetRandomSpawnLocation(FVector& OutLocation)
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (!Player) return false;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (!NavSystem) return false;

	FVector Origin = Player->GetActorLocation();

	FNavLocation RandomLocation;

	bool bFound =
		NavSystem->GetRandomReachablePointInRadius(
			Origin,
			SpawnRadius,
			RandomLocation
		);

	if (bFound)
	{
		OutLocation = RandomLocation.Location;
		return true;
	}

	return false;
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

