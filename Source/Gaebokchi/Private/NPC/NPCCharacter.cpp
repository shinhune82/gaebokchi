#include "NPC/NPCCharacter.h"
#include "NPC/NPCAIController.h"
#include "Schedule/ScheduleComponent.h"

ANPCCharacter::ANPCCharacter()
{
	ScheduleComponent = CreateDefaultSubobject<UScheduleComponent>(TEXT("ScheduleComponent"));

	AIControllerClass = ANPCAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
