#include <iostream> 

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <vector>

#include "gui_main_menu.hpp"
#include "gui_utils.hpp"
#include "constants.hpp"

MainMenu::MainMenu(tgui::Gui& gui, sf::Music& music, sf::RenderWindow& window)
    : Gui_base(gui, music), window(window) {}


void MainMenu::showMainMenu(){

    const float guiW = getGuiWidth();
    const float guiH = getGuiHeight();
    const float topBarH = Constants::Bar::topHeightRatio * guiH;

    gui.removeAllWidgets();
    tgui::Font::setGlobalFont("assets/fonts/Shelten.ttf");

    root = tgui::Panel::create({guiW, guiH});
    root->getRenderer()->setBackgroundColor(color.panel);
    root->getRenderer()->setTextureBackground("assets/images/backgrounds/main_menu.jpg");
    gui.add(root);

    auto topBar = makeBar({0,0});
    volume(topBar, root, mainMenuMusic);

    const float sideBarW = Constants::Bar::sideWidthRatio * guiW;
    const float sideBarH = Constants::Bar::sideHeightRatio * guiH;
    sideBar = makePanel({sideBarW, sideBarH}, tgui::Color::Transparent, root);
    sideBar->setPosition({guiW * 0.015625f, guiH - sideBarH - guiH * 0.037037f});
    sideBar->getRenderer()->setRoundedBorderRadius(10.f);

    auto title = makeLabel("RetroPoker", 225, {0,0,0}, root);
    title->getRenderer()->setFont("assets/fonts/AncientMedium.ttf");
    title->setPosition({(guiW - title->getSize().x ) / 2.f, guiH * 0.296296f});
    title->getRenderer()->setTextColor(color.title);
    title->getRenderer()->setTextOutlineColor(tgui::Color(42, 26, 18));
    title->getRenderer()->setTextOutlineThickness(2.f);                


    float usernamePanelLenght = 0;

    if (loggedUsername.length() <= 0){
        std::cerr<< "ERROR";
    }
    else if (loggedUsername.length() > 0 && loggedUsername.length() < 6){
        usernamePanelLenght = guiW * 0.078125f;
    }
    else if(loggedUsername.length() >= 6 && loggedUsername.length() < 10){
        usernamePanelLenght = guiW * 0.093750f;
    }
    else {
        usernamePanelLenght = guiW * 0.093750f;
    }
    
    

    auto usernamePanel = makePanel({usernamePanelLenght, topBarH}, color.border, topBar);
    usernamePanel->setPosition({0, 0});

    auto usernameShow = makeLabel(loggedUsername, 30, color.text, usernamePanel);
    usernameShow->setPosition({usernamePanelLenght * 0.40f, topBarH * 0.18f});

    auto usernmaeIcon = makeImage("assets/images/login_icon.png", {usernamePanelLenght * 0.08f, topBarH * 0.25f}, {usernamePanelLenght * 0.20f, topBarH * 0.54f}, usernamePanel);


    auto logoutButton = makeButton("Logout", {guiW * 0.002604f, topBarH + guiH * 0.009259f}, {sideBarW * 0.942857f, guiH * 0.033333f}, 20, root);
    logoutButton->setVisible(false);


    bindSoundOnClick(usernamePanel, [this, usernamePanel, logoutButton]{
        if (usernamePanel->getRenderer()->getBackgroundColor() == color.border){
            usernamePanel->getRenderer()->setBackgroundColor(color.down);
        }
        else {
            usernamePanel->getRenderer()->setBackgroundColor(color.border);
        }
        
        logoutButton->setVisible(!logoutButton->isVisible());
    });

    bindSoundOnClick(logoutButton, [this]{
        loggedUsername.clear();
        if (onLogout) {
            onLogout();
        }
    });

    const int16_t money = Constants::START_PLAYER_MONEY;

    auto moneyShowLabel = makeLabel("Balance: " + std::to_string(money) + "$" , 20, color.text, topBar);
    moneyShowLabel->getRenderer()->setFont("assets/fonts/Shelten.ttf");
    moneyShowLabel->setPosition({guiW * 0.114583f, topBarH * 0.29f});


    const float sideButtonX = sideBarW * 0.089286f;
    const float sideButtonW = sideBarW * 0.857143f;
    auto buttonCampaign = makeButton("Campaign", {sideButtonX, sideBarH * 0.125f}, {sideButtonW, sideBarH * 0.172222f}, 33, sideBar);
    auto buttonGameModes = makeButton("Game Modes", {sideButtonX, sideBarH * 0.333333f}, {sideButtonW, sideBarH * 0.133333f}, 30, sideBar);
    auto buttonSettings = makeButton("Settings", {sideButtonX, sideBarH * 0.541667f}, {sideButtonW, sideBarH * 0.133333f}, 30, sideBar);
    auto quitButton = makeButton("Quit", {sideButtonX, sideBarH * 0.750000f}, {sideButtonW, sideBarH * 0.133333f}, 30, sideBar);

    buttonsMainMenu.push_back(buttonCampaign);
    buttonsMainMenu.push_back(buttonGameModes);
    buttonsMainMenu.push_back(buttonSettings);
    buttonsMainMenu.push_back(quitButton);

        bindSoundOnClick(buttonCampaign, [this]{
            //?????
        });

        bindSoundOnClick(buttonGameModes, [this]{
            for (auto button : buttonsMainMenu){
                button->setVisible(!(button->isVisible()));
            }
            
            game_menu(buttonsMainMenu);
        });


        bindSoundOnClick(buttonSettings, [topBar, this]{
            game_settings();
        });


        bindSoundOnClick(quitButton, [this] {
            for (auto x : buttonsMainMenu){
                x->setVisible(!(x->isVisible()));
            }

            tgui::ChildWindow::Ptr quitPanel = makePopUpPanel("Do you want to exit?", {420.f, 190.f}, 22, color.down, sf::Color(253, 253, 253), true);
            tgui::Button::Ptr yesButton = makePopUpButton("Yes", {45.f, 50.f}, {120,50}, 20, quitPanel);
            tgui::Button::Ptr noButton = makePopUpButton("No", {235, 50.f}, {120,50}, 20, quitPanel);


            bindSoundOnClick(yesButton, [this]
            {
                window.close();
            });


            bindSoundOnClick(noButton, [this, quitPanel]
            {
                hideOverlay();
                root->remove(quitPanel);
                for(auto button : buttonsMainMenu){
                    button->setVisible(true);
                }
            });


        });
        
}

void MainMenu::handleGameMenuEscape(){
    if(gameMenuPanel && gameMenuPanel->isVisible()) {
        root->remove(gameMenuPanel);
        gameMenuPanel = nullptr;
        hideOverlay();
        return;
    }
}
void MainMenu::handleGameSettingsEscape(){
    handleEscape();
}

void MainMenu::game_menu(std::vector<tgui::Button::Ptr> buttons){

    openedGameMenuPanel = true;
    showOverlay();
    
    const float gameMenuPanelW = getGuiWidth() * 0.479167f;
    const float gameMenuPanelH = getGuiHeight() * 0.611111f;
    const float buttonW = gameMenuPanelW * Constants::GMButton::widthRatio;
    const float buttonH = gameMenuPanelH * Constants::GMButton::heightRatio;


    gameMenuPanel = makePanel({gameMenuPanelW, gameMenuPanelH}, color.centerpanel, root);
    auto gameMenuTitle = makeLabel("Game Modes", 45, color.text, gameMenuPanel);
    gameMenuTitle->setPosition({(gameMenuPanelW - gameMenuTitle->getSize().x) / 2.f, gameMenuPanelH * 0.030303f});
    auto backIcon = makeImage("assets/images/arrow__left.png", {gameMenuPanelW * 0.043478f, gameMenuPanelH * 0.840909f}, {gameMenuPanelW * 0.076087f, gameMenuPanelH * 0.083333f}, gameMenuPanel);
    auto texasHoldemButton = makeButton("Texas Hold'em", {gameMenuPanelW * Constants::GMButton::leftXRatio, gameMenuPanelH * Constants::GMButton::topYRatio}, {buttonW, buttonH}, 33, gameMenuPanel);
    auto fiveCardDrawButton = makeButton("5-Card draw", {gameMenuPanelW * Constants::GMButton::rightXRatio, gameMenuPanelH * Constants::GMButton::topYRatio}, {buttonW, buttonH}, 33, gameMenuPanel);
    auto backToMenu = makeButton("Back to Menu", {gameMenuPanelW * 0.043478f, gameMenuPanelH * 0.833333f}, {gameMenuPanelW * 0.913043f, gameMenuPanelH * 0.106061f}, 24, gameMenuPanel);

    bindSoundOnClick(backToMenu, [this, buttons]{

        hide_game_menu();
        for(auto x : buttons){
            x->setVisible(true);
        }
        root->remove(gameMenuPanel);
        gameMenuPanel = nullptr;
        hideOverlay();
    });


    bindSoundOnClick(texasHoldemButton, [this]{
        gameMenuPanel->setVisible(false);
        gameplay_settings();
        std::cerr << selectedPlayerCount;
    });

    auto texasHoldemText = makeLabel("Two cards\nCommunity board\nBest five-card hand.", 20, color.text, gameMenuPanel);
    texasHoldemText->setPosition({gameMenuPanelW * 0.054348f, gameMenuPanelH * 0.303030f});
    texasHoldemText->setVisible(false);

    texasHoldemButton->onMouseEnter([this, texasHoldemButton,texasHoldemText]{
        texasHoldemButton->setTextPosition({200,50},{0.5f,0.5f});
        texasHoldemButton->getRenderer()->setTextColorDown(sf::Color(0,0,0));
        texasHoldemText->setVisible(true);
    });

    texasHoldemButton->onMousePress([this, texasHoldemText]{
        texasHoldemText->getRenderer()->setTextColor(sf::Color(0,0,0));
    });

    texasHoldemButton->onMouseRelease([this, texasHoldemText]{
        texasHoldemText->getRenderer()->setTextColor(color.text);
    });

    texasHoldemButton->onMouseLeave([this, texasHoldemButton,texasHoldemText]{
        texasHoldemButton->setTextPosition({200,100},{0.5f,0.5f});
        texasHoldemText->getRenderer()->setTextColor(color.text);
        texasHoldemText->setVisible(false);
    });

    bindSoundOnClick(fiveCardDrawButton, [this]{
        //if(onGame) onGame();
    });

    bindSoundOnClick(backIcon, [this, buttons]{
        for(auto x : buttons){
            x->setVisible(true);
        }
        root->remove(gameMenuPanel);
        hideOverlay();
    });


    }


void MainMenu::gameplay_settings(){

    // position 
    
    const float panelW = Constants::Panel::loginWidthRatio * getGuiWidth();
    const float panelH = Constants::Panel::loginHeightRatio * 2.f * getGuiHeight();
    gameplaySettingsPanel = makePanel({panelW, panelH}, color.centerpanel, root);

    float numOfPlayersY = panelH * 0.125f;
    float smallBlindY = panelH * 0.193f;

    const float padding = panelW * 0.04f;
    const float sliderX = panelW * 0.42f;
    const float sliderW = panelW * 0.43f;

    const float rowH = panelH * 0.055f;

    float numOfPlayerSliderY = numOfPlayersY + panelH * 0.009f;
    float smallBlindSliderY = smallBlindY + panelH * 0.009f;


    auto gameplayTitle = makeLabel("Table Setup", 45, color.text, gameplaySettingsPanel);
    gameplayTitle->setPosition({(panelW - gameplayTitle->getSize().x) / 2.f, panelH * 0.023f});


    auto numOfPlayersPanel = makePanel({panelW * 0.95f, rowH}, color.panel2, gameplaySettingsPanel);
    auto smallBlindPanel = makePanel({panelW * 0.95f, rowH}, color.panel2, gameplaySettingsPanel);


    numOfPlayersPanel->setPosition(panelW * 0.025f, numOfPlayersY - panelH * 0.011f);
    numOfPlayersPanel->getRenderer()->setRoundedBorderRadius(0);

    smallBlindPanel->setPosition(panelW * 0.025f, smallBlindY - panelH * 0.011f);
    smallBlindPanel->getRenderer()->setRoundedBorderRadius(0);



    auto numOfPlayersTitle = makeLabel("Number of players", 20, color.text, gameplaySettingsPanel);
    numOfPlayersTitle->setPosition({padding, numOfPlayersY});

    auto smallBlindTitle = makeLabel("Small Blind amount", 20, color.text, gameplaySettingsPanel);
    smallBlindTitle->setPosition({padding, smallBlindY});

    auto numOfPlayersValue = makeLabel(std::to_string(selectedPlayerCount), 20, color.text, gameplaySettingsPanel);
    numOfPlayersValue->setPosition({panelW * 0.90f, numOfPlayersY});

    auto smallBlindValue = makeLabel(std::to_string(selectedSmallBlind) + "$", 20, color.text, gameplaySettingsPanel);
    smallBlindValue->setPosition({panelW * 0.90f, smallBlindY});

    // panelW * 0.68f, panelH * 0.012f 
    
    auto numOfPlayersSlider = makeSlider({sliderX, numOfPlayerSliderY}, {sliderW, panelH * 0.012f}, selectedPlayerCount, gameplaySettingsPanel);
    auto smallBlindSlider = makeSlider({sliderX, smallBlindSliderY}, {sliderW, panelH * 0.012f}, selectedSmallBlind, gameplaySettingsPanel);
    numOfPlayersSlider->setMinimum(2);
    numOfPlayersSlider->setMaximum(9);

    smallBlindSlider->setMinimum(2);
    smallBlindSlider->setMaximum(50);

    auto confirmButton = makeButton("Start the game", {padding, panelH * 0.80f}, {panelW - 2.f * padding, panelH * 0.08f}, 24, gameplaySettingsPanel);
    auto backToMenuButton = makeButton("Back to Menu", {padding, panelH * 0.90f}, {panelW - 2.f * padding, panelH * 0.08f}, 24, gameplaySettingsPanel);
   
    bindSoundOnClick(confirmButton, [this]{
        root->remove(gameplaySettingsPanel);
        gameplaySettingsPanel = nullptr;
        if(onGame) onGame(selectedPlayerCount, selectedSmallBlind);
    });

    bindSoundOnClick(backToMenuButton, [this]{
        root->remove(gameplaySettingsPanel);
        gameplaySettingsPanel = nullptr;
        gameMenuPanel->setVisible(true);
    });

    numOfPlayersSlider->onValueChange([this, numOfPlayersValue](int value){
        selectedPlayerCount = value;
        numOfPlayersValue->setText(std::to_string(value));
        clickSound.play();
    });

    smallBlindSlider->onValueChange([this, smallBlindValue](int value){
        selectedSmallBlind = value;
        smallBlindValue->setText(std::to_string(value)+"$");
        clickSound.play();
    });
}

