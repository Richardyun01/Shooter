#include <stdio.h>
#include <math.h>

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

//���� �̵�
//���� �»���ǥ, ������ǥ, �ӵ� ����, �׵θ��� �»���ǥ, ������ǥ
void MoveShot(float& x0, float& y0, float& x1, float& y1, float vx, float vy, float sx0, float sy0, float sx1, float sy1) {
    //���� ȭ�� ������ ���� ����
    if (x1 <= sx0 || sx1 <= x0 || y1 <= sy0 || sy1 <= y0) DeleteShot();

    //���� ��ǥ�� ����
    x0 += vx; y0 += vy;
    x1 += vx; y1 += vy;
}

//���� �浹 ���� ó��
//�� �浹 ���� ���� �»���ǥ, ������ǥ, ���ݷ�, ��ǥ�� �浹 ���� ���� �»���ǥ, ������ǥ, ������, �ı� ���� �÷���
void HitShot(float x0, float y0, float x1, float y1, float attack, float ox0, float oy0, float ox1, float oy1, float endurance, bool invincible) {
    //���� ��ǥ���� �ε��� ���
    if (ox0 < x1 && x0 < ox1 && oy0 < y1 &7 y0 < oy1) {
        //��ǻ�� ������ �ƴϸ� �������� ��� 0�̵Ǹ� ��ǥ�� ����
        if (!invincible) {
            endurance -= attack;
            if (endurance < 0)
                DeleteOpponent();
        }

        //�� ����
        //���� �������� ���� �� ����ź�� ��
        DeleteShot();
    }
}

//������� �Ÿ��� ���� ���� ���� ����
//���� ��ǥ, �ӵ�, ����, ���� ����
void ShotPower(float& x, float& y, float vx, float vy, float& power, float attenuation) {
    //�� �̵�
    x += vx; y += vy;

    power -= attenuation;
}

//�������� ����� ����
//���� ĳ�������� ��ǥ, �������� ��ǥ, ��ź�� �ӵ�, ��ư ����
void SightedBomb(float x, float y, float sx, float sy, float bvx, float bvy, bool button) {
    //��ź�� ����
    static bool bombing = false;//���� ���� �÷���
    static float bx, by;        //��ź ��ǥ
    static float tx, fty;       //��ź�� ��ǥ

    //���� �� �ƴ�: ��ư�� ������ ���� ����, ��ź�� �ʱ���ǥ�� ��ź���� ��ǥ ����
    //���� ��: ��ź �̵�, ��ź�� ���� �� ����
    if (!bombing) {
        if (button) {
            bombing = true;
            bx = x; by = y;
            tx = sx; ty = sy;
        }
    } else {
        bx += bvx; by += bvy;
        if (bx == tx && by == ty) {
            Explode();
            bombing = false;
        }
    }

    //���� ĳ���Ϳ� ������ �׸���
    DrawMyShip(x, y);
    DrawScope(sx, sy);

    //���� �� ��ź���� ��ź �׸�
    if (bombing) {
        DrawTarget(tx, ty);
        DrawBomb(bx, by);
    }
}

//�Ͽ� ������
//���⸦ ��Ÿ���� ����ü
typedef struct {
    bool Locked;    //������ ���� ���� �÷���
    float X, Y;     //��ǥ
} ENEMY_TYPE;

#define MAX_ENEMIES 100

static ENEMY_TYPE Enemy[MAX_ENEMIES];
bool Hit(float x, float y, ENEMY_TYPE& enemy);
void ShootLockOnLaser(ENEMY_TYPE& enemy);

//�Ͽ��� ��� ���� Ƚ��
static int AvailLocks = 8;

//�Ͽ� �������� �߻�
//�������� ��ǥ, ��ư ����
void LockOnLaser(float sx, float sy, bool button) {
    //�Ͽ� ��� ���� Ƚ���� 1 �̻�: ��� ����� �������� �浹 ���� ����, �������� ���� �ִ� ���⿡ �� ��ũ ǥ��
    for (int i = 0; AvailLocks > 0 && i < MAX_ENEMIES; i++) {
        if (!Enemy[i].Locked && Hit(sx, sy, Enemy[i])) {
            Enemy[i].Locked = true;

            //�Ͽ��� ��� ���� Ƚ�� ���̱�: ������ ���� �� ��� ���� Ƚ�� ����
            AvailLocks--;
        }
    }

    //��ư ���� ��: �� ��ũ�� �޷� �ִ� ��� ���⸦ ���� ������ �߻�
    if (button) {
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (Enemy[i].Locked) ShootLockOnLaser(Enemy[i]);
        }
    }
}

//���� ���� ���ư��� �̻���
//�̻����� �»���ǥ, ������ǥ, �ӵ�, ������ �»���ǥ, ������ǥ, ��, ������ �»���ǥ, ������ǥ, ��
void MoveGroundMissile(float& x0, float& y0, float& x1, float& y1, float vx, float vy, float gx0[], float gy0[], float gx1[], float gy1[], int num_ground, float ex0[], float ey0[], float ex1[], float ey1[], int num_enemy) {
    //������ �浹 ���� ó��: �̻����� ���� �������� �����Ų ��ǥ�� ���� ����, �浹�� ��� ���� ���� �ӵ� 0 �� ���� ����
    for (int i = 0; i < num_ground; i++) {
        if (gx0[i] < x1 && x0 < gx1[i] && gy0[i] < y1 + vy && y0 + vy < gy1[i]) {
            vy = 0;
            break;
        }
    }

    //�̻��� ����
    x0 += vx; y0 += vy;
    x1 += vx; y1 += vy;

    //������� �浹 ���� ó��: ���� �浹�� ���⿡ ���ظ� �ְ� ���� ����
    for (int i = 0; i < num_enemy; i++) {
        if (ex0[i] < x1 && x0 < ex1[i] && ey0[i] < y1 && y0 < ey1[i]) {
            DamageEnemy(i);
            break;
        }
    }
}

//���� �ݻ� ��
//���� �»���ǥ, ������ǥ, �ӵ�, ������ �»���ǥ, ������ǥ, ��, ������ �»���ǥ, ������ǥ, ��
void MoveReflectShot(float& x0, float& y0, float& x1, float& y1, float vx, float vy, float gx0[], float gy0[], float gx1[], float gy1[], int num_ground, float ex0[], float ey0[], float ex1[], float ey1[], int num_enemy) {
    //������ �浹 ���� ó��: ���� ���� �������� �����Ų ��ǥ�� ���� ����, �浹�� ��� ���� ���� �ӵ� ���� �� ���� ����
    for (int i = 0; i < num_ground; i++) {
        if (gx0[i] < x1 && x0 < gx1[i] && gy0[i] < y1 + vy && y0 + vy < gy1[i]) {
            vy = -vy;
            break;
        }
    }

    //�� ����
    x0 += vx; y0 += vy;
    x1 += vx; y1 += vy;

    //������� �浹 ���� ó��: ���� �浹�� ���⿡ ���ظ� �ְ� ���� ����
    for (int i = 0; i < num_enemy; i++) {
        if (ex0[i] < x1 && x0 < ex1[i] && ey0[i] < y1 && y0 < ey1[i]) {
            DamageEnemy(i);
            break;
        }
    }
}

//���� �ݻ� ������
//�������� �� �κ� ����ü
typedef struct LASER_STRUCT {
    float X, Y;                 //�»���ǥ
    float W, H;                 //���� ����
    float VX, VY;               //�ӵ�
    float OldX, Old Y;          //������ǥ
    struct LASER_STRUCT* Prec;  //�ٷ� �պκ�
                                //������ ��� NULL
} LASER_TYPE;

//������ �̵� �Լ�
//������ �� �κ�, ������ �»���ǥ, ������ǥ, ��
void MoveReflectLaser(LASER_TYPE* laser, float gx0[], float gy0[], float gx1[], float gy1[], int num_ground) {
    //���� �κ�: ���θ� ���� �������� �����Ų ��ǥ�� ���� ����, �浹�� ��� ���� ���� �ӵ� ���� �� ���� ����
    if (!laser->Prec) {
        //���� �浹 ���� ó��: ���κκ��� ������������
        for (int i = 0; i < num_ground; i++) {
            if (gx0[i] < laser->X + laser->W && laser->X < gx1[i] && gy0[i] < laser->Y + laser->W + laser->VY && laser->Y + laser->VY < gy1[i]) {
                laser->VX = -laser->VX;
                break;
            }
        }

        //���� �κ� ����
        laser->X += laser->VX;
        laser->Y += laser->VY;
    //���� �̿�: �ٷ� �պκ��� ���� ��ǥ�� �̵�
    } else {
        laser->X = laser->Prec->OldX;
        laser->Y = laser->Prec->OldY;
    }
}

//���� �ٲٱ�
#define MAX_WEAPONS 5 //������ ��

//���� ��ȯ ó��
//��ư ����
void SelectWeapon(bool button) {
    static int weapon_id = 0;       //���� ���� �� ������ ��ȣ
    static bool prev_button = fase; //���� ��ư ����

    //���� �ٲٱ�
    if (!prev_button && button) {
        weapon_id = (weapon_id + 1) % MAX_WEAPONS;
    }

    //���� ������ ǥ��: �����̰� ��
    for (int i = 0; i < MAX_WEAPONS; i++)
        DrawIcon(i);
    FocusIcon(weapon_id);

    //���� ���� ����
    prev_button = button;
}

//���� ��ȯ�� ���� ������ ���
//���� ĳ������ ��ǥ, �ӵ�, ��ƽ�� ����, ��ȸ ��ư ����, �� ��ư ����
void TurnMarker(float& x, float& y, float speed, bool left, bool right, bool up, bool down, bool turn_button, bool shot_button) {
    static bool turning;            //��ȸ���̸� true
    static float mx, my;            //�� ��Ŀ�� ��ǥ
    static float rad = M_PI * 3 / 2;//��ȸ����
    static float d = 10;            //���� ĳ���Ϳ� �� ��Ŀ�� �Ÿ�

    //��ȸ�� ���: �� ��Ŀ�� ��ǥ ���
    if (!turning && turn_button) {
        turning = true;
        mx = x - d * cos(rad);
        my = y - d * sin(rad);
    }

    //��ȸ ����
    if (!turn_button) turning = false;

    //���� ĳ������ �̵�
    if (up   ) y -= speed;
    if (down ) y += speed;
    if (left ) x -= speed;
    if (right) x += speed;

    //��ȸ���̸� ��ȸ���� ���
    if (turning) rad = atan2(y - my, x - mx);

    //�� �߻�: ���� ĳ���Ͱ� ���� �������� �� �߻�, rad�� �ݴ� ����
    if (shot_button) DirectedShot(x, y, rad + M_PI);
}
