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

//�����ϴ� ĳ����
void Transform(bool button);

//���ӿ��� �̵�
void Underwater1(float& x, float& y, float vx, float vy, float h);
void Underwater2(float& x, float& y, float x0, float y0, float x1, float y1, float vx, float vy, float wx0[], float wy0[], float wx1[], float wy1[], int num_water);

#endif // header guard
