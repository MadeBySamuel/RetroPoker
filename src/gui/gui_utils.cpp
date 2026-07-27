#include <iostream> 
#include "game.hpp"
#include "gui_utils.hpp"


Gui_base::Gui_base(tgui::Gui& gui, sf::Music& music) : gui(gui), music(music), clickSound(clickBuffer) 
    {

        if (!clickBuffer.loadFromFile("assets/sound/button_press.ogg")) {
            std::cerr << "Failed to load click sound\n";
        }
        clickSound.setBuffer(clickBuffer);
    }
        



tgui::Panel::Ptr Gui_base::makeBar(){
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

tgui::CheckBox::Ptr Gui_base::makeCheckBox(tgui::Vector2f pos, tgui::Panel::Ptr centerPanel){

        GuiTheme color;
        
        auto checkBox = tgui::CheckBox::create();
        checkBox->setPosition(pos);
        checkBox->setSize({20, 20});
        checkBox->setText("");
        checkBox->setTextSize(20);
        centerPanel->add(checkBox);


        checkBox->getRenderer()->setTextColor(color.text);
        checkBox->getRenderer()->setTextColorHover(color.down);

        checkBox->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
        checkBox->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
        checkBox->getRenderer()->setBackgroundColorDisabled(tgui::Color::Transparent);

        checkBox->getRenderer()->setBorderColor(color.text);
        checkBox->getRenderer()->setBorderColorHover(color.down);
        checkBox->getRenderer()->setBorderColorFocused(color.text);

        checkBox->getRenderer()->setCheckColor(color.text);
        checkBox->getRenderer()->setCheckColorHover(color.down);


        return checkBox;
}


std::vector<tgui::Button::Ptr> Gui_base::makePopUpButtons(tgui::ChildWindow::Ptr quitLabel){

    GuiTheme color;

    std::vector<tgui::Button::Ptr> quitButtons;

    auto yesButton = tgui::Button::create("Yes");
    yesButton->setPosition({235, 60});

    auto noButton = tgui::Button::create("No");
    noButton->setPosition({45, 60});
    
    quitButtons.push_back(yesButton);
    quitButtons.push_back(noButton);

    for (auto button : quitButtons){

        button->setSize({130, 58});
        button->getRenderer()->setBackgroundColor(color.panel2);

        if (button == noButton){
            button->getRenderer()->setBackgroundColorDown(color.down);
            button->getRenderer()->setBackgroundColorHover(color.down);
        }
        else {
            button->getRenderer()->setBackgroundColorDown(color.yes_quit);
            button->getRenderer()->setBackgroundColorHover(color.yes_quit);
        }

        button->getRenderer()->setBackgroundColorDisabled(color.disabled);

        button->getRenderer()->setTextColor(color.text);
        button->getRenderer()->setTextColorHover(color.text);
        button->getRenderer()->setTextColorDown(color.bg);

        button->getRenderer()->setBorders(0);
        button->getRenderer()->setRoundedBorderRadius(10.f);
        
        quitLabel->add(button);

    }

    return quitButtons;

}


tgui::Button::Ptr Gui_base::makeButton (std::string title, tgui::Vector2f size, int8_t sizeText,tgui::Panel::Ptr panel){
    GuiTheme color;

    auto button = tgui::Button::create();
    button->setText(title);
    button->setSize(size);
    button->setTextSize(sizeText);

    button->getRenderer()->setBackgroundColor(color.panel2);
    button->getRenderer()->setBackgroundColorDownHover(color.down);
    button->getRenderer()->setBackgroundColorDown(color.down);
    button->getRenderer()->setBackgroundColorHover(color.down);

    button->getRenderer()->setTextColor(color.text);
    button->getRenderer()->setTextColorDisabled(color.text);
    button->getRenderer()->setTextColorHover(color.text);
    button->getRenderer()->setTextColorDown(sf::Color(0,0,0));
    button->getRenderer()->setBorders(0);

    panel->add(button);

    return button;

}

 void Gui_base::audio_settings(){

    float gap = 12.f;
    GuiTheme color;

    auto audioTitle = tgui::Label::create("Audio Settings");
    audioTitle->setTextSize(45);
    audioTitle->setPosition((Constants::loginPanelWidth - audioTitle->getSize().x) / 2.f, 20.f);
    audioTitle->getRenderer()->setTextColor(color.text);
    gameSettingsPanel->add(audioTitle);

    float audioH = audioTitle->getSize().y; 
    float audioW = audioTitle->getSize().x;

    auto musicTitle = tgui::Label::create("Music");
    musicTitle->setTextSize(24);
    musicTitle->setPosition(25.f, audioTitle->getSize().y + 2*gap);
    musicTitle->getRenderer()->setTextColor(color.text);
    gameSettingsPanel->add(musicTitle);


    float music_y = musicTitle->getSize().y;

    settingsMusicSlider = makeSlider({musicTitle->getSize().y + 6 * gap, audioTitle->getSize().y  + gap + 22}, {Constants::loginPanelWidth - 190, 10}, music.getVolume(), gameSettingsPanel);

    auto musicValue = tgui::Label::create("0");
    musicValue->setTextSize(24);
    musicValue->setPosition(gameSettingsPanel->getSize().x - 50.f, audioTitle->getSize().y + 2*gap);
    musicValue->getRenderer()->setTextColor(color.text);
    gameSettingsPanel->add(musicValue);

    settingsMusicSlider->onValueChange([this, musicValue](float value){
        musicValue->setText(std::to_string((int)value));
        music.setVolume(value);
        if (volumeSlider) volumeSlider->setValue(value);
    });

    auto effectsTitle = tgui::Label::create("Effects");
    effectsTitle->setTextSize(22);
    effectsTitle->setPosition(25.f, audioTitle->getSize().y + musicTitle->getSize().y * 2);
    effectsTitle->getRenderer()->setTextColor(color.text);
    gameSettingsPanel->add(effectsTitle);

    auto soundEffectCheckBox = tgui::CheckBox::create();

    auto effectsSlider = makeSlider({musicTitle->getSize().y + 6 * gap, audioTitle->getSize().y + musicTitle->getSize().y * 2 + 10}, {Constants::loginPanelWidth - 190, 10}, clickSound.getVolume(), gameSettingsPanel);


    auto effectsValue = tgui::Label::create("100");
    effectsValue->setTextSize(24);
    effectsValue->setPosition(gameSettingsPanel->getSize().x - 50.f, audioTitle->getSize().y + musicTitle->getSize().y * 2);
    effectsValue->getRenderer()->setTextColor(color.text);
    gameSettingsPanel->add(effectsValue);


    effectsSlider->onValueChange([this, effectsValue](float value){
        effectsValue->setText(std::to_string((int)value));
        clickSound.setVolume(value);
        clickSound.play();
    });

 }

void Gui_base::hide_game_settings(){
    hideOverlay();
    
    for(auto x : buttonsMainMenu){
        x->setVisible(true);
    }
    root->remove(gameSettingsPanel);
    settingsMusicSlider = nullptr;
    openedSettingsPanel = false;
}

void Gui_base::hide_game_menu(){
    hideOverlay();

    for(auto x : buttonsMainMenu){
            x->setVisible(true);
    }
    root->remove(gameMenuPanel);
    openedGameMenuPanel = false;
}



void Gui_base::game_settings(){
    openedSettingsPanel = true;
    float gap = 12.f;
    
    if(escapeMenu){
        escapeMenu->setVisible(false);
    }
    else {
        showOverlay();
    }
    GuiTheme color;

    gameSettingsPanel = tgui::Panel::create({Constants::loginPanelWidth, Constants::loginPanelHeight * 2});
    gameSettingsPanel->setPosition((getGuiWidth() - Constants::loginPanelWidth) / 2.f, (getGuiHeight() - Constants::loginPanelHeight * 2) / 2.f);
    gameSettingsPanel->getRenderer()->setBackgroundColor(color.centerpanel);
    gameSettingsPanel->getRenderer()->setRoundedBorderRadius(10.f);

    root->add(gameSettingsPanel);


    auto settingsTitle = tgui::Label::create("Settings");
    settingsTitle->setTextSize(45);
    settingsTitle->setPosition((Constants::loginPanelWidth - settingsTitle->getSize().x) / 2.f, 20.f);
    settingsTitle->getRenderer()->setTextColor(color.text);
    gameSettingsPanel->add(settingsTitle);

    float titleH = settingsTitle->getSize().y;
    float titleW = settingsTitle->getSize().x;
    
    auto videoButton = makeButton("Video", {480, 62}, 28, gameSettingsPanel);
    videoButton->setPosition((gameSettingsPanel->getSize().x - videoButton->getSize().x) / 2.f, 150);

    auto gameplayButton = makeButton("Gameplay", {480, 62}, 28, gameSettingsPanel);
    gameplayButton->setPosition((gameSettingsPanel->getSize().x - videoButton->getSize().x) / 2.f, 250);

    auto audioButton = makeButton("Audio", {480, 62}, 32, gameSettingsPanel);
    audioButton->setPosition((gameSettingsPanel->getSize().x - videoButton->getSize().x) / 2.f, 350);


    bindSoundOnClick(audioButton, [settingsTitle, videoButton, gameplayButton, audioButton, this]{
        settingsTitle->setVisible(false);
        videoButton->setVisible(false);
        gameplayButton->setVisible(false);
        audioButton->setVisible(false);
        audio_settings();  
    });



    auto backToMenu = makeBackToMenu({gap, Constants::loginPanelHeight * 2 - gap - 70}, {Constants::loginPanelWidth - 2 * gap, 70}, gameSettingsPanel); 


    bindSoundOnClick(backToMenu, [this]{
    if(escapeMenu){
        escapeMenu->setVisible(true);
    }
    else{
        hideOverlay();
    }
    for(auto x : buttonsMainMenu){
        x->setVisible(true);
    }
    root->remove(gameSettingsPanel);
    settingsMusicSlider = nullptr;
    });

}

// 420.f a 190.f 

tgui::ChildWindow::Ptr Gui_base::makePopUpPanel(const std::string text){

    GuiTheme color;
    showOverlay();
    auto popUpPanel = tgui::ChildWindow::create(text, tgui::ChildWindow::TitleButton::None);
    popUpPanel->setPosition({(getGuiWidth() - 420.f) / 2.f, (getGuiHeight() - 190.f) / 2.f});
    popUpPanel->setTextSize(20);
    popUpPanel->setTitleTextSize(17);
    popUpPanel->setSize({420, 190});
    popUpPanel->setPositionLocked(true);
    popUpPanel->getRenderer()->setBackgroundColor(color.bg);
    popUpPanel->getRenderer()->setBorderBelowTitleBar(1.00);

    root->add(popUpPanel);
    
    return popUpPanel;

}      


// tgui::Panel::Ptr


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


tgui::Button::Ptr Gui_base::makeBackToMenu(const tgui::Vector2f pos, const tgui::Vector2f size, tgui::Panel::Ptr panel){

    GuiTheme color;

    auto backToMenu = tgui::Button::create("Back to Main Menu");
    backToMenu->setTextSize(25);
    backToMenu->getRenderer()->setTextColor(color.text);
    backToMenu->setPosition(pos);
    backToMenu->setSize(size);

    backToMenu->getRenderer()->setBackgroundColor(color.panel2);
    backToMenu->getRenderer()->setBackgroundColorDownHover(color.down);
    backToMenu->getRenderer()->setBackgroundColorDown(color.down);
    backToMenu->getRenderer()->setBackgroundColorHover(color.down);
    backToMenu->getRenderer()->setTextColor(color.text);
    backToMenu->getRenderer()->setTextColorDisabled(color.text);
    backToMenu->getRenderer()->setTextColorHover(color.text);
    backToMenu->getRenderer()->setTextColorDown(sf::Color(0,0,0));
    backToMenu->getRenderer()->setBorders(0);

    panel->add(backToMenu);
    
    
    return backToMenu;

}



tgui::Slider::Ptr Gui_base::makeSlider(tgui::Vector2f pos, tgui::Vector2f size, float startValue, tgui::Panel::Ptr panel) {

        GuiTheme color;

        auto slider = tgui::Slider::create();
        slider->setPosition(pos);
        slider->setMinimum(0);
        slider->setMaximum(100);
        slider->setValue(startValue);
        slider->setSize(size);

        slider->getRenderer()->setThumbColor(color.text);
        slider->getRenderer()->setTrackColor(color.text);

        slider->getRenderer()->setThumbColorHover(color.down);
        slider->getRenderer()->setTrackColorHover(color.text);

        panel->add(slider);
        

        return slider;
}


void Gui_base::volume(tgui::Panel::Ptr topBar, tgui::Panel::Ptr root, GuiTheme color, sf::Music &music){
    auto volumeIcon = tgui::Picture::create(tgui::Texture("assets/images/volume/volume_muted.png", {}, {}, true));
        volumeIcon->setPosition({getGuiWidth() - 200.f, 12});
        volumeIcon->setSize({40, 31});
        topBar->add(volumeIcon);
        
        auto volumePanel = tgui::Panel::create({300, 90});
        volumePanel->setPosition({getGuiWidth() - 330.f, Constants::topBarHeight + 10.f});
        volumePanel->getRenderer()->setBackgroundColor(color.down);
        volumePanel->getRenderer()->setRoundedBorderRadius(20.0);
        root->add(volumePanel);
        volumePanel->setVisible(false);


        auto volumeLabel = tgui::Label::create("Sound");
        volumeLabel->setPosition({18, 12});
        volumeLabel->getRenderer()->setTextColor(color.text);
        volumeLabel->setTextSize(20);
        volumePanel->add(volumeLabel);

        volumeSlider = makeSlider({18, 52}, {264, 12}, music.getVolume(), volumePanel);

        volumeSlider->onValueChange([this, volumeIcon, volumePanel, &music](float value){
            if (volumePanel->isVisible()){
                if (value == 0) volumeIcon->getRenderer()->setTexture("assets/images/volume/volume_muted_click.png");
                else if (value > 0 && value < 30) volumeIcon->getRenderer()->setTexture("assets/images/volume/volume_first_click.png");
                else if (value >= 30 && value < 60) volumeIcon->getRenderer()->setTexture("assets/images/volume/volume_second_click.png");
                else volumeIcon->getRenderer()->setTexture("assets/images/volume/volume_full_click.png");
            }
            else {
                if(volumeSlider->getValue() == 0) volumeIcon->getRenderer()->setTexture("assets/images/volume/volume_muted.png");
                else if (volumeSlider->getValue() > 0 && volumeSlider->getValue() < 30) volumeIcon->getRenderer()->setTexture("assets/images/volume/volume_first.png");
                else if (volumeSlider->getValue() >= 30 && volumeSlider->getValue() < 60) volumeIcon->getRenderer()->setTexture("assets/images/volume/volume_second.png");
                else volumeIcon->getRenderer()->setTexture("assets/images/volume/volume_full.png");
            }
            musicVolume = value;
            music.setVolume(value);
            if (settingsMusicSlider) settingsMusicSlider->setValue(value);
        });
    
        volumeIcon->onClick([this, volumePanel,volumeIcon]{
        play_button();
        volumePanel->setVisible(!volumePanel->isVisible());

        if (volumePanel->isVisible()){
            if(volumeSlider->getValue() == 0) volumeIcon->getRenderer()->setTexture("assets/images/volume/volume_muted_click.png");
            else if (volumeSlider->getValue() > 0 && volumeSlider->getValue() < 30) volumeIcon->getRenderer()->setTexture("assets/images/volume/volume_first_click.png");
            else if (volumeSlider->getValue() >= 30 && volumeSlider->getValue() < 60) volumeIcon->getRenderer()->setTexture("assets/images/volume/volume_second_click.png");
            else volumeIcon->getRenderer()->setTexture("assets/images/volume/volume_full_click.png");
        }
        else {
            if(volumeSlider->getValue() == 0) volumeIcon->getRenderer()->setTexture("assets/images/volume/volume_muted.png");
            else if (volumeSlider->getValue() > 0 && volumeSlider->getValue() < 30) volumeIcon->getRenderer()->setTexture("assets/images/volume/volume_first.png");
            else if (volumeSlider->getValue() >= 30 && volumeSlider->getValue() < 60) volumeIcon->getRenderer()->setTexture("assets/images/volume/volume_second.png");
            else volumeIcon->getRenderer()->setTexture("assets/images/volume/volume_full.png");
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

