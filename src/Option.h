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

#endif // header guard

