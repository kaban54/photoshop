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