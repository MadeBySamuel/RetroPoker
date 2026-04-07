#include <iostream> 

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <vector>

#include "game.hpp"

class MainMenu{

    private:
        tgui::Gui& gui;
        sf::RenderWindow& window;


    public:
        MainMenu(tgui::Gui& gui, sf::RenderWindow& window) : gui(gui), window(window) {} 

    void showMainMenu(){

    tgui::Font::setGlobalFont("assets/fonts/Shelten.ttf");

    // auto color_picker =rgb(212, 152, 23));


    

    tgui::Color bg(28, 24, 20);        
    tgui::Color panel(58, 46, 38);    
    tgui::Color panel2(72, 58, 48);    
    tgui::Color accent(212, 160, 23);  
    tgui::Color text(244, 230, 200);   
    tgui::Color hover(201, 106, 43);   
    tgui::Color border(120, 92, 60);   
    tgui::Color down(160, 110, 25);    
    tgui::Color disabled(80, 70, 60);  
    tgui::Color yes_quit(133, 22, 22);



    auto root = tgui::Panel::create({"100%", "100%"});
    root->getRenderer()->setBackgroundColor(panel);
    root->getRenderer()->setTextureBackground("assets/images/main_menu4.jpg");
    gui.add(root);

    auto topBar = tgui::Panel::create({"100%", "5%"});
    topBar->setPosition({"0%", "0%"});
    topBar->getRenderer()-> setBackgroundColor(tgui::Color(0, 0, 0, 140));
    topBar->getRenderer()->setRoundedBorderRadius(20.0);
    root->add(topBar);

    auto sideBar = tgui::Panel::create({"15%", "40%"});
    sideBar->setPosition({"0%", "60%" });
    sideBar->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    sideBar->getRenderer()->setRoundedBorderRadius(20.0);
    sideBar->setVisible(true);
    root->add(sideBar);


    auto centerPanel = tgui::Panel::create({"52%", "75%"});
    centerPanel->setPosition({"20%", "15%"});
    centerPanel->getRenderer()->setBackgroundColor(bg);
    centerPanel->getRenderer()->setRoundedBorderRadius(20.0);
    centerPanel->setVisible(false);
    root->add(centerPanel);

    auto rightPanel = tgui::Panel::create({"24%", "75%"});
    rightPanel->setPosition({"74%", "15%"});
    rightPanel->getRenderer()->setBackgroundColor(bg);
    rightPanel->getRenderer()->setRoundedBorderRadius(20.0);
    rightPanel->setVisible(false);
    root->add(rightPanel);


    // titles
    auto title = tgui::Label::create("RetroPoker");
    title->getRenderer()->setFont("assets/fonts/AncientMedium.ttf");

    title->getRenderer()->setTextColor(bg);
    title->setPosition({"28%","30%"});
    title->getRenderer()->setTextOutlineColor(tgui::Color(42, 26, 18));
    title->getRenderer()->setTextOutlineThickness(2.f);                
    title->setTextSize(200);
    root->add(title);

    auto money_lay = tgui::Panel::create({"10%","100%"});
    money_lay->setPosition({"0%", "0%"});
    money_lay->getRenderer()->setBackgroundColor(border);
    money_lay->getRenderer()->setRoundedBorderRadius(10);
    topBar->add(money_lay);


    std::unique_ptr<Game> game = std::make_unique<Game>();
    game->game();
    game->getPlayers();

    auto& players = game->getPlayers();
    int money = 0;

    for(size_t i = 0; i < players.size();i++){
        if(players[i]->getName() == "Player"){
            money = players[i]->getMoney();
        } 
    }



    auto money_show = tgui::Label::create("Balance: " + std::to_string(money) + "$" );
    money_show->getRenderer()->setTextColor(text);
    money_show->getRenderer()->setFont("assets/fonts/Shelten.ttf");
    money_show->setPosition({"40","40%"});
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
    buttonPlay->setPosition({"10%","50%"});
    buttonPlay->setTextSize(33);

    sideBar->add(buttonPlay);

    auto buttonSettings = tgui::Button::create("Settings");
    buttonSettings->setPosition({"10%","60%"});
    buttonSettings->setTextSize(30);
    sideBar->add(buttonSettings);

    auto quitButton = tgui::Button::create("Quit");
    quitButton->setPosition({"10%","70%"});
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
            buttons[i]->setSize({"92%","4%"});
        }
        else{
           buttons[i]->setSize({"92%","6%"}); 
        }
        buttons[i]->getRenderer()->setRoundedBorderRadius(0);

    }

        quitButton->onClick([this,&sideBar,&quitButton,&buttonSettings, &buttonPlay, panel,down,disabled,text,bg, yes_quit] {
            auto overlay = tgui::Panel::create({"100%", "100%"});
            overlay->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 220));
            gui.add(overlay);


            auto quit_label = tgui::ChildWindow::create("Do you really want to quit ?", tgui::ChildWindow::TitleButton::None);
            quit_label->setPosition({"41%","40%"});
            quit_label->setTextSize(20);
            quit_label->setTitleTextSize(17);
            quit_label->setSize({"19%","11%"});
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
                yes_no_buttons[i]->setSize({"35%","50%"});
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

            

            no_button->setPosition({"30","20%"});
            yes_button->setPosition({"55%","20%"});


            yes_button->onClick([this]
            {
                window.close();
            });

            gui.add(quit_label);
            quit_label->add(yes_button);
            quit_label->add(no_button);


            no_button->onClick([overlay,this, quit_label, &sideBar, yes_button, no_button, &quitButton, &buttonSettings, &buttonPlay]
            {
                gui.remove(overlay);
                gui.remove(quit_label);
                quitButton->setVisible(true);
                buttonSettings->setVisible(true);
                buttonPlay->setVisible(true);
            });


        });


        auto label = tgui::Label::create("");
        topBar->add(label);
        label->setTextSize(20);
        label->setPosition({"90%","30%"});
        label->getRenderer()->setTextColor(text);


}
};
