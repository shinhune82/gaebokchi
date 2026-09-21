# Gaebokchi — 무협 서바이벌 프로토타입 (가제)

구파일방·오대세가 시절을 배경으로, 스토리가 정해진 세계에서 NPC의 역할을 플레이어가 대신 맡아
제한 시간 동안 생존하는 게임. 참여 인원과 무관하게 세계가 돌아가는 것이 핵심 가설.

전체 개발 순서와 각 단계의 성립 조건은 [`docs/ROADMAP.md`](docs/ROADMAP.md) 참고.
현재 진행 중: **Step 0 — 단일 NPC 일과 시스템** ([`docs/step0-npc-schedule.md`](docs/step0-npc-schedule.md)).

## 여는 방법
1. Unreal Engine **5.5 이상** 설치.
2. `Gaebokchi.uproject`를 더블클릭(또는 Epic Games Launcher에서 열기)해서 에디터 실행.
3. `docs/step0-npc-schedule.md`를 따라 `Content/` 하위 폴더에 블루프린트/데이터를 만든다. 각 폴더의 `README.md`에 무엇을 만들어야 하는지 적어뒀다.

이 프로젝트는 Blueprint 위주라 C++ 빌드 과정이 없다 — `.uproject`를 열면 바로 에디터가 뜬다.

## 폴더 구조
```
Content/
  Blueprints/Core/    — BP_WorldClock
  Blueprints/NPC/      — BPC_ScheduleComponent, BP_NPC_AIController, BP_NPC_Base
  Blueprints/World/    — BP_ScheduleLocation
  Data/Enums/          — E_ScheduleAction
  Data/Structs/        — S_ScheduleEntry
  Data/Tables/         — DT_Schedule_TestNPC
  Maps/                — L_Step0_TestVillage
docs/
  ROADMAP.md           — 전체 단계별 계획과 성립 조건
  step0-npc-schedule.md — Step 0 상세 빌드 가이드
```

## 현재 상태
Step 0 스켈레톤(프로젝트 구조, 설계 문서)만 존재하며 실제 블루프린트/데이터 에셋은 아직 없다.
에디터가 있는 환경에서만 에셋을 만들 수 있으므로, 로컬에서 `docs/step0-npc-schedule.md`를 따라 작업한 뒤 결과를 커밋해달라.
