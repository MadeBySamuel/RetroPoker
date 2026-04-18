#include <iostream> 

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <functional>
#include <vector>

#include "auth.hpp"
#include <argon2.h>
#include <sqlite3.h>
#include "auth.hpp"




class Login {

    private:
        tgui::Gui& gui;
        std::function<void()> onLogin;
        sf::Music& music;
        sqlite3* db = nullptr;

    public:
        Login(tgui::Gui& gui, std::function<void()> onLogin, sf::Music& music) : gui(gui), onLogin(onLogin), music(music) {} 


    void login_screen(){

       

        if (sqlite3_open("users.db", &db) == SQLITE_OK){
                bool created = auth::createUsersTable(db);
                if (created == 1) std::cout << "Table created " << std::endl;
                if (created == 0) std::cout << "Table is not created " << std::endl;
        }



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

        auto login_title = tgui::Label::create("Welcome!");
        login_title->setPosition({"10%", "10%"});
        login_title->setTextSize(45);   
        login_title->getRenderer()->setTextColor(text);

        centerPanel->add(login_title);



        auto username_box = tgui::EditBox::create();
        username_box->setDefaultText("Username");
        username_box->setPosition("15%", "30%");
        centerPanel->add(username_box);


        auto password_box = tgui::EditBox::create();
        password_box->setDefaultText("Password");
        password_box->setPosition("15%", "50%");
        password_box->setPasswordCharacter('*');

        password_box->getRenderer()->setBackgroundColor(panel);

        centerPanel->add(password_box);



        auto login_icon = tgui::Picture::create("assets/images/login_icon.png");
        login_icon->setPosition("4%", "30%");
        login_icon->setSize("7%", "9%");
        centerPanel->add(login_icon);

        auto lock_icon = tgui::Picture::create("assets/images/lock_icon.png");
        lock_icon->setPosition("2%", "50%");
        lock_icon->setSize("10%", "10%");
        centerPanel->add(lock_icon);




        std::vector<tgui::Button::Ptr> sign_up_pass;


        auto sign_up = tgui::Button::create("Sign up");
        sign_up->setPosition("10%", "90%");
        centerPanel->add(sign_up);


        auto forgot_password = tgui::Button::create("Forgot password");
        forgot_password->setPosition("70%", "90%");
        centerPanel->add(forgot_password);


        sign_up_pass.emplace_back(sign_up);
        sign_up_pass.emplace_back(forgot_password);


        for(size_t i = 0; i < sign_up_pass.size(); i++){
            sign_up_pass[i]->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
            sign_up_pass[i]->getRenderer()->setTextColor(text);
            sign_up_pass[i]->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
            sign_up_pass[i]->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
            sign_up_pass[i]->getRenderer()->setBackgroundColorDown(tgui::Color::Transparent);
            sign_up_pass[i]->getRenderer()->setBackgroundColorDisabled(tgui::Color::Transparent);
            sign_up_pass[i]->getRenderer()->setTextColorHover(down);
            sign_up_pass[i]->getRenderer()->setBorderColorHover(down);
        }


        forgot_password->onClick([this, root, panel, text, centerPanel]{
            centerPanel->setVisible(false);

            auto centerPanel = tgui::Panel::create({"30%", "40%"});
            centerPanel->setPosition({"25%" , "30%"});
            centerPanel->getRenderer()->setBackgroundColor(panel);
            centerPanel->getRenderer()->setRoundedBorderRadius(20.0);
            root->add(centerPanel);

            auto forgot_password_label = tgui::Label::create("Write email of your account");
            forgot_password_label->setPosition({"20%", "20%"});
            forgot_password_label->setTextSize(30);   
            forgot_password_label->getRenderer()->setTextColor(text);
            centerPanel->add(forgot_password_label);


            auto forgot_password_box = tgui::EditBox::create();
            forgot_password_box->setDefaultText("email");
            forgot_password_box->setSize("80%", "8%");
            forgot_password_box->setPosition({"10%", "40%"});

            forgot_password_box->setTextSize(20);
            forgot_password_box->getRenderer()->setBackgroundColor(panel);
            forgot_password_box->getRenderer()->setBorders(0);
            forgot_password_box->getRenderer()->setBackgroundColorHover(panel);
            forgot_password_box->getRenderer()->setTextColor(text);
            forgot_password_box->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
            forgot_password_box->getRenderer()->setBorderColor(text);

            centerPanel->add(forgot_password_box);

            });

        sign_up->onClick([this, centerPanel, root, panel, text, bg,down]{
            centerPanel->setVisible(false);
            
            auto centerPanel = tgui::Panel::create({"30%", "40%"});
            centerPanel->setPosition({"35%" , "30%"});
            centerPanel->getRenderer()->setBackgroundColor(panel);
            centerPanel->getRenderer()->setRoundedBorderRadius(20.0);
            root->add(centerPanel);

            auto login_title = tgui::Label::create("Sign up");
            login_title->setPosition({"10%", "10%"});
            login_title->setTextSize(45);   
            login_title->getRenderer()->setTextColor(text);
            centerPanel->add(login_title);


            auto username_box = tgui::EditBox::create();
            username_box->setDefaultText("Username");
            username_box->setPosition("10%", "30%");
            centerPanel->add(username_box);


            auto email_box = tgui::EditBox::create();
            email_box->setDefaultText("Email");
            email_box->setPosition("10%", "40%");
            centerPanel->add(email_box);

            auto password_box = tgui::EditBox::create();
            password_box->setDefaultText("Password");
            password_box->setPosition("10%", "50%");
            centerPanel->add(password_box);


            std::vector<tgui::EditBox::Ptr> login_boxes;
        
            login_boxes.emplace_back(username_box);
            login_boxes.emplace_back(email_box);
            login_boxes.emplace_back(password_box);

        for(size_t i = 0; i < login_boxes.size(); i++){
            login_boxes[i]->setTextSize(20);
            login_boxes[i]->setSize("80%", "8%");
            login_boxes[i]->getRenderer()->setBackgroundColor(panel);
            login_boxes[i]->getRenderer()->setBorders(0);
            login_boxes[i]->getRenderer()->setBackgroundColorHover(panel);
            login_boxes[i]->getRenderer()->setTextColor(text);
            login_boxes[i]->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
            login_boxes[i]->getRenderer()->setBorderColor(text);
        }



            auto sign_up_button = tgui::Button::create("Sign up");
            sign_up_button->setPosition("10%", "70%");
            sign_up_button->setTextSize(20);
            sign_up_button->setSize("80%", "12%");
            sign_up_button->getRenderer()->setBackgroundColor(sf::Color(0,0,0,100));
            sign_up_button->getRenderer()->setTextColor(text);
            sign_up_button->getRenderer()->setTextColorHover(down);
            sign_up_button->getRenderer()->setBackgroundColorDownHover(bg);
            sign_up_button->getRenderer()->setBackgroundColorDown(bg);
            sign_up_button->getRenderer()->setBackgroundColorHover(bg);
            centerPanel->add(sign_up_button);



            auto wrongLabel = tgui::Label::create("");
            wrongLabel->setTextSize(17);
            wrongLabel->setPosition("10%", "63%");
            wrongLabel->getRenderer()->setTextColor(sf::Color(160, 42, 16));
            centerPanel->add(wrongLabel);


            sign_up_button->onClick([this, username_box, email_box, password_box, wrongLabel]{
            if (username_box->getText().length() == 0 && password_box->getText().length() == 0 && username_box->getText().length() == 0){
                email_box->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
                password_box->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
                username_box->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
            }
            else if(username_box->getText().length() == 0){
                username_box->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
            }
            else if (password_box->getText().length() == 0){
                password_box->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
            }
            else if (email_box->getText().length() == 0){
                email_box->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
            }

            else{

                std::string  email = email_box->getText().toStdString();
                std::string  password = password_box->getText().toStdString();
                std::string  username = username_box->getText().toStdString();

                auth::LoginResult register_user = auth::registerUser(db, email, username, password);

                if(register_user == auth::LoginResult::DatabaseError){
                    std::cout << "Database problem "<< std::endl;
                }
                else if (register_user == auth::LoginResult::Empty){
                    std::cout << "Empty" << std::endl;
                }
                else if (register_user == auth::LoginResult::UserAlreadyExist){
                    wrongLabel->setText("User already exists");
                }
                else if (register_user == auth::LoginResult::Pass){
                    onLogin();
                }
                else {
                    std::cout << "wtf" << std::endl;
                }
            }

            });
        });







        std::vector<tgui::EditBox::Ptr> login_boxes;
        
        login_boxes.emplace_back(username_box);
        login_boxes.emplace_back(password_box);


        for(size_t i = 0; i < login_boxes.size(); i++){
            login_boxes[i]->setTextSize(20);
            login_boxes[i]->setSize("70%", "8%");
            login_boxes[i]->getRenderer()->setBackgroundColor(panel);
            login_boxes[i]->getRenderer()->setBorders(0);
            login_boxes[i]->getRenderer()->setBackgroundColorHover(panel);
            login_boxes[i]->getRenderer()->setTextColor(text);
            login_boxes[i]->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
            login_boxes[i]->getRenderer()->setBorderColor(sf::Color(0,0,0));
        }


        auto login_button = tgui::Button::create("Login");
        login_button->setPosition("10%", "70%");
        login_button->setTextSize(25);
        login_button->setSize("80%", "12%");
        login_button->getRenderer()->setBackgroundColor(sf::Color(0,0,0,100));
        login_button->getRenderer()->setTextColor(text);
        login_button->getRenderer()->setTextColorHover(down);
        login_button->getRenderer()->setBackgroundColorDownHover(bg);
        login_button->getRenderer()->setBackgroundColorDown(bg);
        login_button->getRenderer()->setBackgroundColorHover(bg);
        centerPanel->add(login_button);
     



        auto volume_icon = tgui::Picture::create(tgui::Texture("assets/images/volume/volume_full.png", {}, {}, true));
        volume_icon->setPosition("96%", "3%");
        volume_icon->setSize(40,31);
        root->add(volume_icon);
        


        auto volume_panel = tgui::Panel::create({"20%", "10%"});
        volume_panel->setPosition({"80%" , "8%"});
        volume_panel->getRenderer()->setBackgroundColor(panel);
        volume_panel->getRenderer()->setRoundedBorderRadius(20.0);
        root->add(volume_panel);
        volume_panel->setVisible(false);


        auto volume_label = tgui::Label::create("Sound");
        volume_label->setPosition({"10%" , "10%"});
        volume_label->getRenderer()->setTextColor(text);
        volume_label->setTextSize(20);
        volume_panel->add(volume_label);


        auto volume_slider = tgui::Slider::create();
        volume_slider->setPosition("10%", "50%");
        volume_slider->setMinimum(0);
        volume_slider->setMaximum(100);
        volume_slider->setValue(100);
        volume_slider->setSize("80%", "10%");

        volume_slider->getRenderer()->setThumbColor(text);
        volume_slider->getRenderer()->setTrackColor(text);

        volume_slider->getRenderer()->setThumbColorHover(down);
        volume_slider->getRenderer()->setTrackColorHover(text);

        volume_panel->add(volume_slider);


        volume_slider->onValueChange([this](float value){
            music.setVolume(value);
        });
    


        volume_icon->onClick([this,volume_panel,volume_icon, volume_slider]{
        volume_panel->setVisible(!volume_panel->isVisible());

        if (volume_slider->isVisible()){
            volume_icon->getRenderer()->setTexture("assets/images/volume/volume_full_click.png");
        }
        else {
            volume_icon->getRenderer()->setTexture("assets/images/volume/volume_full.png");
        }
        });



        login_button->onClick([this, username_box, password_box, text, centerPanel]
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
                std::string username = username_box->getText().toStdString();
                std::string password = password_box->getText().toStdString();

                auth::LoginResult loginresult = auth::loginUser(db, username, password);


                auto wrongLabel = tgui::Label::create("");
                wrongLabel->setTextSize(17);
                wrongLabel->setPosition("10%", "63%");
                wrongLabel->getRenderer()->setTextColor(sf::Color(160, 42, 16));
                centerPanel->add(wrongLabel);

                if (loginresult == auth::LoginResult::DatabaseError){
                    std::cout << "Database error "; // toto by sa nemalo stať 
                }
                else if(loginresult == auth::LoginResult::NotFound){
                    wrongLabel->setText("User was not found, please sign up");
                }

                else if (loginresult == auth::LoginResult::WrongPassword){
                    wrongLabel->setText("Wrong password");
                }
                else {
                    onLogin();
                }
            }
        });

    }

};
