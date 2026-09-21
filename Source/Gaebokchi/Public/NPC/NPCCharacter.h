#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCCharacter.generated.h"

class UScheduleComponent;

// 스케줄을 갖는 NPC 캐릭터. ScheduleComponent를 자동으로 부착하고
// ANPCAIController가 기본 컨트롤러로 빙의하도록 설정되어 있다.
UCLASS()
class GAEBOKCHI_API ANPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANPCCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Schedule")
	TObjectPtr<UScheduleComponent> ScheduleComponent;
};
