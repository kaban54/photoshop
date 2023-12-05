#include "test_plugin.h"

void BlackAndWhiteFilter::apply (RenderTargetI *data) {
    Texture* img = data -> getTexture();

    Color* pixels = img -> pixels;
    unsigned int w = img -> width;
    unsigned int h = img -> height;

    for (unsigned int x = 0; x < w; x++) {
        for (unsigned int y = 0; y < h; y++) {
            Color* pix = pixels + y * w + x;
            uint8_t br = pix -> r / 10 * 3 + pix -> g / 10 * 6 + pix -> b / 10;
            *pix = Color(br, br, br, pix -> a);
        }
    }

    data -> drawTexture(Vec2(0, 0), Vec2(w, h), img);
    delete img;
}

Array<const char *> BlackAndWhiteFilter::getParamNames() {
    return Array<const char *> (NUM_OF_PARAMS, nullptr);
}

Array<double> BlackAndWhiteFilter::getParams() {
    return Array<double> (NUM_OF_PARAMS, nullptr);
}

void BlackAndWhiteFilter::setParams(Array<double> new_params) {
    assert(new_params.size == NUM_OF_PARAMS);
}


TestPlugin::TestPlugin() {
    id = 1ull << 54;
    name = "plugin228";
    type = InterfaceType::Filter;
    filter = new BlackAndWhiteFilter;
}

TestPlugin::~TestPlugin() {
    delete filter;
}


extern "C" plugin::Plugin* getInstance(plugin::App *app) {
    return new TestPlugin();
}