#include <iostream> 
#include "game.hpp"
#include "gui_utils.hpp"


Gui_base::Gui_base(tgui::Gui& gui, sf::Music& music) : gui(gui), mainMenuMusic(music), clickSound(clickBuffer) 
    {

        if (!clickBuffer.loadFromFile("assets/sound/button_press.ogg")) {
            std::cerr << "Failed to load click sound\n";
        }
        clickSound.setBuffer(clickBuffer);
    }
        

tgui::Panel::Ptr Gui_base::makeBar(tgui::Vector2f pos){

    auto bar = tgui::Panel::create({getGuiWidth(), Constants::Bar::topHeight});
    bar->setPosition(pos);
    bar->getRenderer()->setBackgroundColor(color.bar_color);
    root->add(bar);

    if (pos == tgui::Vector2f{0,0}){
        timeLabel = makeLabel("", 30, color.text, bar);
        timeLabel->setPosition({"94%", "17%"});
    }
   
    return bar;
}


tgui::Label::Ptr Gui_base::makeLabel(const std::string& title, const int16_t titleSize, tgui::Color titleColor, tgui::Panel::Ptr panel){

    auto label = tgui::Label::create(title);
    label->setTextSize(titleSize);
    label->getRenderer()->setTextColor(color.text);
    panel->add(label);
    return label;
}



tgui::Panel::Ptr Gui_base::makePanel(tgui::Vector2f size, tgui::Color backgroundColor, tgui::Panel::Ptr container){
   
    auto panel = tgui::Panel::create(size);
    panel->setPosition({(getGuiWidth() - size.x) / 2.f, (getGuiHeight() - size.y) / 2.f});
    panel->getRenderer()->setBackgroundColor(backgroundColor);
    panel->getRenderer()->setRoundedBorderRadius(borderRadius);
    container->add(panel);

    return panel;
}

tgui::CheckBox::Ptr Gui_base::makeCheckBox(tgui::Vector2f pos, tgui::Panel::Ptr panel){
        
        auto checkBox = tgui::CheckBox::create();
        checkBox->setPosition(pos);
        checkBox->setSize({20, 20});
        checkBox->setText("");
        checkBox->setTextSize(20);
        panel->add(checkBox);


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


tgui::EditBox::Ptr Gui_base::makeEditBox(const std::string& title, tgui::Vector2f pos, tgui::Panel::Ptr panel){
    
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

tgui::ChildWindow::Ptr Gui_base::makePopUpPanel(const std::string& title, tgui::Vector2f size, const int8_t sizeTitleText, tgui::Color titleBarColor, tgui::Color titleTextColor, bool isOverlay){

    if (isOverlay == true){
        showOverlay();
    }
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



tgui::Button::Ptr Gui_base::makePopUpButton(const std::string& title, tgui::Vector2f pos, const tgui::Vector2f size, const int8_t sizeTitle, tgui::ChildWindow::Ptr quitPanel){

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
    quitPanel->add(button);

    return button;

}


// toto treba premysliet lebo wtf 

tgui::Button::Ptr Gui_base::makeButton(const std::string& title, tgui::Vector2f pos, const tgui::Vector2f size, const int8_t sizeText,tgui::Panel::Ptr panel){

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

    if(panel == topBar || panel == sideBar){
        button->getRenderer()->setBorders(0);
    }

    if(panel == gameSettingsPanel || panel == gameMenuPanel || title == "Logout" || panel == audioSettingsPanel || panel == root || panel == escapeMenuPanel){
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
    

    if((panel == centerPanel && title == "LOGIN" ) || (panel == signUpPanel && title == "Create account")){
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

    if(panel == forgotPasswordPanel){
        button->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});    
        button->getRenderer()->setBorders(0);
    }

    panel->add(button);

    return button;
}


 void Gui_base::audio_settings(){

    float gap = 24.f;
    float valuePos = gameSettingsPanel->getSize().x - 50.f;

    audioSettingsPanel = makePanel({Constants::Panel::loginWidth, Constants::Panel::loginHeight * 2}, color.centerpanel, root);
    auto musicPanel = makePanel({Constants::Panel::loginWidth, 2*gap}, color.panel2, audioSettingsPanel);
    auto clickPanel = makePanel({Constants::Panel::loginWidth - 10, 2*gap}, color.panel2, audioSettingsPanel);
    auto ambienceSoundPanel = makePanel({Constants::Panel::loginWidth * 2 - gap - 70, 2*gap}, color.panel2, audioSettingsPanel);


    auto audioTitle = makeLabel("Audio Settings", 45, color.text, audioSettingsPanel);
    auto musicTitle = makeLabel("Music", 22, color.text, audioSettingsPanel);
    auto clickSoundTitle = makeLabel("Click", 20, color.text, audioSettingsPanel);
    auto ambienceSoundTitle = makeLabel("Ambience", 20, color.text, audioSettingsPanel);

        
    auto musicValue = makeLabel("0", 20, color.text, audioSettingsPanel);
    auto clickSoundValue = makeLabel("100", 20, color.text, audioSettingsPanel);
    auto ambienceSoundValue = makeLabel("100", 20, color.text, audioSettingsPanel);

    float audioWidth = audioTitle->getSize().x;
    float audioHeight = audioTitle->getSize().y; 

    float musicWidth = musicTitle->getSize().x;
    float musicHeight = musicTitle->getSize().y;

    float clickWidth = clickSoundTitle->getSize().x;
    float clickHeight = clickSoundTitle->getSize().y;

    float audioTitleX = (Constants::Panel::loginWidth - audioWidth) / 2.f;

    float musicTitleY = 110;
    float clickSoundTitleY = 170;
    float ambienceSoundTitleY = 230;

    musicPanel->setPosition(gap - 10, musicTitleY - 10);
    musicPanel->getRenderer()->setRoundedBorderRadius(0);

    clickPanel->setPosition(0, 160);
    clickPanel->getRenderer()->setRoundedBorderRadius(0);


    ambienceSoundPanel->setPosition(gap - 10, 160 + 60);
    ambienceSoundPanel->getRenderer()->setRoundedBorderRadius(0);


    std::cout << musicTitleY << std::endl;
    std::cout << clickSoundTitleY << std::endl;
    std::cout << ambienceSoundTitleY << std::endl;

    float sliderX = musicHeight + 3 * gap;

    float musicSliderY = musicTitleY + 8;
    float clickSliderY = clickSoundTitleY + 8;
    float ambienceSoundSliderY = ambienceSoundTitleY + 8;

    audioTitle->setPosition(audioTitleX, 20.f);
    musicTitle->setPosition(25.f, musicTitleY);
    clickSoundTitle->setPosition(25.f, clickSoundTitleY);
    ambienceSoundTitle->setPosition(25.f, ambienceSoundTitleY);

    settingsMusicSlider = makeSlider({sliderX, musicSliderY}, {Constants::Panel::loginWidth - 190, 10}, mainMenuMusic.getVolume(), audioSettingsPanel);
    auto clickSoundSlider = makeSlider({sliderX, clickSliderY}, {Constants::Panel::loginWidth - 190, 10}, clickSound.getVolume(), audioSettingsPanel);
    auto ambienceSoundSlider = makeSlider({sliderX, ambienceSoundSliderY}, {Constants::Panel::loginWidth - 190, 10}, clickSound.getVolume(), audioSettingsPanel);

    musicValue->setPosition(valuePos, musicTitleY);
    clickSoundValue->setPosition(valuePos, clickSoundTitleY);
    ambienceSoundValue->setPosition(valuePos, ambienceSoundTitleY);


    settingsMusicSlider->onValueChange([this, musicValue](float value){
        musicValue->setText(std::to_string((int)value));
        mainMenuMusic.setVolume(value);
        if (volumeSlider) volumeSlider->setValue(value);
    });




    clickSoundSlider->onValueChange([this, clickSoundValue](float value){
        clickSoundValue->setText(std::to_string((int)value));
        clickSound.setVolume(value);
        clickSound.play();
    });



    ambienceSoundSlider->onValueChange([ambienceSoundValue, this](float value){
        ambienceSoundValue->setText(std::to_string((int)value));
        ambienceSound.setVolume(value);
        clickSound.play();
    });

    auto backToSettingsButton = makeButton("Back to Settings", {gap, Constants::Panel::loginHeight * 2 - gap - 70}, {Constants::Panel::loginWidth - 2 * gap, 70}, 24, audioSettingsPanel);

    
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
    
    if(escapeMenuPanel){
        escapeMenuPanel->setVisible(false);
    }
    else {

        showOverlay();
    }

    gameSettingsPanel = makePanel({Constants::Panel::loginWidth, Constants::Panel::loginHeight * 2}, color.centerpanel, root);

    auto settingsTitle = makeLabel("Settings", 45, color.text, gameSettingsPanel);
    settingsTitle->setPosition((Constants::Panel::loginWidth - settingsTitle->getSize().x) / 2.f, 20.f);

    float titleH = settingsTitle->getSize().y;
    float titleW = settingsTitle->getSize().x;

    const float startX = (gameSettingsPanel->getSize().x - 300) / 2.f;


    tgui::Button::Ptr videoButton = makeButton("Video", {startX, 150}, {300,66}, 28, gameSettingsPanel);
    tgui::Button::Ptr gameplayButton = makeButton("Gameplay", {startX, 250}, {300,66}, 28, gameSettingsPanel);
    tgui::Button::Ptr audioButton = makeButton("Audio", {startX, 350},{300,66}, 28, gameSettingsPanel);
    auto backToMenu = makeButton("Back to Main Menu", {gap, Constants::Panel::loginHeight * 2 - gap - 70}, {Constants::Panel::loginWidth - 2 * gap, 70}, 24, gameSettingsPanel); 


    bindSoundOnClick(audioButton, [this]{
        gameSettingsPanel->setVisible(false);
        
        audio_settings();
    });





    bindSoundOnClick(backToMenu, [this]{
    if(escapeMenuPanel){
        escapeMenuPanel->setVisible(true);
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
        modalOverlay = makePanel({getGuiWidth(), getGuiHeight()},tgui::Color(0, 0, 0, 220), root);
        modalOverlay->setPosition({0, 0});
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

        if (currentTimeText != last_time_text && timeLabel) {
            timeLabel->setText(currentTimeText);
            last_time_text = currentTimeText;
        }
}

tgui::Picture::Ptr Gui_base::makeImage(const tgui::Texture& texture, tgui::Vector2f pos, tgui::Vector2f size, tgui::Panel::Ptr panel){
    auto image = tgui::Picture::create(texture);
    image->setPosition({500, 225});
    image->setSize({32, 28});
    panel->add(image);
    return image;
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
        
        auto volumePanel = makePanel({300,90}, color.down, root);
        volumePanel->setPosition({getGuiWidth() - volumePanel->getSize().x - 30, topBar->getSize().y + 10});
        volumePanel->setVisible(false);

        auto volumeLabel = makeLabel("Sound", 20, color.text, volumePanel);
        volumeLabel->setPosition({18, 12});

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
    std::vector<std::string> cardImagePaths;
    std::vector<std::string> suits = {"♥️", "♦️", "♣️", "♠️"};

    cards.reserve(52);
    cardImagePaths.reserve(52);

    cardImagePaths.emplace_back("assets/cards/heart/hearts_2.png");
    cardImagePaths.emplace_back("assets/cards/heart/hearts_3.png");
    cardImagePaths.emplace_back("assets/cards/heart/hearts_4.png");
    cardImagePaths.emplace_back("assets/cards/heart/hearts_5.png");
    cardImagePaths.emplace_back("assets/cards/heart/hearts_6.png");
    cardImagePaths.emplace_back("assets/cards/heart/hearts_7.png");
    cardImagePaths.emplace_back("assets/cards/heart/hearts_8.png");
    cardImagePaths.emplace_back("assets/cards/heart/hearts_9.png");
    cardImagePaths.emplace_back("assets/cards/heart/hearts_10.png");
    cardImagePaths.emplace_back("assets/cards/heart/hearts_A.png");
    cardImagePaths.emplace_back("assets/cards/heart/hearts_J.png");
    cardImagePaths.emplace_back("assets/cards/heart/hearts_K.png");
    cardImagePaths.emplace_back("assets/cards/heart/hearts_Q.png");

    cardImagePaths.emplace_back("assets/cards/diamond/diamonds_2.png");
    cardImagePaths.emplace_back("assets/cards/diamond/diamonds_3.png");
    cardImagePaths.emplace_back("assets/cards/diamond/diamonds_4.png");
    cardImagePaths.emplace_back("assets/cards/diamond/diamonds_5.png");
    cardImagePaths.emplace_back("assets/cards/diamond/diamonds_6.png");
    cardImagePaths.emplace_back("assets/cards/diamond/diamonds_7.png");
    cardImagePaths.emplace_back("assets/cards/diamond/diamonds_8.png");
    cardImagePaths.emplace_back("assets/cards/diamond/diamonds_9.png");
    cardImagePaths.emplace_back("assets/cards/diamond/diamonds_10.png");
    cardImagePaths.emplace_back("assets/cards/diamond/diamonds_A.png");
    cardImagePaths.emplace_back("assets/cards/diamond/diamonds_J.png");
    cardImagePaths.emplace_back("assets/cards/diamond/diamonds_K.png");
    cardImagePaths.emplace_back("assets/cards/diamond/diamonds_Q.png");

    cardImagePaths.emplace_back("assets/cards/club/clubs_2.png");
    cardImagePaths.emplace_back("assets/cards/club/clubs_3.png");
    cardImagePaths.emplace_back("assets/cards/club/clubs_4.png");
    cardImagePaths.emplace_back("assets/cards/club/clubs_5.png");
    cardImagePaths.emplace_back("assets/cards/club/clubs_6.png");
    cardImagePaths.emplace_back("assets/cards/club/clubs_7.png");
    cardImagePaths.emplace_back("assets/cards/club/clubs_8.png");
    cardImagePaths.emplace_back("assets/cards/club/clubs_9.png");
    cardImagePaths.emplace_back("assets/cards/club/clubs_10.png");
    cardImagePaths.emplace_back("assets/cards/club/clubs_A.png");
    cardImagePaths.emplace_back("assets/cards/club/clubs_J.png");
    cardImagePaths.emplace_back("assets/cards/club/clubs_K.png");
    cardImagePaths.emplace_back("assets/cards/club/clubs_Q.png");

    cardImagePaths.emplace_back("assets/cards/spade/spades_2.png");
    cardImagePaths.emplace_back("assets/cards/spade/spades_3.png");
    cardImagePaths.emplace_back("assets/cards/spade/spades_4.png");
    cardImagePaths.emplace_back("assets/cards/spade/spades_5.png");
    cardImagePaths.emplace_back("assets/cards/spade/spades_6.png");
    cardImagePaths.emplace_back("assets/cards/spade/spades_7.png");
    cardImagePaths.emplace_back("assets/cards/spade/spades_8.png");
    cardImagePaths.emplace_back("assets/cards/spade/spades_9.png");
    cardImagePaths.emplace_back("assets/cards/spade/spades_10.png");
    cardImagePaths.emplace_back("assets/cards/spade/spades_A.png");
    cardImagePaths.emplace_back("assets/cards/spade/spades_J.png");
    cardImagePaths.emplace_back("assets/cards/spade/spades_K.png");
    cardImagePaths.emplace_back("assets/cards/spade/spades_Q.png");

    size_t image_iterator = 0;

    
    for (auto s : suits) {
        for (int r = 2; r <= 14; r++) {
            cards.push_back({r, s, cardImagePaths.at(image_iterator)});
            ++image_iterator;
        }
    }
    return cards;

}
