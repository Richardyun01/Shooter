#include <stdio.h>
#include <math.h>

#include "bullet.h"

//조준탄의 초기화
//메인 캐릭터의 위치, 적기의 좌표, 탄환의 속도, 탄환의 좌표, 탄환의 속도 벡터
void Bullet::InitAimingBullet(float mx, float my, float ex, float ey, float speed, float& x, float& y, float vx, float vy) {
    x = ex; y = ey;                                         //탄환의 좌표 설정
    float d = sqrt (pow((mx - ex), 2) + pow((my - ey), 2)); //목표까지의 거리 d

    //속도가 일정한 값이 되도록 속도 벡터 구하기
    //목표까지의 거리 d가 0일 때는 아래쪽으로 발사
    if (d) {
        vx = (mx - ex) / d * speed;
        vy = (my - ey) / d * speed;
    } else {
        vx = 0;
        vy = speed;
    }
}

//조준탄의 이동
//탄환의 좌표, 속도
void Bullet::MoveAimingBullet(float& x, float& y, float vx, float vy) {
    //탄환의 좌표에 속도를 더함
    x += vx;
    y += vy;
}

//DDA: 디지털 미분 해석기, 직선 그리기 위함
//DDA를 사용한 조준탄의 초기화
//메인 캐릭터의 위치, 적기의 좌표, 탄환의 좌표, 탄환의 이동 방향, X방향과 Y방향의 차이, 오차
void Bullet::InitAimingBulletDDA(int mx, int my, int ex, int ey, int& x, int& y, int& vx, int& vy, int& dx, int& dy, int& diff) {
    //탄환의 좌표 설정
    x = ex; y = ey;

    //탄환의 이동 방향 구하기
    vx = mx > ex ? 1 : -1;
    vy = my > ey ? 1 : -1;

    //탄ㅎ목표에 대한 x방향과 y방향의 절대치 구하기
    dx = mx > ex ? mx - ex : ex - mx;
    dy = my > ey ? my - ey : ey - my;

    //오차 diff: dx >= dy일때는 dx/2, 그 외에는 dy/2로 설정
    diff = dx >= dy ? dx / 2 : dy / 2;
}

//DDA를 사용한 조준탄의 이동
//탄환의 좌표, 이동 방향, X방향과 Y방향의 차이, 오차, 속도
void Bullet::MoveAimingBulletDDA(int& x, int& y, int vx, int vy, int dx, int dy, int diff, int speed) {
    //특정 방향으로의 이동거리가 길때
    if (dx >= dy) {
        for (int i = 0; i < speed; i++) {
            //X방향으로 매번 이동
            x += vx;

            //Y방햐으로 오차 누적 시 이동
            diff += dy;
            if (diff >= dx) {
                diff -= dx;
                y += vy;
            }
        }
    } else {
        for (int i = 0; i < speed; i++) {
            //Y방향으로 매번 이동
            y += vy;

            //X방향으로 오차 누적 시 이동
            diff += dx;
            if (diff >= dy) {
                diff -= dy;
                x += vx;
            }
        }
    }
}

//고정소수점수
//고정소수점수를 이용한 조준탄의 초기화
//메인 캐릭터의 위치, 적기의 좌표, 탄환의 좌표, 탄환의 이동 방향, X방향과 Y방향의 차이, 오차
void Bullet::InitAimingBulletFP(int mx, int my, int ex, int ey, int& x, int& y, int& vx, int& vy, int speed) {
    //탄환의 좌표 설정
    x = ex; y = ey;

    //목표에 대한 X방향과 Y방향의 차이의 절댓값
    int dx = mx >= ex ? mx - ex : ex - mx;
    int dy = my >= ey ? my - ey : ey - my;

    //X방향과 Y방향의 차이 중 보다 긴 쪽을 거리 d로 둠
    int d = dx >= dy ? dx : dy;

    //속도 벡터
    vx = (mx - ex) * speed / d;
    vy = (my - ey) * speed / d;
}

//고정소수점수를 이용한 조준탄의 이동
void Bullet::MoveAimingBulletFP(int& x, int& y, int vx, int vy) {
    //탄환의 좌표에 속도 벡터 추가
    x += vx;
    y += vy;

    //탄환 그리기
    //화면에 탄환을 그릴 때 좌표를 x>>8, y>>8로 지정
    //탄환을 그리는 구체적 처리는 Draw함수에서 수행
    Draw(x>>8, y>>8);
}

//방향탄의 초기화
//적기의 좌표, 탄환의 좌표, 탄환의 속도 벡터, 탄환의 속도, 발사 각도
void Bullet::InitDirectedBullet(float ex, float ey, float& x, float& y, float& vx, float& vy, float speed, float theta) {
    x = ex; y = ey;

    vx = cos(M_PI / 180 * theta) * speed;
    vy = cos(M_PI / 180 * theta) * speed;
}

//방향탄의 이동
//탄환의 좌표, 속도 벡터
void Bullet::MoveDirectedBullet(float& x, float& y, float vx, float vy) {
    x += vx;
    y += vy;
}

//3의 속도로 16방향으로 향하는 방향탄의 발사 처리
//적기의 속도, 발사 속도
void Bullet::InitDirectedBullet16_3(float& vx, float& vy, float theta) {
    //속도 3에 대한 속도 테이블
    static int v3[][2] = {
        { 3, 0}, { 3, 1}, { 2, 2}, { 1, 3},
        { 0, 3}, {-1, 3}, {-2, 2}, {-3, 1},
        {-3, 0}, {-3,-1}, {-2,-2}, {-1,-3},
        { 0,-3}, { 1,-3}, { 2,-2}, { 3,-1},
    };

    //각도 theta를 16방향으로 변환
    int dir = (int)(theta * 16 / 360);

    //탄환의 속도 벡터를 테이블로부터 구하기
    vx = v3[dir][0];
    vy = v3[dir][1];
}

//DDA를 사용한 방향탄의 초기화
//발사각도, 적기의 좌표, 탄환의 좌표, 탄환의 이동 방향, X방향과 Y방향의 차이, 오차
void Bullet::InitDirectedBulletDDA(int theta, int ex, int ey, float& x, float& y, float& vx, float& vy, int& dx, int& dy, int& diff) {
    //가상의 메인 캐릭터를 멀리 배치하기, M_PI = 원주율
    int mx = cos(M_PI / 180 * theta) * 1000;
    int my = sin(M_PI / 180 * theta) * 1000;

    //이후는 DDA를 사용한 조준탄 처리와 동일

    //탄환의 좌표 설정
    x = ex; y = ey;

    //탄환의 이동 방향 구하기
    vx = mx > ex ? 1 : -1;
    vy = my > ey ? 1 : -1;

    //탄ㅎ목표에 대한 x방향과 y방향의 절대치 구하기
    dx = mx > ex ? mx - ex : ex - mx;
    dy = my > ey ? my - ey : ey - my;

    //오차 diff: dx >= dy일때는 dx/2, 그 외에는 dy/2로 설정
    diff = dx >= dy ? dx / 2 : dy / 2;
}

//정수형만 가지고 처리
//위치 테이블을 미리 작성
//메인 캐릭터의 위치를 저장하는 테이블
int mpos[360][2];

//테이블 작성
//M_PI = 원주율
void MakeTable() {
    for (int i = 0; i < 360; i++) {
        mpos[i][0] = cos(M_PI / 180 * i) * 1000;
        mpos[i][1] = sin(M_PI / 180 * i) * 1000;
    }
}

//위치 테이블을 이용하여 방향탄을 초기화
//발사각도, 적기의 좌표, 탄환의 좌표, 탄환의 이동 방향, X방향과 Y방향의 차이, 오차
void Bullet::InitDirectedBulletDDA2(int theta, int ex, int ey, float& x, float& y, float& vx, float& vy, int& dx, int& dy, int& diff) {
    //가상의 메인 캐릭터의 위치를 테이블에서 읽어옴
    int dir = (theta % 360 + 360) % 360;
    int mx = mpos[dir][0];
    int my = mpos[dir][1];

    //이후는 DDA를 사용한 메인 캐릭터 방향으로 탄환을 보내는 처리와 같음

    //탄환의 좌표 설정
    x = ex; y = ey;

    //탄환의 이동 방향 구하기
    vx = mx > ex ? 1 : -1;
    vy = my > ey ? 1 : -1;

    //탄ㅎ목표에 대한 x방향과 y방향의 절대치 구하기
    dx = mx > ex ? mx - ex : ex - mx;
    dy = my > ey ? my - ey : ey - my;

    //오차 diff: dx >= dy일때는 dx/2, 그 외에는 dy/2로 설정
    diff = dx >= dy ? dx / 2 : dy / 2;
}


//n-way탄의 발사

//속도 벡터를 회전시키기
//회전각도, 원래 속도, 회전 후의 속도
void Bullet::RotateVelocity(float theta, float vx0, float vy0, float& vx, float& vy) {
    //theta를 rad로 변환하여 cos와 sin값 구하기
    float rad = M_PI / 180 * theta;
    float c = cos(rad), s = sin(rad);

    //속도 벡터를 회전시킨 회전 후 속도 구하기
    vx = vx0 * c - vy0 * c;
    vy = vx0 * c + vy0 * c;
}

//n-way탄의 초기화
//중심이 되는 탄환의 속도, 탄환과 탄환 사이의 각도, 탄환의 수, n-way탄의 속도
void Bullet::InitNWayBullets(float vx0, float vy0, float theta, int n, float vx[], float vy[]) {
    //탄환과 탄환 사이의 각도를 rad로 변환
    float rad_step = M_PI / 180 * theta;

    //가장자리의 탄환과 중심부분의 탄환의 각도를 계산
    float rad = n % 2 ? -n/2 * rad_step : (-n/2 + 0.5) * rad_step;

    //n개의 탄환의 속도를 계산하기
    for (int i = 0; i < n; i++, rad += rad_step) {
        //(vx[i], vy[i]) 구하기
        //속도 벡터를 rad만큼 회전시키기
        float c = cos(rad), s = sin(rad);
        vx[i] = vx0 * c - vy0 * c;
        vy[i] = vx0 * c + vy0 * c;
    }
}


//탄막

//원형탄의 초기화
//탄환 수, 탄환 속도, 홀수 패턴일 때 참, 원형탄의 속도 벡터
void Bullet::InitCircleBullets(int n, float speed, bool odd, float vx[], float vy[]) {
    //탄환과 탄환 사이의 각도를 계산
    float rad_step = M_PI * 2 / n;

    //최초의 탄환 각도 계산
    //홀수 패턴일 경우 rad_step/2만큼 틀기
    float rad = odd ? rad_step / 2 : 0;

    //n개의 탄환 속도 벡터 구하기
    //speed의 속도로 각도 rad 방향으로 날아가는 탄환의 속도 벡터 구하기
    //원형탄을 날리는 처리 응용
    for (int i = 0; i < n; i++, rad += rad_step) {
        vx[i] = cos(rad) * speed;
        vy[i] = sin(rad) * speed;
    }
}

//유도탄의 이동
//탄환의 좌표, 메인 캐릭터의 좌표, 탄환의 속도
void Bullet::MoveSimpleHomingBullet(float& x, float& y, float mx, float my, float speed) {
    //목표까지의 거리 d 계산
    float d = sqrt(pow(mx - x, 2) + pow(my - y, 2));

    //탄환의 속도 벡터 구하기
    //속도가 일정한 값이 되도록 함
    //목표까지의 거리 d가 0일 때는 속도 벡터를 화면 아래쪽으로 함
    float vx, vy;
    if (d) {
        vx = (mx - x) / d * speed;
        vy = (my - y) / d * speed;
    } else {
        vx = 0;
        vy = speed;
    }

    //탄환의 좌표를 갱신하여 탄환을 이동
    x += vx;
    y += vy;
}

//선회각도가 제한된 유도탄의 이동
//탄환의 좌표, 속도 벡터, 메인 캐릭터의 좌표, 탄환의 속도, 선회각도의 상한치
void Bullet::MoveHomingBullet(float& x, float& y, float& vx, float& vy, float mx, float my, float speed, float theta) {
    //탄환의 원래 속도 벡터를 저장
    float vx0 = vx, vy0 = vy;

    //메인 캐릭터 방향으로의 속도 벡터 구하기
    float vx1, vy1;
    float d = sqrt(pow(mx - x, 2) + pow(my - y, 2));
    if (d) {
        vx1 = (mx - x) / d * speed;
        vy1 = (my - y) / d * speed;
    } else {
        vx1 = 0;
        vy1 = speed;
    }

    //시계 방향으로 선회할 때의 상한 각도에 해당하는 속도 벡터를 구하기
    float rad = M_PI / 180 * theta;
    float vx2 = cos(rad) * vx0 - sin(rad) * vy0;
    float vy2 = sin(rad) * vx0 - cos(rad) * vy0;

    //메인 캐릭터 방향으로 선회할지 제한각도반큼만 선회할지 정하기
    if (vx0*vx1 + vy0*vy1 >= vx0*vx2 + vy0*vy2) {
        //메인 캐릭터가 선회 가능한 범위 내에 있을 경우 그 방향으로 선회
        vx = vx1;
        vy = vy1;
    } else {
        //메인 캐릭터가 선회 가능한 범위 밖에 있을 경:
        //방시계 방향으로 선회할 때 상한 각도에 해당하는 속도 벡터 구하기
        float vx3 = cos(rad) * vx0 - sin(rad) * vy0;
        float vy3 = -sin(rad) * vx0 + cos(rad) * vy0;

        //탄환에서 메인 캐릭터까지의 상대 위치 벡터를 구하기
        float px = mx - x, py = my - y;

        //시계 방향으로 선회할지 반시계 방향으로 선회할지 정하기
        if (px*vx2 + py*vy2 >= px*vx3 + py*vy3) {
            //시계 방향일 경우
            vx = vx2;
            vy = vy2;
        } else {
            //반시계 방향일 경우
            vx = vx3;
            vy = vy3;
        }
    }

    //탄환의 좌표를 갱신하여 탄환을 이동
    x += vx;
    y += vy;
}

//유도 레이저

//레이저를 움직이는 함수
//레이저의 선두 혹은 그 이외의 몸체나 꼬리 부분
void Bullet::MoveHomingLaser(LASER_TYPE* laser) {
    //선두부분의 경우 유도탄의 움직임과 같음
    //유도하는 구체적인 처리는 MoveHoming함수에서 수행
    if (laser->Prec == NULL) {
        MoveHoming(laser);
    }

    //선두 이외의 부분일 경우 바로 앞부분을 쫓아감
    //이전 좌표와 이전 회전각도를 복사
    else {
        laser->X = laser->Prec->OldX;
        laser->Y = laser->Prec->OldY;
        laser->Angle = laser->Prec->OldAngle;
    }
}

//레이저의 발사
//발사 지점의 좌표, 레이저의 길이
void Bullet::ShootHomingLaser(float x, float y, int length) {
    LASER_TYPE* laser;          //레이저를 나타내는 구조체의 포인터
    LASER_TYPE* prec = NULL;    //바로 앞부분을 가리키는 포인터

    //레이저의 각 부분 생성
    //레이저의 구조체를 생성하여 좌표를 초기화
    //구조체를 생성하는 구체적인 처리는 NewLaserType 함수에서 수행
    for (int i = 0; i < length; i++, prec = laser) {
        laser = NewLaserType();
        laser->X = laser->OldX = x;
        laser->Y = laser->OldY = y;
        laser->Angle = laser->OldAngle = 0;

        //몸체나 꼬리 부분은 바로 앞부분을 참조
        //선두부분을 참조했을 때는 NULL이 반환됨
        laser->Prec = prec;
    }
}

//미사일

//미사일 발사
//발사 지점의 좌표, 연기의 길이
void Bullet::ShootMissile(float x, float y, int length) {
    MISSILE_TYPE* missile;      //미사일의 구조체를 가리키는 포인터
    MISSILE_TYPE* prec = NULL;  //바로 앞부분을 가리키는 포인터

    //미사일의 각 부분 생성
    //미사일의 구조체를 생성하여 좌표를 초기화
    //구조체를 생성하는 구체적인 처리는 NewMissileType함수에서 수행
    for (int i = 0; i < length; i++, prec = missile) {
        missile = NewMissileType();
        missile->X = missile->OldX = x;
        missile->Y = missile->OldY = y;
        missile->Angle = missile->OldAngle = 0;

        //연기 부분은 바로 앞부분 참조
        //선두 부분 참조시 NULL 반환
        missile->Prec = prec;

        //선두 부분은 미사일, 다른 부분은 연기
        missile->IsMissile = (i == 0);
    }
}

//가속탄
//속도와 속도 벡터의 갱신
//탄환의 속도, 가속도, 속도 벡터의 X성분과 Y성분
void Bullet::MoveAcceleratedBullet(float speed, float accel, float& vx, float& vy) {
    //속도 벡터의 X, Y성분을 이전 속도로 나누기
    if (speed != 0) {
        vx /= speed;
        vy /= speed;
    }

    //속도 갱신
    speed += accel;

    //속도 벡터의 X, Y 성분에 새로운 속도 곱하기
    vx *= speed;
    vy *= speed;
}

//낙하탄
//탄환의 좌표, 가속도, 속도벡터의 성분
void Bullet::MoveDroppingBullet(float& x, float& y, float accel, float& vx, float& vy) {
    //속도 갱신: Y성분만 변화
    vy += accel;

    //좌표 갱신
    x += vx;
    y += vy;
}

//회전탄
//탄환의 좌표, 속도 벡터, 회전 중심의 좌표, 반경, 각도, 한번 이동 시 변화 각도
void MoveLoopingBullet(float& x, float& y, float& vx, float& vy, float cx, float cy, float r, float theta, float omega) {
    //각도 변화
    theta += omega;

    //위치 계산
    x = cx + r * cos(theta);
    y = cy + r * sin(theta);

    //탄환의 속도
    vx -= r * omega * sin(theta);
    vy  = r * omega * cos(theta);
}

//회전반경이 변화하는 회전탄의 이동
//탄환의 좌표, 속도 벡터, 회전 중심의 좌표, 반경, 반경의 변화, 각도, 한번 이동 시 변화 각도
void MoveLoopingBullet2(float& x, float& y, float& vx, float& vy, float cx, float cy, float& r, float vr, float theta, float omega) {
    //각도 변화
    theta += omega;

    //반경 변화
    r += vr;

    //위치 계산
    x = cx + r * cos(theta);
    y = cy + r * sin(theta);

    //탄환의 속도
    vx -= r * omega * sin(theta);
    vy  = r * omega * cos(theta);
}
