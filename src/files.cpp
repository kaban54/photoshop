#include "files.h"

SaveFileController::SaveFileController(CanvasWindow* win_, EventManagerI* event_man, WidgetI* parent):
    MWController(event_man, parent, Vec2(200, 200), Vec2(600, 300)),
    win (win_)
    {
        ModalWindow* mw = GetMW();
        mw -> SetName("Save file");
        edit_box = new EditBox (50, 90, 500, 50, 30);
        mw -> registerSubWidget(edit_box);
        edit_box -> SetText(win -> GetName());
    }

void SaveFileController::OkBtnPress() {
    if (edit_box -> txt.size() == 0) return;

    char full_name[128] = "";
    sprintf(full_name, "%s%s", IMAGE_FOLDER_NAME, edit_box -> txt.c_str());
    
    Texture* texture = win -> GetCanvas() -> GetData() -> getTexture();
    sf::Image sfimg;
    sfimg.create(texture -> width, texture -> height, (uint8_t*)texture -> pixels);
    sfimg.saveToFile(full_name);

    delete texture;
    delete this;
}



void save_btn_action (BtnArgs* btn_args) {
    ImageManager*  image_man  = ((SaveBtnArgs*)btn_args) -> image_man;
    EventManagerI* ev_man     = ((SaveBtnArgs*)btn_args) -> ev_man;
    WidgetI*       parent_wid = ((SaveBtnArgs*)btn_args) -> parent_wid;

    if (image_man -> GetSize() != 0)
        new SaveFileController (image_man -> GetActive(), ev_man, parent_wid);
}

SaveBtn::SaveBtn (double x, double y, double w, double h, uint16_t char_size_,
                  ImageManager* image_man, EventManagerI* ev_man_, WidgetI* parent_wid_):
    TxtButton (x, y, w, h, save_btn_action, &btn_args, "Save", char_size_),
    btn_args (image_man, ev_man_, parent_wid_)
    {}