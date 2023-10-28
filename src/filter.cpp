#include "filter.h"

void TestFilter::Apply (RenderTarget &rt) const {
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


FilterManager::FilterManager ():
    current (nullptr),
    active (false)
    {}

void FilterManager::Apply(RenderTarget& rt) {
    assert(current != nullptr);
    current -> Apply (rt);
    active = false;
}


void filter_btn_action (void* filter_btn_args) {
    FilterManager* filter_man = ((FilterBtnArgs*)filter_btn_args) -> filter_man;
    Filter*        filter     = ((FilterBtnArgs*)filter_btn_args) -> filter;
    filter_man -> SetFilter (filter);
    filter_man->Activate();
}

FilterBtn::FilterBtn (double x, double y, double w, double h, const Text& txt_, FilterManager* fm, Filter* filter_):
    TxtButton (x, y, w, h, filter_btn_action, &filter_btn_args, txt_),
    filter_btn_args {fm, filter_}
    {}