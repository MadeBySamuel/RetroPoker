#include <iostream> 
#include "game.hpp"
#include "gui_utils.hpp"



tgui::Panel::Ptr Gui_base::bar(tgui::Panel::Ptr root){
    GuiTheme color;

    auto topBar = tgui::Panel::create({getGuiWidth(), Constants::topBarHeight});
    topBar->setPosition({0, 0});
    topBar->getRenderer()-> setBackgroundColor(color.bar_color);
    root->add(topBar);

    time_label = tgui::Label::create(""); 
    time_label->setPosition({"94%", "17%"});
    time_label->setTextSize(30);
    time_label->getRenderer()->setTextColor(color.text);
    topBar->add(time_label);

    return topBar;
}

tgui::Panel::Ptr Gui_base::overlay() {
        auto overlay = tgui::Panel::create({getGuiWidth(), getGuiHeight()});
        overlay->setPosition({0, 0});
        overlay->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 220));
        return overlay;
}

void Gui_base::hideOverlay() {
        if (!root || !modalOverlay) {
            return;
        }
        root->remove(modalOverlay);
        modalOverlay = nullptr;
}

void Gui_base::showOverlay() {
        if (!root) {
            return;
        }
        hideOverlay();
        modalOverlay = overlay();
        root->add(modalOverlay);

}


void Gui_base::showEscapeMenu(){

    showOverlay();

    auto escapeMenu = tgui::Panel::create({Constants::signUpPanelWidth, Constants::signUpPanelHeight});
    escapeMenu->setPosition({(getGuiWidth() - Constants::signUpPanelHeight) / 2.f, (getGuiHeight() - Constants::signUpPanelHeight) / 2.f});
    escapeMenu->getRenderer()->setBackgroundColor(sf::Color(45, 35, 28, 230));
    escapeMenu->getRenderer()->setRoundedBorderRadius(20.0);

    gui.add(escapeMenu);

    GuiTheme color;

    std::vector<tgui::Button::Ptr> escapeButtons;

    auto backToGameButton = tgui::Button::create("Back to Game");
    backToGameButton->setPosition({70, 100});


    auto restartGameButton = tgui::Button::create("Restart game");
    restartGameButton->setPosition({70, 200});


    auto backToMenuButton = tgui::Button::create({"Back to Menu"});
    backToMenuButton->setPosition({70, 300});


    auto quitGameButton = tgui::Button::create({"Exit Game"});
    quitGameButton->setPosition({70, 400});

    escapeButtons.push_back(backToGameButton);
    escapeButtons.push_back(restartGameButton);
    escapeButtons.push_back(backToMenuButton);
    escapeButtons.push_back(quitGameButton);

    
    for (size_t i = 0; i < escapeButtons.size(); i++){
        
        escapeButtons[i]->setTextSize(28);
        escapeButtons[i]->setSize({500, 60});
        escapeButtons[i]->getRenderer()->setBackgroundColor(sf::Color(0,0,0,100));
        escapeButtons[i]->getRenderer()->setTextColor(color.text);
        escapeButtons[i]->getRenderer()->setTextColorHover(color.accent);
        escapeButtons[i]->getRenderer()->setBackgroundColorDownHover(color.bg);
        escapeButtons[i]->getRenderer()->setBackgroundColorDown(color.bg);
        escapeButtons[i]->getRenderer()->setBackgroundColorHover(color.bg);

        escapeMenu->add(escapeButtons[i]);
    }

}

std::string Gui_base::getCurrentTimeString() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_c);

    std::ostringstream oss;
    oss << std::put_time(local_time, "%H:%M:%S");
    return oss.str();
}


void Gui_base::update_time(){

        std::string currentTimeText = getCurrentTimeString();

        if (currentTimeText != last_time_text && time_label) {
            time_label->setText(currentTimeText);
            last_time_text = currentTimeText;
        }
}


void Gui_base::volume(tgui::Panel::Ptr topBar, tgui::Panel::Ptr root, GuiTheme color, sf::Music &music){
    auto volume_icon = tgui::Picture::create(tgui::Texture("assets/images/volume/volume_muted.png", {}, {}, true));
        volume_icon->setPosition({getGuiWidth() - 200.f, 12});
        volume_icon->setSize({40, 31});
        topBar->add(volume_icon);
        
        auto volume_panel = tgui::Panel::create({300, 90});
        volume_panel->setPosition({getGuiWidth() - 330.f, Constants::topBarHeight + 10.f});
        volume_panel->getRenderer()->setBackgroundColor(color.down);
        volume_panel->getRenderer()->setRoundedBorderRadius(20.0);
        root->add(volume_panel);
        volume_panel->setVisible(false);


        auto volume_label = tgui::Label::create("Sound");
        volume_label->setPosition({18, 12});
        volume_label->getRenderer()->setTextColor(color.text);
        volume_label->setTextSize(20);
        volume_panel->add(volume_label);


        auto volume_slider = tgui::Slider::create();
        volume_slider->setPosition({18, 52});
        volume_slider->setMinimum(0);
        volume_slider->setMaximum(100);
        volume_slider->setValue(0);
        volume_slider->setSize({264, 12});

        volume_slider->getRenderer()->setThumbColor(color.text);
        volume_slider->getRenderer()->setTrackColor(color.text);

        volume_slider->getRenderer()->setThumbColorHover(color.down);
        volume_slider->getRenderer()->setTrackColorHover(color.text);

        volume_panel->add(volume_slider);


        volume_slider->onValueChange([volume_icon, &music](float value){
            if (value == 0) volume_icon->getRenderer()->setTexture("assets/images/volume/volume_muted_click.png");
            else if (value > 0 && value < 30) volume_icon->getRenderer()->setTexture("assets/images/volume/volume_first_click.png");
            else if (value >= 30 && value < 60) volume_icon->getRenderer()->setTexture("assets/images/volume/volume_second_click.png");
            else volume_icon->getRenderer()->setTexture("assets/images/volume/volume_full_click.png");
            music.setVolume(value);
        });
    


        volume_icon->onClick([this, volume_panel,volume_icon, volume_slider]{
        play_button();
        volume_panel->setVisible(!volume_panel->isVisible());

        if (volume_panel->isVisible()){
            if(volume_slider->getValue() == 0) volume_icon->getRenderer()->setTexture("assets/images/volume/volume_muted_click.png");
            else if (volume_slider->getValue() > 0 && volume_slider->getValue() < 30) volume_icon->getRenderer()->setTexture("assets/images/volume/volume_first_click.png");
            else if (volume_slider->getValue() >= 30 && volume_slider->getValue() < 60) volume_icon->getRenderer()->setTexture("assets/images/volume/volume_second_click.png");
            else volume_icon->getRenderer()->setTexture("assets/images/volume/volume_full_click.png");
        }
        else {
            if(volume_slider->getValue() == 0) volume_icon->getRenderer()->setTexture("assets/images/volume/volume_muted.png");
            else if (volume_slider->getValue() > 0 && volume_slider->getValue() < 30) volume_icon->getRenderer()->setTexture("assets/images/volume/volume_first.png");
            else if (volume_slider->getValue() >= 30 && volume_slider->getValue() < 60) volume_icon->getRenderer()->setTexture("assets/images/volume/volume_second.png");
            else volume_icon->getRenderer()->setTexture("assets/images/volume/volume_full.png");
        }
        });

}


std::vector<Card> gui_utils::game_cards (){
    std::vector<Card> cards;
    std::vector<tgui::Picture::Ptr> cards_images;
    std::vector<std::string> suits = {"♥️", "♦️", "♣️", "♠️"};

    cards_images.emplace_back(tgui::Picture::create("assets/cards/heart/hearts_2.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/heart/hearts_3.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/heart/hearts_4.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/heart/hearts_5.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/heart/hearts_6.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/heart/hearts_7.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/heart/hearts_8.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/heart/hearts_9.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/heart/hearts_10.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/heart/hearts_A.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/heart/hearts_J.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/heart/hearts_K.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/heart/hearts_Q.png")); 


    cards_images.emplace_back(tgui::Picture::create("assets/cards/diamond/diamonds_2.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/diamond/diamonds_3.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/diamond/diamonds_4.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/diamond/diamonds_5.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/diamond/diamonds_6.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/diamond/diamonds_7.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/diamond/diamonds_8.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/diamond/diamonds_9.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/diamond/diamonds_10.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/diamond/diamonds_A.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/diamond/diamonds_J.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/diamond/diamonds_K.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/diamond/diamonds_Q.png")); 

    cards_images.emplace_back(tgui::Picture::create("assets/cards/club/clubs_2.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/club/clubs_3.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/club/clubs_4.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/club/clubs_5.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/club/clubs_6.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/club/clubs_7.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/club/clubs_8.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/club/clubs_9.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/club/clubs_10.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/club/clubs_A.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/club/clubs_J.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/club/clubs_K.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/club/clubs_Q.png")); 

    cards_images.emplace_back(tgui::Picture::create("assets/cards/spade/spades_2.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/spade/spades_3.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/spade/spades_4.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/spade/spades_5.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/spade/spades_6.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/spade/spades_7.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/spade/spades_8.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/spade/spades_9.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/spade/spades_10.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/spade/spades_A.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/spade/spades_J.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/spade/spades_K.png")); 
    cards_images.emplace_back(tgui::Picture::create("assets/cards/spade/spades_Q.png")); 

    size_t image_iterator = 0;

    
    for (auto s : suits) {
        for (int r = 2; r <= 14; r++) {
            cards.push_back({r, s, cards_images.at(image_iterator)});
            ++image_iterator;
        }
    }
    return cards;

}

