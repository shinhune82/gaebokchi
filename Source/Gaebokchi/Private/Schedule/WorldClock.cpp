#include "Schedule/WorldClock.h"

AWorldClock::AWorldClock()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWorldClock::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	GameHour += DeltaSeconds * HoursPerRealSecond;
	if (GameHour >= 24.f)
	{
		GameHour -= 24.f;
	}

	OnHourChanged.Broadcast(GameHour);
}
