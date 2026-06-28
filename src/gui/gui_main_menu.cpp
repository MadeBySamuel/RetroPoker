#include <iostream> 

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <vector>

#include "gui_main_menu.hpp"
#include "gui_utils.hpp"
#include "constants.hpp"

MainMenu::MainMenu(tgui::Gui& gui, sf::RenderWindow& window, sf::Music& music)
    : Gui_base(gui), window(window), music(music) {}




void MainMenu::game_settings(){

    showOverlay();
    GuiTheme color;
    
    auto gameSettingsPanel = tgui::Panel::create({Constants::totalWidth, Constants::loginPanelHeight});
    gameSettingsPanel->setPosition((getGuiWidth() - Constants::totalWidth) / 2.f, (getGuiHeight() - Constants::loginPanelHeight) / 2.f);
    gameSettingsPanel->getRenderer()->setBackgroundColor(color.centerpanel);
    gameSettingsPanel->getRenderer()->setRoundedBorderRadius(10.f);

    root->add(gameSettingsPanel);


    auto settingsTitle = tgui::Label::create("Settings");
    settingsTitle->setTextSize(45);
    settingsTitle->setPosition((Constants::totalWidth - settingsTitle->getSize().x) / 2.f, 20.f);
    settingsTitle->getRenderer()->setTextColor(color.text);
    gameSettingsPanel->add(settingsTitle);



    auto soundEffectCheckBox = tgui::CheckBox::create();




}

void MainMenu::showMainMenu(){

    GuiTheme color;

    gui.removeAllWidgets();


    tgui::Font::setGlobalFont("assets/fonts/Shelten.ttf");

    // auto color_picker =rgb(160, 110, 25));


    root = tgui::Panel::create({getGuiWidth(), getGuiHeight()});
    root->getRenderer()->setBackgroundColor(color.panel);
    root->getRenderer()->setTextureBackground("assets/images/backgrounds/main_menu.jpg");
    gui.add(root);

   

    auto topBar = bar();


    auto sideBar = tgui::Panel::create({Constants::sideBarWidth, Constants::sideBarHeight});
    sideBar->setPosition({30, getGuiHeight() - Constants::sideBarHeight - 40});
    sideBar->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    sideBar->getRenderer()->setRoundedBorderRadius(10.f);
    sideBar->setVisible(true);
    root->add(sideBar);


    auto centerPanel = tgui::Panel::create({Constants::centerPanelWidth, Constants::centerPanelHeight});
    centerPanel->setPosition({320, 110});
    centerPanel->getRenderer()->setBackgroundColor(color.bg);
    centerPanel->getRenderer()->setRoundedBorderRadius(10.f);
    centerPanel->setVisible(false);
    root->add(centerPanel);

    auto rightPanel = tgui::Panel::create({Constants::rightPanelWidth, Constants::rightPanelHeight});
    rightPanel->setPosition({1245, 110});
    rightPanel->getRenderer()->setBackgroundColor(color.bg);
    rightPanel->getRenderer()->setRoundedBorderRadius(10.f);
    rightPanel->setVisible(false);
    root->add(rightPanel);


    volume(topBar, root, color, music);

    // titles
    auto title = tgui::Label::create("RetroPoker");
    title->getRenderer()->setFont("assets/fonts/AncientMedium.ttf");
    title->getRenderer()->setTextColor(color.bg);
    title->setPosition({540, 220});
    title->getRenderer()->setTextOutlineColor(tgui::Color(42, 26, 18));
    title->getRenderer()->setTextOutlineThickness(2.f);                
    title->setTextSize(200);
    root->add(title);

    
    float username_panel_lenght = 0;
    if (logged_username.length() < 6){
        username_panel_lenght = 150;
    }
    else if(logged_username.length() > 6 && logged_username.length() < 14 ){
        username_panel_lenght = 180;
    }

    auto usernameLayer = tgui::Panel::create({username_panel_lenght, Constants::topBarHeight});
    usernameLayer->setPosition({0, 0});
    usernameLayer->getRenderer()->setBackgroundColor(color.border);
    usernameLayer->getRenderer()->setRoundedBorderRadius(10.f);
    topBar->add(usernameLayer);


    auto usernameShow = tgui::Label::create(logged_username);
    usernameShow->setPosition({60,10});
    usernameShow->setTextSize(30);
    usernameShow->getRenderer()->setTextColor(color.text);
    usernameLayer->add(usernameShow);

    auto usernameIcon = tgui::Picture::create("assets/images/login_icon.png");
    usernameIcon->setPosition({12,14});
    usernameIcon->setSize({30,30});
    usernameLayer->add(usernameIcon);



    const int money = Constants::START_PLAYER_MONEY;

    auto logoutButton = tgui::Button::create("Logout");
    logoutButton->setPosition({5, Constants::topBarHeight + 10});
    logoutButton->setSize({264, 36});
    logoutButton->setTextSize(20);

    logoutButton->getRenderer()->setTextColor(color.panel2);
    logoutButton->getRenderer()->setTextColorHover(color.text);
    logoutButton->getRenderer()->setTextColorDown(color.text);

    logoutButton->getRenderer()->setBackgroundColor(color.text);
    logoutButton->getRenderer()->setBackgroundColorHover(color.down);
    logoutButton->getRenderer()->setBackgroundColorDown(color.down);
    logoutButton->getRenderer()->setBorders(0);
    logoutButton->getRenderer()->setRoundedBorderRadius(10.f);
    logoutButton->setVisible(false);
    root->add(logoutButton);

    bindSoundOnClick(usernameLayer, [usernameLayer, color, logoutButton]{
        if (usernameLayer->getRenderer()->getBackgroundColor() == color.border){
            usernameLayer->getRenderer()->setBackgroundColor(color.down);
        }
        else {
            usernameLayer->getRenderer()->setBackgroundColor(color.border);
        }
        
        logoutButton->setVisible(!logoutButton->isVisible());
    });

    bindSoundOnClick(logoutButton, [this]{
        logged_username.clear();
        if (onLogout) {
            onLogout();
        }
    });

    auto moneyShow = tgui::Label::create("Balance: " + std::to_string(money) + "$" );
    moneyShow->getRenderer()->setTextColor(color.text);
    moneyShow->getRenderer()->setFont("assets/fonts/Shelten.ttf");
    moneyShow->setPosition({220, 16});
    moneyShow->setTextSize(20);
    topBar->add(moneyShow);

    auto buttonCampaign = tgui::Button::create("Campaign");
    buttonCampaign->setPosition({25, 45});
    buttonCampaign->setTextSize(33);
    sideBar->add(buttonCampaign);




    auto buttonGameModes = tgui::Button::create("Game Modes");
    buttonGameModes->setPosition({25, 120});
    buttonGameModes->setTextSize(30);
    sideBar->add(buttonGameModes);

    auto buttonSettings = tgui::Button::create("Settings");
    buttonSettings->setPosition({25, 195});
    buttonSettings->setTextSize(30);
    sideBar->add(buttonSettings);

    auto quitButton = tgui::Button::create("Quit");
    quitButton->setPosition({25, 270});
    quitButton->setTextSize(30);
    sideBar->add(quitButton);




    std::vector<tgui::Button::Ptr> buttonsMainMenu;
    buttonsMainMenu.emplace_back(buttonCampaign);
    buttonsMainMenu.emplace_back(buttonGameModes);
    buttonsMainMenu.emplace_back(buttonSettings);
    buttonsMainMenu.emplace_back(quitButton);


    for (auto button : buttonsMainMenu){
        button->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
        button->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
        button->getRenderer()->setBackgroundColorDown(tgui::Color::Transparent);
        button->getRenderer()->setBackgroundColorDisabled(tgui::Color::Transparent);

        button->getRenderer()->setTextStyleHover(tgui::TextStyle::Underlined);

        button->getRenderer()->setTextColor(color.text);
        button->getRenderer()->setTextColorHover(color.down);
        button->getRenderer()->setTextColorDown(color.down);
        button->getRenderer()->setTextColorDisabled(tgui::Color(160, 150, 130));


        button->getRenderer()->setBorders(0);

        if(button != buttonCampaign){
            button->setSize({240, 48});
        }
        else{
           button->setSize({240, 62}); 
        }

    }

        bindSoundOnClick(buttonCampaign, [this]{
            //
        });

        bindSoundOnClick(buttonGameModes, [this, color, buttonsMainMenu]{
            for (auto button : buttonsMainMenu){
                button->setVisible(!(button->isVisible()));
            }

            showOverlay();
            game_menu(buttonsMainMenu);
        });


        bindSoundOnClick(buttonSettings, [this]{
            game_settings();
        });


        bindSoundOnClick(quitButton, [this, buttonsMainMenu, color] {
            for (auto x : buttonsMainMenu){
                x->setVisible(!(x->isVisible()));
            }

            showOverlay();


            auto quitLabel = tgui::ChildWindow::create("Do you really want to quit ?", tgui::ChildWindow::TitleButton::None);

            quitLabel->setPosition({(getGuiWidth() - 420.f) / 2.f, (getGuiHeight() - 190.f) / 2.f});
            quitLabel->setTextSize(20);
            quitLabel->setTitleTextSize(17);
            quitLabel->setSize({420, 190});
            quitLabel->setPositionLocked(true);
            quitLabel->getRenderer()->setBackgroundColor(color.bg);
            quitLabel->getRenderer()->setBorderBelowTitleBar(1.00);

            root->add(quitLabel);

            auto yesButton = tgui::Button::create("Yes");
            yesButton->setPosition({45, 60});

            auto noButton = tgui::Button::create("No");
            noButton->setPosition({235, 60});

            std::vector<tgui::Button::Ptr> quitButtons;

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

                quitLabel->add(yesButton);
                quitLabel->add(noButton);
            }

            bindSoundOnClick(yesButton, [this]
            {
                window.close();
            });


            bindSoundOnClick(noButton, [this, quitLabel, buttonsMainMenu]
            {
                hideOverlay();
                root->remove(quitLabel);
                for(auto button : buttonsMainMenu){
                    button->setVisible(true);
                }
            });


        });

        //prepiš to 
        
        time_label = tgui::Label::create("");
        time_label->setTextSize(30);
        time_label->setPosition({getGuiWidth() - 125.f, 10});
        time_label->getRenderer()->setTextColor(color.text);
        
        topBar->add(time_label);

}

   

void MainMenu::game_menu(std::vector<tgui::Button::Ptr> buttons){


        GuiTheme color;


        float startX = (getGuiWidth() - Constants::totalWidth) / 2.f;



        float gameMenuHeight = Constants::loginPanelHeight * 1.5f;


        auto gameMenu = tgui::Panel::create({Constants::totalWidth, gameMenuHeight});
        gameMenu->setPosition({startX, (getGuiHeight() - gameMenuHeight) / 2.f});
        gameMenu->getRenderer()->setBackgroundColor(color.centerpanel);
        gameMenu->getRenderer()->setRoundedBorderRadius(10.f);
        
        root->add(gameMenu);



        auto gameMenuTitle = tgui::Label::create("Game Modes");
        gameMenuTitle->setTextSize(45);
        gameMenuTitle->getRenderer()->setTextColor(color.text);
        gameMenuTitle->setPosition({(Constants::totalWidth - gameMenuTitle->getSize().x) / 2.f, 20.f});
        gameMenu->add(gameMenuTitle);


        auto backToMenu = tgui::Button::create("Back to Main Menu");
        backToMenu->setTextSize(25);
        backToMenu->getRenderer()->setTextColor(color.text);
        backToMenu->setPosition({Constants::gap, 550.f});


        auto backIcon = tgui::Picture::create("assets/images/arrow__left.png");
        backIcon->setSize({70,55});
        backIcon->setPosition({Constants::gap, 555.f});
        gameMenu->add(backIcon);


        bindSoundOnClick(backToMenu, [gameMenu, this, buttons]{
            for(auto x : buttons){
                x->setVisible(true);
            }
            root->remove(gameMenu);
            hideOverlay();
        });


        auto texasHoldemButton = tgui::Button::create("Texas Hold'em");
        texasHoldemButton->setPosition({Constants::gap,100.f});



        auto fiveCardDrawButton = tgui::Button::create("5-Card draw");
        fiveCardDrawButton->setPosition({Constants::buttonWidth + 2 * Constants::gap, 100.f});


        std::vector<tgui::Button::Ptr> buttonsGameMode;

        buttonsGameMode.emplace_back(texasHoldemButton);
        buttonsGameMode.emplace_back(fiveCardDrawButton);
        buttonsGameMode.emplace_back(backToMenu);


        for(auto button : buttonsGameMode){
            if (button != backToMenu){
                button->setTextSize(33);
            }
            if(button == backToMenu){
                button->setSize({Constants::totalWidth - 2 * Constants::gap , 70});
            }
            else {
                button->setSize({Constants::buttonWidth,Constants::buttonHeight});
            }

            button->getRenderer()->setBackgroundColor(color.panel2);
            button->getRenderer()->setBackgroundColorDownHover(color.down);
            button->getRenderer()->setBackgroundColorDown(color.down);
            button->getRenderer()->setBackgroundColorHover(color.down);

            button->getRenderer()->setTextColor(color.text);
            button->getRenderer()->setTextColorDisabled(color.text);
            button->getRenderer()->setTextColorHover(color.text);
            button->getRenderer()->setTextColorDown(sf::Color(0,0,0));
            button->getRenderer()->setBorders(0);

            gameMenu->add(button);
        }


        bindSoundOnClick(texasHoldemButton, [this]{
            if(onGame) onGame();
        });


        auto texasHoldemText = tgui::Label::create();
        texasHoldemText->setPosition(Constants::gap + 10, 200);
        texasHoldemText->setTextSize(20);
        texasHoldemText->setText("Two cards\nCommunity board\nBest five-card hand.");
        texasHoldemText->setVisible(false);
        gameMenu->add(texasHoldemText);
        texasHoldemText->getRenderer()->setTextColor(color.text);

        texasHoldemButton->onMouseEnter([texasHoldemButton,texasHoldemText, color]{
            texasHoldemButton->setTextPosition({200,50},{0.5f,0.5f});
            texasHoldemButton->getRenderer()->setTextColorDown(sf::Color(0,0,0));
            texasHoldemText->setVisible(true);
        });

        texasHoldemButton->onMousePress([texasHoldemText]{
            texasHoldemText->getRenderer()->setTextColor(sf::Color(0,0,0));
        });

        texasHoldemButton->onMouseRelease([texasHoldemText, color]{
            texasHoldemText->getRenderer()->setTextColor(color.text);
        });

        texasHoldemButton->onMouseLeave([texasHoldemButton,texasHoldemText, color]{
            texasHoldemButton->setTextPosition({200,100},{0.5f,0.5f});
            texasHoldemText->getRenderer()->setTextColor(color.text);
            texasHoldemText->setVisible(false);
        });




        bindSoundOnClick(fiveCardDrawButton, [this]{
            if(onGame) onGame();
        });

        bindSoundOnClick(backIcon, [gameMenu, this, buttons]{
            for(auto x : buttons){
                x->setVisible(true);
            }
            root->remove(gameMenu);
            hideOverlay();
        });


        

    }

