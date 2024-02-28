#include <stdio.h>

//ÆøÅº
//ÆøÅºÀÇ À¯È¿ ½Ã°£, ¹üÀ§ÀÇ ÁÂ»óÁÂÇ¥, ¿ìÇÏÁÂÇ¥, ÅºÈ¯ÀÇ ÁÂ»óÁÂÇ¥, ¿ìÇÏÁÂÇ¥, ¼ö, Àû±âÀÇ ÁÂ»óÁÂÇ¥, ¿ìÇÏÁÂÇ¥, ¼ö
void Bomb(int& bomb_time, float x0, float y0, float x1, float y1, float bx0[], float by0[], float bx1[], float by1[], int num_bullet, float ex0[], float ey0[], float ex1[], float ey1[], int num_enemy) {
    //ÆøÅºÀÌ À¯È¿ÇÑ °æ¿ìÀÇ Ã³¸®
    if (bomb_time > 0) {
        //ÅºÈ¯ Á¦°Å
        for (int i = 0; i < num_bullet; i++) {
            if (bx[0] < x1 && x0 < bx1[i] && by[0] < y1 && y0 < by1[i]) {
                DeleteBullet[i];
            }
        }
    }

    //Àû±â¿¡ ÇÇÇØ ÀÔÈ÷±â
    for (int i = 0; i < num_enemy; i++) {
        if (ex[0] < x1 && x0 < ex1[i] && ey[0] < y1 && y0 < ey1[i]) {
            DamageEnemy[i];
        }
    }

    //ÆøÅºÀÇ À¯È¿ ½Ã°£ °¨¼Ò
    bomb_time--;
}

//±ÙÁ¢°ø°Ý
//¹öÆ° »óÅÂ, ±ÙÁ¢ °ø°Ý ÁÂ»óÁÂÇ¥, ¿ìÇÏÁÂÇ¥, Àû±âÀÇ ÁÂ»óÁÂÇ¥, ¿ìÇÏÁÂÇ¥, ¼ö
void Punch(bool button, float px0, float py0, float px1, float py1, float ex0[], float ey0[], float ex1[], float ey1[], int num_enemy) {
    static bool punching = false;   //±ÙÁ¢ °ø°Ý ÇÃ·¡±×
    static int punch_time;          //±ÙÁ¢ °ø°Ý ½Ç½Ã

    //±ÙÁ¢ °ø°Ý °³½Ã
    if (!punching && button) {
        punching = true;
        punch_time = 30;
    }

    //±ÙÁ¢ °ø°Ý Ã³¸®
    if (punching) {
        //Ãæµ¹ ÆÇÁ¤
        for (int i = 0; i < num_enemy; i++) {
            if (ex[0] < px1 && px0 < ex1[i] && ey[0] < py1 && py0 < ey1[i]) {
                DamageEnemy(i);
            }
        }
    }

    //±ÙÁ¢ °ø°Ý Ç¥½Ã
    DrawPunch();

    //±ÙÁ¢ °ø°Ý Á¾·á ÆÇ´Ü
    if (punch_time == 0) punching = false;
    else punch_time--;
}

//¹üÀ§ Æø¹ß
//Àû±âÀÇ ÁÂ»óÁÂÇ¥, ¿ìÇÏÁÂÇ¥, ¼ö, ÈÄÆøÇ³ÀÇ ÁÂ»óÁÂÇ¥, ¿ìÇÏÁÂÇ¥, ¼ö
void InducedExplosion(float ex0[], float ey0[], float ex1[], float ey1[], int num_enemy, float x0[], float y0[], float x1[], float y1[], int num_explosion) {
    //Àû±â¿Í ÈÄÆøÇ³ÀÇ Ãæµ¹ ÆÇÁ¤ Ã³¸®
    for (int i = 0; i < num_enemy; i++) {
        for (int j = 0; j < num_explosion; j++) {
            if (ex0[i] < x1[j] && x0[j] < ex1[i] && ey0[i] < y1[j] && y0[j] < ey1[i]) {
                DestroyEnemy(i);
                CreateExplosion(ex0[i], ey0[i]);
            }
        }
    }
}
