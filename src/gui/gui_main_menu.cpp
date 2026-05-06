#include <iostream> 

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <vector>

#include "game.hpp"

#include "gui_utils.hpp"

class MainMenu{

    private:
        tgui::Gui& gui;
        sf::RenderWindow& window;
        tgui::Panel::Ptr root;
        tgui::Panel::Ptr modalOverlay;

        tgui::Panel::Ptr topBar;
                
        tgui::Label::Ptr time_label;
        std::string last_time_text;
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



        float getGuiWidth() const{
            return static_cast<float>(gui.getWindow()->getSize().x);
        }

        const float getGuiWidth(){
            return static_cast<float>(gui.getWindow()->getSize().x);
        }


        float getGuiHeight() co

        float get

        float 
        const sf::Vector2u windowSize{gui.getWindow()->getSize()};

        const float guiWidth{static_cast<float>(windowSize.x)};
        const float guiHeight{static_cast<float>(windowSize.y)};




    public:
        MainMenu(tgui::Gui& gui, sf::RenderWindow& window) : gui(gui), window(window) {} 


    
        
    tgui::Panel::Ptr overlay() {
        auto overlay = tgui::Panel::create({guiWidth, guiHeight});
        overlay->setPosition({0, 0});
        overlay->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 220));
        return overlay;
    }

    void showOverlay(const float guiWidth, const float guiHeight) {
        if (!root) {
            return;
        }

        hideOverlay();
        modalOverlay = overlay(guiWidth, guiHeight);
        root->add(modalOverlay);
    }

    void hideOverlay() {
        if (!root || !modalOverlay) {
            return;
        }

        root->remove(modalOverlay);
        modalOverlay = nullptr;
    }

    void showMainMenu(){
    gui.removeAllWidgets();

    tgui::Font::setGlobalFont("assets/fonts/Shelten.ttf");

    // auto color_picker =rgb(72, 58, 48));

    const auto windowSize = gui.getWindow()->getSize();
    const float guiWidth = static_cast<float>(windowSize.x);
    const float guiHeight = static_cast<float>(windowSize.y);

    constexpr float topBarHeight = 56.f;
    constexpr float sideBarWidth = 280.f;
    constexpr float sideBarHeight = 360.f;
    constexpr float centerPanelWidth = 900.f;
    constexpr float centerPanelHeight = 760.f;
    constexpr float rightPanelWidth = 420.f;
    constexpr float rightPanelHeight = 760.f;

    root = tgui::Panel::create({guiWidth, guiHeight});
    root->getRenderer()->setBackgroundColor(panel);
    root->getRenderer()->setTextureBackground("assets/images/main_menu4.jpg");
    gui.add(root);

    topBar = tgui::Panel::create({guiWidth, topBarHeight});
    topBar->setPosition({0, 0});
    topBar->getRenderer()-> setBackgroundColor(tgui::Color(0, 0, 0, 140));
    root->add(topBar);

    auto sideBar = tgui::Panel::create({sideBarWidth, sideBarHeight});
    sideBar->setPosition({30, guiHeight - sideBarHeight - 40});
    sideBar->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    sideBar->getRenderer()->setRoundedBorderRadius(20.0);
    sideBar->setVisible(true);
    root->add(sideBar);


    auto centerPanel = tgui::Panel::create({centerPanelWidth, centerPanelHeight});
    centerPanel->setPosition({320, 110});
    centerPanel->getRenderer()->setBackgroundColor(bg);
    centerPanel->getRenderer()->setRoundedBorderRadius(20.0);
    centerPanel->setVisible(false);
    root->add(centerPanel);

    auto rightPanel = tgui::Panel::create({rightPanelWidth, rightPanelHeight});
    rightPanel->setPosition({1245, 110});
    rightPanel->getRenderer()->setBackgroundColor(bg);
    rightPanel->getRenderer()->setRoundedBorderRadius(20.0);
    rightPanel->setVisible(false);
    root->add(rightPanel);


    // titles
    auto title = tgui::Label::create("RetroPoker");
    title->getRenderer()->setFont("assets/fonts/AncientMedium.ttf");
    title->getRenderer()->setTextColor(bg);
    title->setPosition({540, 220});
    title->getRenderer()->setTextOutlineColor(tgui::Color(42, 26, 18));
    title->getRenderer()->setTextOutlineThickness(2.f);                
    title->setTextSize(200);
    root->add(title);

    auto money_lay = tgui::Panel::create({220, topBarHeight});
    money_lay->setPosition({0, 0});
    money_lay->getRenderer()->setBackgroundColor(border);
    money_lay->getRenderer()->setRoundedBorderRadius(10);
    topBar->add(money_lay);


    const int money = Constants::START_PLAYER_MONEY;



    auto money_show = tgui::Label::create("Balance: " + std::to_string(money) + "$" );
    money_show->getRenderer()->setTextColor(text);
    money_show->getRenderer()->setFont("assets/fonts/Shelten.ttf");
    money_show->setPosition({24, 16});
    money_show->setTextSize(20);
    money_lay->add(money_show);








    // pictures
    
/*
    auto table_icon = tgui::Picture::create("assets/images/retro_icon.png");
    table_icon->setPosition({"-50%","70%"});
    table_icon->setSize({700,300});
    sideBar->add(table_icon);
    */

    // button 
    auto buttonPlay = tgui::Button::create("Play");
    buttonPlay->setPosition({25, 120});
    buttonPlay->setTextSize(33);
    sideBar->add(buttonPlay);

    auto buttonSettings = tgui::Button::create("Settings");
    buttonSettings->setPosition({25, 195});
    buttonSettings->setTextSize(30);
    sideBar->add(buttonSettings);

    auto quitButton = tgui::Button::create("Quit");
    quitButton->setPosition({25, 270});
    quitButton->setTextSize(30);
    sideBar->add(quitButton);




    std::vector<tgui::Button::Ptr> buttons;
    buttons.emplace_back(buttonPlay);
    buttons.emplace_back(buttonSettings);
    buttons.emplace_back(quitButton);


    for (size_t i = 0; i < buttons.size(); i++){
        buttons[i]->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
        buttons[i]->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
        buttons[i]->getRenderer()->setBackgroundColorDown(tgui::Color::Transparent);
        buttons[i]->getRenderer()->setBackgroundColorDisabled(tgui::Color::Transparent);


        buttons[i]->getRenderer()->setTextStyleHover(tgui::TextStyle::Underlined);

        buttons[i]->getRenderer()->setTextColor(text);
        buttons[i]->getRenderer()->setTextColorHover(down);
        buttons[i]->getRenderer()->setTextColorDown(down);
        buttons[i]->getRenderer()->setTextColorDisabled(tgui::Color(160, 150, 130));


        buttons[i]->getRenderer()->setBorders(0);

        if(buttons[i] != buttonPlay ){
            buttons[i]->setSize({240, 48});
        }
        else{
           buttons[i]->setSize({240, 62}); 
        }
        buttons[i]->getRenderer()->setRoundedBorderRadius(0);

    }

        buttonPlay->onClick([this, guiWidth, guiHeight]{
            showOverlay(guiWidth, guiHeight);
            game_menu();
        });


        quitButton->onClick([this, quitButton, buttonSettings, buttonPlay, guiWidth, guiHeight] {
    
            showOverlay(guiWidth, guiHeight);


            auto quit_label = tgui::ChildWindow::create("Do you really want to quit ?", tgui::ChildWindow::TitleButton::None);
            quit_label->setPosition({(guiWidth - 420.f) / 2.f, (guiHeight - 190.f) / 2.f});
            quit_label->setTextSize(20);
            quit_label->setTitleTextSize(17);
            quit_label->setSize({420, 190});
            quit_label->setPositionLocked(true);
            quit_label->getRenderer()->setBackgroundColor(bg);


            quit_label->getRenderer()->setBorderBelowTitleBar(1.00);


            quitButton->setVisible(false);
            buttonSettings->setVisible(false);
            buttonPlay->setVisible(false);
            

            auto yes_button = tgui::Button::create("Yes");
            auto no_button = tgui::Button::create("No");


            std::vector<tgui::Button::Ptr> yes_no_buttons;

            yes_no_buttons.emplace_back(yes_button);
            yes_no_buttons.emplace_back(no_button);


            for (size_t i = 0; i < yes_no_buttons.size(); i++){
                yes_no_buttons[i]->setSize({130, 58});
                yes_no_buttons[i]->getRenderer()->setBackgroundColor(panel);
                yes_no_buttons[i]->getRenderer()->setBackgroundColorHover(panel);

                if (yes_no_buttons[i] == no_button){
                    yes_no_buttons[i]->getRenderer()->setBackgroundColorDown(down);
                }
                else {
                    yes_no_buttons[i]->getRenderer()->setBackgroundColorDown(yes_quit);
                }

                yes_no_buttons[i]->getRenderer()->setBackgroundColorDisabled(disabled);

                yes_no_buttons[i]->getRenderer()->setTextColor(text);
                yes_no_buttons[i]->getRenderer()->setTextColorHover(text);
                yes_no_buttons[i]->getRenderer()->setTextColorDown(bg);

    
                yes_no_buttons[i]->getRenderer()->setBorders(0);

                yes_no_buttons[i]->getRenderer()->setRoundedBorderRadius(10);
            }

            

            no_button->setPosition({45, 60});
            yes_button->setPosition({235, 60});


            yes_button->onClick([this]
            {
                window.close();
            });

            gui.add(quit_label);
            quit_label->add(yes_button);
            quit_label->add(no_button);


            no_button->onClick([this, quit_label,guiWidth, guiHeight, quitButton, buttonSettings, buttonPlay]
            {
                hideOverlay();
                gui.remove(quit_label);
                quitButton->setVisible(true);
                buttonSettings->setVisible(true);
                buttonPlay->setVisible(true);
            });


        });
}

    void time(){
        update_time(time_label, last_time_text, text, topBar);
    }

    void game_menu(){

        if (!root) {
            return;
        }

        const auto windowSize = gui.getWindow()->getSize();
        const float guiWidth = static_cast<float>(windowSize.x);
        const float guiHeight = static_cast<float>(windowSize.y);

        constexpr float topBarHeight = 56.f;
        constexpr float loginPanelWidth = 620.f*1.5;
        constexpr float loginPanelHeight = 440.f*1.5;
        constexpr float forgotPanelWidth = 620.f;
        constexpr float forgotPanelHeight = 350.f;
        constexpr float signUpPanelWidth = 620.f;
        constexpr float signUpPanelHeight = 550.f;

        constexpr float sideBarWidth = 280.f;
        constexpr float sideBarHeight = 360.f;
        constexpr float centerPanelWidth = 900.f;
        constexpr float centerPanelHeight = 760.f;
        constexpr float rightPanelWidth = 420.f;
        constexpr float rightPanelHeight = 760.f;
        

        constexpr float buttonWidth = 400.f;
        constexpr float buttonHeight = 200.f;



        auto game_menu = tgui::Panel::create({loginPanelWidth, loginPanelHeight});
        game_menu->setPosition({(guiWidth - loginPanelWidth) / 2.f, (guiHeight - loginPanelHeight) / 2.f});
        game_menu->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 200));
        game_menu->getRenderer()->setRoundedBorderRadius(20.0);
        
        root->add(game_menu);


        const float gap = 40.f;
        const float totalWidth = buttonWidth + gap + buttonWidth;
        const float startX = (loginPanelWidth - totalWidth) / 2.f;


        auto game_menu_title = tgui::Label::create("Game Modes");
        game_menu_title->setTextSize(45);
        game_menu_title->getRenderer()->setTextColor(text);
        game_menu_title->setPosition({(loginPanelWidth - game_menu_title->getSize().x) / 2.f, 20.f});
        game_menu->add(game_menu_title);



        auto back_menu_button = tgui::Button::create("Back to Main Menu");
        back_menu_button->setTextSize(45);
        back_menu_button->getRenderer()->setTextColor(text);
        back_menu_button->setPosition({startX, 500.f});
        game_menu->add(back_menu_button);

        back_menu_button->onClick([game_menu, this]{
            root->remove(game_menu);
            hideOverlay();
        });


        auto texasHoldem = tgui::Button::create("Texas Hold'em");
        texasHoldem->getRenderer()->setTextColor(text);
        game_menu->add(texasHoldem);

        auto five_card_draw = tgui::Button::create("5-Card draw");
        five_card_draw->getRenderer()->setTextColor(text);
        game_menu->add(five_card_draw);

        texasHoldem->setPosition({startX,100.f});
        five_card_draw->setPosition({startX + buttonWidth + gap, 100.f});


        // auto arrow_back = tgui::Picture::create("assets/images/arrow_left.png");
        // five_card_draw->setSize({100,100});

        // game_menu->add(arrow_back);



        std::vector<tgui::Button::Ptr> buttons;

        buttons.emplace_back(texasHoldem);
        buttons.emplace_back(five_card_draw);
        buttons.emplace_back(back_menu_button);




        for(size_t i = 0; i < buttons.size(); i++){
            buttons[i]->setTextSize(33);
            if(buttons[i] == back_menu_button){
                buttons[i]->setSize({loginPanelWidth - startX * 2, buttonHeight/2});
            }
            else {
                buttons[i]->setSize({buttonWidth,buttonHeight});
            }

            buttons[i]->getRenderer()->setBackgroundColor(bg);
            buttons[i]->getRenderer()->setBackgroundColorDownHover(border);
            buttons[i]->getRenderer()->setBackgroundColorDown(border);
            buttons[i]->getRenderer()->setBackgroundColorHover(border);

            buttons[i]->getRenderer()->setTextColor(text);
            buttons[i]->getRenderer()->setTextColorDisabled(tgui::Color(160, 150, 130));
            buttons[i]->getRenderer()->setTextColorHover(sf::Color(0,0,0));
            buttons[i]->getRenderer()->setTextColorDown(sf::Color(0,0,0));
            buttons[i]->getRenderer()->setBorders(0);

        }



        

















        



    }


};
