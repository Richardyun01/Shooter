#ifndef _BULLET_H_
#define _BULLET_H_

#include <math.h>

// 각종 탄환 유형을 위한 데이터 구조 및 함수 정의

class Bullet {
protected:
    float x, y;     // 좌표
    float vx, vy;   // 속도 벡터

public:
    virtual void Move() = 0;     // 이동 함수
    virtual void Draw() = 0;     // 렌더링 함수

    // 탄환 정보
    struct BulletInfo {
      float x, y;      // 좌표
      float vx, vy;    // 속도 벡터
      int type;        // 탄환 유형
    };

    // 조준탄
    void InitAimingBullet(float mx, float my, float ex, float ey, float speed, float& x, float& y, float vx, float vy);
    void MoveAimingBullet(float& x, float& y, float vx, float vy);
    void InitAimingBulletDDA(int mx, int my, int ex, int ey, int& x, int& y, int& vx, int& vy, int& dx, int& dy, int& diff);
    void MoveAimingBulletDDA(int& x, int& y, int vx, int vy, int dx, int dy, int diff, int speed);
    void InitAimingBulletFP(int mx, int my, int ex, int ey, int& x, int& y, int& vx, int& vy, int speed);
    void MoveAimingBulletFP(int& x, int& y, int vx, int vy);

    // 방향탄
    void InitDirectedBullet(float ex, float ey, float& x, float& y, float& vx, float& vy, float speed, float theta);
    void MoveDirectedBullet(float& x, float& y, float vx, float vy);
    void InitDirectedBullet16_3(float& vx, float& vy, float theta);
    void InitDirectedBulletDDA(int theta, int ex, int ey, float& x, float& y, float& vx, float& vy, int& dx, int& dy, int& diff);
    void InitDirectedBulletDDA2(int theta, int ex, int ey, float& x, float& y, float& vx, float& vy, int& dx, int& dy, int& diff);

    // n-way탄
    void RotateVelocity(float theta, float vx0, float vy0, float& vx, float& vy);
    void InitNWayBullets(float vx0, float vy0, float theta, int n, float vx[], float vy[]);

    // 탄막
    void InitCircleBullets(int n, float speed, bool odd, float vx[], float vy[]);

    // 유도탄
    void MoveSimpleHomingBullet(float& x, float& y, float mx, float my, float speed);
    void MoveHomingBullet(float& x, float& y, float& vx, float& vy, float mx, float my, float speed, float theta);

    // 유도 레이저
    //레이저의 각 부분을 나타내는 구조체
    typedef struct LASER_STRUCT {
        float X, Y;                 //좌표
        float VX, VY;               //속도
        float OldX, OldY;           //이전 좌표
        float Angle;                //회전각도
        float OldAngle;             //이전 회전각도
        struct LASER_STRUCT* Prec;  //바로 앞부분을 가리키는 포인터
                                    //선두부분일 경우 NULL
    } LASER_TYPE;

    void MoveHoming(LASER_TYPE* laser);
    LASER_TYPE* NewLaserType();
    void MoveHomingLaser(LASER_TYPE* laser);
    void ShootHomingLaser(float x, float y, int length);

    //미사일
    //미사일의 각 부분을 나타내는 구조체
    typedef struct MISSILE_STRUCT {
        float X, Y;                 //좌표
        float VX, VY;               //속도
        float OldX, OldY;           //이전 좌표
        float Angle;                //회전각도
        float OldAngle;             //이전 회전각도
        struct MISSILE_STRUCT* Prec;//바로 앞부분을 가리키는 포인터
                                    //선두부분일 경우에는 NULL
        bool IsMissile;             //미사일인지 아닌지: true면 미사일 false면 연기
    } MISSILE_TYPE;

    MISSILE_TYPE* NewMissileType();
    void ShootMissile(float x, float y, int length);

    // 가속탄
    void MoveAcceleratedBullet(float speed, float accel, float& vx, float& vy);

    // 낙하탄
    void MoveDroppingBullet(float& x, float& y, float accel, float& vx, float& vy);

    // 회전탄
    void MoveLoopingBullet(float& x, float& y, float& vx, float& vy, float cx, float cy, float r, float theta, float omega);
    void MoveLoopingBullet2(float& x, float& y, float& vx, float& vy, float cx, float cy, float& r, float vr, float theta, float omega);
};

#endif // _BULLET_H_
