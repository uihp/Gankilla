#pragma once
#ifndef ASTRA_CORE_SRC_ASTRA_UI_SCHEDULER_H_
#define ASTRA_CORE_SRC_ASTRA_UI_SCHEDULER_H_

#include "items/camera.h"
#include "items/menu.h"
#include "items/selector.h"

namespace astra {
  class Launcher {
    private:
      Selector* selector;
      Camera* camera;
    public:
      Menu* currentMenu;
      Widget* currentWidget = nullptr;
      void notice(std::string _info, uint16_t _time);
      void init(Menu* _rootPage);
      bool open();
      void close();
      void update();
      Camera* getCamera() { return camera; }
      Selector* getSelector() { return selector; }
  };
}

#endif //ASTRA_CORE_SRC_ASTRA_UI_SCHEDULER_H_
