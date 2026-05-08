#pragma once

#include <TGUI/TGUI.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <vector>






struct GuiTheme {
    tgui::Color bg{28, 24, 20};        
    tgui::Color panel{8, 46, 38};    
    tgui::Color panel2{72, 58, 48};    
    tgui::Color accent{212, 160, 23};  
    tgui::Color text{244, 230, 200};   
    tgui::Color hover{201, 106, 43};   
    tgui::Color border{120, 92, 60};   
    tgui::Color down{160, 110, 25};    
    tgui::Color disabled{80, 70, 60};  
    tgui::Color yes_quit{133, 22, 22};
};

struct Card{
    int rank;
    std::string name;
    tgui::Picture::Ptr image;

    std::string write_name(){
        return name + " " + rank_to_string(rank);
    }

    std::string rank_to_string(int rank) {
        if (rank == 11) return "J";
        if (rank == 12) return "Q";
        if (rank == 13) return "K";
        if (rank == 14) return "A";
        return std::to_string(rank);
    }
};

namespace gui_utils {

void volume(float guiWidth, float topBarHeight, tgui::Panel::Ptr topBar, tgui::Panel::Ptr root, GuiTheme color, sf::Music &music);

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


std::vector<Card> game_cards ();

};