#include "Schedule/ScheduleComponent.h"
#include "Schedule/WorldClock.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"

UScheduleComponent::UScheduleComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UScheduleComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Clocks;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWorldClock::StaticClass(), Clocks);

	if (Clocks.Num() > 0)
	{
		if (AWorldClock* Clock = Cast<AWorldClock>(Clocks[0]))
		{
			Clock->OnHourChanged.AddDynamic(this, &UScheduleComponent::HandleHourChanged);
		}
	}
}

void UScheduleComponent::HandleHourChanged(float NewHour)
{
	const FScheduleEntry NewEntry = FindEntryForHour(NewHour);

	// 스케줄이 실제로 바뀌었을 때만 이벤트를 발생시켜, 매 틱 재이동 호출을 막는다.
	const bool bChanged = !bHasCurrentEntry
		|| NewEntry.LocationTag != CurrentEntry.LocationTag
		|| NewEntry.Action != CurrentEntry.Action;

	if (bChanged)
	{
		CurrentEntry = NewEntry;
		bHasCurrentEntry = true;
		OnScheduleChanged.Broadcast(CurrentEntry);
	}
}

FScheduleEntry UScheduleComponent::FindEntryForHour(float Hour) const
{
	if (ScheduleTable)
	{
		static const FString Context(TEXT("ScheduleLookup"));
		TArray<FScheduleEntry*> Rows;
		ScheduleTable->GetAllRows(Context, Rows);

		for (const FScheduleEntry* Row : Rows)
		{
			if (Row && Row->Contains(Hour))
			{
				return *Row;
			}
		}
	}

	return FScheduleEntry();
}
