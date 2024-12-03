//
// Created by croshaw on 12/2/24.
//

#ifndef CALL_UI_CALL_APP_HPP
#define CALL_UI_CALL_APP_HPP

#include "call_frame.hpp"

namespace cui {
    class call_app : public wxApp {
    public:
        virtual bool OnInit() override {
            auto *frame = new call_frame();
            frame->Show();
            return true;
        }
    };
}
#endif //CALL_UI_CALL_APP_HPP
