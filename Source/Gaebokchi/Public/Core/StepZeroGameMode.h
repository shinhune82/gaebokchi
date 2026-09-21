#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StepZeroGameMode.generated.h"

class UDataTable;
class UStaticMesh;

// Step 0 테스트용 GameMode. 빈 레벨에서 Play만 누르면 바닥/조명/NavMesh/
// WorldClock/장소 3곳/NPC를 전부 자동으로 스폰한다. 프로젝트 전역 기본
// GameMode로 지정되어 있어(Config/DefaultEngine.ini) 어떤 레벨에서든 동작한다.
UCLASS()
class GAEBOKCHI_API AStepZeroGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AStepZeroGameMode();

protected:
	virtual void BeginPlay() override;

private:
	// 생성자에서 미리 찾아두는 기본 에셋 참조.
	UPROPERTY()
	TObjectPtr<UDataTable> DefaultScheduleTable;

	UPROPERTY()
	TObjectPtr<UStaticMesh> FloorMesh;

	void SpawnFloor();
	void SpawnLight();
	void SpawnNavMesh();
	void SpawnWorldClock();
	void SpawnLocations();
	void SpawnTestNPC();
};
