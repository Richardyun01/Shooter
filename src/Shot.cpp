#include <stdio.h>
#include <math.h>

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

//버튼을 떼고 나서 에너지를 모으기 시작하는 에너지 샷
//버튼 상태, 에너지 샷의 최소 파워, 최대 파워
void PowerShotReleased(bool button, int min_power, int max_power) {
    //모은 에너지
    static int power = 0;

    //버튼 누르지 않을 때: 에너지 모으기
    if (!button && power < max_power) power++;

    //버튼 누를 때: 모은 에너지가 0 초과시 샷 발사 후 에너지 0
    if (button && power > 0) {
        //최대 파워: 특수 샷 발사
        if (power == max_power) MaxShot();
        //그 외: 해당 위력 샷 발사
        else if (min_power <= power) BigShot(power);
        //에너지 0
        power = 0;
    }
}

//연사와 레이저를 같은 버튼으로 사용
//버튼 상태, 에너지 샷의 최소 파워, 최대 파워
void PowerShotReleased(bool button, int min_power, int max_gauge) {
    //모은 에너지, 연사 게이지
    static int power = 0, gauge = 0;

    //버튼 누를 때
    if (button) {
        //에너지 부족: 에너지 증가, 연사 게이지 최대 샷 발사
        if (power < min_power) {
            power ++;
            gauge = max_gauge;
            Shot();
        //에너지 충전: 연사 게이지 0 레이저 발사
        } else {
            gauge = 0;
            Laser();
        }
    //버튼 누르지 않을 때
    } else {
        //연사 게이지 0 초과: 일반 샷 발사 및 연사 게이지 1 감소
        if (gauge > 0) {
            Shot();
            gauge--;
        }

        //에너지 0
        power = 0;
    }
}

//락샷
//버튼 상태, 에너지 최소치, 연사 게이지 최대치, 락샷 추격 적기 ID(미추격시 음수), 메인 캐릭터의 정면 향하는 방향
void LockShot(bool button, int min_power, int max_gauge, int enemy_id, float front_dir) {
    //모은 에너지, 연사 게이지
    static int power = 0, gauge = 0;

    //버튼 누를 때
    if (button) {
        //에너지 부족: 에너지 증가, 연사 게이지 최대 샷 발사
        if (power < min_power) {
            power ++;
            gauge = max_gauge;
            Shot();
        //에너지 충전: 연사 게이지 0 락샷 발사
        } else {
            gauge = 0;

            //락샷 발사: 조준+방향탄
            if (enemy_id >= 0) AimingShot(enemy_id);
            else DirectedShot(front_dir);
        }
    //버튼 누르지 않을 때
    } else {
        //연사 게이지 0 초과: 일반 샷 발사 및 연사 게이지 1 감소
        if (gauge > 0) {
            Shot();
            gauge--;
        }

        //에너지 0
        power = 0;
    }
}

//커맨드 샷
//스틱과 버튼의 상태를 나타내는 플래: 2개 이상의 플래그 조합 가능
#define NONE    0   //초기 상태
#define NEUTRAL 1   //중립
#define UP      2   //상
#define DOWN    4   //하
#define LEFT    8   //좌
#define RIGHT   16  //우
#define BUTTON  32  //버튼

//저장할 입력 내역의 개수, 입력 내역 저장용 배열
#define NUM_HISTORY 30
static int History(NUM_HISTORY);

//커맨드
typedef struct {
    int Length;             //커맨드 길이
    int Limit;              //입력 제한 시간
    int Input[NUM_HISTORY]; //입력 내용
} COMMAND_TYPE;

#define NUM_COMMAND 2
COMMAND_TYPE Command[NUM_COMMAND];

//입력 내역과 커맨드의 초기화
void InitCommandShot() {
    //입력 내역의 초기화
    for (int i = 0; i < NUM_HISTORY; i++)
        History[i] = NONE;

    //커맨드 1: 파동권
    Command[0].Length = 4;
    Command[0].Limit = 30;
    Command[0].Input[0] = DOWN;
    Command[0].Input[1] = DOWN|RIGHT;
    Command[0].Input[2] = RIGHT;
    Command[0].Input[3] = BUTTON;

    //커맨드 2: 승룡권
    Command[1].Length = 4;
    Command[1].Limit = 30;
    Command[1].Input[0] = RIGHT;
    Command[1].Input[1] = DOWN;
    Command[1].Input[2] = DOWN|RIGHT;
    Command[1].Input[3] = BUTTON;
}

//커맨드 샷의 판정 수행
//스틱 상태, 버튼 상태
void CommandShot(bool up, bool down, bool left, bool right, bool button) {
    //입력 내역의 기록 위치
    static int index = 0;

    //입력 내역을 기록하기
    History[index] = (up ? UP : 0) | (down ? DOWN : 0) | (left ? LEFT : 0) | (right ? RIGHT : 0) | (button ? BUTTON : 0);

    //커맨드가 입력되었는지 판정
    int c, j, i;
    for (c = 0; c = NUM_COMMAND; c++) {
        for (i = 0; j = Command[c].Length - 1; j >= 0; j--) {
            for (; i < Command[c].Limit; i++) {
                if (History[(index - i + NUM_HISTORY) % NUM_HISTORY] == Command[c].Input[j]) break;
            }
            if (i == Command[c].Limit) break;
        }

        //커맨드 입력 확인: 커맨드 샷 발사 후 입력 내역 삭제
        if (j == -1) {
            ComShot(j);
            for (i = 0; i < NUM_HISTORY; i++)
                History[i] = NONE;
        }
    }

    //기록 위치를 갱신
    index = (index + 1) % NUM_HISTORY;
}

//샷의 이동
//샷의 좌상좌표, 우하좌표, 속도 방향, 테두리의 좌상좌표, 우하좌표
void MoveShot(float& x0, float& y0, float& x1, float& y1, float vx, float vy, float sx0, float sy0, float sx1, float sy1) {
    //샷이 화면 밖으로 가면 제거
    if (x1 <= sx0 || sx1 <= x0 || y1 <= sy0 || sy1 <= y0) DeleteShot();

    //샷의 좌표를 갱신
    x0 += vx; y0 += vy;
    x1 += vx; y1 += vy;
}

//샷의 충돌 판정 처리
//샷 충돌 판정 영역 좌상좌표, 우하좌표, 공격력, 목표물 충돌 판정 영역 좌상좌표, 우하좌표, 내구력, 파괴 여부 플래그
void HitShot(float x0, float y0, float x1, float y1, float attack, float ox0, float oy0, float ox1, float oy1, float endurance, bool invincible) {
    //샷이 목표물에 부딪힌 경우
    if (ox0 < x1 && x0 < ox1 && oy0 < y1 &7 y0 < oy1) {
        //목퓨가 무적이 아니면 내구력을 깎고 0이되면 목표물 제거
        if (!invincible) {
            endurance -= attack;
            if (endurance < 0)
                DeleteOpponent();
        }

        //샷 제거
        //샷을 제거하지 않을 시 관통탄이 됨
        DeleteShot();
    }
}

//적기와의 거리에 따른 샷의 위력 차이
//샷의 좌표, 속도, 위력, 위력 감쇄
void ShotPower(float& x, float& y, float vx, float vy, float& power, float attenuation) {
    //샷 이동
    x += vx; y += vy;

    power -= attenuation;
}

//조준점을 사용한 폭격
//메인 캐릭터으ㅢ 좌표, 조준점의 좌표, 폭탄의 속도, 버튼 상태
void SightedBomb(float x, float y, float sx, float sy, float bvx, float bvy, bool button) {
    //폭탄의 상태
    static bool bombing = false;//폭격 여부 플래그
    static float bx, by;        //폭탄 좌표
    static float tx, fty;       //착탄점 좌표

    //폭격 중 아님: 버튼이 눌리면 폭격 수행, 폭탄의 초기좌표와 착탄점의 좌표 설정
    //폭격 중: 폭탄 이동, 착탄점 도달 시 폭발
    if (!bombing) {
        if (button) {
            bombing = true;
            bx = x; by = y;
            tx = sx; ty = sy;
        }
    } else {
        bx += bvx; by += bvy;
        if (bx == tx && by == ty) {
            Explode();
            bombing = false;
        }
    }

    //메인 캐릭터와 조준점 그리기
    DrawMyShip(x, y);
    DrawScope(sx, sy);

    //폭격 중 착탄점과 폭탄 그림
    if (bombing) {
        DrawTarget(tx, ty);
        DrawBomb(bx, by);
    }
}

//록온 레이저
//적기를 나타내는 구조체
typedef struct {
    bool Locked;    //고정된 상태 여부 플래그
    float X, Y;     //좌표
} ENEMY_TYPE;

#define MAX_ENEMIES 100

static ENEMY_TYPE Enemy[MAX_ENEMIES];
bool Hit(float x, float y, ENEMY_TYPE& enemy);
void ShootLockOnLaser(ENEMY_TYPE& enemy);

//록온의 사용 가능 횟수
static int AvailLocks = 8;

//록온 레이저의 발사
//조준점의 좌표, 버튼 상태
void LockOnLaser(float sx, float sy, bool button) {
    //록온 사용 가능 횟수가 1 이상: 모든 적기와 주준점의 충돌 판정 수행, 조준점과 겹쳐 있는 적기에 록 마크 표시
    for (int i = 0; AvailLocks > 0 && i < MAX_ENEMIES; i++) {
        if (!Enemy[i].Locked && Hit(sx, sy, Enemy[i])) {
            Enemy[i].Locked = true;

            //록온의 사용 가능 횟수 줄이기: 레이저 명중 시 사용 가능 횟수 증가
            AvailLocks--;
        }
    }

    //버튼 눌릴 시: 록 마크가 달려 있는 모든 적기를 향해 레이저 발사
    if (button) {
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (Enemy[i].Locked) ShootLockOnLaser(Enemy[i]);
        }
    }
}

//지형 따라 날아가는 미사일
//미사일의 좌상좌표, 우하좌표, 속도, 지형의 좌상좌표, 우하좌표, 수, 적기의 좌상좌표, 우하좌표, 수
void MoveGroundMissile(float& x0, float& y0, float& x1, float& y1, float vx, float vy, float gx0[], float gy0[], float gx1[], float gy1[], int num_ground, float ex0[], float ey0[], float ex1[], float ey1[], int num_enemy) {
    //지형과 충돌 판정 처리: 미사일을 수직 방향으로 진행시킨 좌표에 대해 판정, 충돌할 경우 수직 방향 속도 0 및 루프 종료
    for (int i = 0; i < num_ground; i++) {
        if (gx0[i] < x1 && x0 < gx1[i] && gy0[i] < y1 + vy && y0 + vy < gy1[i]) {
            vy = 0;
            break;
        }
    }

    //미사일 진행
    x0 += vx; y0 += vy;
    x1 += vx; y1 += vy;

    //적기와의 충돌 판정 처리: 적기 충돌시 적기에 피해를 주고 루프 종료
    for (int i = 0; i < num_enemy; i++) {
        if (ex0[i] < x1 && x0 < ex1[i] && ey0[i] < y1 && y0 < ey1[i]) {
            DamageEnemy(i);
            break;
        }
    }
}

//지형 반사 샷
//샷의 좌상좌표, 우하좌표, 속도, 지형의 좌상좌표, 우하좌표, 수, 적기의 좌상좌표, 우하좌표, 수
void MoveReflectShot(float& x0, float& y0, float& x1, float& y1, float vx, float vy, float gx0[], float gy0[], float gx1[], float gy1[], int num_ground, float ex0[], float ey0[], float ex1[], float ey1[], int num_enemy) {
    //지형과 충돌 판정 처리: 샷을 수직 방향으로 진행시킨 좌표에 대해 판정, 충돌할 경우 수직 방향 속도 반전 및 루프 종료
    for (int i = 0; i < num_ground; i++) {
        if (gx0[i] < x1 && x0 < gx1[i] && gy0[i] < y1 + vy && y0 + vy < gy1[i]) {
            vy = -vy;
            break;
        }
    }

    //샷 진행
    x0 += vx; y0 += vy;
    x1 += vx; y1 += vy;

    //적기와의 충돌 판정 처리: 적기 충돌시 적기에 피해를 주고 루프 종료
    for (int i = 0; i < num_enemy; i++) {
        if (ex0[i] < x1 && x0 < ex1[i] && ey0[i] < y1 && y0 < ey1[i]) {
            DamageEnemy(i);
            break;
        }
    }
}

//지형 반사 레이저
//레이저의 각 부분 구조체
typedef struct LASER_STRUCT {
    float X, Y;                 //좌상좌표
    float W, H;                 //폭과 높이
    float VX, VY;               //속도
    float OldX, Old Y;          //이전좌표
    struct LASER_STRUCT* Prec;  //바로 앞부분
                                //선두일 경우 NULL
} LASER_TYPE;

//레이저 이동 함수
//레이저 한 부분, 지형의 좌상좌표, 우하좌표, 수
void MoveReflectLaser(LASER_TYPE* laser, float gx0[], float gy0[], float gx1[], float gy1[], int num_ground) {
    //선두 부분: 선두를 수직 방향으로 진행시킨 좌표에 대해 판정, 충돌할 경우 수직 방향 속도 반전 및 루프 종료
    if (!laser->Prec) {
        //지형 충돌 판정 처리: 선두부분을 수직방향으로
        for (int i = 0; i < num_ground; i++) {
            if (gx0[i] < laser->X + laser->W && laser->X < gx1[i] && gy0[i] < laser->Y + laser->W + laser->VY && laser->Y + laser->VY < gy1[i]) {
                laser->VX = -laser->VX;
                break;
            }
        }

        //선두 부분 진행
        laser->X += laser->VX;
        laser->Y += laser->VY;
    //선두 이외: 바루 앞부분의 이전 좌표로 이동
    } else {
        laser->X = laser->Prec->OldX;
        laser->Y = laser->Prec->OldY;
    }
}

//무기 바꾸기
#define MAX_WEAPONS 5 //무기의 수

//무기 전환 처리
//버튼 상태
void SelectWeapon(bool button) {
    static int weapon_id = 0;       //현재 선택 중 무기의 번호
    static bool prev_button = fase; //이전 버튼 상태

    //무기 바꾸기
    if (!prev_button && button) {
        weapon_id = (weapon_id + 1) % MAX_WEAPONS;
    }

    //무기 아이콘 표시: 돋보이게 함
    for (int i = 0; i < MAX_WEAPONS; i++)
        DrawIcon(i);
    FocusIcon(weapon_id);

    //현재 상태 저장
    prev_button = button;
}

//방향 전환에 의한 전방위 사격
//메인 캐릭터의 좌표, 속도, 스틱의 상태, 선회 버튼 상태, 샷 버튼 상태
void TurnMarker(float& x, float& y, float speed, bool left, bool right, bool up, bool down, bool turn_button, bool shot_button) {
    static bool turning;            //선회중이면 true
    static float mx, my;            //턴 마커의 좌표
    static float rad = M_PI * 3 / 2;//선회각도
    static float d = 10;            //메인 캐릭터와 턴 마커간 거리

    //선회할 경우: 턴 마커의 좌표 계산
    if (!turning && turn_button) {
        turning = true;
        mx = x - d * cos(rad);
        my = y - d * sin(rad);
    }

    //선회 종료
    if (!turn_button) turning = false;

    //케인 캐릭터의 이동
    if (up   ) y -= speed;
    if (down ) y += speed;
    if (left ) x -= speed;
    if (right) x += speed;

    //선회중이면 선회각도 계산
    if (turning) rad = atan2(y - my, x - mx);

    //샷 발사: 메인 캐릭터가 향한 방향으로 샷 발사, rad의 반대 방향
    if (shot_button) DirectedShot(x, y, rad + M_PI);
}
