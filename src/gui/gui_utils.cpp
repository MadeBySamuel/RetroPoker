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

    auto topBar = tgui::Panel::create({getGuiWidth(), Constants::topBarHeight});
    topBar->setPosition({0, 0});
    topBar->getRenderer()->setBackgroundColor(color.bar_color);
    root->add(topBar);

    time_label = tgui::Label::create(""); 
    time_label->setPosition({"94%", "17%"});
    time_label->setTextSize(30);
    time_label->getRenderer()->setTextColor(color.text);
    topBar->add(time_label);

    return topBar;
}



tgui::Panel::Ptr Gui_base::makePanel(tgui::Vector2f size, tgui::Color backgroundColor){
   
    auto panel = tgui::Panel::create(size);
    panel->setPosition({(getGuiWidth() - size.x) / 2.f, (getGuiHeight() - size.y) / 2.f});
    panel->getRenderer()->setBackgroundColor(backgroundColor);
    panel->getRenderer()->setRoundedBorderRadius(20.0);
    root->add(panel);

    return panel;
}

tgui::CheckBox::Ptr Gui_base::makeCheckBox(tgui::Vector2f pos, tgui::Panel::Ptr centerPanel){
        
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


tgui::EditBox::Ptr Gui_base::makeEditBox(const std::string title, tgui::Vector2f pos, tgui::Panel::Ptr panel){
    
    auto editBox = tgui::EditBox::create();
    editBox->setTextSize(20);
    editBox->setSize({450, 42});
    editBox->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    editBox->getRenderer()->setBorders(0);
    editBox->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
    editBox->getRenderer()->setTextColor(color.text);
    editBox->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
    editBox->getRenderer()->setBorderColor(sf::Color(0,0,0));
    editBox->getRenderer()->setDefaultTextColor(sf::Color(125, 125, 125));
    editBox->setDefaultText(title);
    editBox->setPosition(pos);

    panel->add(editBox);

    return editBox;
}

tgui::ChildWindow::Ptr Gui_base::makePopUpPanel(const std::string title, tgui::Vector2f size, const int8_t sizeTitleText, tgui::Color titleBarColor, tgui::Color titleTextColor){

    showOverlay();
    auto popUpPanel = tgui::ChildWindow::create(title, tgui::ChildWindow::TitleButton::None);
    popUpPanel->setPosition({(getGuiWidth() - size.x) / 2.f, (getGuiHeight() - size.y) / 2.f});
    popUpPanel->getRenderer()->setTitleBarHeight(50.f);
    popUpPanel->setTitleTextSize(sizeTitleText);
    popUpPanel->setSize(size);
    popUpPanel->setPositionLocked(true);
    popUpPanel->getRenderer()->setTitleBarColor(titleBarColor);
    popUpPanel->getRenderer()->setTitleColor(titleTextColor);
    popUpPanel->getRenderer()->setBackgroundColor(color.bg);
    popUpPanel->getRenderer()->setBorderBelowTitleBar(1.00);

    root->add(popUpPanel);
    
    return popUpPanel;

}      



tgui::Button::Ptr Gui_base::makePopUpButton(const std::string title, tgui::Vector2f pos, const tgui::Vector2f size, const int8_t sizeTitle, tgui::ChildWindow::Ptr quitPanel){

    auto button = tgui::Button::create(title);
    button->setPosition(pos);
    button->setSize(size);
    button->setTextSize(sizeTitle);

    button->getRenderer()->setBackgroundColor(color.panel2);
    button->getRenderer()->setBackgroundColorDown(color.down);
    button->getRenderer()->setBackgroundColorHover(color.down);

    if (title == "Yes"){
        button->getRenderer()->setBackgroundColorDown(color.yes_quit);
        button->getRenderer()->setBackgroundColorHover(color.yes_quit);
    }

    button->getRenderer()->setBackgroundColorDisabled(color.disabled);
    button->getRenderer()->setTextColor(color.text);
    button->getRenderer()->setTextColorHover(color.text);
    button->getRenderer()->setTextColorDown(color.bg);
    button->getRenderer()->setBorders(0);
    button->getRenderer()->setRoundedBorderRadius(10.f);
    quitPanel->add(button);

    return button;

}


tgui::Button::Ptr Gui_base::makeButton (const std::string title, tgui::Vector2f pos, const tgui::Vector2f size, const int8_t sizeText,tgui::Panel::Ptr panel){

    auto button = tgui::Button::create();


    button->setText(title);
    button->setPosition(pos);
    button->setSize(size);
    button->setTextSize(sizeText);

    button->getRenderer()->setTextColor(color.text);
    button->getRenderer()->setTextColorHover(color.down);
    button->getRenderer()->setTextColorDown(color.down);
    button->getRenderer()->setTextColorFocused(color.text);
    button->getRenderer()->setTextColorDownFocused(color.down);

    button->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    button->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
    button->getRenderer()->setBackgroundColorDown(tgui::Color::Transparent);
    button->getRenderer()->setBackgroundColorFocused(tgui::Color::Transparent);
    button->getRenderer()->setBackgroundColorDownFocused(tgui::Color::Transparent);
    button->getRenderer()->setBackgroundColorDisabled(tgui::Color::Transparent);

    button->getRenderer()->setBorderColor(color.text);
    button->getRenderer()->setBorderColorHover(color.down);
    button->getRenderer()->setBorderColorDown(color.down);
    button->getRenderer()->setBorderColorDownHover(color.down);
    button->getRenderer()->setBorderColorFocused(color.text);
    button->getRenderer()->setBorderColorDownFocused(color.down);

    if(panel == topBar){
        button->getRenderer()->setBorders(0);
    }

    if(panel == gameSettingsPanel || panel == gameMenuPanel || title == "Logout"){
        button->getRenderer()->setBackgroundColorFocused(color.panel2);
        button->getRenderer()->setTextColorDownFocused(sf::Color(0,0,0));
        button->getRenderer()->setBackgroundColor(color.panel2);
        button->getRenderer()->setBackgroundColorDownHover(color.down);
        button->getRenderer()->setBackgroundColorDown(color.down);
        button->getRenderer()->setBackgroundColorHover(color.down);
        button->getRenderer()->setBackgroundColorDisabled(color.down);
        button->getRenderer()->setBackgroundColorDownFocused(color.down);
        button->getRenderer()->setTextColorDisabled(color.text);
        button->getRenderer()->setTextColorHover(color.text);
        button->getRenderer()->setTextColorDown(color.text);
        button->getRenderer()->setBorders(0);
    }
    

    if(panel == centerPanel && title == "LOGIN"){
        button->getRenderer()->setBorders(0);
        button->getRenderer()->setBackgroundColor(sf::Color(0,0,0,100));
        button->getRenderer()->setTextColorHover(color.down);
        button->getRenderer()->setTextColorDown(color.down);
        button->getRenderer()->setBackgroundColorDownHover(color.bg);
        button->getRenderer()->setBackgroundColorDown(color.bg);
        button->getRenderer()->setBackgroundColorHover(color.bg);
        button->getRenderer()->setBackgroundColorDown(sf::Color(0,0,0,100));
        button->getRenderer()->setBackgroundColorFocused(sf::Color(0,0,0,100));
        button->getRenderer()->setBackgroundColorDownFocused(sf::Color(0,0,0,100));
        button->getRenderer()->setBackgroundColorDisabled(sf::Color(0,0,0,100));

    }
    
    panel->add(button);

    return button;

}

 void Gui_base::audio_settings(){

    float gap = 12.f;

    audioSettingsPanel = tgui::Panel::create({Constants::loginPanelWidth, Constants::loginPanelHeight * 2});
    audioSettingsPanel->setPosition((getGuiWidth() - Constants::loginPanelWidth) / 2.f, (getGuiHeight() - Constants::loginPanelHeight * 2) / 2.f);
    audioSettingsPanel->getRenderer()->setBackgroundColor(color.centerpanel);
    audioSettingsPanel->getRenderer()->setRoundedBorderRadius(10.f);

    root->add(audioSettingsPanel);


    auto audioTitle = tgui::Label::create("Audio Settings");
    audioTitle->setTextSize(45);
    audioTitle->setPosition((Constants::loginPanelWidth - audioTitle->getSize().x) / 2.f, 20.f);
    audioTitle->getRenderer()->setTextColor(color.text);
    audioSettingsPanel->add(audioTitle);

    float audioH = audioTitle->getSize().y; 
    float audioW = audioTitle->getSize().x;

    auto musicTitle = tgui::Label::create("Music");
    musicTitle->setTextSize(24);
    musicTitle->setPosition(25.f, audioTitle->getSize().y + 2*gap);
    musicTitle->getRenderer()->setTextColor(color.text);
    audioSettingsPanel->add(musicTitle);


    float music_y = musicTitle->getSize().y;

    settingsMusicSlider = makeSlider({musicTitle->getSize().y + 6 * gap, audioTitle->getSize().y  + gap + 22}, {Constants::loginPanelWidth - 190, 10}, music.getVolume(), audioSettingsPanel);

    auto musicValue = tgui::Label::create("0");
    musicValue->setTextSize(24);
    musicValue->setPosition(gameSettingsPanel->getSize().x - 50.f, audioTitle->getSize().y + 2*gap);
    musicValue->getRenderer()->setTextColor(color.text);
    audioSettingsPanel->add(musicValue);

    settingsMusicSlider->onValueChange([this, musicValue](float value){
        musicValue->setText(std::to_string((int)value));
        music.setVolume(value);
        if (volumeSlider) volumeSlider->setValue(value);
    });

    auto effectsTitle = tgui::Label::create("Effects");
    effectsTitle->setTextSize(22);
    effectsTitle->setPosition(25.f, audioTitle->getSize().y + musicTitle->getSize().y * 2);
    effectsTitle->getRenderer()->setTextColor(color.text);
    audioSettingsPanel->add(effectsTitle);

    auto effectsSlider = makeSlider({musicTitle->getSize().y + 6 * gap, audioTitle->getSize().y + musicTitle->getSize().y * 2 + 10}, {Constants::loginPanelWidth - 190, 10}, clickSound.getVolume(), audioSettingsPanel);

    auto effectsValue = tgui::Label::create("100");
    effectsValue->setTextSize(24);
    effectsValue->setPosition(gameSettingsPanel->getSize().x - 50.f, audioTitle->getSize().y + musicTitle->getSize().y * 2);
    effectsValue->getRenderer()->setTextColor(color.text);
    audioSettingsPanel->add(effectsValue);


    effectsSlider->onValueChange([this, effectsValue](float value){
        effectsValue->setText(std::to_string((int)value));
        clickSound.setVolume(value);
        clickSound.play();
    });
    
    auto backToSettingsButton = makeBackToMenu({gap, Constants::loginPanelHeight * 2 - gap - 70}, {Constants::loginPanelWidth - 2 * gap, 70}, audioSettingsPanel);
    backToSettingsButton->setText("Back to Settings");

    bindSoundOnClick(backToSettingsButton, [this]{
        root->remove(audioSettingsPanel);
        gameSettingsPanel->setVisible(true);
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

    std::vector<tgui::Button::Ptr> buttons;

    tgui::Button::Ptr videoButton = makeButton("Video", {480, 62}, {300,66} ,28 , gameSettingsPanel);
    videoButton->setPosition((gameSettingsPanel->getSize().x - videoButton->getSize().x) / 2.f, 150);

    tgui::Button::Ptr gameplayButton = makeButton("Gameplay", {480, 62}, {300,66}, 28 , gameSettingsPanel);
    gameplayButton->setPosition((gameSettingsPanel->getSize().x - videoButton->getSize().x) / 2.f, 250);

    tgui::Button::Ptr audioButton = makeButton("Audio", {480, 62},{300,66}, 28, gameSettingsPanel);
    audioButton->setPosition((gameSettingsPanel->getSize().x - videoButton->getSize().x) / 2.f, 350);

    buttons.push_back(videoButton);
    buttons.push_back(gameplayButton);
    buttons.push_back(audioButton);


    auto backToMenu = makeBackToMenu({gap, Constants::loginPanelHeight * 2 - gap - 70}, {Constants::loginPanelWidth - 2 * gap, 70}, gameSettingsPanel); 


    bindSoundOnClick(audioButton, [this]{
        gameSettingsPanel->setVisible(false);
        std::cout << std::endl;
        audio_settings();
    });





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


//lol 

tgui::Button::Ptr Gui_base::makeBackToMenu(const tgui::Vector2f pos, const tgui::Vector2f size, tgui::Panel::Ptr panel){

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


void Gui_base::volume(tgui::Panel::Ptr topBar, tgui::Panel::Ptr root, sf::Music &music){
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

