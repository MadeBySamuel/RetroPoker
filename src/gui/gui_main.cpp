#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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

    tgui::Font::setGlobalFont("assets/fonts/Shelten.ttf");

    // auto color_picker =rgb(212, 160, 23));


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
    gui.add(root);

    auto topBar = tgui::Panel::create({"100%", "7%"});
    topBar->setPosition({"0%", "0%"});
    topBar->getRenderer()-> setBackgroundColor(bg);
    topBar->getRenderer()->setRoundedBorderRadius(20.0);
    root->add(topBar);


    auto sideBar = tgui::Panel::create({"18%", "90%"});
    sideBar->setPosition({"0%", "10%" });
    sideBar->getRenderer()->setBackgroundColor(bg);
    sideBar->getRenderer()->setRoundedBorderRadius(20.0);
    root->add(sideBar);


    auto centerPanel = tgui::Panel::create({"52%", "75%"});
    centerPanel->setPosition({"20%", "15%"});
    centerPanel->getRenderer()->setBackgroundColor(bg);
    centerPanel->getRenderer()->setRoundedBorderRadius(20.0);
    root->add(centerPanel);

    auto rightPanel = tgui::Panel::create({"24%", "75%"});
    rightPanel->setPosition({"74%", "15%"});
    rightPanel->getRenderer()->setBackgroundColor(bg);
    rightPanel->getRenderer()->setRoundedBorderRadius(20.0);
    root->add(rightPanel);


    // titles
    auto title = tgui::Label::create("RetroPoker");
    title->getRenderer()->setFont("assets/fonts/AncientMedium.ttf");

    title->getRenderer()->setTextColor({244, 230, 200});
    title->setPosition({"17%","2%"});
    title->setTextSize(50);
    sideBar->add(title);


    // pictures
    

    auto table_icon = tgui::Picture::create("assets/images/retro_icon.png");
    table_icon->setPosition({"-50%","70%"});
    table_icon->setSize({700,300});
    sideBar->add(table_icon);


    // button 
    auto buttonPlay = tgui::Button::create("Play");
    buttonPlay->setPosition({"5%","10%"});


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
        buttons[i]->getRenderer()->setBackgroundColor(panel);
        buttons[i]->getRenderer()->setBackgroundColorHover(panel);
        buttons[i]->getRenderer()->setBackgroundColorDown(down);
        buttons[i]->getRenderer()->setBackgroundColorDisabled(disabled);

        buttons[i]->getRenderer()->setTextColor(text);
        buttons[i]->getRenderer()->setTextColorHover(text);
        buttons[i]->getRenderer()->setTextColorDown(bg);
        buttons[i]->getRenderer()->setTextColorDisabled(tgui::Color(160, 150, 130));


        buttons[i]->getRenderer()->setBorders(0);

        buttons[i]->setSize({"92%","4%"});
        buttons[i]->getRenderer()->setRoundedBorderRadius(10);

    }


        buttonPlay->onMouseEnter([buttonPlay, buttonSettings, quitButton] {
            buttonPlay->setTextSize(30);
            buttonPlay->resizeWithAnimation({"92%", 100}, 120ms);
            buttonSettings->moveWithAnimation({"5%", "20%"}, 120ms);
            quitButton->moveWithAnimation({"5%", "25%"}, 120ms);
        });

        buttonPlay->onMouseLeave([buttonPlay,buttonSettings, quitButton] {
            buttonPlay->setTextSize(20);
            buttonPlay->resizeWithAnimation({"92%", 44}, 120ms);
            buttonSettings->moveWithAnimation({"5%", "15%"}, 120ms);
            quitButton->moveWithAnimation({"5%", "20%"}, 120ms);
        });


        buttonSettings->onMouseEnter([buttonSettings, quitButton] {
            buttonSettings->setTextSize(30);
            buttonSettings->resizeWithAnimation({"92%", 100}, 120ms);
            quitButton->moveWithAnimation({"5%", "25%"}, 120ms);
        });

        buttonSettings->onMouseLeave([buttonSettings, quitButton] {
            buttonSettings->setTextSize(20);
            buttonSettings->resizeWithAnimation({"92%", 44}, 120ms);
            quitButton->moveWithAnimation({"5%", "20%"}, 120ms);
        });


        quitButton->onMouseEnter([quitButton] {
            quitButton->setTextSize(30);
            quitButton->resizeWithAnimation({"92%", 100}, 120ms);
        });

        quitButton->onMouseLeave([quitButton] {
            quitButton->setTextSize(20);
            quitButton->resizeWithAnimation({"92%", 44}, 120ms);
        });

        quitButton->onClick([&gui,&window,&sideBar,&quitButton,&buttonSettings, &buttonPlay, panel,down,disabled,text,bg, yes_quit] {
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


            yes_button->onClick([&window]
            {
                window.close();
            });

            gui.add(quit_label);
            quit_label->add(yes_button);
            quit_label->add(no_button);


            no_button->onClick([overlay,&gui, quit_label, &sideBar, yes_button, no_button, &quitButton, &buttonSettings, &buttonPlay]
            {
                gui.remove(overlay);
                gui.remove(quit_label);
                quitButton->setVisible(true);
                buttonSettings->setVisible(true);
                buttonPlay->setVisible(true);
            });


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
