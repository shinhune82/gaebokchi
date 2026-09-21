# Step 0 — 단일 NPC 일과 시스템 (블루프린트 빌드 가이드)

대상: UE 5.5 이상. 이 문서를 그대로 따라가면 로컬 에디터에서 Step 0을 완성할 수 있다.

## 성립 조건 (완료 기준)
데이터(DataTable)만 바꾸면 블루프린트 그래프를 손대지 않고 NPC의 일과가 즉시 바뀐다.
테스트: 스케줄 테이블의 행 순서/시간/장소를 바꾼 뒤, 재컴파일 없이(또는 그래프 수정 없이) NPC가 새 스케줄대로 움직이는지 확인한다.

## 1. 프로젝트 폴더 구조
```
Content/
  Blueprints/
    Core/
      BP_WorldClock
    NPC/
      BPC_ScheduleComponent   (Actor Component)
      BP_NPC_AIController
      BP_NPC_Base             (Character)
    World/
      BP_ScheduleLocation     (Actor, 장소 마커)
  Data/
    Enums/
      E_ScheduleAction
    Structs/
      S_ScheduleEntry
    Tables/
      DT_Schedule_TestNPC
  Maps/
    L_Step0_TestVillage
```

## 2. 데이터 정의

### Enum: `E_ScheduleAction`
NPC가 도착 후 수행할 행동 타입. Step 0에서는 표현만 하고 실제 애니메이션은 생략 가능.
- `Idle`
- `Train` (수련)
- `Eat` (식사)
- `Sleep` (취침)
- `Patrol` (순찰)

### Struct: `S_ScheduleEntry`
| 필드 | 타입 | 설명 |
|---|---|---|
| `StartHour` | Float (0~24) | 이 행동이 시작되는 게임 시각 |
| `EndHour` | Float (0~24) | 종료 시각 (다음 행동 시작 전까지) |
| `LocationTag` | Gameplay Tag 또는 Name | 이동할 장소를 찾는 태그 |
| `Action` | `E_ScheduleAction` | 도착 후 수행할 행동 |

### DataTable: `DT_Schedule_TestNPC` (Row Struct = `S_ScheduleEntry`)
예시 데이터 (StartHour 오름차순으로 반드시 정렬):
| RowName | StartHour | EndHour | LocationTag | Action |
|---|---|---|---|---|
| Row0 | 6.0 | 8.0 | Loc.TrainingGround | Train |
| Row1 | 8.0 | 12.0 | Loc.Market | Idle |
| Row2 | 12.0 | 13.0 | Loc.Home | Eat |
| Row3 | 13.0 | 22.0 | Loc.TrainingGround | Train |
| Row4 | 22.0 | 6.0 | Loc.Home | Sleep |

> Row4처럼 자정을 걸치는 구간(22시~다음날 6시)은 조회 함수에서 `StartHour > EndHour`인 경우를 "자정 넘김"으로 별도 처리해야 한다 (아래 4-2 참고).

## 3. `BP_WorldClock` (Actor, 레벨에 1개 배치)
게임 내 시간을 흐르게 하고, 매 틱마다 갱신한다.

**변수**
- `GameHour` (Float, 0~24, 기본값 6.0)
- `HoursPerRealSecond` (Float, 기본값 테스트용으로 크게, 예: `0.2` → 게임 5시간 = 실제 1초. 테스트 시 전체 하루 사이클을 몇 분 안에 볼 수 있게)

**이벤트 디스패처**
- `OnHourChanged (NewHour: Float)`

**로직 (Event Tick)**
1. `GameHour += DeltaSeconds * HoursPerRealSecond`
2. `GameHour`이 24 이상이면 `GameHour -= 24` (Wrap)
3. `Call OnHourChanged(GameHour)` — 매 틱 브로드캐스트해도 되고, 정수 시간이 바뀔 때만 호출하도록 최적화해도 됨(Step 0에서는 매 틱 브로드캐스트로 충분)

레벨 블루프린트나 GameMode에서 `BP_WorldClock` 인스턴스를 쉽게 참조할 수 있도록 `Get Actor Of Class`로 찾거나, GameState에 참조 변수를 두는 것을 권장.

## 4. `BPC_ScheduleComponent` (Actor Component)
NPC 캐릭터에 붙이는 컴포넌트. "지금 몇 시니까 뭘 해야 하는가"를 데이터에서 조회하는 역할만 한다. 이동/행동 실행은 AIController가 담당(관심사 분리).

**변수**
- `ScheduleTable` (DataTable Reference, Row Struct = `S_ScheduleEntry`) — 인스턴스 편집 가능(Instance Editable)으로 노출해서 NPC마다 다른 테이블을 꽂을 수 있게 한다.
- `CurrentEntry` (`S_ScheduleEntry`, 캐시된 현재 행동)

**이벤트 디스패처**
- `OnScheduleChanged (NewEntry: S_ScheduleEntry)`

**함수: `FindEntryForHour(Hour: Float) -> S_ScheduleEntry`**
1. `ScheduleTable`의 모든 행을 가져온다 (`Get Data Table Row Names` + `Get Data Table Row`, 또는 `Get All Data Table Rows` 유틸리티 함수 사용).
2. 각 행에 대해:
   - 자정 안 넘김 (`StartHour <= EndHour`): `Hour >= StartHour AND Hour < EndHour` 이면 매치
   - 자정 넘김 (`StartHour > EndHour`): `Hour >= StartHour OR Hour < EndHour` 이면 매치
3. 매치되는 첫 행을 반환. (테이블은 StartHour 오름차순 정렬이 전제이므로 첫 매치가 곧 정답)

**로직 (`BeginPlay`)**
1. `WorldClock` 참조를 얻어 `OnHourChanged`에 바인딩(Bind Event)
2. 바인딩된 함수에서: `NewEntry = FindEntryForHour(NewHour)`
3. `NewEntry`가 `CurrentEntry`와 다르면(`LocationTag`나 `Action`이 다르면) → `CurrentEntry = NewEntry`, `Call OnScheduleChanged(NewEntry)`

이 "달라졌을 때만 이벤트 발생" 처리가 핵심이다. 매 틱 `OnHourChanged`가 와도, 스케줄이 실제로 바뀌는 시점에만 AIController가 반응하게 해서 매 틱 MoveTo를 재호출하는 낭비를 막는다.

## 5. `BP_ScheduleLocation` (Actor)
레벨에 배치하는 장소 마커. `Target Point`를 부모로 만들면 간단하다.

**변수**
- `LocationTag` (Gameplay Tag 또는 Name) — 이 장소를 식별하는 태그. `DT_Schedule_TestNPC`의 `LocationTag`와 매칭됨.

레벨(`L_Step0_TestVillage`)에 최소 3개 배치: `Loc.Home`, `Loc.TrainingGround`, `Loc.Market`.

## 6. `BP_NPC_AIController`
`BP_NPC_Base`(Character)에 기본 AIController로 지정.

**로직 (`BeginPlay` 또는 `OnPossess`)**
1. 빙의한 Pawn에서 `BPC_ScheduleComponent`를 가져와 `OnScheduleChanged`에 바인딩
2. 바인딩된 함수(`HandleScheduleChanged(NewEntry)`):
   a. `NewEntry.LocationTag`와 일치하는 `BP_ScheduleLocation`을 찾는다 (`Get All Actors Of Class` + 태그 필터, 또는 매 NPC BeginPlay 시 한 번 캐싱해서 Tag→Actor 맵을 만들어두면 더 좋음)
   b. `AI MoveTo` (Move To Location or Actor) 호출, 목적지 = 해당 장소 Actor
   c. `On Success` 델리게이트에서: 행동 상태를 표시 (Step 0에서는 단순히 `Print String`으로 "NPC가 {Action} 시작"만 찍어도 충분. 이후 스텝에서 애니메이션/BT Task로 교체)

## 7. 테스트 절차 (성립 조건 검증)
1. `L_Step0_TestVillage`에 `BP_WorldClock` 1개, `BP_ScheduleLocation` 3개(Home/TrainingGround/Market), `BP_NPC_Base` 1개(스케줄 컴포넌트에 `DT_Schedule_TestNPC` 연결) 배치.
2. `HoursPerRealSecond`를 크게 설정(예: 하루 24시간이 실제 2~3분 안에 다 돌도록)하고 플레이 인 에디터(PIE) 실행.
3. NPC가 시간에 따라 TrainingGround → Market → Home → TrainingGround → Home 순으로 이동하는지 관찰.
4. **핵심 테스트**: PIE를 멈추고 `DT_Schedule_TestNPC`에서 Row1(Market)의 `LocationTag`를 `Loc.Home`으로 바꾸거나 행 순서를 조정한 뒤, 블루프린트 그래프를 전혀 수정하지 않고 다시 PIE 실행 → NPC가 바뀐 데이터대로 움직이면 Step 0 통과.

## 8. Step 0 완료 후 다음 단계
Step 1(다중 NPC 동시 실행)로 넘어가려면 `BP_NPC_Base`를 레벨에 10~20개 복제 배치하고, 각자 다른 `DT_Schedule_*` 테이블을 물려서 같은 장소(예: Market)에 여러 NPC가 겹칠 때 자연스럽게 공존하는지 확인한다. `ROADMAP.md` 참고.
