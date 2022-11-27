#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <thread>
#include <mutex>
#include <atomic>
int fcnt, pfcnt;
HWND hWnd;
#define ST1
#include "2DBPGAME.h"
#undef ST1
#include "2DBPANIM.h"
#include "2DBPGAME.h"
int lBYTE = (WNDx * 3 + 3) / 4 * 4;
BYTE* disBUF = new BYTE[lBYTE * WNDy];
void timerFUNC() {
    USERtimerFUNC();
}
void init2DBP() {
    ancnt = 1;
    anu[0] = 1;
    initANIM(anims, WNDx, WNDy);
    USERinit2DBP();
}
void disp(HWND hWnd) {
    if (fcnt == pfcnt)return;
    pfcnt = fcnt;
    drawANIM(anims);
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    HDC hdc2 = CreateCompatibleDC(hdc);
    HBITMAP bm = CreateCompatibleBitmap(hdc, WNDx, WNDy);
    SelectObject(hdc2, bm);
    BITMAPINFO bmpInfo;
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = WNDx;
    bmpInfo.bmiHeader.biHeight = -WNDy;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 24;
    bmpInfo.bmiHeader.biCompression = BI_RGB;
    bmpInfo.bmiHeader.biSizeImage = 0;
    bmpInfo.bmiHeader.biXPelsPerMeter = 0;
    bmpInfo.bmiHeader.biYPelsPerMeter = 0;
    bmpInfo.bmiHeader.biClrUsed = 0;
    bmpInfo.bmiHeader.biClrImportant = 0;
    for (int i = 0; i < WNDy; i++)for (int j = 0; j < WNDx; j++) {
        disBUF[i * lBYTE + j * 3 + 2] = anims[0].p[(i * anims[0].x + j) * 3];
        disBUF[i * lBYTE + j * 3 + 1] = anims[0].p[(i * anims[0].x + j) * 3 + 1];
        disBUF[i * lBYTE + j * 3] = anims[0].p[(i * anims[0].x + j) * 3 + 2];
    }
    SetDIBits(hdc2, bm, 0, WNDy, disBUF, &bmpInfo, DIB_RGB_COLORS);
    BitBlt(hdc, 0, 0, WNDx, WNDy, hdc2, 0, 0, SRCCOPY);
    DeleteObject(bm);
    DeleteDC(hdc2);
    EndPaint(hWnd, &ps);
}
void th2() {
    long long k = GetTickCount64() / FRMms;
    while (1) {
        long long t = GetTickCount64();
        long long r = t / FRMms;
        if (r > k)timerFUNC();
        k = r;
        Sleep(max(((r + 1) * FRMms - t), 0));
    }
}
void th3() {
    long long k = (GetTickCount64()-ALTDRAW) / DRAWms;
    while (1) {
        long long t = (GetTickCount64()-ALTDRAW);
        long long r = t / DRAWms;
        if (r > k)fcnt++, InvalidateRect(hWnd, nullptr, 1), UpdateWindow(hWnd);
        k = r;
        Sleep(max(((r + 1) * DRAWms - t), 0));
    }
}
