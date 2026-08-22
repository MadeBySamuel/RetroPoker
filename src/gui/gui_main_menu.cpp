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

    gui.removeAllWidgets();
    tgui::Font::setGlobalFont("assets/fonts/Shelten.ttf");

    root = tgui::Panel::create({getGuiWidth(), getGuiHeight()});
    root->getRenderer()->setBackgroundColor(color.panel);
    root->getRenderer()->setTextureBackground("assets/images/backgrounds/main_menu.jpg");
    gui.add(root);

    auto topBar = makeBar({0,0});
    volume(topBar, root, mainMenuMusic);

    sideBar = makePanel({Constants::Bar::sideWidth, Constants::Bar::sideHeight}, tgui::Color::Transparent, root);
    sideBar->setPosition({30, getGuiHeight() - Constants::Bar::sideHeight - 40});
    sideBar->getRenderer()->setRoundedBorderRadius(10.f);

    auto title = makeLabel("RetroPoker", 225, {0,0,0}, root);
    title->getRenderer()->setFont("assets/fonts/AncientMedium.ttf");
    title->setPosition({(getGuiWidth() - title->getSize().x ) / 2.f , 320});
    title->getRenderer()->setTextColor(color.title);
    title->getRenderer()->setTextOutlineColor(tgui::Color(42, 26, 18));
    title->getRenderer()->setTextOutlineThickness(2.f);                


    float usernamePanelLenght = 0;

    if (loggedUsername.length() <= 0){
        std::cerr<< "ERROR";
    }
    else if (loggedUsername.length() > 0 && loggedUsername.length() < 6){
        usernamePanelLenght = 150;
    }
    else if(loggedUsername.length() >= 6 && loggedUsername.length() < 10){
        usernamePanelLenght = 180;
    }
    else {
        usernamePanelLenght = 180;
    }
    
    

    auto usernamePanel = makePanel({usernamePanelLenght, Constants::Bar::topHeight}, color.border, topBar);
    usernamePanel->setPosition({0, 0});

    auto usernameShow = makeLabel(loggedUsername, 30, color.text, usernamePanel);
    usernameShow->setPosition({60,10});

    auto usernmaeIcon = makeImage("assets/images/login_icon.png", {12,14}, {30,30}, usernamePanel);


    auto logoutButton = makeButton("Logout", {5, Constants::Bar::topHeight + 10}, {264, 36}, 20, root);
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
    moneyShowLabel->setPosition({220, 16});


    auto buttonCampaign = makeButton("Campaign", {25, 45}, {240, 62}, 33, sideBar);
    auto buttonGameModes = makeButton("Game Modes", {25, 120}, {240, 48}, 30, sideBar);
    auto buttonSettings = makeButton("Settings", {25, 195}, {240, 48}, 30, sideBar);
    auto quitButton = makeButton("Quit", {25, 270}, {240, 48}, 30, sideBar);


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
        
        timeLabel = makeLabel("", 30, color.text, topBar);       
        timeLabel->setPosition({getGuiWidth() - 125.f, 10});
        topBar->add(timeLabel);

}



void MainMenu::game_menu(std::vector<tgui::Button::Ptr> buttons){

    showOverlay();
    
    const float gameMenuHeight = Constants::Panel::loginHeight * 1.5f;
    const float totalWidth = Constants::GMButton::width * 2 + 3 * Constants::gap;

    gameMenuPanel = makePanel({totalWidth, gameMenuHeight}, color.centerpanel, root);
    auto gameMenuTitle = makeLabel("Game Modes", 45, color.text, gameMenuPanel);
    gameMenuTitle->setPosition((gameMenuPanel->getSize().x - gameMenuTitle->getSize().x) / 2.f, 20);
    auto backIcon = makeImage("assets/images/arrow__left.png", {Constants::gap, 555.f}, {70,55}, gameMenuPanel);
    auto texasHoldemButton = makeButton("Texas Hold'em",{Constants::gap,100.f}, {Constants::GMButton::width,Constants::GMButton::height}, 33, gameMenuPanel);
    auto fiveCardDrawButton = makeButton("5-Card draw", {Constants::GMButton::cardDrawPosX, Constants::GMButton::posY}, {Constants::GMButton::width,Constants::GMButton::height}, 33, gameMenuPanel);
    auto backToMenu = makeButton("Back to login", {Constants::gap, 550.f}, {totalWidth - 2 * Constants::gap , 70}, 20, gameMenuPanel);

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
    texasHoldemText->setPosition(Constants::gap + 10, 200);
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

