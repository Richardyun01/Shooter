#include <math.h>
#include <stdio.h>

#include "CharAir.h"

//���� ĳ������ �̵�
//���� ĳ������ ��ǥ, �ӵ�, ���� ������, �¿� ������
void CharAir::MoveMyShip(float& x, float& y, float speed, bool up, bool down, bool left, bool right) {
    if (up   ) y -= speed;
    if (down ) y += speed;
    if (left ) x -= speed;
    if (right) x += speed;
}

//�̵����� ��� ���� ĳ������ �̵�
//���� ĳ������ ��ǥ, �ӵ�, �»���ǥ, ������ǥ, �׵θ��� �»���ǥ, ������ǥ, ���� ������, �¿� ������
void CharAir::MoveMyShip2(float& x, float& y, float speed, float x0, float y0, float x1, float y1, float sx0, float sy0, float sx1, float sy1, bool up, bool down, bool left, bool right) {
    if (up    && sy0 <= y0) y -= speed;
    if (down  && y1 <= sy1) y += speed;
    if (left  && sx0 <= x0) x -= speed;
    if (right && x1 <= sx1) x += speed;
}

//�� ǥ��
//���� ����, �¿� ������
void CharAir::RollMyShip(int& roll, bool left, bool right) {
    //���� �̵�: -20<roll�Ͻ� roll -1
    if (left && -20 < roll) roll--;

    //���� �̵�: roll<20�Ͻ� roll +1
    if (right && roll < 20) roll++;

    //���̵�: roll�� 0�� ����
    if (!left && !right) {
        if (roll < 0) roll++;
        else if (roll > 0) roll--;
    }

    //roll�� ���� ���� �ٸ� ���� ǥ��
    if (-20 <= roll && roll < -15)     Draw(LEFT_BIG_ROLL);
    else if (-15 <= roll && roll < -5) Draw(LEFT_SMALL_ROLL);
    else if (-5 <= roll && roll <= 5)  Draw(CENTER);
    else if (5 < roll && roll <= 15)   Draw(RIGHT_SMALL_ROLL);
    else if (15 < roll && roll <= 20)  Draw(RIGHT_BIG_ROLL);
}

//3D���� �� ǥ��
//���� ����, �¿� ������
void CharAir::RollMyShip2(int& roll, bool left, bool right) {
    //���� �̵�: -60<roll�Ͻ� roll -3
    if (left && -60 < roll) roll-=3;

    //���� �̵�: roll<60�Ͻ� roll +3
    if (right && roll < 60) roll+=3;

    //���̵�: roll�� 0�� ����
    if (!left && !right) {
        if (roll < 0) roll+=3;
        else if (roll > 0) roll-=3;
    }

    //roll�� ���� ���� �ٸ� ���� ǥ��
    Draw(roll);
}

//�����¿�� �밢�������� �ӵ��� ������ �����Ͽ� �̵�
//���� ĳ������ ��ǥ, �ӵ�, ���� �̵�, �¿� �̵�
void CharAir::MoveAtConstantSpeed(float& x, float& y, float speed, bool up, bool down, bool left, bool right) {
    //�밢�� �̵� �ӵ�: �� 0.7��
    float s = speed / sqrt(2);

    //�밢�� �̵�
    if (up   && left ) x -= s, y -= s; else
    if (up   && right) x += s, y -= s; else
    if (down && left ) x -= s, y += s; else
    if (down && right) x += s, y += s; else

    //�����¿� �̵�
    if (up    && !down ) y -= speed; else
    if (down  && !up   ) y += speed; else
    if (left  && !right) x -= speed; else
    if (right && !left ) x += speed;
}


//�Ƴ��α� �Է� �̵�
//���� ĳ������ ��ǥ, �ְ� �ӵ�, ��ƽ �Է°�, �Է°� ����, margin
void CharAir::MoveByAnalog(float& x, float& y, float speed, int jx, int jy, int range, int margin) {
    //X���� �̵�
    if (jx <= -range + margin) x -= speed; else
    if (jx >= +range + margin) x += speed; else
    if (jx < -margin) x += speed * (jx + margin) / (range - margin * 2); else
    if (jx > +margin) x += speed * (jx - margin) / (range - margin * 2);

    //Y���� �̵�
    if (jy <= -range + margin) y -= speed; else
    if (jy >= +range + margin) y += speed; else
    if (jy < -margin) y += speed * (jy + margin) / (range - margin * 2); else
    if (jy > +margin) y += speed * (jy - margin) / (range - margin * 2);
}

//���� �̵�
//���� ĳ������ ��ǥ, �̵� �ӵ�, ���� �̵� �Ÿ�, ���� �̵�, �¿� �̵�
void CharAir::Warp(float& x, float& y, float speed, float warp_dist, bool up, bool down, bool left, bool right) {
    static bool
        up0 = false, down0 = false,     //���� ���� ����
        left0 = false, right0 = false,  //�����¿�
        released = true;                //���Է� ���� �÷���
    static int time = 0;                //���� ���� �Է��� ���� �ð�

    //���� ó��: ���� �Է� �ð� ���� Ư�� �������� ���� �� ������ ���ڸ��� �ǵ��ȴٰ� �ٽ� ���� �������� �������� ��
    //���� ��ó: ���� ���� ���°� ���Է� ���°� �ƴϰų�, ���ѽð��� �����ų�, ������ �ٸ� �������� �������� �� ���� ��� �� ���� �ð� ����
    if (up || down || left || right) {
        if (released && time > 0 && up == up0 && down == down0 && left == left0 && right == right0) {
            if (up   ) y -= warp_dist;
            if (down ) y -= warp_dist;
            if (left ) y -= warp_dist;
            if (right) y -= warp_dist;
        }  else {
            up0 = up; down0 = down; left0 = left; right0 = right;
            time = 10;

            //�׳� �̵�
            if (up   ) y -= warp_dist;
            if (down ) y -= warp_dist;
            if (left ) y -= warp_dist;
            if (right) y -= warp_dist;
        }

        released = false;
    } else {
        released = true; //���� ���Է� ����
    }

    //�ð� ����
    if (time > 0) time--;
}

//���ǵ� ����
//���ǵ�, ���ǵ�� ����, �ְ� �ӵ�, ���� �ӵ�, �ӵ� ���� ����1
void CharAir::SpeedControlByButton(float& speed, float accel, flat max_speed, float min_speed, bool button) {
    //����1 ���� ����
    static bool prev_button = false;

    //����1 ���� �� �̵� : �ӵ� ���� ����
    if (!prev_button && button) {
        if (speed >= max_speed) speed = min_speed;
        else speed += accel;
    }

    //���� ���� ���
    prev_button = button;
}

//��ü ó��
//�Ʊ��� ��ǥ, ���� ��ǥ, ���� ĳ������ ��ǥ, �Ʊ��� ����
void CharAir::Docking(float& cx, float& cy, float ex, float ey, float mx, float my, DOCKING_STATE& state) {
    switch (state) {
    //��ȹ: ����� �Բ� ������, ���� �ı��� ���� ���·� �̵�
    case CAPTURED:
        cx = ex; cy = ey;
        if (EnemyDestroyed()) state = FLOATING;
        break;
    //����: ȭ�� �Ʒ���������, ����, ���� ĳ���Ϳ� �浹 �� ��ü ���·� �̵�
    case FLOATING:
        cy++;
        if (Catched()) state = DOCKED;
        break;
    //��ü: ���� ĳ���Ϳ� �Բ� ������
    case DOCKED:
        cx = mx; cy = my;
        break;
    }
}
