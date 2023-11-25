#include "filter.h"

//--------------------------------------------------------------------------------------------------

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

//--------------------------------------------------------------------------------------------------

ClearFilter::ClearFilter () {
    params[0] = 255;
    params[1] = 255;
    params[2] = 255;
}

const char* const ClearFilter::PARAM_NAMES[] = {"red", "green", "blue"};

void ClearFilter::apply (RenderTargetI *data) {
    Texture* img = data -> getTexture();

    Color* pixels = img -> pixels;
    unsigned int w = img -> width;
    unsigned int h = img -> height;
    Color col(params[0], params[1], params[2]);

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            Color* pix = pixels + y * w + x;
            *pix = col;
        }
    }

    data -> drawTexture(Vec2(0, 0), Vec2(w, h), img);
    delete img;
}

Array<const char *> ClearFilter::getParamNames() {
    MyVector<const char *> ret (NUM_OF_PARAMS);
    for (size_t i = 0; i < NUM_OF_PARAMS; i++) ret[i] = PARAM_NAMES[i];
    return Array<const char *> (ret);
}

Array<double> ClearFilter::getParams() {
    return Array<double> (NUM_OF_PARAMS, params);
}

void ClearFilter::setParams(Array<double> new_params) {
    assert(new_params.size == NUM_OF_PARAMS);
    for (size_t i = 0; i < NUM_OF_PARAMS; i++) {
        params[i] = new_params.data[i];
        if      (params[i] <   0) params[i] = 0;
        else if (params[i] > 255) params[i] = 255;
    }
}

//--------------------------------------------------------------------------------------------------


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

SetFilterController::SetFilterController (FilterManagerI* fm, FilterI* filt, EventManagerI* ev_man_, WidgetI* parent_wid_):
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
    FilterManagerI* filter_man = ((FilterBtnArgs*)filter_btn_args) -> filter_man;
    FilterI*        filter     = ((FilterBtnArgs*)filter_btn_args) -> filter;
    EventManagerI*  ev_man     = ((FilterBtnArgs*)filter_btn_args) -> ev_man;
    WidgetI*        parent_wid = ((FilterBtnArgs*)filter_btn_args) -> parent_wid;

    if (filter -> getParamNames().size != 0)
        new SetFilterController (filter_man, filter, ev_man, parent_wid);
    else {
        filter_man -> setFilter (filter);
        filter_man -> applyFilter();
    }
}

FilterBtn::FilterBtn (double x, double y, double w, double h, const char *str, uint16_t char_size_,
                      FilterManagerI* fm, FilterI* filter_, EventManagerI* ev_man_, WidgetI* parent_wid_):
    TxtButton (x, y, w, h, filter_btn_action, &filter_btn_args, str, char_size_),
    filter_btn_args (fm, filter_, ev_man_, parent_wid_)
    {}