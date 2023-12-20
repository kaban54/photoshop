#include "bwplug.h"

const char* const BlackAndWhiteFilter::PARAM_NAMES[] = {"border"};

BlackAndWhiteFilter::BlackAndWhiteFilter() {
    params[0] = 128;
    for (size_t i = 0; i < NUM_OF_PARAMS; i++) p_names.PushBack(PARAM_NAMES[i]);
}

void BlackAndWhiteFilter::apply (RenderTargetI *data) {
    Texture* img = data -> getTexture();

    Color* pixels = img -> pixels;
    unsigned int w = img -> width;
    unsigned int h = img -> height;

    for (unsigned int x = 0; x < w; x++) {
        for (unsigned int y = 0; y < h; y++) {
            Color* pix = pixels + y * w + x;
            uint16_t sum = (uint16_t) pix -> r + (uint16_t) pix -> g + (uint16_t) pix -> b;
            *pix = sum >= params[0] * 3 ? Color(255, 255, 255) : Color(0, 0, 0);
        }
    }

    data -> drawTexture(Vec2(0, 0), Vec2(w, h), img);
    delete img;
}

Array<const char *> BlackAndWhiteFilter::getParamNames() const {
    return Array<const char *> (p_names);
}

Array<double> BlackAndWhiteFilter::getParams() const {
    return Array<double> (NUM_OF_PARAMS, params);
}

void BlackAndWhiteFilter::setParams(Array<double> new_params) {
    assert(new_params.size == NUM_OF_PARAMS);
    for (size_t i = 0; i < NUM_OF_PARAMS; i++) {
        params[i] = new_params.data[i];
        if      (params[i] <   0) params[i] = 0;
        else if (params[i] > 255) params[i] = 255;
    }
}


TestPlugin::TestPlugin() {
    id = 1ull << 54;
    name = "BlackWhite";
    type = InterfaceType::Filter;
    filter = new BlackAndWhiteFilter;
}

TestPlugin::~TestPlugin() {
    delete filter;
}


extern "C" plugin::Plugin* getInstance(plugin::App *app) {
    return new TestPlugin();
}