#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ScheduleTypes.generated.h"

UENUM(BlueprintType)
enum class EScheduleAction : uint8
{
	Idle,
	Train,
	Eat,
	Sleep,
	Patrol
};

USTRUCT(BlueprintType)
struct FScheduleEntry : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
	float StartHour = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
	float EndHour = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
	FName LocationTag = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
	EScheduleAction Action = EScheduleAction::Idle;

	// 자정을 걸치는 구간(예: 22시~다음날 6시)도 올바르게 판정한다.
	bool Contains(float Hour) const
	{
		if (StartHour <= EndHour)
		{
			return Hour >= StartHour && Hour < EndHour;
		}
		return Hour >= StartHour || Hour < EndHour;
	}
};
