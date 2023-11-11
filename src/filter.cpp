#include "filter.h"

extern Font GLOBAL_FONT;

void TestFilter::Apply (RenderTarget &rt) const {
    
    std::cout << "Applying TestFilter: param = " << test_param << "\n";

    Image img;
    rt.GetImg(img);

    Color* pixels = img.GetPixels();
    unsigned int w = img.GetWidth();
    unsigned int h = img.GetHeight();

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            Color* pix = pixels + y * w + x;
            *pix = Color(~pix -> r, ~pix -> g, ~pix -> b, pix -> a);
        }
    }

    rt.DrawImg(img, Vec(0, 0));
}

void TestFilter::SetParams (const std::vector<double>& params) {
    assert (params.size == 1);
    test_param = params[0];
}


void ClearFilter::Apply (RenderTarget &rt) const {
    Image img;
    rt.GetImg(img);

    Color* pixels = img.GetPixels();
    unsigned int w = img.GetWidth();
    unsigned int h = img.GetHeight();

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            Color* pix = pixels + y * w + x;
            *pix = col;
        }
    }

    rt.DrawImg(img, Vec(0, 0));
}

void ClearFilter::SetParams (const std::vector<double>& params) {
    assert (params.size() == 3);
    double red   = params[0];
    double green = params[1];
    double blue  = params[2];

    if (red < 0) red = 0;
    else if (red > 255) red = 255;
    if (green < 0) green = 0;
    else if (green > 255) green = 255;
    if (blue < 0) blue = 0;
    else if (blue > 255) blue = 255;

    col = Color (red, green, blue);

}

std::vector<const char*> ClearFilter::GetParamNames() const {
    return std::vector<const char*> {"Red", "Green", "Blue"};
}


FilterManager::FilterManager ():
    current (nullptr),
    active (false)
    {}

void FilterManager::Apply(RenderTarget& rt) {
    assert(current != nullptr);
    current -> Apply (rt);
    active = false;
}

//-----------------------------------------------------------------------------------------------------------

SetFilterOkBtn::SetFilterOkBtn (double x, double y, double w, double h, SetFilterController* sfc_):
    TxtButton (x, y, w, h, nullptr, nullptr, Text ("OK", GLOBAL_FONT, 30)),
    sfc (sfc_)
    {}

void SetFilterOkBtn::MousePress (const MouseState& mstate) {
    if (state == BTN_DISABLED) return;

    if (MouseOnWidget (mstate.pos)) {
        state = BTN_PRESSED;
        sfc -> OkBtnPress();
    }
}

SetFilterController::SetFilterController (FilterManager* fm, Filter* filt, EventManager* ev_man_, Widget* parent_wid_):
    filter_man (fm),
    filter (filt),
    ev_man (ev_man_),
    parent_wid (parent_wid_),
    editboxes ()
    {
        mw = new ModalWindow (150, 150, 400, 100 * (filter -> num_of_params) + 125, ev_man);
        mw -> AddSubWidget (new SetFilterOkBtn (100, 100 * (filter -> num_of_params) + 50, 200, 50, this));

        std::vector<const char*> param_names = filter -> GetParamNames();

        for (unsigned int i = 0; i < filter -> num_of_params; i++) {
            editboxes.push_back(new FloatNumEditBox (200, 50 + 100 * i, 180, 50, GLOBAL_FONT, 30));
            mw -> AddSubWidget (editboxes.back());
            //mw -> AddSubWidget (new TxtWidget (20, 50 + 100 * i, 180, 50, param_names[i], GLOBAL_FONT, 30));
        }

        parent_wid -> AddSubWidget (mw);
    }

SetFilterController::~SetFilterController() {
    mw -> need_to_close = true;
}

void SetFilterController::OkBtnPress() {
    std::vector<double> params;
    for (int i = 0; i < filter -> num_of_params; i++) {
        params.push_back(editboxes[i] -> TextToDouble());
    }
    filter -> SetParams (params);
    filter_man -> SetFilter (filter);
    filter_man -> Activate();
    delete this;
}


void filter_btn_action (BtnArgs* filter_btn_args) {
    FilterManager* filter_man = ((FilterBtnArgs*)filter_btn_args) -> filter_man;
    Filter*        filter     = ((FilterBtnArgs*)filter_btn_args) -> filter;
    EventManager*  ev_man     = ((FilterBtnArgs*)filter_btn_args) -> ev_man;
    Widget*        parent_wid = ((FilterBtnArgs*)filter_btn_args) -> parent_wid;

    new SetFilterController (filter_man, filter, ev_man, parent_wid);
}

FilterBtn::FilterBtn (double x, double y, double w, double h, const Text& txt_,
                      FilterManager* fm, Filter* filter_, EventManager* ev_man_, Widget* parent_wid_):
    TxtButton (x, y, w, h, filter_btn_action, &filter_btn_args, txt_),
    filter_btn_args (fm, filter_, ev_man_, parent_wid_)
    {}