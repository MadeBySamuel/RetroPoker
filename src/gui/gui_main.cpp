#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <vector>



#include <iostream>
int main (){
    using namespace std::chrono_literals;


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

    // auto color_picker =rgb(94, 171, 212));

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

    title->getRenderer()->setTextColor({94, 171, 212});
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
        buttons[i]->getRenderer()->setBackgroundColor({28, 36, 46});
        buttons[i]->getRenderer()->setBackgroundColorHover({36, 46, 58});
        buttons[i]->getRenderer()->setBackgroundColorDown({46, 204, 113});
        buttons[i]->getRenderer()->setBackgroundColorDisabled({60, 60, 60});

        buttons[i]->getRenderer()->setTextColor({235, 239, 244});
        buttons[i]->getRenderer()->setTextColorHover({255, 255, 255});
        buttons[i]->getRenderer()->setTextColorDown({12, 17, 23});
        buttons[i]->getRenderer()->setTextColorDisabled({140, 140, 140});

        buttons[i]->getRenderer()->setBorders(1);

        buttons[i]->setSize({"92%","4%"});
        buttons[i]->getRenderer()->setRoundedBorderRadius(10);

    }


        buttonPlay->onMouseEnter([buttonPlay,buttonSettings, quitButton] {
            buttonPlay->getRenderer()->setTextureHover("assets/images/poker.png");
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
            buttonSettings->getRenderer()->setTextureHover("assets/images/settings_icon.png"); 
            buttonSettings->resizeWithAnimation({"92%", 100}, 120ms);
            quitButton->moveWithAnimation({"5%", "25%"}, 120ms);
        });

        buttonSettings->onMouseLeave([buttonSettings, quitButton] {
            buttonSettings->resizeWithAnimation({"92%", 44}, 120ms);
            quitButton->moveWithAnimation({"5%", "20%"}, 120ms);
        });


        quitButton->onMouseEnter([quitButton] {
            quitButton->getRenderer()->setTextureHover("assets/images/quit_icon.png"); 
            quitButton->resizeWithAnimation({"92%", 100}, 120ms);
        });

        quitButton->onMouseLeave([quitButton] {
            quitButton->resizeWithAnimation({"92%", 44}, 120ms);
        });

        quitButton->onClick([&window,&sideBar,&quitButton] {
            quitButton->setVisible(false);
            auto yes_button = tgui::Button::create("Yes");
            auto no_button = tgui::Button::create("No");



            yes_button->setPosition({"30","20%"});
            no_button->setPosition({"70%","20%"});


            yes_button->onClick([&window]
            {
                window.close();
            });


            no_button->onClick([&sideBar, yes_button, no_button]
            {
                sideBar->remove(yes_button);
                sideBar->remove(no_button);

            });

            sideBar->add(yes_button);
            sideBar->add(no_button);
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
