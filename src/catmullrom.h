#ifndef CATMULLROM_H
#define CATMULLROM_H

#include <cmath>
#include "standart2.h"

using namespace plugin;

double GetT (Vec2 p1, Vec2 p2, double t_prev);

void DrawSpline (RenderTargetI* rt, Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, Color color, double thikness);
void DrawSpline (RenderTargetI* rt, Vec2 p0, Vec2 p1, Vec2 p2,          Color color, double thikness);
void DrawSpline (RenderTargetI* rt, Vec2 p0, Vec2 p1,                   Color color, double thikness);


#endif