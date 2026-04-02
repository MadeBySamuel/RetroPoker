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
    root->getRenderer()->setBackgroundColor({12, 17, 23});
    gui.add(root);

    // bars 

    // auto color_picker =rgb(86, 128, 91));

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
    topBar->getRenderer()-> setBackgroundColor({16, 22, 29});
    topBar->setPosition({"0%", "0%"});
    root->add(topBar);


    auto sideBar = tgui::Panel::create({"18%", "90%"});
    sideBar->setPosition({"0%", "10%" });
    sideBar->getRenderer()->setBackgroundColor({20, 26, 34});
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

    title->getRenderer()->setTextColor({255,255,255});
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
    buttonPlay->setTextSize(18);
    sideBar->add(buttonPlay);



    using namespace std::chrono_literals;

    buttonPlay->getRenderer()->setBackgroundColor({28, 36, 46});
    buttonPlay->getRenderer()->setBackgroundColorHover({36, 46, 58});
    buttonPlay->getRenderer()->setBackgroundColorDown({46, 204, 113});
    buttonPlay->getRenderer()->setBackgroundColorDisabled({60, 60, 60});
    buttonPlay->getRenderer()->setTextureHover("assets/images/poker.png");

    buttonPlay->getRenderer()->setTextColor({235, 239, 244});
    buttonPlay->getRenderer()->setTextColorHover({255, 255, 255});
    buttonPlay->getRenderer()->setTextColorDown({12, 17, 23});
    buttonPlay->getRenderer()->setTextColorDisabled({140, 140, 140});

    buttonPlay->getRenderer()->setBorders(0);


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


        auto button = buttons[i];
        button->onMouseEnter([button] {
            button->resizeWithAnimation({"92%", 100}, 120ms);
        });

        button->onMouseLeave([button] {
            button->resizeWithAnimation({"88%", 44}, 120ms);
        });

        buttons[i]->getRenderer()-> setBackgroundColor({0, 0, 0});
        buttons[i]->getRenderer()-> setTextColor({153, 160, 157});
        buttons[i]->setSize({"84%","4%"});
        buttons[i]->getRenderer()->setBackgroundColorHover({86, 128, 91});
        buttons[i]->getRenderer()->setTextColorHover({0, 0, 0});
        buttons[i]->getRenderer()->setRoundedBorderRadius(10);
    }


        buttonPlay->onMouseEnter([buttonPlay,buttonSettings, quitButton] {
            buttonPlay->resizeWithAnimation({"92%", 100}, 120ms);
            buttonSettings->moveWithAnimation({"5%", "20%"}, 120ms);
            quitButton->moveWithAnimation({"5%", "25%"}, 120ms);
        });

        buttonPlay->onMouseLeave([buttonPlay,buttonSettings, quitButton] {
            buttonPlay->resizeWithAnimation({"92%", 44}, 120ms);
            buttonSettings->moveWithAnimation({"5%", "15%"}, 120ms);
            quitButton->moveWithAnimation({"5%", "20%"}, 120ms);
        });


        buttonSettings->onMouseEnter([buttonSettings, quitButton] {
            buttonSettings->resizeWithAnimation({"92%", 100}, 120ms);
            quitButton->moveWithAnimation({"5%", "25%"}, 120ms);
        });

        buttonSettings->onMouseLeave([buttonSettings, quitButton] {
            buttonSettings->resizeWithAnimation({"92%", 44}, 120ms);
            quitButton->moveWithAnimation({"5%", "20%"}, 120ms);
        });






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
