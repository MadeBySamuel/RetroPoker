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
    
    const float totalWidth = getGuiWidth() * 0.479167f;
    const float gameMenuHeight = getGuiHeight() * 0.611111f;
    const float buttonW = totalWidth * Constants::GMButton::widthRatio;
    const float buttonH = gameMenuHeight * Constants::GMButton::heightRatio;

    gameMenuPanel = makePanel({totalWidth, gameMenuHeight}, color.centerpanel, root);
    auto gameMenuTitle = makeLabel("Game Modes", 45, color.text, gameMenuPanel);
    gameMenuTitle->setPosition({(totalWidth - gameMenuTitle->getSize().x) / 2.f, gameMenuHeight * 0.030303f});
    auto backIcon = makeImage("assets/images/arrow__left.png", {totalWidth * 0.043478f, gameMenuHeight * 0.840909f}, {totalWidth * 0.076087f, gameMenuHeight * 0.083333f}, gameMenuPanel);
    auto texasHoldemButton = makeButton("Texas Hold'em", {totalWidth * Constants::GMButton::leftXRatio, gameMenuHeight * Constants::GMButton::topYRatio}, {buttonW, buttonH}, 33, gameMenuPanel);
    auto fiveCardDrawButton = makeButton("5-Card draw", {totalWidth * Constants::GMButton::rightXRatio, gameMenuHeight * Constants::GMButton::topYRatio}, {buttonW, buttonH}, 33, gameMenuPanel);
    auto backToMenu = makeButton("Back to login", {totalWidth * 0.043478f, gameMenuHeight * 0.833333f}, {totalWidth * 0.913043f, gameMenuHeight * 0.106061f}, 20, gameMenuPanel);

    bindSoundOnClick(backToMenu, [this, buttons]{

        hide_game_menu();
        for(auto x : buttons){
            x->setVisible(true);
        }
        root->remove(gameMenuPanel);
        hideOverlay();
    });

    bindSoundOnClick(texasHoldemButton, [this]{
        if(onGame) onGame();
    });

    auto texasHoldemText = makeLabel("Two cards\nCommunity board\nBest five-card hand.", 20, color.text, gameMenuPanel);
    texasHoldemText->setPosition({totalWidth * 0.054348f, gameMenuHeight * 0.303030f});
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
        if(onGame) onGame();
    });

    bindSoundOnClick(backIcon, [this, buttons]{
        for(auto x : buttons){
            x->setVisible(true);
        }
        root->remove(gameMenuPanel);
        hideOverlay();
    });


    }
