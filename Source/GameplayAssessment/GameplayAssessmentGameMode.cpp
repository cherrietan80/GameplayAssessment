// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayAssessmentGameMode.h"
#include "Kismet/GameplayStatics.h"

AGameplayAssessmentGameMode::AGameplayAssessmentGameMode()
{
	// stub
}

void AGameplayAssessmentGameMode::PlayHitSlowMotion(float TimeScale, float Duration)
{
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeScale);

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(
        TimerHandle,
        [this]()
        {
            UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
        },
        Duration,
        false
    );
}
