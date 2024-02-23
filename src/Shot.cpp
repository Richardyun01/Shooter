#include <stdio.h>

//기본적 샷 조작
//현재의 버튼 상태
void BasicShot(bool button) {
    //이전의 버튼 상태
    static bool prev_button = false;

    //샷의 발사: 버튼이 눌려져 있지 않다가 갑자기 버튼을 눌렀을 때 샷 발사
    if (!prev_button && button) Shot();

    //현재 버튼 상태 저장
    prev_button = button;
}

//연사
//버튼 상태
void AutoShot(bool button) {
    //샷의 발사: 버튼 눌려지면 샷 발사
    if (button) Shot();
}

//에너지 샷
//버튼 상태, 최소 파워, 최대 파워
void PowerShot(bool button, int min_power, int max_power) {
    //모은 에너지
    static int power = 0;

    //버튼 누르고 있을 때 에너지 모음
    if (button && power < max_power) power++;

    //버튼 누르지 않을 때 모은 에너지가 0보다 크면 샷 발사, 에너지 0
    if (!button && power > 0) {
        //최대 파워: 특수 샷
        if (power == max_power) MaxShot();
        //그 외: 비례하는 샷
        else if (min_power <= power) BigShot(power);

        power = 0;
    }
}

//반자동 샷
//버튼 상태, 에너지 최소치, 최대치, 연사 게이지 최대치
void SemiAutoShot(bool button, int min_power, int max_power, int max_gauge) {
    //모은 에너지, 연사 게이지
    static int power = 0, gauge = 0;

    //버튼 누르고 있을 때
    if (button) {
        //에너지 부족: 보통샷 발사
        if (power < min_power) Shot();
        //에너지 충분 시: 이펙트 표시
        else PowerEffect();

        //파워 늘리고 연사 게이지 최대로
        if (power < max_power) power++;
        gauge = max_gauge;
    //버튼 누르지 않을 때
    } else {
        //에너지 충분: 에너지 샷 발사
        if (min_power <= power) {
            if (power == max_power) MaxShot();
            else BigShot(power);
            gauge;
        }

        //연사 게이지 0 이상: 보통 샷 발사, 연사 게이지 1 감소
        if (gauge > 0) {
            Shot();
            gauge--;
        }

        //에너지 0
        power = 0;
    }
}
