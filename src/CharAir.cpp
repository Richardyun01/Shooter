#include <math.h>
#include <stdio.h>

#include "CharAir.h"

//메인 캐릭터의 이동
//메인 캐릭터의 좌표, 속도, 상하 움직임, 좌우 움직임
void CharAir::MoveMyShip(float& x, float& y, float speed, bool up, bool down, bool left, bool right) {
    if (up   ) y -= speed;
    if (down ) y += speed;
    if (left ) x -= speed;
    if (right) x += speed;
}

//이동범위 고려 메인 캐릭터의 이동
//메인 캐릭터의 좌표, 속도, 좌상좌표, 우하좌표, 테두리의 좌상좌표, 우하좌표, 상하 움직임, 좌우 움직임
void CharAir::MoveMyShip2(float& x, float& y, float speed, float x0, float y0, float x1, float y1, float sx0, float sy0, float sx1, float sy1, bool up, bool down, bool left, bool right) {
    if (up    && sy0 <= y0) y -= speed;
    if (down  && y1 <= sy1) y += speed;
    if (left  && sx0 <= x0) x -= speed;
    if (right && x1 <= sx1) x += speed;
}

//롤 표현
//롤의 각도, 좌우 움직임
void CharAir::RollMyShip(int& roll, bool left, bool right) {
    //좌측 이동: -20<roll일시 roll -1
    if (left && -20 < roll) roll--;

    //우측 이동: roll<20일시 roll +1
    if (right && roll < 20) roll++;

    //미이동: roll이 0에 근접
    if (!left && !right) {
        if (roll < 0) roll++;
        else if (roll > 0) roll--;
    }

    //roll의 값에 따라 다른 패턴 표시
    if (-20 <= roll && roll < -15)     Draw(LEFT_BIG_ROLL);
    else if (-15 <= roll && roll < -5) Draw(LEFT_SMALL_ROLL);
    else if (-5 <= roll && roll <= 5)  Draw(CENTER);
    else if (5 < roll && roll <= 15)   Draw(RIGHT_SMALL_ROLL);
    else if (15 < roll && roll <= 20)  Draw(RIGHT_BIG_ROLL);
}

//3D에서 롤 표현
//롤의 각도, 좌우 움직임
void CharAir::RollMyShip2(int& roll, bool left, bool right) {
    //좌측 이동: -60<roll일시 roll -3
    if (left && -60 < roll) roll-=3;

    //우측 이동: roll<60일시 roll +3
    if (right && roll < 60) roll+=3;

    //미이동: roll이 0에 근접
    if (!left && !right) {
        if (roll < 0) roll+=3;
        else if (roll > 0) roll-=3;
    }

    //roll의 값에 따라 다른 패턴 표시
    Draw(roll);
}

//상하좌우와 대각선방향의 속도가 같도록 조절하여 이동
//메인 캐릭터의 좌표, 속도, 상하 이동, 좌우 이동
void CharAir::MoveAtConstantSpeed(float& x, float& y, float speed, bool up, bool down, bool left, bool right) {
    //대각선 이동 속도: 약 0.7배
    float s = speed / sqrt(2);

    //대각선 이동
    if (up   && left ) x -= s, y -= s; else
    if (up   && right) x += s, y -= s; else
    if (down && left ) x -= s, y += s; else
    if (down && right) x += s, y += s; else

    //상하좌우 이동
    if (up    && !down ) y -= speed; else
    if (down  && !up   ) y += speed; else
    if (left  && !right) x -= speed; else
    if (right && !left ) x += speed;
}


//아날로그 입력 이동
//메인 캐릭터의 좌표, 최고 속도, 스틱 입력값, 입력값 범위, margin
void CharAir::MoveByAnalog(float& x, float& y, float speed, int jx, int jy, int range, int margin) {
    //X방향 이동
    if (jx <= -range + margin) x -= speed; else
    if (jx >= +range + margin) x += speed; else
    if (jx < -margin) x += speed * (jx + margin) / (range - margin * 2); else
    if (jx > +margin) x += speed * (jx - margin) / (range - margin * 2);

    //Y방향 이동
    if (jy <= -range + margin) y -= speed; else
    if (jy >= +range + margin) y += speed; else
    if (jy < -margin) y += speed * (jy + margin) / (range - margin * 2); else
    if (jy > +margin) y += speed * (jy - margin) / (range - margin * 2);
}

//워프 이동
//메인 캐릭터의 좌표, 이동 속도, 워프 이동 거리, 상하 이동, 좌우 이동
void CharAir::Warp(float& x, float& y, float speed, float warp_dist, bool up, bool down, bool left, bool right) {
    static bool
        up0 = false, down0 = false,     //이전 조작 방향
        left0 = false, right0 = false,  //상하좌우
        released = true;                //무입력 상태 플래그
    static int time = 0;                //조작 방향 입력의 제한 시간

    //워프 처리: 조작 입력 시간 내에 특정 방향으로 조작 후 조작을 제자리로 되돌렸다가 다시 같은 방향으로 조작했을 때
    //워프 미처: 조작 이전 상태가 무입력 상태가 아니거나, 제한시간이 지났거나, 이전과 다른 방향으로 조작했을 때 방향 기록 후 제한 시간 설정
    if (up || down || left || right) {
        if (released && time > 0 && up == up0 && down == down0 && left == left0 && right == right0) {
            if (up   ) y -= warp_dist;
            if (down ) y -= warp_dist;
            if (left ) y -= warp_dist;
            if (right) y -= warp_dist;
        }  else {
            up0 = up; down0 = down; left0 = left; right0 = right;
            time = 10;

            //그냥 이동
            if (up   ) y -= warp_dist;
            if (down ) y -= warp_dist;
            if (left ) y -= warp_dist;
            if (right) y -= warp_dist;
        }

        released = false;
    } else {
        released = true; //조작 무입력 상태
    }

    //시간 감소
    if (time > 0) time--;
}

//스피드 조절
//스피드, 스피드업 간격, 최고 속도, 최저 속도, 속도 조절 조작1
void CharAir::SpeedControlByButton(float& speed, float accel, flat max_speed, float min_speed, bool button) {
    //조작1 이전 상태
    static bool prev_button = false;

    //조작1 수행 후 이동 : 속도 조절 수행
    if (!prev_button && button) {
        if (speed >= max_speed) speed = min_speed;
        else speed += accel;
    }

    //조작 상태 기록
    prev_button = button;
}

//합체 처리
//아군의 좌표, 적의 좌표, 메인 캐릭터의 좌표, 아군의 상태
void CharAir::Docking(float& cx, float& cy, float ex, float ey, float mx, float my, DOCKING_STATE& state) {
    switch (state) {
    //포획: 적기와 함께 움직임, 적기 파괴시 부유 상태로 이동
    case CAPTURED:
        cx = ex; cy = ey;
        if (EnemyDestroyed()) state = FLOATING;
        break;
    //부유: 화면 아랫방향으로, 직진, 메인 캐릭터에 충돌 시 합체 상태로 이동
    case FLOATING:
        cy++;
        if (Catched()) state = DOCKED;
        break;
    //합체: 메인 캐릭터와 함께 움직임
    case DOCKED:
        cx = mx; cy = my;
        break;
    }
}
