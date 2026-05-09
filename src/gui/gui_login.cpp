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
#include "constants.hpp"
#include "constants.hpp"


class Login : public Gui_base {

    private:
        sf::Music& music;

        tgui::Label::Ptr time_label;
        std::string last_time_text;
        tgui::Color text;
           


        sqlite3* db = nullptr;

    public:
        Login(tgui::Gui& gui, sf::Music& music) : Gui_base(gui), music(music) {} 
        std::function<void(std::string)> onLogin;
        std::string logged_username;





    void login_screen(){

        GuiTheme color;

    
        if (sqlite3_open("users.db", &db) == SQLITE_OK){
                bool created = auth::createUsersTable(db);
                if (created == 1) std::cout << "Table created " << std::endl;
                if (created == 0) std::cout << "Table is not created " << std::endl;
        }


        gui.removeAllWidgets();

        tgui::Font::setGlobalFont("assets/fonts/Shelten.ttf");



        auto root = tgui::Panel::create({getGuiWidth(), getGuiWidth()});
        root->getRenderer()->setTextureBackground("assets/images/backgrounds/login_background.jpeg");
        gui.add(root);

        auto centerPanel = tgui::Panel::create({Constants::loginPanelWidth, Constants::loginPanelHeight});
        centerPanel->setPosition({(getGuiWidth() - Constants::loginPanelWidth) / 2.f, (getGuiHeight() - Constants::loginPanelHeight) / 2.f});
        centerPanel->getRenderer()->setBackgroundColor(sf::Color(45, 35, 28, 230));
        centerPanel->getRenderer()->setRoundedBorderRadius(20.0);
        root->add(centerPanel);

        auto topBar = bar(root);


        auto login_title = tgui::Label::create("Welcome!");
        login_title->setPosition({55, 35});
        login_title->setTextSize(45);   
        login_title->getRenderer()->setTextColor(color.text);

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

            sign_up_pass[i]->getRenderer()->setTextColor(color.text);
            sign_up_pass[i]->getRenderer()->setTextColorHover(color.down);
            sign_up_pass[i]->getRenderer()->setTextColorDown(color.down);
            sign_up_pass[i]->getRenderer()->setTextColorFocused(color.text);
            sign_up_pass[i]->getRenderer()->setTextColorDownFocused(color.down);

            sign_up_pass[i]->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
            sign_up_pass[i]->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
            sign_up_pass[i]->getRenderer()->setBackgroundColorDown(tgui::Color::Transparent);
            sign_up_pass[i]->getRenderer()->setBackgroundColorFocused(tgui::Color::Transparent);
            sign_up_pass[i]->getRenderer()->setBackgroundColorDownFocused(tgui::Color::Transparent);
            sign_up_pass[i]->getRenderer()->setBackgroundColorDisabled(tgui::Color::Transparent);

            sign_up_pass[i]->getRenderer()->setBorderColor(color.text);
            sign_up_pass[i]->getRenderer()->setBorderColorHover(color.down);
            sign_up_pass[i]->getRenderer()->setBorderColorDown(color.down);
            sign_up_pass[i]->getRenderer()->setBorderColorDownHover(color.down);
            sign_up_pass[i]->getRenderer()->setBorderColorFocused(color.text);
            sign_up_pass[i]->getRenderer()->setBorderColorDownFocused(color.down);

        }



        auto rememberBox = tgui::CheckBox::create();
        rememberBox->setPosition({50, 390});
        rememberBox->setSize({20, 20});
        rememberBox->setText("");
        rememberBox->setTextSize(20);
        centerPanel->add(rememberBox);


        rememberBox->getRenderer()->setTextColor(color.text);
        rememberBox->getRenderer()->setTextColorHover(color.down);

        rememberBox->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
        rememberBox->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
        rememberBox->getRenderer()->setBackgroundColorDisabled(tgui::Color::Transparent);

        rememberBox->getRenderer()->setBorderColor(color.text);
        rememberBox->getRenderer()->setBorderColorHover(color.down);
        rememberBox->getRenderer()->setBorderColorFocused(color.text);

        rememberBox->getRenderer()->setCheckColor(color.text);
        rememberBox->getRenderer()->setCheckColorHover(color.down);




        auto rememberLabel = tgui::Label::create("Remember me");
        rememberLabel->setPosition({80, 390});
        rememberLabel->setTextSize(18);
        rememberLabel->getRenderer()->setTextColor(color.down);
        centerPanel->add(rememberLabel);
        rememberLabel->getRenderer()->setTextColor(color.text);
        rememberLabel->getRenderer()->setBorderColor(color.text);


        rememberLabel->onClick([rememberBox]{
            rememberBox->setChecked(!rememberBox->isChecked());
        });

        rememberLabel->onMouseEnter([color, rememberLabel, rememberBox]{
            rememberLabel->getRenderer()->setTextColor(color.down);
            rememberBox->getRenderer()->setTextColorHover(color.down);
            rememberBox->getRenderer()->setBorderColorHover(color.down);
            rememberBox->getRenderer()->setCheckColorHover(color.down);
        });

        rememberLabel->onMouseLeave([color, rememberLabel]{
            rememberLabel->getRenderer()->setTextColor(color.text);
        });

        rememberLabel->onMousePress([color, rememberLabel,rememberBox]{
            rememberLabel->getRenderer()->setTextColor(color.down);
            rememberBox->getRenderer()->setTextColorHover(color.down);
            rememberBox->getRenderer()->setBorderColorHover(color.down);
            rememberBox->getRenderer()->setCheckColorHover(color.down);
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
            login_boxes[i]->getRenderer()->setTextColor(color.text);
            login_boxes[i]->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
            login_boxes[i]->getRenderer()->setBorderColor(sf::Color(0,0,0));
            login_boxes[i]->getRenderer()->setDefaultTextColor(sf::Color(125, 125, 125));
        }


        auto login_button = tgui::Button::create("LOGIN");
        login_button->setPosition({70, 300});
        login_button->setTextSize(28);
        login_button->setSize({500, 60});
        login_button->getRenderer()->setBackgroundColor(sf::Color(0,0,0,100));
        login_button->getRenderer()->setTextColor(color.text);
        login_button->getRenderer()->setTextColorHover(color.accent);
        login_button->getRenderer()->setBackgroundColorDownHover(color.bg);
        login_button->getRenderer()->setBackgroundColorDown(color.bg);
        login_button->getRenderer()->setBackgroundColorHover(color.bg);
        centerPanel->add(login_button);


        login_button->onClick([this, username_box, password_box, color, centerPanel]{
            play_button();
            
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

                auto loginresult = auth::loginUser(db, username, password);


                std::cout << "status: " << (int)loginresult.status << "\n";
                std::cout << "username vstup: " << username << "\n";
                std::cout << "username vystup: " << loginresult.username << "\n";


                auto wrongLabel = tgui::Label::create("");
                wrongLabel->setTextSize(17);
                wrongLabel->setPosition({70, 270});
                wrongLabel->getRenderer()->setTextColor(sf::Color(160, 42, 16));
                centerPanel->add(wrongLabel);


                if (loginresult.status == auth::LoginResult::Status::DatabaseError){
                    std::cout << "Database error "; // toto by sa nemalo stať 
                }
                else if(loginresult.status == auth::LoginResult::Status::NotFound){
                    wrongLabel->setText("User was not found, please sign up");
                }

                else if (loginresult.status == auth::LoginResult::Status::WrongPassword){
                    wrongLabel->setText("Wrong password");
                }
                else {
                    if (onLogin) onLogin(loginresult.username);
                }
            }
        });



        forgot_password->onClick([this, root, color, centerPanel]{
            play_button();
            centerPanel->setVisible(false);
            

            auto forgot_password_panel = tgui::Panel::create({Constants::forgotPanelWidth, Constants::forgotPanelHeight});
            forgot_password_panel->setPosition({(getGuiWidth() - Constants::forgotPanelWidth) / 2.f, (getGuiHeight() - Constants::forgotPanelHeight) / 2.f});
            forgot_password_panel->getRenderer()->setBackgroundColor(color.panel2);
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
            back_to_login->getRenderer()->setTextColorHover(color.down);
            back_to_login->getRenderer()->setBorderColorHover(color.down);

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
            next_button->getRenderer()->setTextColorHover(color.down);
            next_button->getRenderer()->setBorderColorHover(color.down);

            forgot_password_panel->add(next_button);

            next_button->onClick([root, forgot_password_panel, centerPanel, forgot_password_box, color, this]{
                if(forgot_password_box->getText().length() == 0){
                    forgot_password_box->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
                }                
                else {

                root->remove(forgot_password_panel);

                auto code_email_panel = tgui::Panel::create({Constants::forgotPanelWidth, Constants::forgotPanelHeight});
                code_email_panel->setPosition({(getGuiWidth() - Constants::forgotPanelWidth) / 2.f, (getGuiHeight() - Constants::forgotPanelHeight) / 2.f});
                code_email_panel->getRenderer()->setBackgroundColor(color.panel2);
                code_email_panel->getRenderer()->setRoundedBorderRadius(20.0);
                
                root->add(code_email_panel);

                }
            });

            });

        sign_up->onClick([this, centerPanel, root, color]{
            centerPanel->setVisible(false);
            
            auto signUpPanel = tgui::Panel::create({Constants::signUpPanelWidth, Constants::signUpPanelHeight});
            signUpPanel->setPosition({(getGuiWidth() - Constants::signUpPanelWidth) / 2.f , (getGuiHeight() - Constants::signUpPanelHeight) / 2.f});
            signUpPanel->getRenderer()->setBackgroundColor(color.panel2);
            signUpPanel->getRenderer()->setRoundedBorderRadius(20.0);
            root->add(signUpPanel);

            auto login_title = tgui::Label::create("Sign up");
            login_title->setPosition({40, 30});
            login_title->setTextSize(45);   
            login_title->getRenderer()->setTextColor(color.text);
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
            back_to_login->getRenderer()->setTextColor(color.text);
            back_to_login->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
            back_to_login->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
            back_to_login->getRenderer()->setBackgroundColorDown(tgui::Color::Transparent);
            back_to_login->getRenderer()->setBackgroundColorDisabled(tgui::Color::Transparent);
            back_to_login->getRenderer()->setTextColorHover(color.down);
            back_to_login->getRenderer()->setBorderColorHover(color.down);

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
            login_boxes[i]->getRenderer()->setTextColor(color.text);
            login_boxes[i]->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
            login_boxes[i]->getRenderer()->setBorderColor(sf::Color(0,0,0));
        }



            auto sign_up_button = tgui::Button::create("Create account");
            sign_up_button->setPosition({70, 380});
            sign_up_button->setTextSize(28);
            sign_up_button->setSize({500, 60});
            sign_up_button->getRenderer()->setBackgroundColor(sf::Color(0,0,0,100));
            sign_up_button->getRenderer()->setTextColor(color.text);
            sign_up_button->getRenderer()->setTextColorHover(color.down);
            sign_up_button->getRenderer()->setBackgroundColorDownHover(color.bg);
            sign_up_button->getRenderer()->setBackgroundColorDown(color.bg);
            sign_up_button->getRenderer()->setBackgroundColorHover(color.bg);
            signUpPanel->add(sign_up_button);



            auto wrongLabel = tgui::Label::create("");
            wrongLabel->setTextSize(17);
            wrongLabel->setPosition({90, 420});
            wrongLabel->getRenderer()->setTextColor(sf::Color(160, 42, 16));
            signUpPanel->add(wrongLabel);


        sign_up_button->onClick([this, username_box, email_box, password_box, wrongLabel]{
            play_button();

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

                auth::LoginResult register_user = auth::registerUser(db, username, email, password);

                
                if(register_user.status == auth::LoginResult::Status::DatabaseError){
                    std::cout << "Database problem "<< std::endl;
                }
                else if (register_user.status == auth::LoginResult::Status::Empty){
                    std::cout << "Empty" << std::endl;
                }
                else if (register_user.status == auth::LoginResult::Status::UserAlreadyExist){
                    wrongLabel->setText("User already exists");
                }
                else if (register_user.status == auth::LoginResult::Status::Pass){
                    onLogin(register_user.username);
                }
                else {
                    std::cout << "wtf" << std::endl;
                }
            }

            });
        });


        time_label = tgui::Label::create("");
        time_label->setTextSize(30);
        time_label->setPosition({getGuiWidth() - 125.f, 10});
        time_label->getRenderer()->setTextColor(text);
        
        topBar->add(time_label);

        
    }


    

};
