#pragma once

#include <TGUI/TGUI.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>



inline std::string getCurrentTimeString() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_c);

    std::ostringstream oss;
    oss << std::put_time(local_time, "%H:%M:%S");
    return oss.str();
}



inline void update_time(tgui::Label::Ptr time_label,std::string last_time_text, sf::Color text, tgui::Panel::Ptr topBar ){

        if (!time_label) {
                return;
        }

        std::string currentTimeText = getCurrentTimeString();

        if (currentTimeText != last_time_text && time_label) {
            time_label->setText(currentTimeText);
            last_time_text = currentTimeText;
        }
    }

