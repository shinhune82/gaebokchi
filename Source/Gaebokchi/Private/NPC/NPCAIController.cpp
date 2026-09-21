#include "NPC/NPCAIController.h"
#include "World/ScheduleLocation.h"
#include "Schedule/ScheduleComponent.h"
#include "Kismet/GameplayStatics.h"

void ANPCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{
		if (UScheduleComponent* ScheduleComp = InPawn->FindComponentByClass<UScheduleComponent>())
		{
			ScheduleComp->OnScheduleChanged.AddDynamic(this, &ANPCAIController::HandleScheduleChanged);
		}
	}
}

void ANPCAIController::HandleScheduleChanged(FScheduleEntry NewEntry)
{
	if (AActor* Destination = FindLocationActorByTag(NewEntry.LocationTag))
	{
		MoveToActor(Destination, 50.f);

		UE_LOG(LogTemp, Log, TEXT("%s: %s(으)로 이동, 행동=%d"),
			*GetNameSafe(GetPawn()),
			*NewEntry.LocationTag.ToString(),
			static_cast<int32>(NewEntry.Action));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: LocationTag '%s'에 해당하는 AScheduleLocation을 찾지 못함"),
			*GetNameSafe(GetPawn()),
			*NewEntry.LocationTag.ToString());
	}
}

AActor* ANPCAIController::FindLocationActorByTag(FName Tag) const
{
	TArray<AActor*> Locations;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AScheduleLocation::StaticClass(), Locations);

	for (AActor* Actor : Locations)
	{
		if (AScheduleLocation* Location = Cast<AScheduleLocation>(Actor))
		{
			if (Location->LocationTag == Tag)
			{
				return Location;
			}
		}
	}

	return nullptr;
}
