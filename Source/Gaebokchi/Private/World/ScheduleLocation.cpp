#include "World/ScheduleLocation.h"
#include "Components/SceneComponent.h"

AScheduleLocation::AScheduleLocation()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}
