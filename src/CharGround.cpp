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
