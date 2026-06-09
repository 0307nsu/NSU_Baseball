# NSU\_Baseball

---

# 멀티플레이 숫자 야구 게임

Unreal Engine 5.5 환경에서 C++ 기반으로 구현한 턴제 멀티플레이 숫자 야구 게임입니다.



## Tech Stack

* **Engine:** Unreal Engine 5.5
* **Language:** C++ \& Blueprint(UMG)
* **Revision Control:** GitHub



---



## 시스템 아키텍처

게임의 핵심 로직은 서버 권한으로 동작하고, 클라이언트는 서버에 요청하고 결과를 받아와 UI에 동기화합니다.

* **GameModeBase**
  * 각 플레이어에게 정답 숫자(3자리) 생성
  * 채팅 입력 유효성 검사(길이, 중복, 문자 확인)
  * 스트라이크 / 볼 판정, 턴 타이머 제어, 게임 오버 판정
  
* **GameStateBase**
  * 현재 턴인 플레이어와 남은 시간 관리
  
* **PlayerState**
  * 각 플레이어의 남은 입력 기회 관리
  
* **PlayerController**
  * ServerRPC: 클라이언트의 채팅 입력을 서버로 전송
  * ClientRPC: 서버의 판정 결과나 경고 메시지를 클라이언트의 UI로 전송
  
* **UserWidget**
  * 로직은 C++에서 담당. 디자인은 UMG에서 담당



---



## 구현 과정



### 기본 판정 및 데이터 분리

* GameModeBase에서 접속한 플레이어에게 정답을 부여하고, TMap으로 관리
* PlayerController에서 플레이어로부터 입력받은 문자열을 서버로 전달하는 기초 구조 설립



### 턴제 및 동기화 최적화

* GameState에서 타이머와 현재 턴인 플레이어 변수 추가
* OnRep을 활용하여 값이 변할 때만 클라이언트 UI가 업데이트되도록 구현



### C++ \& UMG 연동

* UUserWidget을 상속받는 C++클래스를 생성하고 meta=(BindWidget)으로 블루프린트 UI와 결합
* 플레이어의 이전 입력 및 결과가 표시되도록 UI에 추가. 엔터 키 입력 지원 추가



### UX 개선

* 플레이어의 입력이 잘못되었거나, 상대 턴 입력 시 경고 문구 출력하여 알림
* BindWidgetAnim을 통해 경고 문구가 페이드 아웃하는 애니메이션 효과를 줌