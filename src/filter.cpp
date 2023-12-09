#include "filter.h"

//--------------------------------------------------------------------------------------------------

void InvFilter::apply (RenderTargetI *data) {
    Texture* img = data -> getTexture();

    Color* pixels = img -> pixels;
    unsigned int w = img -> width;
    unsigned int h = img -> height;

    for (unsigned int x = 0; x < w; x++) {
        for (unsigned int y = 0; y < h; y++) {
            Color* pix = pixels + y * w + x;
            *pix = Color(~pix -> r, ~pix -> g, ~pix -> b, pix -> a);
        }
    }

    data -> drawTexture(Vec2(0, 0), Vec2(w, h), img);
    delete img;
}

Array<const char *> InvFilter::getParamNames() const {
    return Array<const char *> (NUM_OF_PARAMS, nullptr);
}

Array<double> InvFilter::getParams() const {
    return Array<double> (NUM_OF_PARAMS, nullptr);
}

void InvFilter::setParams(Array<double> new_params) {
    assert(new_params.size == NUM_OF_PARAMS);
}

//--------------------------------------------------------------------------------------------------

ClearFilter::ClearFilter () {
    params[0] = 255;
    params[1] = 255;
    params[2] = 255;
    for (size_t i = 0; i < NUM_OF_PARAMS; i++) p_names.PushBack(PARAM_NAMES[i]);
}

const char* const ClearFilter::PARAM_NAMES[] = {"red", "green", "blue"};

void ClearFilter::apply (RenderTargetI *data) {
    Texture* img = data -> getTexture();

    Color* pixels = img -> pixels;
    unsigned int w = img -> width;
    unsigned int h = img -> height;
    Color col(params[0], params[1], params[2]);

    for (unsigned int x = 0; x < w; x++) {
        for (unsigned int y = 0; y < h; y++) {
            Color* pix = pixels + y * w + x;
            *pix = col;
        }
    }

    data -> drawTexture(Vec2(0, 0), Vec2(w, h), img);
    delete img;
}

Array<const char *> ClearFilter::getParamNames() const {
    return Array<const char *> (p_names);
}

Array<double> ClearFilter::getParams() const {
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


FilterManager::FilterManager (ImageManager* img_man):
    filter (nullptr),
    image_manager (img_man) {}

void FilterManager::setFilter(FilterI *filt) {
    filter = filt;
}

void FilterManager::applyFilter() {
    assert(filter != nullptr);
    RenderTarget* rt = image_manager -> GetActive() -> GetCanvas() -> GetData();
    filter -> apply(rt);
    rt -> display();
}


//-----------------------------------------------------------------------------------------------------------


SetFilterController::SetFilterController (FilterManager* fm, FilterI* filt, size_t num_of_params_,
                                          EventManagerI* ev_man_, WidgetI* parent_wid_):
    MWController (ev_man_, parent_wid_, Vec2(150, 150), Vec2(400, 100 * (num_of_params_) + 125)),
    filter_man (fm),
    filter (filt),
    editboxes (),
    num_of_params (num_of_params_)
    {
        Array<const char*> param_names = filter -> getParamNames();

        ModalWindow* mw = GetMW();
        mw -> setSize (Vec2(400, 100 * (num_of_params) + 125));

        for (unsigned int i = 0; i < num_of_params; i++) {
            editboxes.push_back(new FloatNumEditBox (200, 50 + 100 * i, 180, 50, 30));
            mw -> registerSubWidget (editboxes.back());
            mw -> registerSubWidget (new TxtWidget (20, 50 + 100 * i, 180, 50, param_names.data[i], 30,
                                                    Color(0, 0, 0), Color(128, 128, 128)));
        }
        mw -> Show();
    }

void SetFilterController::OkBtnPress() {
    MyVector<double> params;
    for (size_t i = 0; i < num_of_params; i++) {
        params.PushBack(editboxes[i] -> TextToDouble());
    }
    filter -> setParams (Array(params));
    filter_man -> setFilter (filter);
    filter_man -> applyFilter();
    delete this;
}


void filter_btn_action (BtnArgs* filter_btn_args) {
    FilterManager* filter_man = ((FilterBtnArgs*)filter_btn_args) -> filter_man;
    FilterI*        filter     = ((FilterBtnArgs*)filter_btn_args) -> filter;
    EventManagerI*  ev_man     = ((FilterBtnArgs*)filter_btn_args) -> ev_man;
    WidgetI*        parent_wid = ((FilterBtnArgs*)filter_btn_args) -> parent_wid;

    if (filter -> getParamNames().size != 0)
        new SetFilterController (filter_man, filter, filter -> getParamNames().size,ev_man, parent_wid);
    else {
        filter_man -> setFilter (filter);
        filter_man -> applyFilter();
    }
}

FilterBtn::FilterBtn (double x, double y, double w, double h, const char *str, uint16_t char_size_,
                      FilterManager* fm, FilterI* filter_, EventManagerI* ev_man_, WidgetI* parent_wid_):
    TxtButton (x, y, w, h, filter_btn_action, &filter_btn_args, str, char_size_),
    filter_btn_args (fm, filter_, ev_man_, parent_wid_)
    {}