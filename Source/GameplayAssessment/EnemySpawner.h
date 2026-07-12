// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class GAMEPLAYASSESSMENT_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void SpawnEnemy();

	UFUNCTION()
	void OnEnemyDeath();

	bool GetRandomSpawnLocation(FVector& OutLocation);
	
protected:
	UPROPERTY()
	class AEnemyCharacter* CurrentEnemy;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<AEnemyCharacter> EnemyClass;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SpawnRadius = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float MinSpawnRadius = 600.f;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
