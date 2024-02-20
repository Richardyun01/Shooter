#ifndef _CHARAIR_H_
#define _CHARAIR_H_

#include <math.h>

class Bullet {
protected:
    float x, y;     // ��ǥ
    float vx, vy;   // �ӵ� ����

public:
    virtual void Move() = 0;     // �̵� �Լ�
    virtual void Draw() = 0;     // ������ �Լ�

    //�̵�
    void MoveMyShip(float& x, float& y, float speed, bool up, bool down, bool left, bool right);
    void MoveMyShip2(float& x, float& y, float speed, float x0, float y0, float x1, float y1, float sx0, float sy0, float sx1, float sy1, bool up, bool down, bool left, bool right);

    //��
    void RollMyShip(int& roll, bool left, bool right);
    void RollMyShip2(int& roll, bool left, bool right);

    //�ӵ� ���� �̵�
    void MoveAtConstantSpeed(float& x, float& y, float speed, bool up, bool down, bool left, bool right);

    //�Ƴ��α� �̵�
    void MoveByAnalog(float& x, float& y, float speed, int jx, int jy, int range, int margin);

    //���� �̵�
    void Warp(float& x, float& y, float speed, float warp_dist, bool up, bool down, bool left, bool right);

    //���ǵ� ����
    void SpeedControlByButton(float& speed, float accel, flat max_speed, float min_speed, bool button);

    //��ü�ϴ� �Ʊ� ĳ������ ������
    //�Ʊ� ĳ������ ������
    typedef enum {
        CAPTURED, FLOATING, DOCKED
    } DOCKING_STATE;

    //��ü ó��
    void Docking(float& cx, float& cy, float ex, float ey, float mx, float my, DOCKING_STATE& state);
}

#endif // header guard

