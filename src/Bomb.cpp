#include <stdio.h>

//��ź
//��ź�� ��ȿ �ð�, ������ �»���ǥ, ������ǥ, źȯ�� �»���ǥ, ������ǥ, ��, ������ �»���ǥ, ������ǥ, ��
void Bomb(int& bomb_time, float x0, float y0, float x1, float y1, float bx0[], float by0[], float bx1[], float by1[], int num_bullet, float ex0[], float ey0[], float ex1[], float ey1[], int num_enemy) {
    //��ź�� ��ȿ�� ����� ó��
    if (bomb_time > 0) {
        //źȯ ����
        for (int i = 0; i < num_bullet; i++) {
            if (bx[0] < x1 && x0 < bx1[i] && by[0] < y1 && y0 < by1[i]) {
                DeleteBullet[i];
            }
        }
    }

    //���⿡ ���� ������
    for (int i = 0; i < num_enemy; i++) {
        if (ex[0] < x1 && x0 < ex1[i] && ey[0] < y1 && y0 < ey1[i]) {
            DamageEnemy[i];
        }
    }

    //��ź�� ��ȿ �ð� ����
    bomb_time--;
}

//��������
//��ư ����, ���� ���� �»���ǥ, ������ǥ, ������ �»���ǥ, ������ǥ, ��
void Punch(bool button, float px0, float py0, float px1, float py1, float ex0[], float ey0[], float ex1[], float ey1[], int num_enemy) {
    static bool punching = false;   //���� ���� �÷���
    static int punch_time;          //���� ���� �ǽ�

    //���� ���� ����
    if (!punching && button) {
        punching = true;
        punch_time = 30;
    }

    //���� ���� ó��
    if (punching) {
        //�浹 ����
        for (int i = 0; i < num_enemy; i++) {
            if (ex[0] < px1 && px0 < ex1[i] && ey[0] < py1 && py0 < ey1[i]) {
                DamageEnemy(i);
            }
        }
    }

    //���� ���� ǥ��
    DrawPunch();

    //���� ���� ���� �Ǵ�
    if (punch_time == 0) punching = false;
    else punch_time--;
}

//���� ����
//������ �»���ǥ, ������ǥ, ��, ����ǳ�� �»���ǥ, ������ǥ, ��
void InducedExplosion(float ex0[], float ey0[], float ex1[], float ey1[], int num_enemy, float x0[], float y0[], float x1[], float y1[], int num_explosion) {
    //����� ����ǳ�� �浹 ���� ó��
    for (int i = 0; i < num_enemy; i++) {
        for (int j = 0; j < num_explosion; j++) {
            if (ex0[i] < x1[j] && x0[j] < ex1[i] && ey0[i] < y1[j] && y0[j] < ey1[i]) {
                DestroyEnemy(i);
                CreateExplosion(ex0[i], ey0[i]);
            }
        }
    }
}
