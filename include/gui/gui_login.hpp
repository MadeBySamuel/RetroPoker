#pragma once

#include <functional>
#include <string>

#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <sqlite3.h>

#include "gui_utils.hpp"

class Login : public Gui_base {

    private:
        sf::Music& music;
        tgui::Label::Ptr time_label;
        std::string last_time_text;
        sqlite3* db = nullptr;

    public:
        Login(tgui::Gui& gui, sf::Music& music);
        std::function<void(std::string)> onLogin;
        std::string logged_username;
        void login_screen();
};
