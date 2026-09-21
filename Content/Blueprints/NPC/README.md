# Blueprints/NPC

여기에 만들 것 (`docs/step0-npc-schedule.md` 4·6절 참고):
- `BPC_ScheduleComponent` — Actor Component. NPC의 현재 스케줄 항목을 DataTable에서 조회.
- `BP_NPC_AIController` — 스케줄 변경에 반응해 이동/행동을 실행.
- `BP_NPC_Base` — Character. `BPC_ScheduleComponent`를 부착하고 `BP_NPC_AIController`를 기본 컨트롤러로 지정.
