# Gaebokchi — 무협 서바이벌 프로토타입 (가제)

구파일방·오대세가 시절을 배경으로, 스토리가 정해진 세계에서 NPC의 역할을 플레이어가 대신 맡아
제한 시간 동안 생존하는 게임. 참여 인원과 무관하게 세계가 돌아가는 것이 핵심 가설.

전체 개발 순서와 각 단계의 성립 조건은 [`docs/ROADMAP.md`](docs/ROADMAP.md) 참고.
현재 진행 중: **Step 0 — 단일 NPC 일과 시스템** ([`docs/step0-npc-schedule.md`](docs/step0-npc-schedule.md)).

## 여는 방법
1. Unreal Engine **5.5 이상** 설치. C++ 빌드가 필요하므로 Xcode + Command Line Tools도 설치되어 있어야 함(`xcode-select -p`로 확인).
2. `Gaebokchi.uproject`를 더블클릭해서 에디터 실행. C++ 코드가 있어 처음 열 때 빌드를 물어보면 승인.
3. `docs/step0-npc-schedule.md`를 따라 DataTable 임포트, 테스트 레벨 배치, 테스트를 진행한다.

핵심 로직(시간 흐름, 스케줄 조회, NPC 이동)은 C++로 작성되어 있다 — 블루프린트로 새로 만들 필요 없이 바로 액터로 배치해서 쓰면 된다.

## 폴더 구조
```
Source/Gaebokchi/
  Public/Schedule/  — ScheduleTypes.h(EScheduleAction, FScheduleEntry), WorldClock.h, ScheduleComponent.h
  Public/World/     — ScheduleLocation.h
  Public/NPC/       — NPCAIController.h, NPCCharacter.h
  Private/          — 위 헤더들의 구현(.cpp)
Data/
  Schedule_TestNPC.csv — 스케줄 데이터 원본. 여길 고치고 DataTable을 리임포트하면 NPC 일과가 바뀐다.
Content/
  Data/Tables/ — DT_Schedule_TestNPC (CSV 임포트로 생성)
  Maps/        — L_Step0_TestVillage
docs/
  ROADMAP.md            — 전체 단계별 계획과 성립 조건
  step0-npc-schedule.md — Step 0 빌드/테스트 가이드
```

## 현재 상태
Step 0 로직(C++)과 스켈레톤은 완성. 로컬 에디터에서 빌드 → DataTable 임포트 → 테스트 레벨 배치 → PIE 테스트가 남은 작업이다. `docs/step0-npc-schedule.md` 2~5절 참고.
