#include <math.h>
#include <stdio.h>

//게이지를 사용한 파워업
//파워업 버튼을 누름
void PowerUpGauge(bool power_up_button) {
    static POWER_UP_TYPE cursor = NONE;

    //아이템 획득 처리: 아이템 획득 시 우측 이동, 최우측 이동 시 좌측으로 초기화
    if (PickItem()) {
        cursor++;
        if (cursor == END) cursor = SPEED;
    }

    //파워업 처: 버튼이 눌리면 현재 커서가 가리키는 파워업 적용
    if (power_up_button) PowerUp(cursor);
}

//옵션의 초기화와 이동
//메인 캐릭터의 좌표, 이전 좌표, 이전 좌표의 개수
void InitOption(float x, float y, float ox[], float oy[], int length) {
    //배열의 모든 요소를 메인 캐릭터의 좌표로 초기화
    for (int i = 0; i < length; i++) {
        ox[i] = x;
        oy[i] = y;
    }
}

//옵션의 이동
//메인 캐릭터의 좌표, 이전 좌표, 이전 좌표의 개수, 좌표의 저장 위치, 옵션의 개수, 간격
void MoveOption(float x, float y, float ox[], float oy[], int length, int& index, int opt_count, int opt_interval) {
    //옵션 그리기
    for (int c = 0; i = index, c < opt_count; c++) {
        i = (i - opt_interval + length) % length;
        DrawOption(ox[i], oy[i]);
    }

    //메인 캐릭터의 좌표 저장 후 좌표의 저장 위치 갱신
    ox[index] = x;
    oy[index] = y;
    index = (index + 1) % length;
}

//보호막
//보호막 충돌 판정 영역, 좌상/우하, 탄환 충돌 판정 영역, 좌상/우하, 탄환 수, 적기 충돌 판정 영역, 좌상/우하, 적의 수, 보호막 피해량, 보호막 한계치
void Barrier1(float x0, float y0, float x1, float y1, float bx0[], float by0[], float bx1[], float by1[], int num_bullet, float ex0[], float ey0[], float ex1[], float ey1[], int num_enemy, int& damage, int max_damage) {
    //보호막과 탄환의 충돌 판정 처리: 탄환과 충돌 시 탄환 삭제/보호막 피해치 증가
    for (int i = 0; i < num_bullet && damage < max_damage; i++) {
        if (bx0[i] < x1 && x0 < bx1[i] && by0[i] < y1 && y0 < by1[i]) {
            DeleteBulltet(i);
            damage++;
        }
    }

    //보호막과 적의 충돌 판정 처리: 탄환과 충돌 시 적 피해/보호막 피해치 증가
    for (int i = 0; i < num_enemy && damage < max_damage; i++) {
        if (ex0[i] < x1 && x0 < ex1[i] && ey0[i] < y1 && y0 < ey1[i]) {
            DamageEnemy(i);
            damage++;
        }
    }

    //피해 한계 초과: 보호막 제거
    if (damage >= max_damage) DeleteBarrier();
}

//버튼을 사용하여 펼치는 보호막
//보호막 충돌 판정 영역, 좌상/우하, 탄환 충돌 판정 영역, 좌상/우하, 탄환 수, 적기 충돌 판정 영역, 좌상/우하, 적의 수, 보호막 에너지, 보호막 버튼 상태
void Barrier1(float x0, float y0, float x1, float y1, float bx0[], float by0[], float bx1[], float by1[], int num_bullet, float ex0[], float ey0[], float ex1[], float ey1[], int num_enemy, int& energy, bool barrier_button) {
    //보호막 발동: 보호막 버튼이 눌렸고 에너지가 있을 때
    if (barrier_button && energy > 0) {
        //보호막과 탄환의 충돌 판정 처리: 탄환과 충돌 시 탄환 삭제/보호막 피해치 증가
        for (int i = 0; i < num_bullet && energy > 0; i++) {
            if (bx0[i] < x1 && x0 < bx1[i] && by0[i] < y1 && y0 < by1[i]) {
                DeleteBulltet(i);
                energy--;
            }
        }

        //보호막과 적의 충돌 판정 처리: 탄환과 충돌 시 적 피해/보호막 피해치 증가
        for (int i = 0; i < num_enemy && energy > 0; i++) {
            if (ex0[i] < x1 && x0 < ex1[i] && ey0[i] < y1 && y0 < ey1[i]) {
                DamageEnemy(i);
                energy--;
            }
        }

        //에너지 소비
        energy--;
    }
}
