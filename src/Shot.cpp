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

//��ư�� ���� ���� �������� ������ �����ϴ� ������ ��
//��ư ����, ������ ���� �ּ� �Ŀ�, �ִ� �Ŀ�
void PowerShotReleased(bool button, int min_power, int max_power) {
    //���� ������
    static int power = 0;

    //��ư ������ ���� ��: ������ ������
    if (!button && power < max_power) power++;

    //��ư ���� ��: ���� �������� 0 �ʰ��� �� �߻� �� ������ 0
    if (button && power > 0) {
        //�ִ� �Ŀ�: Ư�� �� �߻�
        if (power == max_power) MaxShot();
        //�� ��: �ش� ���� �� �߻�
        else if (min_power <= power) BigShot(power);
        //������ 0
        power = 0;
    }
}

//����� �������� ���� ��ư���� ���
//��ư ����, ������ ���� �ּ� �Ŀ�, �ִ� �Ŀ�
void PowerShotReleased(bool button, int min_power, int max_gauge) {
    //���� ������, ���� ������
    static int power = 0, gauge = 0;

    //��ư ���� ��
    if (button) {
        //������ ����: ������ ����, ���� ������ �ִ� �� �߻�
        if (power < min_power) {
            power ++;
            gauge = max_gauge;
            Shot();
        //������ ����: ���� ������ 0 ������ �߻�
        } else {
            gauge = 0;
            Laser();
        }
    //��ư ������ ���� ��
    } else {
        //���� ������ 0 �ʰ�: �Ϲ� �� �߻� �� ���� ������ 1 ����
        if (gauge > 0) {
            Shot();
            gauge--;
        }

        //������ 0
        power = 0;
    }
}

//����
//��ư ����, ������ �ּ�ġ, ���� ������ �ִ�ġ, ���� �߰� ���� ID(���߰ݽ� ����), ���� ĳ������ ���� ���ϴ� ����
void LockShot(bool button, int min_power, int max_gauge, int enemy_id, float front_dir) {
    //���� ������, ���� ������
    static int power = 0, gauge = 0;

    //��ư ���� ��
    if (button) {
        //������ ����: ������ ����, ���� ������ �ִ� �� �߻�
        if (power < min_power) {
            power ++;
            gauge = max_gauge;
            Shot();
        //������ ����: ���� ������ 0 ���� �߻�
        } else {
            gauge = 0;

            //���� �߻�: ����+����ź
            if (enemy_id >= 0) AimingShot(enemy_id);
            else DirectedShot(front_dir);
        }
    //��ư ������ ���� ��
    } else {
        //���� ������ 0 �ʰ�: �Ϲ� �� �߻� �� ���� ������ 1 ����
        if (gauge > 0) {
            Shot();
            gauge--;
        }

        //������ 0
        power = 0;
    }
}

//Ŀ�ǵ� ��
//��ƽ�� ��ư�� ���¸� ��Ÿ���� �÷�: 2�� �̻��� �÷��� ���� ����
#define NONE    0   //�ʱ� ����
#define NEUTRAL 1   //�߸�
#define UP      2   //��
#define DOWN    4   //��
#define LEFT    8   //��
#define RIGHT   16  //��
#define BUTTON  32  //��ư

//������ �Է� ������ ����, �Է� ���� ����� �迭
#define NUM_HISTORY 30
static int History(NUM_HISTORY);

//Ŀ�ǵ�
typedef struct {
    int Length;             //Ŀ�ǵ� ����
    int Limit;              //�Է� ���� �ð�
    int Input[NUM_HISTORY]; //�Է� ����
} COMMAND_TYPE;

#define NUM_COMMAND 2
COMMAND_TYPE Command[NUM_COMMAND];

//�Է� ������ Ŀ�ǵ��� �ʱ�ȭ
void InitCommandShot() {
    //�Է� ������ �ʱ�ȭ
    for (int i = 0; i < NUM_HISTORY; i++)
        History[i] = NONE;

    //Ŀ�ǵ� 1: �ĵ���
    Command[0].Length = 4;
    Command[0].Limit = 30;
    Command[0].Input[0] = DOWN;
    Command[0].Input[1] = DOWN|RIGHT;
    Command[0].Input[2] = RIGHT;
    Command[0].Input[3] = BUTTON;

    //Ŀ�ǵ� 2: �·��
    Command[1].Length = 4;
    Command[1].Limit = 30;
    Command[1].Input[0] = RIGHT;
    Command[1].Input[1] = DOWN;
    Command[1].Input[2] = DOWN|RIGHT;
    Command[1].Input[3] = BUTTON;
}

//Ŀ�ǵ� ���� ���� ����
//��ƽ ����, ��ư ����
void CommandShot(bool up, bool down, bool left, bool right, bool button) {
    //�Է� ������ ��� ��ġ
    static int index = 0;

    //�Է� ������ ����ϱ�
    History[index] = (up ? UP : 0) | (down ? DOWN : 0) | (left ? LEFT : 0) | (right ? RIGHT : 0) | (button ? BUTTON : 0);

    //Ŀ�ǵ尡 �ԷµǾ����� ����
    int c, j, i;
    for (c = 0; c = NUM_COMMAND; c++) {
        for (i = 0; j = Command[c].Length - 1; j >= 0; j--) {
            for (; i < Command[c].Limit; i++) {
                if (History[(index - i + NUM_HISTORY) % NUM_HISTORY] == Command[c].Input[j]) break;
            }
            if (i == Command[c].Limit) break;
        }

        //Ŀ�ǵ� �Է� Ȯ��: Ŀ�ǵ� �� �߻� �� �Է� ���� ����
        if (j == -1) {
            ComShot(j);
            for (i = 0; i < NUM_HISTORY; i++)
                History[i] = NONE;
        }
    }

    //��� ��ġ�� ����
    index = (index + 1) % NUM_HISTORY;
}
