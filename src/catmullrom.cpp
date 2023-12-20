#include "catmullrom.h"
#include <iostream>

double GetT (Vec2 p1, Vec2 p2, double t_prev) {
    return sqrt((p2 - p1).GetLen()) + t_prev;
}


void DrawSpline (RenderTargetI* rt, Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, Color color, double thikness) {
    double t0 = 0;
    double t1 = GetT(p0, p1, t0);
    double t2 = GetT(p1, p2, t1);
    double t3 = GetT(p2, p3, t2);

    double step = (t2 - t1) / ((p2 - p1).GetLen() * 3);

    for (double t = t1; t < t2; t += step) {
        Vec2 a1  = ((t1 - t) * p0 + (t - t0) * p1) / (t1 - t0);
        Vec2 a2  = ((t2 - t) * p1 + (t - t1) * p2) / (t2 - t1);
        Vec2 a3  = ((t3 - t) * p2 + (t - t2) * p3) / (t3 - t2);
        Vec2 b1  = ((t2 - t) * a1 + (t - t0) * a2) / (t2 - t0);
        Vec2 b2  = ((t3 - t) * a2 + (t - t1) * a3) / (t3 - t1);
        Vec2 pos = ((t2 - t) * b1 + (t - t1) * b2) / (t2 - t1); 

        rt -> drawEllipse(pos - Vec2(thikness, thikness) / 2, Vec2(thikness, thikness), color);
    }
}

void DrawSpline (RenderTargetI* rt, Vec2 p0, Vec2 p1, Vec2 p2, Color color, double thikness) {
    double t0 = 0;
    double t1 = GetT(p0, p1, t0);
    double t2 = GetT(p1, p2, t1);

    double step = (t2 - t1) / ((p2 - p1).GetLen() * 3);

    for (double t = t1; t < t2; t += step) {
        Vec2 a1  = ((t1 - t) * p0 + (t - t0) * p1) / (t1 - t0);
        Vec2 a2  = ((t2 - t) * p1 + (t - t1) * p2) / (t2 - t1);
        Vec2 pos = ((t2 - t) * a1 + (t - t0) * a2) / (t2 - t0);

        rt -> drawEllipse(pos - Vec2(thikness, thikness) / 2, Vec2(thikness, thikness), color);
    }
}

void DrawSpline (RenderTargetI* rt, Vec2 p0, Vec2 p1, Color color, double thikness) {
    double step = 0.5 / (p1 - p0).GetLen();
    Vec2 d_pos = (p1 - p0) * step;
    Vec2 pos = p0;

    for (double t = 0; t <= 1; t += step) {
        rt -> drawEllipse(pos - Vec2(thikness, thikness) / 2, Vec2(thikness, thikness), color);
        pos += d_pos;
    }
}