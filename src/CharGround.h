#ifndef _CHARGROUND_H_
#define _CHARGROUND_H_

//메인 캐릭터의 상태(보행, 상승, 하강)
typedef enum {
    WALKING, JUMP_UP, JUMP_DOWN
} MYSHIP_STATE;

void Walk(float& x, float& y, float speed, bool left, bool right, bool jump_button);

//메인 캐릭터의 형태(보행, 비행)
typedef enum {
    WALKING, FLYING
} MYSHIP_FORM;

//변신하는 캐릭터
void Transform(bool button);

//물속에서 이동
void Underwater1(float& x, float& y, float vx, float vy, float h);
void Underwater2(float& x, float& y, float x0, float y0, float x1, float y1, float vx, float vy, float wx0[], float wy0[], float wx1[], float wy1[], int num_water);

#endif // header guard
