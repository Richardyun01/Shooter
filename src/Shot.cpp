#include <stdio.h>

//�⺻�� �� ����
//������ ��ư ����
void BasicShot(bool button) {
    //������ ��ư ����
    static bool prev_button = false;

    //���� �߻�: ��ư�� ������ ���� �ʴٰ� ���ڱ� ��ư�� ������ �� �� �߻�
    if (!prev_button && button) Shot();

    //���� ��ư ���� ����
    prev_button = button;
}

//����
//��ư ����
void AutoShot(bool button) {
    //���� �߻�: ��ư �������� �� �߻�
    if (button) Shot();
}

//������ ��
//��ư ����, �ּ� �Ŀ�, �ִ� �Ŀ�
void PowerShot(bool button, int min_power, int max_power) {
    //���� ������
    static int power = 0;

    //��ư ������ ���� �� ������ ����
    if (button && power < max_power) power++;

    //��ư ������ ���� �� ���� �������� 0���� ũ�� �� �߻�, ������ 0
    if (!button && power > 0) {
        //�ִ� �Ŀ�: Ư�� ��
        if (power == max_power) MaxShot();
        //�� ��: ����ϴ� ��
        else if (min_power <= power) BigShot(power);

        power = 0;
    }
}

//���ڵ� ��
//��ư ����, ������ �ּ�ġ, �ִ�ġ, ���� ������ �ִ�ġ
void SemiAutoShot(bool button, int min_power, int max_power, int max_gauge) {
    //���� ������, ���� ������
    static int power = 0, gauge = 0;

    //��ư ������ ���� ��
    if (button) {
        //������ ����: ���뼦 �߻�
        if (power < min_power) Shot();
        //������ ��� ��: ����Ʈ ǥ��
        else PowerEffect();

        //�Ŀ� �ø��� ���� ������ �ִ��
        if (power < max_power) power++;
        gauge = max_gauge;
    //��ư ������ ���� ��
    } else {
        //������ ���: ������ �� �߻�
        if (min_power <= power) {
            if (power == max_power) MaxShot();
            else BigShot(power);
            gauge;
        }

        //���� ������ 0 �̻�: ���� �� �߻�, ���� ������ 1 ����
        if (gauge > 0) {
            Shot();
            gauge--;
        }

        //������ 0
        power = 0;
    }
}
