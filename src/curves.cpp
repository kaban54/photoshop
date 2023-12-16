#include "curves.h"
#include "catmullrom.h"

const double CurveWid::POINT_RADIUS = 5;
const Color CurveWid::BG_COLOR = Color (64, 64, 64);

CurveWid::CurveWid (GuiI* gui, Vec2 pos_, Vec2 size_):
    PluginWidget (gui, pos_ - Vec2(POINT_RADIUS, POINT_RADIUS), size_ + 2 * Vec2(POINT_RADIUS, POINT_RADIUS)),
    size (size_),
    color (255, 255, 255),
    moving_point (-1)
    {
        points.PushBack(Vec2(0, size.y));
        points.PushBack(Vec2(size.x, 0));
    }


void CurveWid::render (RenderTargetI* rt) {
    Vec2 pos = host -> getPos() + Vec2(POINT_RADIUS, POINT_RADIUS);
    rt -> drawRect(host -> getPos() , host -> getSize(), Color(0, 0, 0));
    rt -> drawRect(pos, size, BG_COLOR);
    size_t sz = points.GetSize();
    if (sz == 2) {
            DrawSpline(rt, pos + points[0], pos + points[1], color, 2);
    }
    else if (sz > 2) {
        DrawSpline(rt, pos + points[2], pos + points[1], pos + points[0], color, 2);
        DrawSpline(rt, pos + points[sz - 3], pos + points[sz - 2], pos + points[sz - 1], color, 2);

        for (size_t i = 1; i < sz - 2; ++i) {
            DrawSpline(rt, pos + points[i - 1], pos + points[i], pos + points[i + 1], pos + points[i + 2], color, 2);
        }
    }

    for (size_t i = 0; i < sz; ++i) {
        if (i == moving_point) {
            rt -> drawEllipse(pos + points[i] - Vec2(POINT_RADIUS + 1, POINT_RADIUS + 1),
                          2 * Vec2(POINT_RADIUS + 1, POINT_RADIUS + 1), Color(255, 255, 255));
        }
        rt -> drawEllipse(pos + points[i] - Vec2(POINT_RADIUS, POINT_RADIUS),
                          2 * Vec2(POINT_RADIUS, POINT_RADIUS), color);
    }
}


bool CurveWid::onMousePress(MouseContext context) {
    Vec2 mpos = context.position - host -> getPos() - Vec2(POINT_RADIUS, POINT_RADIUS);
    double min_len = (mpos - points[0]).GetLen();
    size_t closest = 0;
    size_t prev = 0;
    for (size_t i = 1; i < points.GetSize(); ++i) {
        if (points[i].x < mpos.x) prev = i;
        double len = (mpos - points[i]).GetLen();
        if (len < min_len) {
            closest = i;
            min_len = len;
        }
    }
    if (context.button == MouseButton::Left) {
        if (min_len > 20 && points.GetSize() < MAX_POINTS && prev != points.GetSize() - 1) {
            if ((points[prev + 1] - points[prev]).x >= 4 * POINT_RADIUS) {
                points.Insert(prev + 1, mpos);
                moving_point = prev + 1;
            }
        }
        else {
            moving_point = closest;
            move_point(mpos);
        }
    }
    else if (context.button == MouseButton::Right) {
        if (min_len <= 20 && closest != 0 && closest != points.GetSize() - 1) {
            points.Remove(closest);
        }
    }
    return true;
}

bool CurveWid::onMouseRelease(MouseContext context) {
    moving_point = -1;
    return false;
}

bool CurveWid::onMouseMove(MouseContext context) {
    if (moving_point != -1) move_point(context.position - host -> getPos() - Vec2(POINT_RADIUS, POINT_RADIUS));
    return false;
}

void CurveWid::move_point(Vec2 mpos) {
    if (moving_point == -1) return;
    if (mpos.y < 0 || mpos.y > size.y ||
        mpos.x < -POINT_RADIUS || mpos.x > size.x + POINT_RADIUS) return;

    if (moving_point == 0 || moving_point == points.GetSize() - 1) {
        points[moving_point].y = mpos.y;
    }
    else {
        if (mpos.x < points[moving_point - 1].x + POINT_RADIUS * 2)
            mpos.x = points[moving_point - 1].x + POINT_RADIUS * 2;
        else if (mpos.x > points[moving_point + 1].x - POINT_RADIUS * 2)
            mpos.x = points[moving_point + 1].x - POINT_RADIUS * 2;
        points[moving_point] = mpos;
    }
}

void CurveWid::GetValues(uint8_t values[256]) {
    size_t sz = points.GetSize();
    if (sz == 2) {
        Vec2 p0 = Get256Coords(points[0], size);
        Vec2 p1 = Get256Coords(points[1], size);
        get_values(values, p0, p1);
    }
    else if (sz > 2) {
        get_values(values, Get256Coords(points[2], size),
                           Get256Coords(points[1], size),
                           Get256Coords(points[0], size));
        get_values(values, Get256Coords(points[sz - 3], size),
                           Get256Coords(points[sz - 2], size),
                           Get256Coords(points[sz - 1], size));

        for (size_t i = 1; i < sz - 2; ++i) {
            get_values(values, Get256Coords(points[i - 1], size),
                               Get256Coords(points[i    ], size),
                               Get256Coords(points[i + 1], size),
                               Get256Coords(points[i + 2], size));
        }
    }
}

Vec2 Get256Coords(Vec2 point, Vec2 size) {
    return Vec2 (int(point.x * 255 / size.x), (size.y - point.y) * 255 / size.y);
}


void get_values (uint8_t values[256], Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3) {
    assert(   0 <= p0.x);
    assert(p0.x <= p1.x);
    assert(p1.x <= p2.x);
    assert(p2.x <= p3.x);
    assert(p3.x <= 255);

    double t0 = 0;
    double t1 = GetT(p0, p1, t0);
    double t2 = GetT(p1, p2, t1);
    double t3 = GetT(p2, p3, t2);

    double step = (t2 - t1) / ((p2 - p1).GetLen() * 3);
    int x = p1.x;
    values[x] = uint8_t(p1.y);

    for (double t = t1; t < t2; t += step) {
        Vec2 a1  = ((t1 - t) * p0 + (t - t0) * p1) / (t1 - t0);
        Vec2 a2  = ((t2 - t) * p1 + (t - t1) * p2) / (t2 - t1);
        Vec2 a3  = ((t3 - t) * p2 + (t - t2) * p3) / (t3 - t2);
        Vec2 b1  = ((t2 - t) * a1 + (t - t0) * a2) / (t2 - t0);
        Vec2 b2  = ((t3 - t) * a2 + (t - t1) * a3) / (t3 - t1);
        Vec2 pos = ((t2 - t) * b1 + (t - t1) * b2) / (t2 - t1); 

        if (pos.x > x) {
            x = pos.x;
            if      (pos.y <   0) values[int(x)] = 0;
            else if (pos.y > 255) values[int(x)] = 255;
            else                  values[int(x)] = uint8_t(pos.y);
        }
    }
}

void get_values (uint8_t values[256], Vec2 p0, Vec2 p1, Vec2 p2) {
    if (p0.x < p1.x) {
        assert(   0 <= p0.x);
        assert(p0.x <= p1.x);
        assert(p1.x <= p2.x);
        assert(p2.x <= 255);

        double t0 = 0;
        double t1 = GetT(p0, p1, t0);
        double t2 = GetT(p1, p2, t1);

        double step = (t2 - t1) / ((p2 - p1).GetLen() * 3);
        int x = p1.x;
        values[x] = uint8_t(p1.y);

        for (double t = t1; t < t2; t += step) {
            Vec2 a1  = ((t1 - t) * p0 + (t - t0) * p1) / (t1 - t0);
            Vec2 a2  = ((t2 - t) * p1 + (t - t1) * p2) / (t2 - t1);
            Vec2 pos = ((t2 - t) * a1 + (t - t0) * a2) / (t2 - t0);

            if (pos.x > x) {
                x = pos.x;
                if (x >= p2.x) break;
                if      (pos.y <   0) values[int(x)] = 0;
                else if (pos.y > 255) values[int(x)] = 255;
                else                  values[int(x)] = uint8_t(pos.y);
            }
        }
        values[int(p2.x)] = uint8_t(p2.y);
    }
    else {
        assert(   0 <= p2.x);
        assert(p2.x <= p1.x);
        assert(p1.x <= p0.x);
        assert(p0.x <= 255);

        double t0 = 0;
        double t1 = GetT(p0, p1, t0);
        double t2 = GetT(p1, p2, t1);

        double step = (t2 - t1) / ((p2 - p1).GetLen() * 3);
        int x = p1.x;
        values[x] = uint8_t(p1.y);

        for (double t = t1; t < t2; t += step) {
            Vec2 a1  = ((t1 - t) * p0 + (t - t0) * p1) / (t1 - t0);
            Vec2 a2  = ((t2 - t) * p1 + (t - t1) * p2) / (t2 - t1);
            Vec2 pos = ((t2 - t) * a1 + (t - t0) * a2) / (t2 - t0);

            if (pos.x < x) {
                x = pos.x;
                if (x <= p2.x) break;
                if      (pos.y <   0) values[int(x)] = 0;
                else if (pos.y > 255) values[int(x)] = 255;
                else                  values[int(x)] = uint8_t(pos.y);
            }
        }
        values[int(p2.x)] = uint8_t(p2.y);
    }
}

void get_values (uint8_t values[256], Vec2 p0, Vec2 p1) {
    assert(p1.x > p0.x);
    assert(p0.x == 0);
    assert(p1.x == 255);

    int min_x = int(p0.x);
    int max_x = int(p1.x);
    double ystep = (p1 - p0).y / (max_x - min_x);
    double y = p0.y;
    for (int x = min_x; x <= max_x; ++x) {
        values[x] = uint8_t(y);
        y += ystep;
    }
}


CurvesFilter::CurvesFilter(GuiI* gui_):
    gui (gui_)
    {}

void CurvesFilter::apply (RenderTargetI *data) {
    PluginWindow* win = new PluginWindow(gui, Vec2(100, 100), Vec2(400, 400));
    CurveWid* curve = new CurveWid(gui, Vec2(100, 100), Vec2(200, 200));
    curve -> SetColor (Color(255, 0, 0));
    win -> host -> registerSubWidget(curve -> host);
    gui -> getRoot() -> registerSubWidget(win -> host);
}

Array<const char *> CurvesFilter::getParamNames() const {
    return Array<const char *> (0, nullptr);
}

Array<double> CurvesFilter::getParams() const {
    return Array<double> (0, nullptr);
}

void CurvesFilter::setParams(Array<double> new_params) {
    assert(new_params.size == 0);
}


CurvesFilterPlugin::CurvesFilterPlugin(App* app_):
    app (app_)
    {
        id = (1ull << 54) + 'c' + 'u' * 'r' + 'v' * 'e' + 's';
        name = "Curves";
        type = InterfaceType::Filter;
        filter = new CurvesFilter(app -> root);
    }

CurvesFilterPlugin::~CurvesFilterPlugin() {
    delete filter;
}


extern "C" plugin::Plugin* getInstance(plugin::App *app) {
    return new CurvesFilterPlugin(app);
}