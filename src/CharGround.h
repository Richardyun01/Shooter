#ifndef _CHARGROUND_H_
#define _CHARGROUND_H_

//���� ĳ������ ����(����, ���, �ϰ�)
typedef enum {
    WALKING, JUMP_UP, JUMP_DOWN
} MYSHIP_STATE;

void Walk(float& x, float& y, float speed, bool left, bool right, bool jump_button);

//���� ĳ������ ����(����, ����)
typedef enum {
    WALKING, FLYING
} MYSHIP_FORM;

void Transform(bool button);

#endif // header guard
