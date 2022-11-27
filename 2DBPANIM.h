struct MAT22 {
    double a1=1, a2=0, b1=0, b2=1;
};
struct PNT {
    double x=0, y=0;
};
struct AFFtr {
    MAT22 mat;
    PNT pnt;
};
struct COL {
    BYTE R=0, G=0, B=0;
};
struct META {
    AFFtr aff;
    COL col;
    double rat = 0;
}Nmeta;
struct ANIM {
    int tp;//0图片1图片序列2多图片
    int act;
    ANIM* fa;
    int x, y;
    int dt;
    BYTE* p;
    ANIM** anims;
    META** metas;
    int animcnt;
    double prog, spd;
    int nAFF;
    /*act=活动的 x,y=图片大小 p=图片 meta=后处理
    tp=0->不更新图片 1,2->dt=需要更新
    1->图片meta变换 +叠加(用子图片的meta)
    2->ANIM中选prog spd速度移动(用子图片的meta)
    nAFF=meta无仿射*/
};
COL mixCOL(COL a, COL b, double cr) {
    COL c;
    c.R = floor(a.R * (1 - cr) + b.R * cr);
    c.G = floor(a.G * (1 - cr) + b.G * cr);
    c.B = floor(a.B * (1 - cr) + b.B * cr);
    return c;
}
void nAFFcpIMG(ANIM* img, ANIM* anim, META* meta, int typ) {
    int kx = 0;
    int ky = 0;
    if (typ) {
        kx = floor(meta->aff.pnt.x);
        ky = floor(meta->aff.pnt.y);
    }
    for (int i = 0; i < anim->y; i++)for (int j = 0; j < anim->x; j++) {
        int cy = i + ky;
        int cx = j + kx;
        if (cx >= 0 && cy >= 0 && cx < img->x && cy < img->y) {
            COL col1;
            col1.R = anim->p[(i * anim->x + j) * 3];
            col1.G = anim->p[(i * anim->x + j) * 3 + 1];
            col1.B = anim->p[(i * anim->x + j) * 3 + 2];
            if (typ && meta->rat) {
                COL col2 = mixCOL(col1, meta->col, meta->rat);
                img->p[(cy * img->x + cx) * 3] = col2.R;
                img->p[(cy * img->x + cx) * 3 + 1] = col2.G;
                img->p[(cy * img->x + cx) * 3 + 2] = col2.B;
            }
            else {
                img->p[(cy * img->x + cx) * 3] = col1.R;
                img->p[(cy * img->x + cx) * 3 + 1] = col1.G;
                img->p[(cy * img->x + cx) * 3 + 2] = col1.B;
            }
        }
    }
}
META mataCO(META ma, META mb) {//后面加仿射
    META mc;
    mc.rat = ma.rat + mb.rat - ma.rat * mb.rat;
    mc.col.R = floor((ma.col.R * ma.rat + mb.col.R * mb.rat - mb.col.R * ma.rat * mb.rat) / mc.rat);
    mc.col.G = floor((ma.col.G * ma.rat + mb.col.G * mb.rat - mb.col.G * ma.rat * mb.rat) / mc.rat);
    mc.col.B = floor((ma.col.B * ma.rat + mb.col.B * mb.rat - mb.col.B * ma.rat * mb.rat) / mc.rat);
    mc.aff.pnt.x = ma.aff.pnt.x + mb.aff.pnt.x;
    mc.aff.pnt.y = ma.aff.pnt.y + mb.aff.pnt.y;
    return mc;
}
ANIM anims[MAXANIM];//根不可变换!
int ancnt, anfcnt, anf[MAXANIM], anu[MAXANIM];
void initANIM(ANIM* anim, int x, int y) {
    anim->x = x;
    anim->y = y;
    anim->act = 1;
    anim->dt = 0;
    anim->tp = 0;
    anim->fa = nullptr;
    anim->metas = nullptr;
    anim->nAFF = 1;
    anim->animcnt = 0;
    anim->anims = nullptr;
    anim->prog = 0;
    anim->spd = 0;
    anim->p = new BYTE[x * y * 3];
    for (int i = 0; i < x * y * 3; i++)anim->p[i] = 0;
}
void drawANIM(ANIM* t) {
    if (t->tp == 0)return;
    if (!t->dt)return;
    t->dt = 0;
    for (int i = 0; i < t->x * t->y * 3; i++)t->p[i] = 0;
    if (t->tp == 1)if ((*(t->anims + int(floor(t->prog))))->act)nAFFcpIMG(t, (t->anims)[int(floor(t->prog))], t->metas[int(floor(t->prog))], 1);
    if (t->tp == 2)for (int i = 0; i < t->animcnt; i++)if ((*(t->anims) + i)->act)nAFFcpIMG(t, (t->anims)[i], (t->metas)[i], 1);
}
int alloANIM() {
    if (anfcnt == 0) { anu[ancnt] = 1; return ++ancnt; }
    anu[--anfcnt] = 1;
    return anf[anfcnt];
}
void freeANIM(int k, int id) {
    anf[anfcnt++] = id;
    anu[id] = 0;
    delete[] anims[id].p;
    delete[] anims[id].anims;
    delete[] anims[id].metas;
}
void dtanim(ANIM* t) {
dt:
    t->dt = 1;
    t = t->fa;
    if (t == nullptr)return;
    goto dt;
}
//TODO:AFFcpIMG