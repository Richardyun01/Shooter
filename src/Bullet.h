#ifndef _BULLET_H_
#define _BULLET_H_

#include <math.h>

// ���� źȯ ������ ���� ������ ���� �� �Լ� ����

class Bullet {
protected:
    float x, y;     // ��ǥ
    float vx, vy;   // �ӵ� ����

public:
    virtual void Move() = 0;     // �̵� �Լ�
    virtual void Draw() = 0;     // ������ �Լ�

    // źȯ ����
    struct BulletInfo {
      float x, y;      // ��ǥ
      float vx, vy;    // �ӵ� ����
      int type;        // źȯ ����
    };

    // ����ź
    void InitAimingBullet(float mx, float my, float ex, float ey, float speed, float& x, float& y, float vx, float vy);
    void MoveAimingBullet(float& x, float& y, float vx, float vy);
    void InitAimingBulletDDA(int mx, int my, int ex, int ey, int& x, int& y, int& vx, int& vy, int& dx, int& dy, int& diff);
    void MoveAimingBulletDDA(int& x, int& y, int vx, int vy, int dx, int dy, int diff, int speed);
    void InitAimingBulletFP(int mx, int my, int ex, int ey, int& x, int& y, int& vx, int& vy, int speed);
    void MoveAimingBulletFP(int& x, int& y, int vx, int vy);

    // ����ź
    void InitDirectedBullet(float ex, float ey, float& x, float& y, float& vx, float& vy, float speed, float theta);
    void MoveDirectedBullet(float& x, float& y, float vx, float vy);
    void InitDirectedBullet16_3(float& vx, float& vy, float theta);
    void InitDirectedBulletDDA(int theta, int ex, int ey, float& x, float& y, float& vx, float& vy, int& dx, int& dy, int& diff);
    void InitDirectedBulletDDA2(int theta, int ex, int ey, float& x, float& y, float& vx, float& vy, int& dx, int& dy, int& diff);

    // n-wayź
    void RotateVelocity(float theta, float vx0, float vy0, float& vx, float& vy);
    void InitNWayBullets(float vx0, float vy0, float theta, int n, float vx[], float vy[]);

    // ź��
    void InitCircleBullets(int n, float speed, bool odd, float vx[], float vy[]);

    // ����ź
    void MoveSimpleHomingBullet(float& x, float& y, float mx, float my, float speed);
    void MoveHomingBullet(float& x, float& y, float& vx, float& vy, float mx, float my, float speed, float theta);

    // ���� ������
    //�������� �� �κ��� ��Ÿ���� ����ü
    typedef struct LASER_STRUCT {
        float X, Y;                 //��ǥ
        float VX, VY;               //�ӵ�
        float OldX, OldY;           //���� ��ǥ
        float Angle;                //ȸ������
        float OldAngle;             //���� ȸ������
        struct LASER_STRUCT* Prec;  //�ٷ� �պκ��� ����Ű�� ������
                                    //���κκ��� ��� NULL
    } LASER_TYPE;

    void MoveHoming(LASER_TYPE* laser);
    LASER_TYPE* NewLaserType();
    void MoveHomingLaser(LASER_TYPE* laser);
    void ShootHomingLaser(float x, float y, int length);

    //�̻���
    //�̻����� �� �κ��� ��Ÿ���� ����ü
    typedef struct MISSILE_STRUCT {
        float X, Y;                 //��ǥ
        float VX, VY;               //�ӵ�
        float OldX, OldY;           //���� ��ǥ
        float Angle;                //ȸ������
        float OldAngle;             //���� ȸ������
        struct MISSILE_STRUCT* Prec;//�ٷ� �պκ��� ����Ű�� ������
                                    //���κκ��� ��쿡�� NULL
        bool IsMissile;             //�̻������� �ƴ���: true�� �̻��� false�� ����
    } MISSILE_TYPE;

    MISSILE_TYPE* NewMissileType();
    void ShootMissile(float x, float y, int length);

    // ����ź
    void MoveAcceleratedBullet(float speed, float accel, float& vx, float& vy);

    // ����ź
    void MoveDroppingBullet(float& x, float& y, float accel, float& vx, float& vy);

    // ȸ��ź
    void MoveLoopingBullet(float& x, float& y, float& vx, float& vy, float cx, float cy, float r, float theta, float omega);
    void MoveLoopingBullet2(float& x, float& y, float& vx, float& vy, float cx, float cy, float& r, float vr, float theta, float omega);
};

#endif // _BULLET_H_
