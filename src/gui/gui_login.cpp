#include <iostream> 

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <functional>
#include <vector>

class Login {

    private:
        tgui::Gui& gui;
        std::function<void()> onLogin;


    public:
        Login(tgui::Gui& gui, std::function<void()> onLogin) : gui(gui), onLogin(onLogin) {} 

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

        tgui::Font::setGlobalFont("assets/fonts/Shelten.ttf");


        auto root = tgui::Panel::create({"100%", "100%"});
        root->getRenderer()->setBackgroundColor(bg);
        gui.add(root);

        auto centerPanel = tgui::Panel::create({"30%", "40%"});
        centerPanel->setPosition({"35%" , "30%"});
        centerPanel->getRenderer()->setBackgroundColor(panel);
        centerPanel->getRenderer()->setRoundedBorderRadius(20.0);
        root->add(centerPanel);

        auto login_title = tgui::Label::create("User Login");
        login_title->setPosition({"32%", "20%"});
        login_title->setTextSize(40);   
        login_title->getRenderer()->setTextColor(sf::Color(0,0,0));

        centerPanel->add(login_title);



    
        auto username_box = tgui::EditBox::create();
        username_box->setDefaultText("Username");
        username_box->setPosition("15%", "40%");
        centerPanel->add(username_box);

        auto login_icon = tgui::Picture::create("assets/images/login_icon.png");
        login_icon->setPosition("4%", "40%");
        login_icon->setSize("9%", "12%");
        centerPanel->add(login_icon);

        auto lock_icon = tgui::Picture::create("assets/images/lock_icon.png");
        lock_icon->setPosition("4%", "61%");
        lock_icon->setSize("10%", "10%");
        centerPanel->add(lock_icon);



        auto password_box = tgui::EditBox::create();
        password_box->setDefaultText("Password");
        password_box->setPosition("15%", "60%");
        password_box->setPasswordCharacter('*');
        centerPanel->add(password_box);


        std::vector<tgui::EditBox::Ptr> login_boxes;
        
        login_boxes.emplace_back(username_box);
        login_boxes.emplace_back(password_box);


        for(size_t i = 0; i < login_boxes.size(); i++){
            login_boxes[i]->setTextSize(20);
            login_boxes[i]->setSize("80%", "8%");
            login_boxes[i]->getRenderer()->setBackgroundColor(panel);
            login_boxes[i]->getRenderer()->setBorders(0);
            login_boxes[i]->getRenderer()->setBackgroundColorHover(panel);
            login_boxes[i]->getRenderer()->setTextColor(text);
            login_boxes[i]->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
            login_boxes[i]->getRenderer()->setBorderColor(sf::Color(0,0,0));
        }


        auto login_button = tgui::Button::create("Login");
        login_button->setPosition("10%", "80%");
        login_button->setTextSize(14);
        login_button->setSize("80%", "12%");
        login_button->getRenderer()->setBackgroundColor(sf::Color(0,0,0,100));
        login_button->getRenderer()->setTextColor(text);
        login_button->getRenderer()->setTextColorHover(text);
        login_button->getRenderer()->setBackgroundColorDownHover(bg);
        login_button->getRenderer()->setBackgroundColorDown(bg);
        login_button->getRenderer()->setBackgroundColorHover(bg);
        centerPanel->add(login_button);

    
        login_button->onClick([this, username_box, password_box]
        {
            if (username_box->getText().length() == 0 && password_box->getText().length() == 0){
                password_box->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
                username_box->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
            }
            else if(username_box->getText().length() == 0){
                username_box->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
            }
            else if (password_box->getText().length() == 0){
                password_box->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
            }
            else {
                onLogin();
            }
        });

    }

};
