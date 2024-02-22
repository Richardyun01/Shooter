#include <math.h>
#include <stdio.h>

#include "CharGround.h"

//땅 위를 걷는 메인 캐릭터의 움직임
//메인 캐릭터의 좌표, 속도, 좌우이동, 점프
void CharGround::Walk(float& x, float& y, float speed, bool left, bool right, bool jump_button) {
    static MYSHIP_STATE state = WALKING;    //메인 캐릭터의 상태
    static int time;                        //점프 시간

    //좌우이동
    if (left ) x -= speed;
    if (right) x += speed;

    //상태에 따라 분기
    switch (state) {
    //보행: 점프시 상승 상태로 이동
    case WALKING:
        if (jump_button) {
            state = JUMP_UP;
            time = 40;
        }
        break;
    //보행: 일정 시간 상승시 하강 상태로 이동
    case JUMP_UP:
        y -= speed;
        if (time == 0) {
            state = JUMP_DOWN;
            time = 40;
        } else {
            time--;
        }
        break;
    //보행: 일정 시간 하강 시 보행 상태로 이동
    case JUMP_DOWN:
        y += speed;
        if (time == 0) state = WALKING;
        else time--;
        break;
    }
}

//변신하는 캐릭터
//메인 캐릭터의 변신
void CharGround::Transform(bool button) {
    static MYSHIP_FORM formation = WALKING;
    static bool prev_button = false;

    if (!prev_button && button) {
        if (formation == WALKING) formation = FLYING;
        else formation = WALKING;
    }
    prev_button = button;

    if (formation == WALKING) Walk;
    else Fly;
}

//물속에서 이동(고도 계산)
//메인 캐릭터의 좌표, 속도 벡터, 수면 높이
void Underwater1(float& x, float& y, float vx, float vy, float h) {
    if (y > h) {
        x += vx; y += vy;           //공중: 보통 속도
    } else {
        x += vx / 2; y += vy / 2;   //수중: 젊반 속도
    }
}

//물속에서 이동(물과 메인 캐릭터의 충돌 판정)
//메인 캐릭터의 좌표, 좌상좌표, 우하좌표, 속도벡터, 수면 충돌 판정 영역, 좌상/우하 좌표, 수명 충돌 판정 영역 수
void Underwater2(float& x, float& y, float x0, float y0, float x1, float y1, float vx, float vy, float wx0[], float wy0[], float wx1[], float wy1[], int num_water) {
    //메인 캐릭터의 위치를 체크
    int i;
    for (i = 0; i < num_water; i++) {
        if (wx[0] <= x0 && x1 <= wx1[i] && wy0[i] <= y0 && y1 <= wy[i]) break;
    }

    //수중에 있는 경우: 속도 절반
    if (i < num_water) {
        x += vx / 2;
        y += vy / 2;
    } else {
    //그 외의 경우: 보통 속도
        x += vx;
        y += vy;

        //메인 캐릭터가 수면에 충돌했는지 체크
        for (i = 0; i < num_water; i++) {
            if (wx[0] < x0 && x1 < wx1[i] && wy0[i] < y0 && y1 < wy[i]) break;
        }

        //메인 캐릭터가 수면에 충: 물 튀는 효과 구현
        if (i < num_water) WaveEffect();
    }
}
