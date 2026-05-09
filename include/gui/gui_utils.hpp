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
    tgui::Color bar_color{28, 22, 18, 200};
};


class Gui_base{
    protected:
        tgui::Gui& gui;
        std::vector<tgui::Button::Ptr> buttons_sound;
        sf::SoundBuffer clickBuffer;  
        sf::Sound clickSound;         
        tgui::Label::Ptr time_label;
        std::string last_time_text;
        




    public: 
    Gui_base(tgui::Gui& gui) : gui(gui), clickSound(clickBuffer) {
        if (!clickBuffer.loadFromFile("assets/sound/button_press.ogg")) {
            std::cerr << "Failed to load click sound\n";
        }
        clickSound.setBuffer(clickBuffer);
        }
        

    void play_button(){
        clickSound.play();
    }

    float getGuiWidth() const{
            return static_cast<float>(gui.getWindow()->getSize().x);
    }

    float getGuiHeight() const{
            return static_cast<float>(gui.getWindow()->getSize().y);
    }



    void volume(tgui::Panel::Ptr topBar, tgui::Panel::Ptr root, GuiTheme color, sf::Music &music);

    tgui::Panel::Ptr bar(tgui::Panel::Ptr root);  


    std::string getCurrentTimeString();
    void update_time();


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











std::vector<Card> game_cards ();

};