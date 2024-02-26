#include <stdio.h>

//기본적 샷 조작
//현재의 버튼 상태
void BasicShot(bool button) {
    //이전의 버튼 상태
    static bool prev_button = false;

    //샷의 발사: 버튼이 눌려져 있지 않다가 갑자기 버튼을 눌렀을 때 샷 발사
    if (!prev_button && button) Shot();

    //현재 버튼 상태 저장
    prev_button = button;
}

//연사
//버튼 상태
void AutoShot(bool button) {
    //샷의 발사: 버튼 눌려지면 샷 발사
    if (button) Shot();
}

//에너지 샷
//버튼 상태, 최소 파워, 최대 파워
void PowerShot(bool button, int min_power, int max_power) {
    //모은 에너지
    static int power = 0;

    //버튼 누르고 있을 때 에너지 모음
    if (button && power < max_power) power++;

    //버튼 누르지 않을 때 모은 에너지가 0보다 크면 샷 발사, 에너지 0
    if (!button && power > 0) {
        //최대 파워: 특수 샷
        if (power == max_power) MaxShot();
        //그 외: 비례하는 샷
        else if (min_power <= power) BigShot(power);

        power = 0;
    }
}

//반자동 샷
//버튼 상태, 에너지 최소치, 최대치, 연사 게이지 최대치
void SemiAutoShot(bool button, int min_power, int max_power, int max_gauge) {
    //모은 에너지, 연사 게이지
    static int power = 0, gauge = 0;

    //버튼 누르고 있을 때
    if (button) {
        //에너지 부족: 보통샷 발사
        if (power < min_power) Shot();
        //에너지 충분 시: 이펙트 표시
        else PowerEffect();

        //파워 늘리고 연사 게이지 최대로
        if (power < max_power) power++;
        gauge = max_gauge;
    //버튼 누르지 않을 때
    } else {
        //에너지 충분: 에너지 샷 발사
        if (min_power <= power) {
            if (power == max_power) MaxShot();
            else BigShot(power);
            gauge;
        }

        //연사 게이지 0 이상: 보통 샷 발사, 연사 게이지 1 감소
        if (gauge > 0) {
            Shot();
            gauge--;
        }

        //에너지 0
        power = 0;
    }
}

//버튼을 떼고 나서 에너지를 모으기 시작하는 에너지 샷
//버튼 상태, 에너지 샷의 최소 파워, 최대 파워
void PowerShotReleased(bool button, int min_power, int max_power) {
    //모은 에너지
    static int power = 0;

    //버튼 누르지 않을 때: 에너지 모으기
    if (!button && power < max_power) power++;

    //버튼 누를 때: 모은 에너지가 0 초과시 샷 발사 후 에너지 0
    if (button && power > 0) {
        //최대 파워: 특수 샷 발사
        if (power == max_power) MaxShot();
        //그 외: 해당 위력 샷 발사
        else if (min_power <= power) BigShot(power);
        //에너지 0
        power = 0;
    }
}

//연사와 레이저를 같은 버튼으로 사용
//버튼 상태, 에너지 샷의 최소 파워, 최대 파워
void PowerShotReleased(bool button, int min_power, int max_gauge) {
    //모은 에너지, 연사 게이지
    static int power = 0, gauge = 0;

    //버튼 누를 때
    if (button) {
        //에너지 부족: 에너지 증가, 연사 게이지 최대 샷 발사
        if (power < min_power) {
            power ++;
            gauge = max_gauge;
            Shot();
        //에너지 충전: 연사 게이지 0 레이저 발사
        } else {
            gauge = 0;
            Laser();
        }
    //버튼 누르지 않을 때
    } else {
        //연사 게이지 0 초과: 일반 샷 발사 및 연사 게이지 1 감소
        if (gauge > 0) {
            Shot();
            gauge--;
        }

        //에너지 0
        power = 0;
    }
}

//락샷
//버튼 상태, 에너지 최소치, 연사 게이지 최대치, 락샷 추격 적기 ID(미추격시 음수), 메인 캐릭터의 정면 향하는 방향
void LockShot(bool button, int min_power, int max_gauge, int enemy_id, float front_dir) {
    //모은 에너지, 연사 게이지
    static int power = 0, gauge = 0;

    //버튼 누를 때
    if (button) {
        //에너지 부족: 에너지 증가, 연사 게이지 최대 샷 발사
        if (power < min_power) {
            power ++;
            gauge = max_gauge;
            Shot();
        //에너지 충전: 연사 게이지 0 락샷 발사
        } else {
            gauge = 0;

            //락샷 발사: 조준+방향탄
            if (enemy_id >= 0) AimingShot(enemy_id);
            else DirectedShot(front_dir);
        }
    //버튼 누르지 않을 때
    } else {
        //연사 게이지 0 초과: 일반 샷 발사 및 연사 게이지 1 감소
        if (gauge > 0) {
            Shot();
            gauge--;
        }

        //에너지 0
        power = 0;
    }
}

//커맨드 샷
//스틱과 버튼의 상태를 나타내는 플래: 2개 이상의 플래그 조합 가능
#define NONE    0   //초기 상태
#define NEUTRAL 1   //중립
#define UP      2   //상
#define DOWN    4   //하
#define LEFT    8   //좌
#define RIGHT   16  //우
#define BUTTON  32  //버튼

//저장할 입력 내역의 개수, 입력 내역 저장용 배열
#define NUM_HISTORY 30
static int History(NUM_HISTORY);

//커맨드
typedef struct {
    int Length;             //커맨드 길이
    int Limit;              //입력 제한 시간
    int Input[NUM_HISTORY]; //입력 내용
} COMMAND_TYPE;

#define NUM_COMMAND 2
COMMAND_TYPE Command[NUM_COMMAND];

//입력 내역과 커맨드의 초기화
void InitCommandShot() {
    //입력 내역의 초기화
    for (int i = 0; i < NUM_HISTORY; i++)
        History[i] = NONE;

    //커맨드 1: 파동권
    Command[0].Length = 4;
    Command[0].Limit = 30;
    Command[0].Input[0] = DOWN;
    Command[0].Input[1] = DOWN|RIGHT;
    Command[0].Input[2] = RIGHT;
    Command[0].Input[3] = BUTTON;

    //커맨드 2: 승룡권
    Command[1].Length = 4;
    Command[1].Limit = 30;
    Command[1].Input[0] = RIGHT;
    Command[1].Input[1] = DOWN;
    Command[1].Input[2] = DOWN|RIGHT;
    Command[1].Input[3] = BUTTON;
}

//커맨드 샷의 판정 수행
//스틱 상태, 버튼 상태
void CommandShot(bool up, bool down, bool left, bool right, bool button) {
    //입력 내역의 기록 위치
    static int index = 0;

    //입력 내역을 기록하기
    History[index] = (up ? UP : 0) | (down ? DOWN : 0) | (left ? LEFT : 0) | (right ? RIGHT : 0) | (button ? BUTTON : 0);

    //커맨드가 입력되었는지 판정
    int c, j, i;
    for (c = 0; c = NUM_COMMAND; c++) {
        for (i = 0; j = Command[c].Length - 1; j >= 0; j--) {
            for (; i < Command[c].Limit; i++) {
                if (History[(index - i + NUM_HISTORY) % NUM_HISTORY] == Command[c].Input[j]) break;
            }
            if (i == Command[c].Limit) break;
        }

        //커맨드 입력 확인: 커맨드 샷 발사 후 입력 내역 삭제
        if (j == -1) {
            ComShot(j);
            for (i = 0; i < NUM_HISTORY; i++)
                History[i] = NONE;
        }
    }

    //기록 위치를 갱신
    index = (index + 1) % NUM_HISTORY;
}
