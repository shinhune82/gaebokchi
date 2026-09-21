#include "Core/StepZeroGameMode.h"
#include "Schedule/WorldClock.h"
#include "Schedule/ScheduleComponent.h"
#include "World/ScheduleLocation.h"
#include "NPC/NPCCharacter.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/DirectionalLight.h"
#include "Components/StaticMeshComponent.h"
#include "Components/LightComponent.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "NavigationSystem.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

AStepZeroGameMode::AStepZeroGameMode()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ScheduleTableFinder(TEXT("/Game/Data/Tables/Schedule_TestNPC"));
	if (ScheduleTableFinder.Succeeded())
	{
		DefaultScheduleTable = ScheduleTableFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMeshFinder.Succeeded())
	{
		FloorMesh = CubeMeshFinder.Object;
	}
}

void AStepZeroGameMode::BeginPlay()
{
	Super::BeginPlay();

	SpawnFloor();
	SpawnLight();
	SpawnNavMesh();
	SpawnWorldClock();
	SpawnLocations();
	SpawnTestNPC();
	PositionPlayerCamera();
}

void AStepZeroGameMode::SpawnFloor()
{
	if (!FloorMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("StepZeroGameMode: 바닥 메시(엔진 기본 Cube)를 찾지 못함"));
		return;
	}

	FActorSpawnParameters Params;
	// 큐브(한 변 100, Z스케일 0.5 → 반두께 25) 윗면이 정확히 Z=0에 오도록 중심을 -25만큼 내린다.
	// 원점(0,0,0)에서 스폰되는 DefaultPawn이 바닥 속에 파묻히는 걸 막기 위함.
	AStaticMeshActor* Floor = GetWorld()->SpawnActor<AStaticMeshActor>(FVector(0.f, 0.f, -25.f), FRotator::ZeroRotator, Params);
	if (Floor)
	{
		if (UStaticMeshComponent* MeshComp = Floor->GetStaticMeshComponent())
		{
			// 런타임에 스폰되므로 라이트맵 빌드가 불가능함 — Movable로 둬서 빌드 없이 바로 보이게 한다.
			MeshComp->SetMobility(EComponentMobility::Movable);
			MeshComp->SetStaticMesh(FloorMesh);
			MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		// 큐브(한 변 100)를 얇고 넓은 바닥 슬래브로 스케일.
		Floor->SetActorScale3D(FVector(50.f, 50.f, 0.5f));
	}
}

void AStepZeroGameMode::SpawnLight()
{
	FActorSpawnParameters Params;
	if (ADirectionalLight* Light = GetWorld()->SpawnActor<ADirectionalLight>(FVector(0.f, 0.f, 500.f), FRotator(-45.f, -45.f, 0.f), Params))
	{
		// Movable이어야 라이트맵 빌드 없이 런타임에 바로 빛을 낸다.
		Light->GetLightComponent()->SetMobility(EComponentMobility::Movable);
	}
}

void AStepZeroGameMode::SpawnNavMesh()
{
	FActorSpawnParameters Params;
	if (ANavMeshBoundsVolume* NavVolume = GetWorld()->SpawnActor<ANavMeshBoundsVolume>(FVector::ZeroVector, FRotator::ZeroRotator, Params))
	{
		// 바닥(스케일 50 = 반경 2500) 전체를 덮도록 넉넉하게.
		NavVolume->SetActorScale3D(FVector(50.f, 50.f, 10.f));
	}

	if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		NavSys->Build();
	}
}

void AStepZeroGameMode::SpawnWorldClock()
{
	FActorSpawnParameters Params;
	GetWorld()->SpawnActor<AWorldClock>(FVector::ZeroVector, FRotator::ZeroRotator, Params);
}

void AStepZeroGameMode::SpawnLocations()
{
	FActorSpawnParameters Params;

	if (AScheduleLocation* Home = GetWorld()->SpawnActor<AScheduleLocation>(FVector(-800.f, 0.f, 100.f), FRotator::ZeroRotator, Params))
	{
		Home->LocationTag = FName(TEXT("Loc.Home"));
	}

	if (AScheduleLocation* Training = GetWorld()->SpawnActor<AScheduleLocation>(FVector(800.f, 0.f, 100.f), FRotator::ZeroRotator, Params))
	{
		Training->LocationTag = FName(TEXT("Loc.TrainingGround"));
	}

	if (AScheduleLocation* Market = GetWorld()->SpawnActor<AScheduleLocation>(FVector(0.f, 800.f, 100.f), FRotator::ZeroRotator, Params))
	{
		Market->LocationTag = FName(TEXT("Loc.Market"));
	}
}

void AStepZeroGameMode::SpawnTestNPC()
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANPCCharacter* NPC = GetWorld()->SpawnActor<ANPCCharacter>(FVector(0.f, 0.f, 100.f), FRotator::ZeroRotator, Params);
	if (NPC && NPC->ScheduleComponent)
	{
		if (DefaultScheduleTable)
		{
			NPC->ScheduleComponent->ScheduleTable = DefaultScheduleTable;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("StepZeroGameMode: DataTable(/Game/Data/Tables/Schedule_TestNPC)를 찾지 못함 — 임포트했는지 확인"));
		}
	}
}

void AStepZeroGameMode::PositionPlayerCamera()
{
	// PlayerStart가 없어 DefaultPawn이 원점에 스폰되므로, 씬 전체가 보이는
	// 위치/각도로 직접 옮겨준다.
	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		PlayerPawn->SetActorLocationAndRotation(FVector(-1200.f, -1200.f, 900.f), FRotator(-30.f, 45.f, 0.f));
	}
}
