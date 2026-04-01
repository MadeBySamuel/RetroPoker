#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <vector>



#include <iostream>
int main (){


    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    sf:: VideoMode desktop = sf::VideoMode::getDesktopMode();

    sf::RenderWindow window(desktop, "Retro Poker", sf::Style::Default, sf::State::Fullscreen, settings);
    window.setFramerateLimit(60);

    tgui::Gui gui{window};

  tgui::Font::setGlobalFont("assets/fonts/Roboto-Bold.ttf");

   // root 
    auto root = tgui::Panel::create({"100%", "100%"});
    root->getRenderer()->setBackgroundColor({66, 32, 32});
    gui.add(root);

    // bars 

    // auto color_picker =rgb(17, 18, 35));

    /*
    
    background      = {12, 17, 23};
    sidebar         = {22, 28, 36};
    topbar          = {18, 24, 32};
    panel           = {26, 34, 44};
    panelSoft       = {31, 40, 52};
    accent          = {46, 204, 113};   // zelená
    textPrimary     = {235, 239, 244};
    textSecondary   = {150, 160, 175};
    border          = {45, 55, 70};
    hover           = {36, 46, 58};

    */

    auto topBar = tgui::Panel::create({"100%", "7%"});
    topBar->getRenderer()-> setBackgroundColor({18, 24, 32});
    topBar->setPosition({"0%", "0%"});
    root->add(topBar);


    auto sideBar = tgui::Panel::create({"18%", "90%"});
    sideBar->setPosition({"0%", "10%" });
    sideBar->getRenderer()->setBackgroundColor({22, 28, 36});
    sideBar->getRenderer()->setRoundedBorderRadius(20.0);
    root->add(sideBar);


    auto centerPanel = tgui::Panel::create({"52%", "75%"});
    centerPanel->setPosition({"20%", "15%"});
    root->add(centerPanel);

    auto rightPanel = tgui::Panel::create({"24%", "75%"});
    rightPanel->setPosition({"74%", "15%"});
    root->add(rightPanel);


    // titles
    auto title = tgui::Label::create("RetroPoker");
    title->getRenderer()->setFont("assets/fonts/AncientMedium.ttf");
    title->setPosition({"2%","2%"});
    title->setTextSize(50);
    sideBar->add(title);


    // pictures

    auto table_icon = tgui::Picture::create("assets/images/table_icon.png");
    table_icon->setPosition({"1%","10%"});
    table_icon->setSize({"8%","100%"});
    topBar->add(table_icon);



    // button 
    auto buttonPlay = tgui::Button::create("Play");
    buttonPlay->setPosition({"5%","10%"});
    buttonPlay->setTextSize(20);
    sideBar->add(buttonPlay);

    auto buttonSettings = tgui::Button::create("Settings");
    buttonSettings->setPosition({"5%","15%"});
    buttonSettings->setTextSize(18);
    sideBar->add(buttonSettings);

    auto quitButton = tgui::Button::create("Quit");
    quitButton->setPosition({"5%","20%"});
    quitButton->setTextSize(18);
    sideBar->add(quitButton);




    std::vector<tgui::Button::Ptr> buttons;
    buttons.emplace_back(buttonPlay);
    buttons.emplace_back(buttonSettings);
    buttons.emplace_back(quitButton);

    for (int i = 0; i < buttons.size(); i++){
        buttons[i]->getRenderer()-> setBackgroundColor({0, 0, 0});
        buttons[i]->getRenderer()-> setTextColor({255,255,255});
        buttons[i]->setSize({"88%","4%"});
        buttons[i]->getRenderer()->setBorders(0);
        buttons[i]->getRenderer()->setBackgroundColorHover({55, 194, 64});
    }



    while(window.isOpen()){
        while(const auto event = window.pollEvent()){

            gui.handleEvent(*event);


            if(event->is<sf::Event::Closed>()){
                window.close();
            }
        }

        
    

        

        gui.draw();
        window.display();
    }
    return 0;
}
