#include <math.h>
#include <stdio.h>

#include "CharGround.h"

//�� ���� �ȴ� ���� ĳ������ ������
//���� ĳ������ ��ǥ, �ӵ�, �¿��̵�, ����
void CharGround::Walk(float& x, float& y, float speed, bool left, bool right, bool jump_button) {
    static MYSHIP_STATE state = WALKING;    //���� ĳ������ ����
    static int time;                        //���� �ð�

    //�¿��̵�
    if (left ) x -= speed;
    if (right) x += speed;

    //���¿� ���� �б�
    switch (state) {
    //����: ������ ��� ���·� �̵�
    case WALKING:
        if (jump_button) {
            state = JUMP_UP;
            time = 40;
        }
        break;
    //����: ���� �ð� ��½� �ϰ� ���·� �̵�
    case JUMP_UP:
        y -= speed;
        if (time == 0) {
            state = JUMP_DOWN;
            time = 40;
        } else {
            time--;
        }
        break;
    //����: ���� �ð� �ϰ� �� ���� ���·� �̵�
    case JUMP_DOWN:
        y += speed;
        if (time == 0) state = WALKING;
        else time--;
        break;
    }
}

//�����ϴ� ĳ����
//���� ĳ������ ����
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

//���ӿ��� �̵�(�� ���)
//���� ĳ������ ��ǥ, �ӵ� ����, ���� ����
void Underwater1(float& x, float& y, float vx, float vy, float h) {
    if (y > h) {
        x += vx; y += vy;           //����: ���� �ӵ�
    } else {
        x += vx / 2; y += vy / 2;   //����: ���� �ӵ�
    }
}
