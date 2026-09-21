#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScheduleLocation.generated.h"

// 레벨에 배치하는 장소 마커. LocationTag가 스케줄 데이터의 LocationTag와 매칭된다.
UCLASS(Blueprintable)
class GAEBOKCHI_API AScheduleLocation : public AActor
{
	GENERATED_BODY()

public:
	AScheduleLocation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
	FName LocationTag = NAME_None;
};
