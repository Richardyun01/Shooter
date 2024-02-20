#include <stdio.h>
#include <math.h>

#include "bullet.h"

//����ź�� �ʱ�ȭ
//���� ĳ������ ��ġ, ������ ��ǥ, źȯ�� �ӵ�, źȯ�� ��ǥ, źȯ�� �ӵ� ����
void Bullet::InitAimingBullet(float mx, float my, float ex, float ey, float speed, float& x, float& y, float vx, float vy) {
    x = ex; y = ey;                                         //źȯ�� ��ǥ ����
    float d = sqrt (pow((mx - ex), 2) + pow((my - ey), 2)); //��ǥ������ �Ÿ� d

    //�ӵ��� ������ ���� �ǵ��� �ӵ� ���� ���ϱ�
    //��ǥ������ �Ÿ� d�� 0�� ���� �Ʒ������� �߻�
    if (d) {
        vx = (mx - ex) / d * speed;
        vy = (my - ey) / d * speed;
    } else {
        vx = 0;
        vy = speed;
    }
}

//����ź�� �̵�
//źȯ�� ��ǥ, �ӵ�
void Bullet::MoveAimingBullet(float& x, float& y, float vx, float vy) {
    //źȯ�� ��ǥ�� �ӵ��� ����
    x += vx;
    y += vy;
}

//DDA: ������ �̺� �ؼ���, ���� �׸��� ����
//DDA�� ����� ����ź�� �ʱ�ȭ
//���� ĳ������ ��ġ, ������ ��ǥ, źȯ�� ��ǥ, źȯ�� �̵� ����, X����� Y������ ����, ����
void Bullet::InitAimingBulletDDA(int mx, int my, int ex, int ey, int& x, int& y, int& vx, int& vy, int& dx, int& dy, int& diff) {
    //źȯ�� ��ǥ ����
    x = ex; y = ey;

    //źȯ�� �̵� ���� ���ϱ�
    vx = mx > ex ? 1 : -1;
    vy = my > ey ? 1 : -1;

    //ź����ǥ�� ���� x����� y������ ����ġ ���ϱ�
    dx = mx > ex ? mx - ex : ex - mx;
    dy = my > ey ? my - ey : ey - my;

    //���� diff: dx >= dy�϶��� dx/2, �� �ܿ��� dy/2�� ����
    diff = dx >= dy ? dx / 2 : dy / 2;
}

//DDA�� ����� ����ź�� �̵�
//źȯ�� ��ǥ, �̵� ����, X����� Y������ ����, ����, �ӵ�
void Bullet::MoveAimingBulletDDA(int& x, int& y, int vx, int vy, int dx, int dy, int diff, int speed) {
    //Ư�� ���������� �̵��Ÿ��� �涧
    if (dx >= dy) {
        for (int i = 0; i < speed; i++) {
            //X�������� �Ź� �̵�
            x += vx;

            //Y�������� ���� ���� �� �̵�
            diff += dy;
            if (diff >= dx) {
                diff -= dx;
                y += vy;
            }
        }
    } else {
        for (int i = 0; i < speed; i++) {
            //Y�������� �Ź� �̵�
            y += vy;

            //X�������� ���� ���� �� �̵�
            diff += dx;
            if (diff >= dy) {
                diff -= dy;
                x += vx;
            }
        }
    }
}

//�����Ҽ�����
//�����Ҽ������� �̿��� ����ź�� �ʱ�ȭ
//���� ĳ������ ��ġ, ������ ��ǥ, źȯ�� ��ǥ, źȯ�� �̵� ����, X����� Y������ ����, ����
void Bullet::InitAimingBulletFP(int mx, int my, int ex, int ey, int& x, int& y, int& vx, int& vy, int speed) {
    //źȯ�� ��ǥ ����
    x = ex; y = ey;

    //��ǥ�� ���� X����� Y������ ������ ����
    int dx = mx >= ex ? mx - ex : ex - mx;
    int dy = my >= ey ? my - ey : ey - my;

    //X����� Y������ ���� �� ���� �� ���� �Ÿ� d�� ��
    int d = dx >= dy ? dx : dy;

    //�ӵ� ����
    vx = (mx - ex) * speed / d;
    vy = (my - ey) * speed / d;
}

//�����Ҽ������� �̿��� ����ź�� �̵�
void Bullet::MoveAimingBulletFP(int& x, int& y, int vx, int vy) {
    //źȯ�� ��ǥ�� �ӵ� ���� �߰�
    x += vx;
    y += vy;

    //źȯ �׸���
    //ȭ�鿡 źȯ�� �׸� �� ��ǥ�� x>>8, y>>8�� ����
    //źȯ�� �׸��� ��ü�� ó���� Draw�Լ����� ����
    Draw(x>>8, y>>8);
}

//����ź�� �ʱ�ȭ
//������ ��ǥ, źȯ�� ��ǥ, źȯ�� �ӵ� ����, źȯ�� �ӵ�, �߻� ����
void Bullet::InitDirectedBullet(float ex, float ey, float& x, float& y, float& vx, float& vy, float speed, float theta) {
    x = ex; y = ey;

    vx = cos(M_PI / 180 * theta) * speed;
    vy = cos(M_PI / 180 * theta) * speed;
}

//����ź�� �̵�
//źȯ�� ��ǥ, �ӵ� ����
void Bullet::MoveDirectedBullet(float& x, float& y, float vx, float vy) {
    x += vx;
    y += vy;
}

//3�� �ӵ��� 16�������� ���ϴ� ����ź�� �߻� ó��
//������ �ӵ�, �߻� �ӵ�
void Bullet::InitDirectedBullet16_3(float& vx, float& vy, float theta) {
    //�ӵ� 3�� ���� �ӵ� ���̺�
    static int v3[][2] = {
        { 3, 0}, { 3, 1}, { 2, 2}, { 1, 3},
        { 0, 3}, {-1, 3}, {-2, 2}, {-3, 1},
        {-3, 0}, {-3,-1}, {-2,-2}, {-1,-3},
        { 0,-3}, { 1,-3}, { 2,-2}, { 3,-1},
    };

    //���� theta�� 16�������� ��ȯ
    int dir = (int)(theta * 16 / 360);

    //źȯ�� �ӵ� ���͸� ���̺�κ��� ���ϱ�
    vx = v3[dir][0];
    vy = v3[dir][1];
}

//DDA�� ����� ����ź�� �ʱ�ȭ
//�߻簢��, ������ ��ǥ, źȯ�� ��ǥ, źȯ�� �̵� ����, X����� Y������ ����, ����
void Bullet::InitDirectedBulletDDA(int theta, int ex, int ey, float& x, float& y, float& vx, float& vy, int& dx, int& dy, int& diff) {
    //������ ���� ĳ���͸� �ָ� ��ġ�ϱ�, M_PI = ������
    int mx = cos(M_PI / 180 * theta) * 1000;
    int my = sin(M_PI / 180 * theta) * 1000;

    //���Ĵ� DDA�� ����� ����ź ó���� ����

    //źȯ�� ��ǥ ����
    x = ex; y = ey;

    //źȯ�� �̵� ���� ���ϱ�
    vx = mx > ex ? 1 : -1;
    vy = my > ey ? 1 : -1;

    //ź����ǥ�� ���� x����� y������ ����ġ ���ϱ�
    dx = mx > ex ? mx - ex : ex - mx;
    dy = my > ey ? my - ey : ey - my;

    //���� diff: dx >= dy�϶��� dx/2, �� �ܿ��� dy/2�� ����
    diff = dx >= dy ? dx / 2 : dy / 2;
}

//�������� ������ ó��
//��ġ ���̺��� �̸� �ۼ�
//���� ĳ������ ��ġ�� �����ϴ� ���̺�
int mpos[360][2];

//���̺� �ۼ�
//M_PI = ������
void MakeTable() {
    for (int i = 0; i < 360; i++) {
        mpos[i][0] = cos(M_PI / 180 * i) * 1000;
        mpos[i][1] = sin(M_PI / 180 * i) * 1000;
    }
}

//��ġ ���̺��� �̿��Ͽ� ����ź�� �ʱ�ȭ
//�߻簢��, ������ ��ǥ, źȯ�� ��ǥ, źȯ�� �̵� ����, X����� Y������ ����, ����
void Bullet::InitDirectedBulletDDA2(int theta, int ex, int ey, float& x, float& y, float& vx, float& vy, int& dx, int& dy, int& diff) {
    //������ ���� ĳ������ ��ġ�� ���̺��� �о��
    int dir = (theta % 360 + 360) % 360;
    int mx = mpos[dir][0];
    int my = mpos[dir][1];

    //���Ĵ� DDA�� ����� ���� ĳ���� �������� źȯ�� ������ ó���� ����

    //źȯ�� ��ǥ ����
    x = ex; y = ey;

    //źȯ�� �̵� ���� ���ϱ�
    vx = mx > ex ? 1 : -1;
    vy = my > ey ? 1 : -1;

    //ź����ǥ�� ���� x����� y������ ����ġ ���ϱ�
    dx = mx > ex ? mx - ex : ex - mx;
    dy = my > ey ? my - ey : ey - my;

    //���� diff: dx >= dy�϶��� dx/2, �� �ܿ��� dy/2�� ����
    diff = dx >= dy ? dx / 2 : dy / 2;
}


//n-wayź�� �߻�

//�ӵ� ���͸� ȸ����Ű��
//ȸ������, ���� �ӵ�, ȸ�� ���� �ӵ�
void Bullet::RotateVelocity(float theta, float vx0, float vy0, float& vx, float& vy) {
    //theta�� rad�� ��ȯ�Ͽ� cos�� sin�� ���ϱ�
    float rad = M_PI / 180 * theta;
    float c = cos(rad), s = sin(rad);

    //�ӵ� ���͸� ȸ����Ų ȸ�� �� �ӵ� ���ϱ�
    vx = vx0 * c - vy0 * c;
    vy = vx0 * c + vy0 * c;
}

//n-wayź�� �ʱ�ȭ
//�߽��� �Ǵ� źȯ�� �ӵ�, źȯ�� źȯ ������ ����, źȯ�� ��, n-wayź�� �ӵ�
void Bullet::InitNWayBullets(float vx0, float vy0, float theta, int n, float vx[], float vy[]) {
    //źȯ�� źȯ ������ ������ rad�� ��ȯ
    float rad_step = M_PI / 180 * theta;

    //�����ڸ��� źȯ�� �߽ɺκ��� źȯ�� ������ ���
    float rad = n % 2 ? -n/2 * rad_step : (-n/2 + 0.5) * rad_step;

    //n���� źȯ�� �ӵ��� ����ϱ�
    for (int i = 0; i < n; i++, rad += rad_step) {
        //(vx[i], vy[i]) ���ϱ�
        //�ӵ� ���͸� rad��ŭ ȸ����Ű��
        float c = cos(rad), s = sin(rad);
        vx[i] = vx0 * c - vy0 * c;
        vy[i] = vx0 * c + vy0 * c;
    }
}


//ź��

//����ź�� �ʱ�ȭ
//źȯ ��, źȯ �ӵ�, Ȧ�� ������ �� ��, ����ź�� �ӵ� ����
void Bullet::InitCircleBullets(int n, float speed, bool odd, float vx[], float vy[]) {
    //źȯ�� źȯ ������ ������ ���
    float rad_step = M_PI * 2 / n;

    //������ źȯ ���� ���
    //Ȧ�� ������ ��� rad_step/2��ŭ Ʋ��
    float rad = odd ? rad_step / 2 : 0;

    //n���� źȯ �ӵ� ���� ���ϱ�
    //speed�� �ӵ��� ���� rad �������� ���ư��� źȯ�� �ӵ� ���� ���ϱ�
    //����ź�� ������ ó�� ����
    for (int i = 0; i < n; i++, rad += rad_step) {
        vx[i] = cos(rad) * speed;
        vy[i] = sin(rad) * speed;
    }
}

//����ź�� �̵�
//źȯ�� ��ǥ, ���� ĳ������ ��ǥ, źȯ�� �ӵ�
void Bullet::MoveSimpleHomingBullet(float& x, float& y, float mx, float my, float speed) {
    //��ǥ������ �Ÿ� d ���
    float d = sqrt(pow(mx - x, 2) + pow(my - y, 2));

    //źȯ�� �ӵ� ���� ���ϱ�
    //�ӵ��� ������ ���� �ǵ��� ��
    //��ǥ������ �Ÿ� d�� 0�� ���� �ӵ� ���͸� ȭ�� �Ʒ������� ��
    float vx, vy;
    if (d) {
        vx = (mx - x) / d * speed;
        vy = (my - y) / d * speed;
    } else {
        vx = 0;
        vy = speed;
    }

    //źȯ�� ��ǥ�� �����Ͽ� źȯ�� �̵�
    x += vx;
    y += vy;
}

//��ȸ������ ���ѵ� ����ź�� �̵�
//źȯ�� ��ǥ, �ӵ� ����, ���� ĳ������ ��ǥ, źȯ�� �ӵ�, ��ȸ������ ����ġ
void Bullet::MoveHomingBullet(float& x, float& y, float& vx, float& vy, float mx, float my, float speed, float theta) {
    //źȯ�� ���� �ӵ� ���͸� ����
    float vx0 = vx, vy0 = vy;

    //���� ĳ���� ���������� �ӵ� ���� ���ϱ�
    float vx1, vy1;
    float d = sqrt(pow(mx - x, 2) + pow(my - y, 2));
    if (d) {
        vx1 = (mx - x) / d * speed;
        vy1 = (my - y) / d * speed;
    } else {
        vx1 = 0;
        vy1 = speed;
    }

    //�ð� �������� ��ȸ�� ���� ���� ������ �ش��ϴ� �ӵ� ���͸� ���ϱ�
    float rad = M_PI / 180 * theta;
    float vx2 = cos(rad) * vx0 - sin(rad) * vy0;
    float vy2 = sin(rad) * vx0 - cos(rad) * vy0;

    //���� ĳ���� �������� ��ȸ���� ���Ѱ�����ŭ�� ��ȸ���� ���ϱ�
    if (vx0*vx1 + vy0*vy1 >= vx0*vx2 + vy0*vy2) {
        //���� ĳ���Ͱ� ��ȸ ������ ���� ���� ���� ��� �� �������� ��ȸ
        vx = vx1;
        vy = vy1;
    } else {
        //���� ĳ���Ͱ� ��ȸ ������ ���� �ۿ� ���� ��:
        //��ð� �������� ��ȸ�� �� ���� ������ �ش��ϴ� �ӵ� ���� ���ϱ�
        float vx3 = cos(rad) * vx0 - sin(rad) * vy0;
        float vy3 = -sin(rad) * vx0 + cos(rad) * vy0;

        //źȯ���� ���� ĳ���ͱ����� ��� ��ġ ���͸� ���ϱ�
        float px = mx - x, py = my - y;

        //�ð� �������� ��ȸ���� �ݽð� �������� ��ȸ���� ���ϱ�
        if (px*vx2 + py*vy2 >= px*vx3 + py*vy3) {
            //�ð� ������ ���
            vx = vx2;
            vy = vy2;
        } else {
            //�ݽð� ������ ���
            vx = vx3;
            vy = vy3;
        }
    }

    //źȯ�� ��ǥ�� �����Ͽ� źȯ�� �̵�
    x += vx;
    y += vy;
}

//���� ������

//�������� �����̴� �Լ�
//�������� ���� Ȥ�� �� �̿��� ��ü�� ���� �κ�
void Bullet::MoveHomingLaser(LASER_TYPE* laser) {
    //���κκ��� ��� ����ź�� �����Ӱ� ����
    //�����ϴ� ��ü���� ó���� MoveHoming�Լ����� ����
    if (laser->Prec == NULL) {
        MoveHoming(laser);
    }

    //���� �̿��� �κ��� ��� �ٷ� �պκ��� �Ѿư�
    //���� ��ǥ�� ���� ȸ�������� ����
    else {
        laser->X = laser->Prec->OldX;
        laser->Y = laser->Prec->OldY;
        laser->Angle = laser->Prec->OldAngle;
    }
}

//�������� �߻�
//�߻� ������ ��ǥ, �������� ����
void Bullet::ShootHomingLaser(float x, float y, int length) {
    LASER_TYPE* laser;          //�������� ��Ÿ���� ����ü�� ������
    LASER_TYPE* prec = NULL;    //�ٷ� �պκ��� ����Ű�� ������

    //�������� �� �κ� ����
    //�������� ����ü�� �����Ͽ� ��ǥ�� �ʱ�ȭ
    //����ü�� �����ϴ� ��ü���� ó���� NewLaserType �Լ����� ����
    for (int i = 0; i < length; i++, prec = laser) {
        laser = NewLaserType();
        laser->X = laser->OldX = x;
        laser->Y = laser->OldY = y;
        laser->Angle = laser->OldAngle = 0;

        //��ü�� ���� �κ��� �ٷ� �պκ��� ����
        //���κκ��� �������� ���� NULL�� ��ȯ��
        laser->Prec = prec;
    }
}

//�̻���

//�̻��� �߻�
//�߻� ������ ��ǥ, ������ ����
void Bullet::ShootMissile(float x, float y, int length) {
    MISSILE_TYPE* missile;      //�̻����� ����ü�� ����Ű�� ������
    MISSILE_TYPE* prec = NULL;  //�ٷ� �պκ��� ����Ű�� ������

    //�̻����� �� �κ� ����
    //�̻����� ����ü�� �����Ͽ� ��ǥ�� �ʱ�ȭ
    //����ü�� �����ϴ� ��ü���� ó���� NewMissileType�Լ����� ����
    for (int i = 0; i < length; i++, prec = missile) {
        missile = NewMissileType();
        missile->X = missile->OldX = x;
        missile->Y = missile->OldY = y;
        missile->Angle = missile->OldAngle = 0;

        //���� �κ��� �ٷ� �պκ� ����
        //���� �κ� ������ NULL ��ȯ
        missile->Prec = prec;

        //���� �κ��� �̻���, �ٸ� �κ��� ����
        missile->IsMissile = (i == 0);
    }
}

//����ź
//�ӵ��� �ӵ� ������ ����
//źȯ�� �ӵ�, ���ӵ�, �ӵ� ������ X���а� Y����
void Bullet::MoveAcceleratedBullet(float speed, float accel, float& vx, float& vy) {
    //�ӵ� ������ X, Y������ ���� �ӵ��� ������
    if (speed != 0) {
        vx /= speed;
        vy /= speed;
    }

    //�ӵ� ����
    speed += accel;

    //�ӵ� ������ X, Y ���п� ���ο� �ӵ� ���ϱ�
    vx *= speed;
    vy *= speed;
}

//����ź
//źȯ�� ��ǥ, ���ӵ�, �ӵ������� ����
void Bullet::MoveDroppingBullet(float& x, float& y, float accel, float& vx, float& vy) {
    //�ӵ� ����: Y���и� ��ȭ
    vy += accel;

    //��ǥ ����
    x += vx;
    y += vy;
}

//ȸ��ź
//źȯ�� ��ǥ, �ӵ� ����, ȸ�� �߽��� ��ǥ, �ݰ�, ����, �ѹ� �̵� �� ��ȭ ����
void MoveLoopingBullet(float& x, float& y, float& vx, float& vy, float cx, float cy, float r, float theta, float omega) {
    //���� ��ȭ
    theta += omega;

    //��ġ ���
    x = cx + r * cos(theta);
    y = cy + r * sin(theta);

    //źȯ�� �ӵ�
    vx -= r * omega * sin(theta);
    vy  = r * omega * cos(theta);
}

//ȸ���ݰ��� ��ȭ�ϴ� ȸ��ź�� �̵�
//źȯ�� ��ǥ, �ӵ� ����, ȸ�� �߽��� ��ǥ, �ݰ�, �ݰ��� ��ȭ, ����, �ѹ� �̵� �� ��ȭ ����
void MoveLoopingBullet2(float& x, float& y, float& vx, float& vy, float cx, float cy, float& r, float vr, float theta, float omega) {
    //���� ��ȭ
    theta += omega;

    //�ݰ� ��ȭ
    r += vr;

    //��ġ ���
    x = cx + r * cos(theta);
    y = cy + r * sin(theta);

    //źȯ�� �ӵ�
    vx -= r * omega * sin(theta);
    vy  = r * omega * cos(theta);
}
