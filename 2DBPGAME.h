#ifdef ST1
#include<stdlib.h>
#define TITL 100
#define MAXANIM 16384
#define ALTDRAW 5
int FRMms = 10, DRAWms = 30;
WCHAR WNDtit[TITL] = L"2DBP BUILD2 TEST0";
int WNDx = 768, WNDy = 512;
#else
void USERinit2DBP() {
    initANIM(anims + 1, 16, 16);        
    initANIM(anims + 2, 16, 16);
    anims[0].tp = 2;
    anims[0].animcnt = 2;
    anims[0].anims = new ANIM * [2];
    anims[0].metas = new META * [2];
    for (int i = 0; i < 2; i++)anims[0].metas[i] = new META;
    anims[0].anims[0] = anims + 1;
    anims[0].anims[1] = anims + 2;
    anims[1].fa = anims;
    anims[2].fa = anims;
}
int px, py, dx = 1, dy = 4;
void USERtimerFUNC() {
    /*for (int i = 0; i < WNDy; i++)for (int j = 0; j < WNDx; j++) {
        anims[0].p[(i * anims[0].x + j) * 3] = i;
        anims[0].p[(i * anims[0].x + j) * 3 + 1] = i + j;
        anims[0].p[(i * anims[0].x + j) * 3 + 2] = i + j + fcnt;
    }*/
    for (int i = 0; i < 16; i++)for (int j = 0; j < 16; j++) {//先列后行!
        anims[1].p[(i * anims[1].x + j) * 3] = i + 128;
        anims[1].p[(i * anims[1].x + j) * 3 + 1] = j + 128;
        anims[1].p[(i * anims[1].x + j) * 3 + 2] = i + j + fcnt;
    }
    for (int i = 0; i < 16; i++)for (int j = 0; j < 16; j++) {//先列后行!
        anims[2].p[(i * anims[2].x + j) * 3] = i + 128;
        anims[2].p[(i * anims[2].x + j) * 3 + 1] = j + 128;
        anims[2].p[(i * anims[2].x + j) * 3 + 2] = i + j - fcnt;
    }
    px += dx;
    py += dy;
    if (px >= 768 - 16)px = 768 - 16 - 1, dx = -dx;
    if (px < 0)px = 0, dx = -dx;
    if (py >= 512 - 16)py = 512 - 16 - 1, dy = -(rand() % 8 + 1) - 4, dx = rand() % 17 - 8;
    if (py < 0)py = 0, dy = -dy;
    anims[0].metas[1]->aff.pnt.x = px;
    anims[0].metas[1]->aff.pnt.y = py;
    dtanim(anims);
}
#endif