# Final_Project
임베디드 시스템 기말 대체 프로젝트를 위한 레포지토리 입니다. **2분반 1조**
## 목차
- [조원 및 역할](#조원)
- [일정 정리](#일정-정리)
- [1차 계획 발표 내용 정리](#1차-계획-발표-내용)
- [2차 계획 수립](#2차-계획-수립)
- [개발 현황 정리](#개발-현황-정리)
- [프로젝트 구현 결과](#프로젝트-구현-결과)





###    조원

|학번|이름|역할|
|------|---|---|
|20190053|곽지호|프로젝트 총괄, LED / 스피커 구현|
|20210104|김경미|LED/스위치 구현, 서보모터 구현|
|20210347|김현서|로터리 엔코더 구현|
|20210908|이지윤|초음파 센서/ 카메라 구현|


### 일정 정리
|기한|내용|
|------|---|
|12월 14일|프로젝트 결과 발표 제출[팀장 제출]|
|12월 21일|팀원 상호 평가 제출[개인 제출]|
|12월 21일|Github [보고서 대체] 제출|
|12월 21일|전체 데모 영상 제작 및 발표|

### 1차 계획 발표 내용
![계획](https://github.com/ES-TEAM-0201/Final_Project/assets/94772949/26e23579-344d-4260-9e13-bc9d78848bb6)
![일정](https://github.com/ES-TEAM-0201/Final_Project/assets/94772949/eab3f602-022c-42ef-9637-6675661cad6d)

### 2차 계획 수립
**(1차 계획 발표와 변경된 점)**
- 엑추에이터들의 동시성 처리를 위한 쓰레드 계획
- 라즈베리 파이 GPIO Port 부족으로 인한 라즈베리 파이간 통신 엑츄에이터인 UART 통신 계획

- 각 라즈베리 파이의 엑추에이터 세팅 계획
 ![그림2](https://github.com/ES-TEAM-0201/Final_Project/assets/94772949/f876b9b4-3527-4725-884d-c00c4e5140b2)
![그림3](https://github.com/ES-TEAM-0201/Final_Project/assets/94772949/83080d41-02d5-4338-b280-2d126dd3eb99)


### 개발 현황 정리
|개발 내용|완료 여부|담당자|
|------|---|---|
|스피커/LED 경고 엑추에이터 구현|완료|곽지호|
|LED/스위치 구현||
|서보모터 구현|||
|로터리 엔코더 구현|||
|초음파 센서/카메라 구현|||
|멀티 쓰레드 동작 구현|||
|각 라즈베리 파이의 UART 통신 구현|||

### 각 엑추에이터의 기능 설명
1. 스피거/LED 경고 장치
	- 3개의 잠금창치에서 3번이상 오류가 발생할 경우 스피커와 LED 를 통한 경고장치 동작
	
2. LED/스위치 잠금장치
3. 서보모터 잠금 해제 장치
4. 로터리 엔코더 잠금장치
5. 초음파/카메라 장치

###   프로젝트 구현 결과
