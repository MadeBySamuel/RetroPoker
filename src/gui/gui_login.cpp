#include <iostream> 

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <vector>

class Login {

    private:
        tgui::Gui& gui;


    public:
        Login(tgui::Gui& gui) : gui(gui) {} 

    void login_screen(){
        gui.removeAllWidgets();

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
        root->getRenderer()->setBackgroundColor(bg);
        root->getRenderer()->setTextureBackground("assets/images/login_main_menu.jpg");
        gui.add(root);

        auto centerPanel = tgui::Panel::create({"20%", "20%"});
        centerPanel->setPosition({"40%", "40%"});
        centerPanel->getRenderer()->setBackgroundColor(sf::Color(0,0,0,100));
        centerPanel->getRenderer()->setRoundedBorderRadius(20.0);
        root->add(centerPanel);




        auto username_box = tgui::EditBox::create();
        username_box->setDefaultText("Username");
        username_box->setPosition("10%", "40%");
        centerPanel->add(username_box);

        auto login_icon = tgui::Picture::create("assets/images/login_icon.png");
        login_icon->setPosition("40%", "15%");
        login_icon->setSize("14%", "18%");
        centerPanel->add(login_icon);


        auto password_box = tgui::EditBox::create();
        password_box->setDefaultText("Password");
        password_box->setPosition("10%", "60%");
        centerPanel->add(password_box);


        std::vector<tgui::EditBox::Ptr> login_boxes;
        
        login_boxes.emplace_back(username_box);
        login_boxes.emplace_back(password_box);


        for(size_t i = 0; i < login_boxes.size(); i++){
            login_boxes[i]->setTextSize(10);
            login_boxes[i]->setSize("80%", "12%");
            login_boxes[i]->getRenderer()->setRoundedBorderRadius(10);
            login_boxes[i]->getRenderer()->setBackgroundColor(sf::Color(0,0,0,100));
            login_boxes[i]->getRenderer()->setBorders(0);
        }






        auto login_button = tgui::Button::create("Login");
        login_button->setPosition("40%", "80%");
        login_button->setTextSize(14);
        login_button->setSize("20%", "12%");
        login_button->getRenderer()->setRoundedBorderRadius(10);
        login_button->getRenderer()->setBackgroundColor(sf::Color(0,0,0,100));
        centerPanel->add(login_button);








    }

};
