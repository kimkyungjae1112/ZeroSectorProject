# 4-1 캡스톤 디자인 프로젝트

### 제 0 구역 ( ZeroSector )

- 개발 인원 : 김경재 곽준혁 김예빈 정지민 최민준
- 개발 기간 : 25.03.11 ~ 25.06.09
- 게임 엔진 : 언리얼 엔진 5.5.3
- 장르 : 추리 + 1인칭 FPS
- 게임 소개 : 좀비 바이러스의 치료제를 개발할 수 있는 유일한 연구원을 찾아 폐쇄된 연구소에서 탈출하는 이야기

<br><br>



# 아키텍처 요약
게임 컨텐츠 개발에 있어 필요한 주요 클래스들의 설계는 다음과 같습니다.

<br>

## Character 클래스 상속 구조

![Character 클래스 상속 구조](https://github.com/user-attachments/assets/30b26a16-b0c0-4ebe-b04d-28faf7a6d9a8)

**설계 의도:** 
Player와 Zombie가 공통으로 필요로 하는 기능을 공통 인터페이스로 제공하여 코드의 재사용성과 일관성을 확보했습니다.

**공통 인터페이스:** 
- 팀을 구별하고, 데이터 참조에 필요한 각 객체의 이름을 가져오는 기능을 포함합니다.

**AI 확장성:** 
- `Base_Zombie` 클래스를 별도로 두어, 모든 좀비 타입이 공유할 AI 공통 인터페이스를 구현할 기반을 마련했습니다.

<br>

## Player 클래스: 컴포넌트 기반 설계

![Player 클래스 컴포넌트 구조](https://github.com/user-attachments/assets/b5546de3-740e-43a8-a9a0-8ee246535adf)

**유연한 구조:** 
Player 클래스 자체는 최소한의 기능만 보유하고, 대부분의 기능은 독립적인 컴포넌트로 구현하여 부착하는 방식을 채택했습니다. 이를 통해 프로젝트 확장성을 높이고 컴포넌트 간 **디커플링(Decoupling)** 을 구현했습니다.

**메모리 최적화:** 
낮과 밤의 조작법이 다른 게임 특성을 고려해, `DayInputComponent`와 `NightInputComponent`를 분리했습니다. 
이로써 현재 게임 상태(낮/밤)에 필요한 데이터와 로직만 메모리에 로드하여 효율적인 메모리 관리가 가능해졌습니다.

<br>

## 컴포넌트 간 통신: 델리게이트 활용

![컴포넌트 간 델리게이트 통신](https://github.com/user-attachments/assets/d25f6b39-b5a3-4486-9ca8-7a5a29ff3a69)

**느슨한 결합:** 
각 컴포넌트는 서로를 직접 참조하는 대신 **델리게이트(Delegate)** 를 외부에 노출합니다.

**중앙 관리:** 
Player 클래스가 각 컴포넌트의 델리게이트에 필요한 함수를 **바인딩(Binding)** 하여 컴포넌트 간의 통신을 중재합니다. 
이 구조는 컴포넌트들의 독립성을 보장하고 시스템 전체의 결합도를 크게 낮춥니다.

<br>

## 무기 시스템: 샌드박스 패턴과 캡슐화

![무기 시스템 구현](https://github.com/user-attachments/assets/bb52de94-2b81-4c6b-823b-3dad34183cb2)

**확장에 열린 구조**: 
샌드박스 패턴(Sandbox Pattern)을 적용하여, 새로운 무기가 추가되더라도 기존 코드에 미치는 영향을 최소화하고 기능 확장이 용이하도록 설계했습니다.

**Unreal 특성을 활용한 캡슐화:** 
- 일반적으로 상위 클래스의 기능을 하위 클래스에서 사용하려면 `protected` 접근 지정자를 사용하지만, 이 시스템에서는 상위 클래스의 핵심 기능을 `private`으로 캡슐화했습니다.
- 하위 클래스에서는 가상 함수를 재정의(override)하고, 그 내부에서 `Super::` 키워드로 상위 클래스의 함수를 명시적으로 호출합니다.
- 이를 통해 캡슐화를 유지하면서도 기능을 안전하게 확장할 수 있습니다.


<br><br>


# 클래스별 역할
프로젝트에서 작성된 클래스별 역할은 다음과 같습니다.

<br>

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
- 면담 모드: 신뢰도 레벨에 따라 프리롤(특수 인덱스) 대사 테이블로 전환.
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
