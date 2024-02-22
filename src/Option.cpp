#include <math.h>
#include <stdio.h>

//�������� ����� �Ŀ���
//�Ŀ��� ��ư�� ����
void PowerUpGauge(bool power_up_button) {
    static POWER_UP_TYPE cursor = NONE;

    //������ ȹ�� ó��: ������ ȹ�� �� ���� �̵�, �ֿ��� �̵� �� �������� �ʱ�ȭ
    if (PickItem()) {
        cursor++;
        if (cursor == END) cursor = SPEED;
    }

    //�Ŀ��� ó: ��ư�� ������ ���� Ŀ���� ����Ű�� �Ŀ��� ����
    if (power_up_button) PowerUp(cursor);
}

//�ɼ��� �ʱ�ȭ�� �̵�
//���� ĳ������ ��ǥ, ���� ��ǥ, ���� ��ǥ�� ����
void InitOption(float x, float y, float ox[], float oy[], int length) {
    //�迭�� ��� ��Ҹ� ���� ĳ������ ��ǥ�� �ʱ�ȭ
    for (int i = 0; i < length; i++) {
        ox[i] = x;
        oy[i] = y;
    }
}

//�ɼ��� �̵�
//���� ĳ������ ��ǥ, ���� ��ǥ, ���� ��ǥ�� ����, ��ǥ�� ���� ��ġ, �ɼ��� ����, ����
void MoveOption(float x, float y, float ox[], float oy[], int length, int& index, int opt_count, int opt_interval) {
    //�ɼ� �׸���
    for (int c = 0; i = index, c < opt_count; c++) {
        i = (i - opt_interval + length) % length;
        DrawOption(ox[i], oy[i]);
    }

    //���� ĳ������ ��ǥ ���� �� ��ǥ�� ���� ��ġ ����
    ox[index] = x;
    oy[index] = y;
    index = (index + 1) % length;
}

//��ȣ��
//��ȣ�� �浹 ���� ����, �»�/����, źȯ �浹 ���� ����, �»�/����, źȯ ��, ���� �浹 ���� ����, �»�/����, ���� ��, ��ȣ�� ���ط�, ��ȣ�� �Ѱ�ġ
void Barrier1(float x0, float y0, float x1, float y1, float bx0[], float by0[], float bx1[], float by1[], int num_bullet, float ex0[], float ey0[], float ex1[], float ey1[], int num_enemy, int& damage, int max_damage) {
    //��ȣ���� źȯ�� �浹 ���� ó��: źȯ�� �浹 �� źȯ ����/��ȣ�� ����ġ ����
    for (int i = 0; i < num_bullet && damage < max_damage; i++) {
        if (bx0[i] < x1 && x0 < bx1[i] && by0[i] < y1 && y0 < by1[i]) {
            DeleteBulltet(i);
            damage++;
        }
    }

    //��ȣ���� ���� �浹 ���� ó��: źȯ�� �浹 �� �� ����/��ȣ�� ����ġ ����
    for (int i = 0; i < num_enemy && damage < max_damage; i++) {
        if (ex0[i] < x1 && x0 < ex1[i] && ey0[i] < y1 && y0 < ey1[i]) {
            DamageEnemy(i);
            damage++;
        }
    }

    //���� �Ѱ� �ʰ�: ��ȣ�� ����
    if (damage >= max_damage) DeleteBarrier();
}

//��ư�� ����Ͽ� ��ġ�� ��ȣ��
//��ȣ�� �浹 ���� ����, �»�/����, źȯ �浹 ���� ����, �»�/����, źȯ ��, ���� �浹 ���� ����, �»�/����, ���� ��, ��ȣ�� ������, ��ȣ�� ��ư ����
void Barrier1(float x0, float y0, float x1, float y1, float bx0[], float by0[], float bx1[], float by1[], int num_bullet, float ex0[], float ey0[], float ex1[], float ey1[], int num_enemy, int& energy, bool barrier_button) {
    //��ȣ�� �ߵ�: ��ȣ�� ��ư�� ���Ȱ� �������� ���� ��
    if (barrier_button && energy > 0) {
        //��ȣ���� źȯ�� �浹 ���� ó��: źȯ�� �浹 �� źȯ ����/��ȣ�� ����ġ ����
        for (int i = 0; i < num_bullet && energy > 0; i++) {
            if (bx0[i] < x1 && x0 < bx1[i] && by0[i] < y1 && y0 < by1[i]) {
                DeleteBulltet(i);
                energy--;
            }
        }

        //��ȣ���� ���� �浹 ���� ó��: źȯ�� �浹 �� �� ����/��ȣ�� ����ġ ����
        for (int i = 0; i < num_enemy && energy > 0; i++) {
            if (ex0[i] < x1 && x0 < ex1[i] && ey0[i] < y1 && y0 < ey1[i]) {
                DamageEnemy(i);
                energy--;
            }
        }

        //������ �Һ�
        energy--;
    }
}
