#ifndef _CHARAIR_H_
#define _CHARAIR_H_

#include <math.h>

class Bullet {
protected:
    float x, y;     // 좌표
    float vx, vy;   // 속도 벡터

public:
    virtual void Move() = 0;     // 이동 함수
    virtual void Draw() = 0;     // 렌더링 함수

    //이동
    void MoveMyShip(float& x, float& y, float speed, bool up, bool down, bool left, bool right);
    void MoveMyShip2(float& x, float& y, float speed, float x0, float y0, float x1, float y1, float sx0, float sy0, float sx1, float sy1, bool up, bool down, bool left, bool right);

    //롤
    void RollMyShip(int& roll, bool left, bool right);
    void RollMyShip2(int& roll, bool left, bool right);

    //속도 조절 이동
    void MoveAtConstantSpeed(float& x, float& y, float speed, bool up, bool down, bool left, bool right);

    //아날로그 이동
    void MoveByAnalog(float& x, float& y, float speed, int jx, int jy, int range, int margin);

    //워프 이동
    void Warp(float& x, float& y, float speed, float warp_dist, bool up, bool down, bool left, bool right);

    //스피드 조절
    void SpeedControlByButton(float& speed, float accel, flat max_speed, float min_speed, bool button);

    //합체하는 아군 캐릭터의 움직임
    //아군 캐릭터의 움직임
    typedef enum {
        CAPTURED, FLOATING, DOCKED
    } DOCKING_STATE;

    //합체 처리
    void Docking(float& cx, float& cy, float ex, float ey, float mx, float my, DOCKING_STATE& state);
}

#endif // header guard

