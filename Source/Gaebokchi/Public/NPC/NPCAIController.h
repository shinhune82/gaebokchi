#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Schedule/ScheduleTypes.h"
#include "NPCAIController.generated.h"

UCLASS()
class GAEBOKCHI_API ANPCAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UFUNCTION()
	void HandleScheduleChanged(FScheduleEntry NewEntry);

	AActor* FindLocationActorByTag(FName Tag) const;
};
