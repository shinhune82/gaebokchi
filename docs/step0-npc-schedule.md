# Step 0 — 단일 NPC 일과 시스템 (C++ 버전)

핵심 로직(WorldClock, ScheduleComponent, NPCAIController, NPCCharacter, ScheduleLocation)은
`Source/Gaebokchi/`에 C++로 이미 작성되어 있다. 이 문서는 로컬 에디터에서 **빌드하고, 레벨에
배치하고, 테스트하는** 절차만 다룬다.

## 성립 조건 (완료 기준)
데이터(`Data/Schedule_TestNPC.csv`)만 바꾸면 코드를 손대지 않고 NPC의 일과가 즉시 바뀐다.

## 0. 사전 준비 (맥 기준)
- Xcode + Command Line Tools 설치되어 있어야 C++ 컴파일 가능. 터미널에서 확인:
  ```bash
  xcode-select -p
  ```
  경로가 안 나오면 `xcode-select --install` 실행.

## 1. 코드 구조
```
Source/
  Gaebokchi.Target.cs
  GaebokchiEditor.Target.cs
  Gaebokchi/
    Gaebokchi.Build.cs
    Public/Schedule/ScheduleTypes.h        — EScheduleAction, FScheduleEntry
    Public/Schedule/WorldClock.h            — AWorldClock (시간 흐름 담당)
    Public/Schedule/ScheduleComponent.h     — UScheduleComponent (현재 스케줄 조회)
    Public/World/ScheduleLocation.h         — AScheduleLocation (장소 마커)
    Public/NPC/NPCAIController.h            — ANPCAIController (이동 실행)
    Public/NPC/NPCCharacter.h               — ANPCCharacter (ScheduleComponent 부착)
    Public/Core/StepZeroGameMode.h          — AStepZeroGameMode (테스트 씬 자동 스폰)
    Private/...                              — 각 헤더의 구현
Data/
  Schedule_TestNPC.csv                       — 스케줄 데이터 원본 (여길 고치면 DataTable에 반영)
```

로직 요약:
1. `AWorldClock`이 매 틱 `GameHour`를 흘리고 `OnHourChanged`를 방송한다.
2. `UScheduleComponent`가 그걸 구독해서, 현재 시각에 맞는 `FScheduleEntry`를 `ScheduleTable`(DataTable)에서 조회하고, 값이 바뀌면 `OnScheduleChanged`를 방송한다.
3. `ANPCAIController`가 그걸 구독해서, `LocationTag`가 일치하는 `AScheduleLocation`으로 `MoveToActor`를 실행한다.

## 2. 빌드
1. `git pull`로 최신 코드를 받는다 (`Source/`, `Data/` 폴더가 새로 생김).
2. `Gaebokchi.uproject`를 더블클릭해서 연다.
3. "이 프로젝트는 C++ 코드가 있는데 컴파일된 바이너리가 없습니다. 지금 빌드할까요?" 같은 대화상자가 뜨면 **예/Build** 선택. (안 뜨면 `.uproject` 우클릭 → `Generate Xcode Project Files` 후 다시 열기)
4. 빌드가 끝나면 에디터가 열린다. 에러가 나면 스크린샷으로 알려달라.

## 3. DataTable 만들기 (CSV 임포트)
1. Content Browser에서 `Data > Tables` 폴더로 이동
2. 상단 `임포트(Import)` 버튼 클릭 → `Data/Schedule_TestNPC.csv` 선택
3. 임포트 옵션 창에서 Row Struct로 `Schedule Entry`(우리가 만든 `FScheduleEntry`) 선택 → 임포트
4. 이름을 `DT_Schedule_TestNPC`로 저장

이후 CSV 원본을 고치고 DataTable 에셋을 우클릭 → `리임포트(Reimport)` 하면 데이터가 갱신된다. **이게 Step 0 성립 조건 테스트 방법이다.**

## 4. 테스트 레벨 — 수동 배치 불필요
`AStepZeroGameMode`(`Source/Gaebokchi/Public/Core/StepZeroGameMode.h`)가 프로젝트 전역
기본 GameMode로 지정되어 있다(`Config/DefaultEngine.ini`의 `GlobalDefaultGameMode`).
**어떤 레벨이든 Play를 누르는 순간** 이 GameMode의 `BeginPlay()`가 자동으로:
- 바닥(큐브 슬래브), 방향광, NavMesh Bounds Volume(+ 런타임 내비메시 빌드)
- `WorldClock` 1개
- `ScheduleLocation` 3개 (`Loc.Home`, `Loc.TrainingGround`, `Loc.Market`, 서로 떨어진 위치)
- `NPCCharacter` 1개, `ScheduleComponent`에 `/Game/Data/Tables/Schedule_TestNPC` DataTable 자동 연결

을 스폰한다. `Content > Maps`에 빈 레벨(`L_Step0_TestVillage`)을 하나 만들어서 저장만 해두면 된다
(아무 액터도 직접 놓을 필요 없음). 이전에 수동으로 놓은 Plane 등이 있다면 지워도 되고 그냥 둬도
무방하다(자동 스폰된 바닥과 겹칠 수 있으니 지우는 걸 권장).

## 5. 테스트
1. 레벨을 열고 플레이 버튼(PIE) 실행
2. `출력 로그(Output Log)`에서 `StepZeroGameMode`나 `NPCAIController` 관련 로그, 경고가 있는지 확인 (특히 "DataTable을 찾지 못함" 경고가 뜨면 3절의 임포트 경로/이름을 다시 확인)
3. NPC가 TrainingGround → Market → Home → TrainingGround → Home 순으로 이동하는지 관찰
4. **핵심 테스트**: PIE 멈추고 `Data/Schedule_TestNPC.csv`에서 아무 행이나 `LocationTag`나 시간을 바꾼 뒤, `Schedule_TestNPC` DataTable을 리임포트(코드 재컴파일 불필요) → 다시 PIE 실행 → NPC가 바뀐 데이터대로 움직이면 Step 0 통과.

기본 `HoursPerRealSecond`는 0.2(게임 5시간당 실제 1초)로 하루가 몇 분 안에 다 돌아간다. 더 빠르게
보고 싶으면 `Source/Gaebokchi/Public/Schedule/WorldClock.h`의 기본값을 바꾸고 재컴파일(에디터에서
Ctrl+Alt+F11 라이브 코딩 또는 재시작)하면 된다.

## 6. Step 0 완료 후
Step 1(다중 NPC 동시 실행)로 넘어가려면 `NPCCharacter`를 레벨에 여러 개 배치하고, 각자 다른 DataTable(CSV에서 새로 만들어 임포트)을 물려서 여러 NPC가 같은 장소(Market 등)에 겹칠 때 자연스럽게 공존하는지 확인한다. `docs/ROADMAP.md` 참고.
