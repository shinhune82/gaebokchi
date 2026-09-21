#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldClock.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHourChanged, float, NewHour);

// 게임 내 시간을 흘려보내는 액터. 레벨에 1개만 배치한다.
UCLASS(Blueprintable)
class GAEBOKCHI_API AWorldClock : public AActor
{
	GENERATED_BODY()

public:
	AWorldClock();

	// 0~24 사이의 게임 내 시각.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clock")
	float GameHour = 6.f;

	// 실제 1초당 게임 내 몇 시간이 흐르는지. 테스트 시 크게 설정해 빠르게 하루를 돌려본다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clock")
	float HoursPerRealSecond = 0.2f;

	UPROPERTY(BlueprintAssignable, Category = "Clock")
	FOnHourChanged OnHourChanged;

protected:
	virtual void Tick(float DeltaSeconds) override;
};
