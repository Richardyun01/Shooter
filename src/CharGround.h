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

void Transform(bool button);

#endif // header guard
