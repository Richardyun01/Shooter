#ifndef _OPTION_H_
#define _OPTION_H_

//게이지를 사용한 파워업
//파워업의 종류
typedef enum {
    NONE,   //커서가 없는 상태
    SPEED, MISSILE, DOUBLE,
    LASER, OPTION, BARRIER,
    END     //게이지의 맨 오른쪽
} POWER_UP_TYPE;

void PowerUpGauge(bool power_up_button);

void InitOption(float x, float y, float ox[], float oy[], int length);
void MoveOption(float x, float y, float ox[], float oy[], int length, int& index, int opt_count, int opt_interval);

void Barrier1(float x0, float y0, float x1, float y1, float bx0[], float by0[], float bx1[], float by1[], int num_bullet, float ex0[], float ey0[], float ex1[], float ey1[], int num_enemy, int& damage, int max_damage);

void Barrier2(float x0, float y0, float x1, float y1, float bx0[], float by0[], float bx1[], float by1[], int num_bullet, float ex0[], float ey0[], float ex1[], float ey1[], int num_enemy, int& energy, bool barrier_button);

#endif // header guard

