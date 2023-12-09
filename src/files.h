#ifndef FILES_H
#define FILES_H

#include "controller.h"
#include "canvas.h"
#include "editbox.h"

const char* const IMAGE_FOLDER_NAME = "images/";

class SaveFileController : public MWController {
    CanvasWindow* win;
    EditBox* edit_box;

    public:

    explicit SaveFileController(CanvasWindow* win_, EventManagerI* event_man, WidgetI* parent);

    virtual ~SaveFileController() = default;

    virtual void OkBtnPress() override;
};

struct SaveBtnArgs : public BtnArgs {
    ImageManager* image_man;
    EventManagerI* ev_man;
    WidgetI* parent_wid;
    explicit SaveBtnArgs (ImageManager* image_man_, EventManagerI* ev_man_, WidgetI* parent_wid_):
        image_man (image_man_),
        ev_man (ev_man_),
        parent_wid (parent_wid_) {}
};

class SaveBtn : public TxtButton {
    SaveBtnArgs btn_args;

    public:

    explicit SaveBtn (double x, double y, double w, double h, uint16_t char_size_,
                      ImageManager* image_man, EventManagerI* ev_man_, WidgetI* parent_wid_);
};

//-----------------------------------------------------------------------------------------------------------

class OpenFileController : public MWController {
    ImageManager* image_man;
    ToolManager* tool_man;
    EditBox* edit_box;

    public:

    explicit OpenFileController(ImageManager* image_man_, ToolManager* tool_man_, EventManagerI* event_man, WidgetI* parent);

    virtual ~OpenFileController() = default;

    virtual void OkBtnPress() override;
};

struct OpenBtnArgs : public BtnArgs {
    ImageManager* image_man;
    ToolManager* tool_man;
    EventManagerI* ev_man;
    WidgetI* parent_wid;
    explicit OpenBtnArgs (ImageManager* image_man_, ToolManager* tool_man_, EventManagerI* ev_man_, WidgetI* parent_wid_):
        image_man (image_man_),
        tool_man(tool_man_),
        ev_man (ev_man_),
        parent_wid (parent_wid_) {}
};

class OpenBtn : public TxtButton {
    OpenBtnArgs btn_args;

    public:

        explicit OpenBtn (double x, double y, double w, double h, uint16_t char_size_, ImageManager* image_man,
                        ToolManager* tool_man_, EventManagerI* ev_man_, WidgetI* parent_wid_);
};

#endif