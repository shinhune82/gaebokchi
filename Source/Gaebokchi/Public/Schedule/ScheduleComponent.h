#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Schedule/ScheduleTypes.h"
#include "ScheduleComponent.generated.h"

class UDataTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScheduleChanged, FScheduleEntry, NewEntry);

// 현재 시각에 맞는 스케줄 항목을 DataTable에서 조회한다.
// 이동/행동 실행은 담당하지 않는다 (관심사 분리, AIController가 담당).
UCLASS(ClassGroup = (Schedule), meta = (BlueprintSpawnableComponent))
class GAEBOKCHI_API UScheduleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UScheduleComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
	TObjectPtr<UDataTable> ScheduleTable;

	UPROPERTY(BlueprintReadOnly, Category = "Schedule")
	FScheduleEntry CurrentEntry;

	UPROPERTY(BlueprintAssignable, Category = "Schedule")
	FOnScheduleChanged OnScheduleChanged;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void HandleHourChanged(float NewHour);

	FScheduleEntry FindEntryForHour(float Hour) const;

	bool bHasCurrentEntry = false;
};
