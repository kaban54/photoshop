#include "filter.h"

TestFilter::TestFilter () {
    params[0] = 0;
}

const char* const TestFilter::PARAM_NAMES[] = {"test param"};

void TestFilter::apply (RenderTargetI *data) {
    
    std::cout << "Applying TestFilter: param = " << params[0] << "\n";

    Texture* img = data -> getTexture();

    Color* pixels = img -> pixels;
    unsigned int w = img -> width;
    unsigned int h = img -> height;

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            Color* pix = pixels + y * w + x;
            *pix = Color(~pix -> r, ~pix -> g, ~pix -> b, pix -> a);
        }
    }

    data -> drawTexture(Vec2(0, 0), Vec2(w, h), img);
    delete img;
}

Array<const char *> TestFilter::getParamNames() {
    MyVector<const char *> ret (NUM_OF_PARAMS);
    for (size_t i = 0; i < NUM_OF_PARAMS; i++) ret[i] = PARAM_NAMES[i];
    return Array<const char *> (ret);
}

Array<double> TestFilter::getParams() {
    return Array<double> (NUM_OF_PARAMS, params);
}

void TestFilter::setParams(Array<double> new_params) {
    assert(new_params.size == NUM_OF_PARAMS);
    params[0] = new_params.data[0];
}


/*
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
}*/


FilterManager::FilterManager ():
    filter (nullptr),
    rt (nullptr) {}

void FilterManager::setRenderTarget(RenderTargetI *target) {
    rt = target;
}

void FilterManager::setFilter(FilterI *filt) {
    filter = filt;
}

void FilterManager::applyFilter() {
    assert(filter != nullptr);
    if (rt == nullptr) return;
    filter -> apply(rt);
    rt -> display();
}


//-----------------------------------------------------------------------------------------------------------

SetFilterOkBtn::SetFilterOkBtn (double x, double y, double w, double h, SetFilterController* sfc_):
    TxtButton (x, y, w, h, nullptr, nullptr, "OK", 30),
    sfc (sfc_)
    {}

bool SetFilterOkBtn::onMousePress (MouseContext context) {
    if (state == BTN_DISABLED) return false;

    if (MouseOnWidget (context.position)) {
        state = BTN_PRESSED;
        sfc -> OkBtnPress();
    }
    return false;
}

SetFilterController::SetFilterController (FilterManager* fm, FilterI* filt, EventManager* ev_man_, Widget* parent_wid_):
    filter_man (fm),
    filter (filt),
    ev_man (ev_man_),
    parent_wid (parent_wid_),
    editboxes ()
    {
        Array<const char*> param_names = filter -> getParamNames();
        num_of_params = param_names.size;

        mw = new ModalWindow (150, 150, 400, 100 * (num_of_params) + 125, ev_man);
        mw -> registerSubWidget (new SetFilterOkBtn (100, 100 * (num_of_params) + 50, 200, 50, this));

        for (unsigned int i = 0; i < num_of_params; i++) {
            editboxes.push_back(new FloatNumEditBox (200, 50 + 100 * i, 180, 50, 30));
            mw -> registerSubWidget (editboxes.back());
            //mw -> AddSubWidget (new TxtWidget (20, 50 + 100 * i, 180, 50, param_names[i], GLOBAL_FONT, 30));
        }

        parent_wid -> registerSubWidget (mw);
    }

SetFilterController::~SetFilterController() {
    mw -> need_to_close = true;
}

void SetFilterController::OkBtnPress() {
    MyVector<double> params;
    for (int i = 0; i < num_of_params; i++) {
        params.PushBack(editboxes[i] -> TextToDouble());
    }
    filter -> setParams (Array(params));
    filter_man -> setFilter (filter);
    filter_man -> applyFilter();
    delete this;
}


void filter_btn_action (BtnArgs* filter_btn_args) {
    FilterManager* filter_man = ((FilterBtnArgs*)filter_btn_args) -> filter_man;
    FilterI*       filter     = ((FilterBtnArgs*)filter_btn_args) -> filter;
    EventManager*  ev_man     = ((FilterBtnArgs*)filter_btn_args) -> ev_man;
    Widget*        parent_wid = ((FilterBtnArgs*)filter_btn_args) -> parent_wid;

    new SetFilterController (filter_man, filter, ev_man, parent_wid);
}

FilterBtn::FilterBtn (double x, double y, double w, double h, const char *str, uint16_t char_size_,
                      FilterManager* fm, FilterI* filter_, EventManager* ev_man_, Widget* parent_wid_):
    TxtButton (x, y, w, h, filter_btn_action, &filter_btn_args, str, char_size_),
    filter_btn_args (fm, filter_, ev_man_, parent_wid_)
    {}