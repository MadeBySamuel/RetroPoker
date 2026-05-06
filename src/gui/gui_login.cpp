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


#include "gui_utils.hpp"



class Login {

    private:
        tgui::Gui& gui;
        std::function<void()> onLogin;
        sf::Music& music;

        tgui::Panel::Ptr topBar;
        tgui::Label::Ptr time_label;
        std::string last_time_text;
        tgui::Color text;
           


        sqlite3* db = nullptr;

    public:
        Login(tgui::Gui& gui, std::function<void()> onLogin, sf::Music& music) : gui(gui), onLogin(onLogin), music(music) {} 



    void login_screen(){

       

        if (sqlite3_open("users.db", &db) == SQLITE_OK){
                bool created = auth::createUsersTable(db);
                if (created == 1) std::cout << "Table created " << std::endl;
                if (created == 0) std::cout << "Table is not created " << std::endl;
        }

        tgui::Color bg(28, 24, 20);        
        tgui::Color panel(58, 46, 38, 230);    
        tgui::Color panel2(72, 58, 48);    
        tgui::Color accent(212, 160, 23);  
        tgui::Color text(244, 230, 200);
        tgui::Color hover(201, 106, 43);   
        tgui::Color border(120, 92, 60);   
        tgui::Color down(160, 110, 25);    
        tgui::Color disabled(80, 70, 60);  
        tgui::Color yes_quit(133, 22, 22);



        gui.removeAllWidgets();

        this->text = text;

        tgui::Font::setGlobalFont("assets/fonts/Shelten.ttf");

        const auto windowSize = gui.getWindow()->getSize();
        const float guiWidth = static_cast<float>(windowSize.x);
        const float guiHeight = static_cast<float>(windowSize.y);

        constexpr float topBarHeight = 56.f;
        constexpr float loginPanelWidth = 620.f;
        constexpr float loginPanelHeight = 440.f;
        constexpr float forgotPanelWidth = 620.f;
        constexpr float forgotPanelHeight = 350.f;
        constexpr float signUpPanelWidth = 620.f;
        constexpr float signUpPanelHeight = 550.f;

        auto root = tgui::Panel::create({guiWidth, guiHeight});
        root->getRenderer()->setTextureBackground("assets/images/login_background.jpeg");
        gui.add(root);

        auto centerPanel = tgui::Panel::create({loginPanelWidth, loginPanelHeight});
        centerPanel->setPosition({(guiWidth - loginPanelWidth) / 2.f, (guiHeight - loginPanelHeight) / 2.f});
        centerPanel->getRenderer()->setBackgroundColor(panel);
        centerPanel->getRenderer()->setRoundedBorderRadius(20.0);
        root->add(centerPanel);

        topBar = tgui::Panel::create({guiWidth, topBarHeight});
        topBar->setPosition({0, 0});
        topBar->getRenderer()-> setBackgroundColor(panel);
        root->add(topBar);


        auto login_title = tgui::Label::create("Welcome!");
        login_title->setPosition({55, 35});
        login_title->setTextSize(45);   
        login_title->getRenderer()->setTextColor(text);

        centerPanel->add(login_title);



        auto username_box = tgui::EditBox::create();
        username_box->setDefaultText("Username");
        username_box->setPosition({100, 145});
        centerPanel->add(username_box);


        auto password_box = tgui::EditBox::create();
        password_box->setDefaultText("Password");
        password_box->setPosition({100, 225});
        password_box->setPasswordCharacter('*');
        centerPanel->add(password_box);



        auto eye = tgui::Picture::create(tgui::Texture("assets/images/eye_off.png", {}, {}, true));
        eye->setPosition({500, 225});
        eye->setSize({32, 32});
        centerPanel->add(eye);


        eye->onClick([eye, password_box]{
            if (password_box->getPasswordCharacter() == '*'){
                eye->getRenderer()->setTexture("assets/images/eye_on.png");
                password_box->setPasswordCharacter(false);
            }
            else {
                eye->getRenderer()->setTexture("assets/images/eye_off.png");
                password_box->setPasswordCharacter('*');
            }
        });



        auto login_icon = tgui::Picture::create("assets/images/login_icon.png");
        login_icon->setPosition({55, 147});
        login_icon->setSize({28, 28});
        centerPanel->add(login_icon);

        auto lock_icon = tgui::Picture::create("assets/images/lock_icon.png");
        lock_icon->setPosition({52, 222});
        lock_icon->setSize({34, 34});
        centerPanel->add(lock_icon);


        std::vector<tgui::Button::Ptr> sign_up_pass;




        auto sign_up = tgui::Button::create("Sign up");
        sign_up->setPosition({450, 30});
        sign_up->setSize({120, 28});
        
        centerPanel->add(sign_up);


        

        auto forgot_password = tgui::Button::create("Forgot password");
        forgot_password->setPosition({400, 390});
        forgot_password->setSize({170, 28});
        centerPanel->add(forgot_password);


        sign_up_pass.emplace_back(sign_up);
        sign_up_pass.emplace_back(forgot_password);

        for(size_t i = 0; i < sign_up_pass.size(); i++){
            sign_up_pass[i]->setTextSize(18);

            if (sign_up_pass[i] == forgot_password)
                sign_up_pass[i]->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
            else
                sign_up_pass[i]->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 0});

            sign_up_pass[i]->getRenderer()->setTextColor(text);
            sign_up_pass[i]->getRenderer()->setTextColorHover(down);
            sign_up_pass[i]->getRenderer()->setTextColorDown(down);
            sign_up_pass[i]->getRenderer()->setTextColorFocused(text);
            sign_up_pass[i]->getRenderer()->setTextColorDownFocused(down);

            sign_up_pass[i]->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
            sign_up_pass[i]->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
            sign_up_pass[i]->getRenderer()->setBackgroundColorDown(tgui::Color::Transparent);
            sign_up_pass[i]->getRenderer()->setBackgroundColorFocused(tgui::Color::Transparent);
            sign_up_pass[i]->getRenderer()->setBackgroundColorDownFocused(tgui::Color::Transparent);
            sign_up_pass[i]->getRenderer()->setBackgroundColorDisabled(tgui::Color::Transparent);

            sign_up_pass[i]->getRenderer()->setBorderColor(text);
            sign_up_pass[i]->getRenderer()->setBorderColorHover(down);
            sign_up_pass[i]->getRenderer()->setBorderColorDown(down);
            sign_up_pass[i]->getRenderer()->setBorderColorDownHover(down);
            sign_up_pass[i]->getRenderer()->setBorderColorFocused(text);
            sign_up_pass[i]->getRenderer()->setBorderColorDownFocused(down);

        

        }



        auto rememberBox = tgui::CheckBox::create();
        rememberBox->setPosition({50, 390});
        rememberBox->setSize({20, 20});
        rememberBox->setText("");
        rememberBox->setTextSize(20);
        centerPanel->add(rememberBox);


        rememberBox->getRenderer()->setTextColor(text);
        rememberBox->getRenderer()->setTextColorHover(down);

        rememberBox->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
        rememberBox->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
        rememberBox->getRenderer()->setBackgroundColorDisabled(tgui::Color::Transparent);

        rememberBox->getRenderer()->setBorderColor(text);
        rememberBox->getRenderer()->setBorderColorHover(down);
        rememberBox->getRenderer()->setBorderColorFocused(text);

        rememberBox->getRenderer()->setCheckColor(text);
        rememberBox->getRenderer()->setCheckColorHover(down);




        auto rememberLabel = tgui::Label::create("Remember me");
        rememberLabel->setPosition({80, 390});
        rememberLabel->setTextSize(18);
        rememberLabel->getRenderer()->setTextColor(down);
        centerPanel->add(rememberLabel);
        rememberLabel->getRenderer()->setTextColor(text);
        rememberLabel->getRenderer()->setBorderColor(text);


        rememberLabel->onClick([rememberBox]{
            rememberBox->setChecked(!rememberBox->isChecked());
        });

        rememberLabel->onMouseEnter([down, rememberLabel, rememberBox]{
            rememberLabel->getRenderer()->setTextColor(down);
            rememberBox->getRenderer()->setTextColorHover(down);
            rememberBox->getRenderer()->setBorderColorHover(down);
            rememberBox->getRenderer()->setCheckColorHover(down);
        });

        rememberLabel->onMouseLeave([text, rememberLabel]{
            rememberLabel->getRenderer()->setTextColor(text);
        });

        rememberLabel->onMousePress([down, rememberLabel,rememberBox]{
            rememberLabel->getRenderer()->setTextColor(down);
            rememberBox->getRenderer()->setTextColorHover(down);
            rememberBox->getRenderer()->setBorderColorHover(down);
            rememberBox->getRenderer()->setCheckColorHover(down);
        });

        



       

        std::vector<tgui::EditBox::Ptr> login_boxes;
        
        login_boxes.emplace_back(username_box);
        login_boxes.emplace_back(password_box);


        for(size_t i = 0; i < login_boxes.size(); i++){
            login_boxes[i]->setTextSize(20);
            login_boxes[i]->setSize({450, 42});
            login_boxes[i]->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
            login_boxes[i]->getRenderer()->setBorders(0);
            login_boxes[i]->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
            login_boxes[i]->getRenderer()->setTextColor(text);
            login_boxes[i]->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
            login_boxes[i]->getRenderer()->setBorderColor(sf::Color(0,0,0));
            login_boxes[i]->getRenderer()->setDefaultTextColor(sf::Color(125, 125, 125));
        }


        auto login_button = tgui::Button::create("LOGIN");
        login_button->setPosition({70, 300});
        login_button->setTextSize(28);
        login_button->setSize({500, 60});
        login_button->getRenderer()->setBackgroundColor(sf::Color(0,0,0,100));
        login_button->getRenderer()->setTextColor(text);
        login_button->getRenderer()->setTextColorHover(down);
        login_button->getRenderer()->setBackgroundColorDownHover(bg);
        login_button->getRenderer()->setBackgroundColorDown(bg);
        login_button->getRenderer()->setBackgroundColorHover(bg);
        centerPanel->add(login_button);
     



        auto volume_icon = tgui::Picture::create(tgui::Texture("assets/images/volume/volume_muted.png", {}, {}, true));
        volume_icon->setPosition({guiWidth - 200.f, 12});
        volume_icon->setSize({40, 31});
        topBar->add(volume_icon);
        


        auto volume_panel = tgui::Panel::create({300, 90});
        volume_panel->setPosition({guiWidth - 330.f, topBarHeight + 10.f});
        volume_panel->getRenderer()->setBackgroundColor(panel);
        volume_panel->getRenderer()->setRoundedBorderRadius(20.0);
        root->add(volume_panel);
        volume_panel->setVisible(false);


        auto volume_label = tgui::Label::create("Sound");
        volume_label->setPosition({18, 12});
        volume_label->getRenderer()->setTextColor(text);
        volume_label->setTextSize(20);
        volume_panel->add(volume_label);


        auto volume_slider = tgui::Slider::create();
        volume_slider->setPosition({18, 52});
        volume_slider->setMinimum(0);
        volume_slider->setMaximum(100);
        volume_slider->setValue(0);
        volume_slider->setSize({264, 12});

        volume_slider->getRenderer()->setThumbColor(text);
        volume_slider->getRenderer()->setTrackColor(text);

        volume_slider->getRenderer()->setThumbColorHover(down);
        volume_slider->getRenderer()->setTrackColorHover(text);

        volume_panel->add(volume_slider);


        volume_slider->onValueChange([this, volume_icon](float value){
            if (value == 0) volume_icon->getRenderer()->setTexture("assets/images/volume/volume_muted_click.png");
            else if (value > 0 && value < 30) volume_icon->getRenderer()->setTexture("assets/images/volume/volume_first_click.png");
            else if (value >= 30 && value < 60) volume_icon->getRenderer()->setTexture("assets/images/volume/volume_second_click.png");
            else volume_icon->getRenderer()->setTexture("assets/images/volume/volume_full_click.png");
            music.setVolume(value);
        });
    


        volume_icon->onClick([this,volume_panel,volume_icon, volume_slider]{
        volume_panel->setVisible(!volume_panel->isVisible());

        if (volume_panel->isVisible()){
            if(volume_slider->getValue() == 0) volume_icon->getRenderer()->setTexture("assets/images/volume/volume_muted_click.png");
            else if (volume_slider->getValue() > 0 && volume_slider->getValue() < 30) volume_icon->getRenderer()->setTexture("assets/images/volume/volume_first_click.png");
            else if (volume_slider->getValue() >= 30 && volume_slider->getValue() < 60) volume_icon->getRenderer()->setTexture("assets/images/volume/volume_second_click.png");
            else volume_icon->getRenderer()->setTexture("assets/images/volume/volume_full_click.png");
        }
        else {
            if(volume_slider->getValue() == 0) volume_icon->getRenderer()->setTexture("assets/images/volume/volume_muted.png");
            else if (volume_slider->getValue() > 0 && volume_slider->getValue() < 30) volume_icon->getRenderer()->setTexture("assets/images/volume/volume_first.png");
            else if (volume_slider->getValue() >= 30 && volume_slider->getValue() < 60) volume_icon->getRenderer()->setTexture("assets/images/volume/volume_second.png");
            else volume_icon->getRenderer()->setTexture("assets/images/volume/volume_full.png");
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
                wrongLabel->setPosition({70, 270});
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



        forgot_password->onClick([this, root, panel, text, centerPanel, guiWidth, guiHeight, forgotPanelWidth, forgotPanelHeight, down]{
            centerPanel->setVisible(false);
            

            auto forgot_password_panel = tgui::Panel::create({forgotPanelWidth, forgotPanelHeight});
            forgot_password_panel->setPosition({(guiWidth - forgotPanelWidth) / 2.f, (guiHeight - forgotPanelHeight) / 2.f});
            forgot_password_panel->getRenderer()->setBackgroundColor(panel);
            forgot_password_panel->getRenderer()->setRoundedBorderRadius(20.0);
            root->add(forgot_password_panel);

            auto forgot_password_label = tgui::Label::create("Write email of your account");
            forgot_password_label->setPosition({40, 55});
            forgot_password_label->setTextSize(30);   
            forgot_password_label->getRenderer()->setTextColor(text);
            forgot_password_panel->add(forgot_password_label);


            auto forgot_password_box = tgui::EditBox::create();
            forgot_password_box->setDefaultText("email");
            forgot_password_box->setPosition({100, 145});
            forgot_password_box->setTextSize(20);
            forgot_password_box->setSize({450, 42});
            forgot_password_box->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
            forgot_password_box->getRenderer()->setBorders(0);
            forgot_password_box->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
            forgot_password_box->getRenderer()->setTextColor(text);
            forgot_password_box->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
            forgot_password_box->getRenderer()->setBorderColor(sf::Color(0,0,0));
            forgot_password_box->getRenderer()->setDefaultTextColor(sf::Color(125, 125, 125));

            forgot_password_panel->add(forgot_password_box);



            auto email_icon = tgui::Picture::create("assets/images/email_icon.png");
            email_icon->setPosition({40, 150});
            email_icon->setSize({32, 25});
            forgot_password_panel->add(email_icon);


            auto back_to_login = tgui::Button::create("Back to login");
            back_to_login->setPosition({50, 300});
            back_to_login->setSize({160, 30}); 

            back_to_login->setTextSize(18);
            back_to_login->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
            back_to_login->getRenderer()->setTextColor(text);
            back_to_login->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
            back_to_login->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
            back_to_login->getRenderer()->setBackgroundColorDown(tgui::Color::Transparent);
            back_to_login->getRenderer()->setBackgroundColorDisabled(tgui::Color::Transparent);
            back_to_login->getRenderer()->setTextColorHover(down);
            back_to_login->getRenderer()->setBorderColorHover(down);

            forgot_password_panel->add(back_to_login);

            back_to_login->onClick([root, forgot_password_panel, centerPanel]{
                root->remove(forgot_password_panel);
                centerPanel->setVisible(true);
            });



            auto next_button = tgui::Button::create("Next");
            next_button->setPosition({400, 300});
            next_button->setSize({160, 30}); 
            next_button->setTextSize(18);
            next_button->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
            next_button->getRenderer()->setTextColor(text);
            next_button->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
            next_button->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
            next_button->getRenderer()->setBackgroundColorDown(tgui::Color::Transparent);
            next_button->getRenderer()->setBackgroundColorDisabled(tgui::Color::Transparent);
            next_button->getRenderer()->setTextColorHover(down);
            next_button->getRenderer()->setBorderColorHover(down);

            forgot_password_panel->add(next_button);

            next_button->onClick([root, forgot_password_panel, centerPanel, forgot_password_box,guiWidth,guiHeight, panel]{
                if(forgot_password_box->getText().length() == 0){
                    forgot_password_box->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
                }                
                else {

                root->remove(forgot_password_panel);

                auto code_email_panel = tgui::Panel::create({forgotPanelWidth, forgotPanelHeight});
                code_email_panel->setPosition({(guiWidth - forgotPanelWidth) / 2.f, (guiHeight - forgotPanelHeight) / 2.f});
                code_email_panel->getRenderer()->setBackgroundColor(panel);
                code_email_panel->getRenderer()->setRoundedBorderRadius(20.0);
                
                root->add(code_email_panel);

                }
            });

            });

        sign_up->onClick([this, centerPanel, root, panel, text, bg,down, guiWidth, guiHeight, signUpPanelWidth, signUpPanelHeight]{
            centerPanel->setVisible(false);
            
            auto signUpPanel = tgui::Panel::create({signUpPanelWidth, signUpPanelHeight});
            signUpPanel->setPosition({(guiWidth - signUpPanelWidth) / 2.f , (guiHeight - signUpPanelHeight) / 2.f});
            signUpPanel->getRenderer()->setBackgroundColor(panel);
            signUpPanel->getRenderer()->setRoundedBorderRadius(20.0);
            root->add(signUpPanel);

            auto login_title = tgui::Label::create("Sign up");
            login_title->setPosition({40, 30});
            login_title->setTextSize(45);   
            login_title->getRenderer()->setTextColor(text);
            signUpPanel->add(login_title);


            auto username_box = tgui::EditBox::create();
            username_box->setDefaultText("Username");
            username_box->setPosition({100, 145});
            signUpPanel->add(username_box);


            auto email_box = tgui::EditBox::create();
            email_box->setDefaultText("Email");
            email_box->setPosition({100, 225});
            signUpPanel->add(email_box);

            auto password_box = tgui::EditBox::create();
            password_box->setDefaultText("Password");
            password_box->setPosition({100, 305});
            signUpPanel->add(password_box);


            auto username_icon = tgui::Picture::create("assets/images/login_icon.png");
            username_icon->setPosition({50, 150});
            username_icon->setSize({32, 28});
            signUpPanel->add(username_icon);


            auto email_icon = tgui::Picture::create("assets/images/email_icon.png");
            email_icon->setPosition({50, 230});
            email_icon->setSize({32, 24});
            signUpPanel->add(email_icon);


            // treba upravit 
            auto password_icon = tgui::Picture::create("assets/images/lock_icon.png");
            password_icon->setPosition({50,310});
            password_icon->setSize({32, 28});
            signUpPanel->add(password_icon);

            

            auto back_to_login = tgui::Button::create("Back to login");
            back_to_login->setPosition({400, 500});
            back_to_login->setSize({160, 30}); 
            back_to_login->setTextSize(18);
            back_to_login->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
            back_to_login->getRenderer()->setTextColor(text);
            back_to_login->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
            back_to_login->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
            back_to_login->getRenderer()->setBackgroundColorDown(tgui::Color::Transparent);
            back_to_login->getRenderer()->setBackgroundColorDisabled(tgui::Color::Transparent);
            back_to_login->getRenderer()->setTextColorHover(down);
            back_to_login->getRenderer()->setBorderColorHover(down);

            signUpPanel->add(back_to_login);


            auto eye = tgui::Picture::create(tgui::Texture("assets/images/eye_off.png", {}, {}, true));
            eye->setPosition({500, 300});
            eye->setSize({32, 32});
            signUpPanel->add(eye);


            eye->onClick([eye, password_box]{
                if (password_box->getPasswordCharacter() == '*'){
                    eye->getRenderer()->setTexture("assets/images/eye_on.png");
                    password_box->setPasswordCharacter(false);
                }
                else {
                    eye->getRenderer()->setTexture("assets/images/eye_off.png");
                    password_box->setPasswordCharacter('*');
                }
            });

            back_to_login->onClick([root, signUpPanel, centerPanel]{
                root->remove(signUpPanel);
                centerPanel->setVisible(true);
            });

            std::vector<tgui::EditBox::Ptr> login_boxes;
        
            login_boxes.emplace_back(username_box);
            login_boxes.emplace_back(email_box);
            login_boxes.emplace_back(password_box);

        for(size_t i = 0; i < login_boxes.size(); i++){
            login_boxes[i]->setTextSize(20);
            login_boxes[i]->setSize({450, 42});
            login_boxes[i]->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
            login_boxes[i]->getRenderer()->setBorders(0);
            login_boxes[i]->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
            login_boxes[i]->getRenderer()->setTextColor(text);
            login_boxes[i]->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
            login_boxes[i]->getRenderer()->setBorderColor(sf::Color(0,0,0));
        }



            auto sign_up_button = tgui::Button::create("Create account");
            sign_up_button->setPosition({70, 380});
            sign_up_button->setTextSize(28);
            sign_up_button->setSize({500, 60});
            sign_up_button->getRenderer()->setBackgroundColor(sf::Color(0,0,0,100));
            sign_up_button->getRenderer()->setTextColor(text);
            sign_up_button->getRenderer()->setTextColorHover(down);
            sign_up_button->getRenderer()->setBackgroundColorDownHover(bg);
            sign_up_button->getRenderer()->setBackgroundColorDown(bg);
            sign_up_button->getRenderer()->setBackgroundColorHover(bg);
            signUpPanel->add(sign_up_button);



            auto wrongLabel = tgui::Label::create("");
            wrongLabel->setTextSize(17);
            wrongLabel->setPosition({90, 420});
            wrongLabel->getRenderer()->setTextColor(sf::Color(160, 42, 16));
            signUpPanel->add(wrongLabel);


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


        time_label = tgui::Label::create("");
        time_label->setTextSize(30);
        time_label->setPosition({guiWidth - 125.f, 10});
        time_label->getRenderer()->setTextColor(text);
        
        topBar->add(time_label);

        
    }


    void time(){
        update_time(time_label, last_time_text, text, topBar);
    }


};
