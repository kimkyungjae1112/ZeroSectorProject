# 4-1 캡스톤 디자인 프로젝트

### 제 0 구역 ( ZeroSector )

- 개발 인원 : 김경재 곽준혁 김예빈 정지민 최민준
- 개발 기간 : 25.03.11 ~ 25.06.09
- 게임 엔진 : 언리얼 엔진 5.5.3
- 장르 : 추리 + 1인칭 FPS
- 게임 소개 : 좀비 바이러스의 치료제를 개발할 수 있는 유일한 연구원을 찾아 폐쇄된 연구소에서 탈출하는 이야기

<br><br>



# 아키텍처

## 기반 클래스

**AZeroCharacterBase**
- 모든 캐릭터의 공통 기반.
- 팀 식별(`IGenericTeamAgentInterface`)과 클래스 식별(`IZeroClassIdentifierInterface`) 제공.
- 파생 클래스가 팀 아이디와 클래스명만 설정하면, `GetGenericTeamId()`와 `GetClassName()`으로 게임 전반의 판정(아군/적군, 타입 구분) 통일.

<br>

**AZeroGimmick**
- 모든 상호작용 오브젝트의 베이스.
- 공통 콜리전과 메시 제공.
- `IZeroOutlineInterface` 구현으로 하이라이트 온/오프 표준화.

<br>

## 플레이어 & 컨트롤

**AZeroCharacterPlayer**
- 낮/밤 모드 오가며 입력 컴포넌트 교체.
- 모드 전환 시 카메라/입력 매핑 변경, HUD/UI 컴포넌트 델리게이트 묶음.
- 입력 라우팅은 현재 입력 컴포넌트로 위임 (AfternoonInputComponent, NightInputComponent).
- 대화 진입 시 전용 카메라로 전환, 종료 시 복귀.
- 피해 처리에서 HP UI 경고, `SetDead()`로 사망 플로우(충돌 비활성, UI-only 모드, GameHasEnded) 마무리.

<br>

**AZeroCharacterNPC**
- 이름표/면접 마커 위젯 관리.
- 낮 시간엔 플레이어와 거리 기반 표시 토글.
- 오후 시작 위치를 브로드캐스트로 받아 이동.
- 대화 시작 시 순찰 속도를 0으로 만들었다가 원래 속도로 전환.

<br>

## 입력 컴포넌트

**UZeroInputBaseComponent**
- 이동·보기·달리기/걷기, 상호작용, 전투, 메뉴 등 공통 입력 인터페이스를 순수가상 함수로 정의.
- 낮/밤 전용 컴포넌트가 구현.
- 상호작용(오퍼/증거/강화/노트), 일시정지는 델리게이트로 UI 컴포넌트에 연결.

<br>

**UZeroInputAfternoonComponent**
- 낮 전용.
- 화면 중앙 레이 트레이스로 상호작용 대상(NPC/Proviso/Boards) 감지, 윤곽선 표시 및 “F키 안내” 제공.
- `DialogueInteract()`로 대화 진입/종료 콜백, 진입 시 이동 막고 카메라 스위칭.
- 발걸음 SFX 타이머 주기로 재생 간격 조절.

<br>

**UZeroInputNightComponent**
- 밤 전투 전용.
- 선택 무기 세트 스폰, 소켓에 부착(HUD 델리게이트 바인딩 포함).
- `ChangeWeapon()`으로 Pistol ↔ Rifle/Shotgun 전환.
- `Fire()`는 무기 타입에 맞는 발사 몽타주 재생.
- `Reloading()`은 리로드 몽타주 종료 델리게이트에서 사격 가능 상태 복구.

<br>

## UI & 대화

**UZeroUIComponent**
- UI 허브.
- 페이드 인/아웃, 노트 토글, 오퍼레이션 보드, 증거 획득, 강화 보드, 일시정지 등 모든 UI 열기/닫기와 입력 모드 전환 단일 창구.
- `OperationInteract()`로 UI 띄우고 컨트롤러를 UI-Only로 변경, Next/Cancel 처리, 필요 시 낮→밤 전환과 무기 세팅 델리게이트 호출.
- 증거 상호작용은 오브젝트 파괴 및 수집 목록 추가, 행동력(AP) 소모.

<br>

**UZeroDialogueComponent**
- DataTable 기반 대화 시스템.
- 시작 시 소유자 클래스명으로 대화 테이블/연구원 데이터(신뢰도) 로드.
- 진행 중 옵션 열리면 선택지 동적 스폰.
- 면접 모드: 신뢰도 레벨에 따라 프리롤(특수 인덱스) 대사 테이블로 전환.
- 완료 시 입력/이동/카메라 원상복구, NPC AI 재개.
- 다음 날 대사는 인덱스 증가로 자동 전환.

<br>

## 좀비 캐릭터

**AZeroCharacterBaseZombie**
- 좀비 공통 베이스.
- AI가 질의할 이동·공격 스펙 노출.
- 공격 종료 델리게이트로 애님 종료와 BT/TT 재개 연결.
- 포인트 대미지 받으면 `UAISense_Damage`에 보고, 넉백 방향 저장.

<br>

**AZeroCharacterMeleeZombie**
- 근접형.
- 두 가지 공격 몽타주, 피격/비틀거림, 일정 확률로 스태거 재생.
- 사망 시 콜리전을 래그돌로 전환, 튕겨져 나간 후 5초 뒤 소멸.
- 보스 몬스터에 의해 스폰될 시 연출용 `SpawnInit`(물리 임펄스), 낙하 착지 후 메시/캡슐 정렬 복구 루틴 포함.

<br>

**AZeroCharacterBossZombie**
- 러시/소환/일반 공격 분리.
- 사망 시 게임모드에 종료 보고, 보스 전용 SFX 재생 후 지연 파괴.
- 이동 중 간헐적으로 움찔/움직임 SFX 재생.

<br>

## 게임모드 & 흐름

**AZeroGameModeBase**
- 낮/밤 전환, 웨이브 진행 중심.
- 시작 시 밤(전투) 진입, 스폰 테이블 적용.
- 웨이브 시작 시 좀비 스포너에 타입 수치 전달.
- 모든 적 쓰러지면 파워업 포인트 지급, 승리 연출.
- 낮 전환 시 `AZeroAfternoonPlayerStart` 위치 브로드캐스트, BGM 낮 테마로 변경.
- 밤 복귀 시 전투 맵 위치/스포너 트리거 생성, HUD 전환, 타이머 시작.
- 시간 초과는 패배 처리.

<br>

## 기믹 / 상호작용

**AZeroWaveTrigger**
- 플레이어가 부딪히면 `StartWave()` 호출, 즉시 제거.
- 트리거 SFX 재생.

<br>

**AZeroOperationBoard**
- 낮에만 보이는 회전 마커 UI로 무기 선택 유도.
- 밤 시작 신호 받으면 자동 숨김.

<br>

**AZeroEnforceBoard**
- 무기를 강화할 수 있는 기믹
- UI에서 현재 무기 레벨을 정적 변수로 소유
- 무기를 소폰할 때 해당 무기 레벨 데이터를 받음

<br>

**AZeroProvisoActor**
- 증거 상호작용 대상.
- 공통 태그/하이라이트 제공.
- UI 컴포넌트가 수집/파기/행동력 차감 처리.

<br>

## 인터페이스

- 팀/클래스 식별: `IGenericTeamAgentInterface`, `IZeroClassIdentifierInterface`
- 좀비 AI: `IZeroCharacterAIInterface` (사거리·속도, 공격 트리거 질의)
- UI 흐름: `IZeroUIComponentInterface` (컨트롤러 입력 모드 전환 표준화)
- 대화: `IZeroDialogueInterface` (시작/종료 연결)
- 기믹 하이라이트: `IZeroOutlineInterface`
- 플레이어 낮/밤 전용 인터페이스: `IZeroAfternoonInputInterface`, `IZeroNightInputInterface` (HUD/카메라/무기 HUD 연결)

<br>

# 무기 & HUD

**AZeroWeaponBase**
- 모든 총기 공통 로직.
- 발사 요청 시 탄약 검사, 총구 소켓에서 레이 트레이스, 피격 판정.
- 피격 시 나이아가라 이펙트 스폰, `TakeDamage` 호출.
- 스탯은 무기 타입별 DataTable에서 레벨 인덱스로 로딩.
- 발사/재장전 몽타주는 애님 데이터 테이블에서 즉시/지연 로딩.
- 반동: 컨트롤 로테이션에 무작위 피치/요 더함.
- 샷건: 총열 방향 기준 퍼짐각 적용, 복수 트레이스.
- 모든 발사/재장전/빈 탄창/피격 알림은 전용 SFX.

<br>

**AZeroWeaponPistol / AZeroWeaponRifle / AZeroWeaponShotgun**
- 각각 메시/애님 블루프린트 지정.
- 베이스의 발사·재장전 구현 그대로 사용(라이플은 업그레이드 레벨 세터 직접 노출).

<br>

**UZeroHUDWidget**
- 밤 전투 HUD. 현재/최대 탄약, 무기 아이콘, 체력 바, 남은 시간·웨이브, 크로스헤어 스프레드, 히트 피드백.
- 게임모드 웨이브/타이머 델리게이트 바인딩, 실시간 수치 갱신.
- 피격 성공 시 히트 크로스헤어 잠깐 표시, 타이머로 원복.
- 무기와 HUD는 델리게이트로 느슨하게 연결.
- `AZeroWeaponBase`가 `OnChangedAmmo`, `OnSetMaxAmmo` 브로드캐스트 → `UZeroHUDWidget`이 현재/최대 탄약 반영.
- 바인딩은 밤 모드에서 `UZeroInputNightComponent::SetupWeapon()`에서 수행.
- 조준점 스프레드는 플레이어 컨트롤러가 관리, 무기 발사 시 `CrosshairSpread()` 호출.

<br>

**UZeroAfternoonHUDWidget**
- 낮 전용. 행동력(AP) 막대, 면담 대상 텍스트 표시/숨김.
