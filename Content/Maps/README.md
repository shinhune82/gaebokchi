# Maps

여기 만드는 것:
- `L_Step0_TestVillage` — 액터 배치(전부 C++ 클래스를 "액터 배치" 메뉴에서 검색해서 바로 놓으면 됨, 블루프린트 래핑 불필요):
  - `WorldClock` 1개
  - `ScheduleLocation` 3개 (`LocationTag`를 각각 `Loc.Home`, `Loc.TrainingGround`, `Loc.Market`으로 설정)
  - `NPCCharacter` 1개 (`ScheduleComponent > Schedule Table`에 `DT_Schedule_TestNPC` 연결)

만든 뒤 Project Settings > Maps & Modes > Editor Startup Map을 이 레벨로 지정해두면 다음에 열 때 편하다.
