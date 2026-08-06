#pragma once

#include <TGUI/TGUI.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <vector>



class Gui_base{
    protected:
        tgui::Gui& gui;
        sf::Music& music; 
        tgui::Panel::Ptr root;
        tgui::Panel::Ptr topBar;
        std::vector<tgui::Button::Ptr> buttons_sound;
        sf::SoundBuffer clickBuffer;  
        sf::Sound clickSound;  
        tgui::Label::Ptr time_label;
        std::string last_time_text;
        tgui::Panel::Ptr modalOverlay;
        tgui::CheckBox::Ptr checkBox;
        tgui::Slider::Ptr volumeSlider;
        tgui::Slider::Ptr settingsMusicSlider;
        tgui::Panel::Ptr gameSettingsPanel;
        tgui::Panel::Ptr gameMenuPanel;
        tgui::Panel::Ptr escapeMenu;
        std::vector<tgui::Button::Ptr> buttonsMainMenu;
        tgui::Panel::Ptr audioSettingsPanel;
        tgui::Panel::Ptr forgotPasswordPanel;
        tgui::Panel::Ptr centerPanel;


        
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
            tgui::Color centerpanel{45, 35, 28, 230};
        };

        float musicVolume = 0.f;
        int8_t borderRadius = 20.f;
        


    public: 
        Gui_base(tgui::Gui& gui, sf::Music& music);
        GuiTheme color;
        bool openedSettingsPanel = false;
        bool openedGameMenuPanel = false;
        
    void play_button(){
        clickSound.play();
    }

    tgui::Panel::Ptr makePanel(tgui::Vector2f size, tgui::Color backgroundColor);

    tgui::CheckBox::Ptr makeCheckBox(tgui::Vector2f pos, tgui::Panel::Ptr centerPanel);
    tgui::EditBox::Ptr makeEditBox(const std::string title, tgui::Vector2f pos, tgui::Panel::Ptr panel);

    tgui::Slider::Ptr makeSlider(tgui::Vector2f pos, tgui::Vector2f size, float startValue, tgui::Panel::Ptr panel);
    tgui::Button::Ptr makeBackToMenu(tgui::Vector2f pos, tgui::Vector2f size, tgui::Panel::Ptr panel);
    void game_settings();
    tgui::Button::Ptr makeButton (const std::string title, tgui::Vector2f pos, const tgui::Vector2f size, const int8_t sizeText, tgui::Panel::Ptr panel);
    void audio_settings();
    
    tgui::Panel::Ptr overlay();

    void hide_game_settings();
    void hide_game_menu();
    void showOverlay();
    void hideOverlay();
    tgui::Button::Ptr makePopUpButton(const std::string title, tgui::Vector2f pos, const tgui::Vector2f size, const int8_t sizeTitle, tgui::ChildWindow::Ptr quitPanel);
    tgui::ChildWindow::Ptr makePopUpPanel(const std::string title, tgui::Vector2f size, const int8_t sizeTitleText, tgui::Color titleBarColor, tgui::Color titleTextColor);


    template<typename WidgetPtr, typename Callback>
    void bindSoundOnClick(const WidgetPtr& widget, Callback&& callback, bool soundOn = true) {
        widget->onClick([this, cb = std::forward<Callback>(callback), soundOn]() mutable {
            if(soundOn) {
                play_button();
            }
            cb();
        });
    }


    const sf::Vector2u getWindowSize() const{
        return gui.getWindow()->getSize();
    }
    
    float getGuiWidth() const {
            return static_cast<float>(gui.getWindow()->getSize().x);
    }

    float getGuiHeight() const {
            return static_cast<float>(gui.getWindow()->getSize().y);
    }

    void volume(tgui::Panel::Ptr topBar, tgui::Panel::Ptr root, sf::Music &music);

    tgui::Panel::Ptr makeBar();  


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