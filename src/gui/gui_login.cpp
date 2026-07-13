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


#include "gui_login.hpp"
#include "gui_utils.hpp"
#include "constants.hpp"


Login::Login(tgui::Gui& gui, sf::RenderWindow& window, sf::Music& music) : Gui_base(gui), window(window), music(music) {}


tgui::Panel::Ptr Login::makePanel(float x, float y){
   
    GuiTheme color;
    auto panel = tgui::Panel::create({x, y});
    panel->setPosition({(getGuiWidth() - x) / 2.f, (getGuiHeight() - y) / 2.f});
    panel->getRenderer()->setBackgroundColor(color.centerpanel);
    panel->getRenderer()->setRoundedBorderRadius(20.0);
    root->add(panel);

    return panel;
}

tgui::Button::Ptr makeBackToLoginButton(float x, float y){

    GuiTheme color;

    auto backToLogin = tgui::Button::create("Back to login");
    backToLogin->setPosition({x,y});
    backToLogin->setSize({160, 30}); 
    backToLogin->setTextSize(18);
    backToLogin->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
    backToLogin->getRenderer()->setTextColor(color.text);
    backToLogin->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    backToLogin->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
    backToLogin->getRenderer()->setBackgroundColorDown(tgui::Color::Transparent);
    backToLogin->getRenderer()->setBackgroundColorDisabled(tgui::Color::Transparent);
    backToLogin->getRenderer()->setTextColorHover(color.down);
    backToLogin->getRenderer()->setBorderColorHover(color.down);

    return backToLogin;
}

void Login::login_screen(){

        if (sqlite3_open("users.db", &db) == SQLITE_OK){
            bool database_created = auth::createUsersTable(db);
        }
        else {
            std::cerr << "Failed to open Database" << sqlite3_errmsg(db) << std::endl;
        }

        GuiTheme color;
        
        gui.removeAllWidgets();

        tgui::Font::setGlobalFont("assets/fonts/Shelten.ttf");



        root = tgui::Panel::create({getGuiWidth(), getGuiWidth()});
        root->getRenderer()->setTextureBackground("assets/images/backgrounds/login_background.jpeg");
        gui.add(root);

    
        auto topBar = bar();

        volume(topBar, root, color, music);
    
        centerPanel = makePanel(Constants::loginPanelWidth, Constants::loginPanelHeight);

        auto loginTitle = tgui::Label::create("Welcome!");
        loginTitle->setPosition({55, 35});
        loginTitle->setTextSize(45);   
        loginTitle->getRenderer()->setTextColor(color.text);
        centerPanel->add(loginTitle);


        auto login_icon = tgui::Picture::create("assets/images/login_icon.png");
        login_icon->setPosition({55, 147});
        login_icon->setSize({28, 28});
        centerPanel->add(login_icon);

        auto lock_icon = tgui::Picture::create("assets/images/lock_icon.png");
        lock_icon->setPosition({52, 222});
        lock_icon->setSize({34, 34});
        centerPanel->add(lock_icon);



        std::vector<tgui::Button::Ptr> loginButtons;


        auto signUpButton = tgui::Button::create("Sign up");
        signUpButton->setPosition({400, 40});

        auto forgotPasswordButton = tgui::Button::create("Forgot password");
        forgotPasswordButton->setPosition({400, 390});

        loginButtons.emplace_back(signUpButton);
        loginButtons.emplace_back(forgotPasswordButton);

        for(auto button : loginButtons){
            button->setTextSize(18);
            button->setSize({170, 28});

            if (button == forgotPasswordButton){
                button->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
            }
            else if (button == signUpButton) {
                button->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 0});
            }

            button->getRenderer()->setTextColor(color.text);
            button->getRenderer()->setTextColorHover(color.down);
            button->getRenderer()->setTextColorDown(color.down);
            button->getRenderer()->setTextColorFocused(color.text);
            button->getRenderer()->setTextColorDownFocused(color.down);

            button->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
            button->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
            button->getRenderer()->setBackgroundColorDown(tgui::Color::Transparent);
            button->getRenderer()->setBackgroundColorFocused(tgui::Color::Transparent);
            button->getRenderer()->setBackgroundColorDownFocused(tgui::Color::Transparent);
            button->getRenderer()->setBackgroundColorDisabled(tgui::Color::Transparent);

            button->getRenderer()->setBorderColor(color.text);
            button->getRenderer()->setBorderColorHover(color.down);
            button->getRenderer()->setBorderColorDown(color.down);
            button->getRenderer()->setBorderColorDownHover(color.down);
            button->getRenderer()->setBorderColorFocused(color.text);
            button->getRenderer()->setBorderColorDownFocused(color.down);

            centerPanel->add(button);
        }


        auto rememberBox = makeCheckBox(centerPanel, 50, 390);

        auto rememberLabel = tgui::Label::create("Remember me");
        rememberLabel->setPosition({80, 390});
        rememberLabel->setTextSize(18);
        rememberLabel->getRenderer()->setTextColor(color.text);
        rememberLabel->getRenderer()->setBorderColor(color.text);
        centerPanel->add(rememberLabel);

        bindSoundOnClick(rememberLabel, [rememberBox, color]{
            rememberBox->setChecked(!rememberBox->isChecked());
        });

        sf::Color hoverColor = color.down;
        sf::Color normalColor = color.text;


        rememberLabel->onMouseEnter([hoverColor, rememberLabel, rememberBox]{
            rememberLabel->getRenderer()->setTextColor(hoverColor);
            rememberBox->getRenderer()->setBorderColor(hoverColor);
            rememberBox->getRenderer()->setCheckColor(hoverColor);
        });

        rememberLabel->onMouseLeave([normalColor, rememberBox, rememberLabel]{
            rememberLabel->getRenderer()->setTextColor(normalColor);
            rememberBox->getRenderer()->setBorderColor(normalColor);
            rememberBox->getRenderer()->setCheckColor(normalColor);
        });

        rememberLabel->onMousePress([color, rememberLabel,rememberBox]{
            rememberLabel->getRenderer()->setTextColor(color.down);
            rememberBox->getRenderer()->setTextColorHover(color.down);
            rememberBox->getRenderer()->setBorderColorHover(color.down);
            rememberBox->getRenderer()->setCheckColorHover(color.down);
        });

        
        auto usernameBox = tgui::EditBox::create();
        usernameBox->setDefaultText("Username");
        usernameBox->setPosition({100, 145});


        auto passwordBox = tgui::EditBox::create();
        passwordBox->setDefaultText("Password");
        passwordBox->setPosition({100, 225});
        passwordBox->setPasswordCharacter('*');

        std::vector<tgui::EditBox::Ptr> loginEditBoxes;
        
        loginEditBoxes.emplace_back(usernameBox);
        loginEditBoxes.emplace_back(passwordBox);


        for(auto button : loginEditBoxes){
            button->setTextSize(20);
            button->setSize({450, 42});
            button->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
            button->getRenderer()->setBorders(0);
            button->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
            button->getRenderer()->setTextColor(color.text);
            button->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
            button->getRenderer()->setBorderColor(sf::Color(0,0,0));
            button->getRenderer()->setDefaultTextColor(sf::Color(125, 125, 125));

            centerPanel->add(button);
        }

        auto eye = tgui::Picture::create(tgui::Texture("assets/images/eye_off.png", {}, {}, true));
        eye->setPosition({500, 225});
        eye->setSize({32, 32});
        centerPanel->add(eye);




        bindSoundOnClick(eye, [eye, passwordBox]{
            std::cout << "YO" << std::endl;
            if (passwordBox->getPasswordCharacter() == '*'){
                eye->getRenderer()->setTexture("assets/images/eye_on.png");
                passwordBox->setPasswordCharacter(false);
            }
            else {
                eye->getRenderer()->setTexture("assets/images/eye_off.png");
                passwordBox->setPasswordCharacter('*');
            }
        });




        auto login_button = tgui::Button::create("LOGIN");
        login_button->setPosition({70, 300});
        login_button->setTextSize(28);
        login_button->setSize({500, 60});
        login_button->getRenderer()->setBorders(0);
        login_button->getRenderer()->setBackgroundColor(sf::Color(0,0,0,100));
        login_button->getRenderer()->setTextColor(color.text);
        login_button->getRenderer()->setTextColorHover(color.down);
        login_button->getRenderer()->setTextColorDown(color.down);
        login_button->getRenderer()->setBackgroundColorDownHover(color.bg);
        login_button->getRenderer()->setBackgroundColorDown(color.bg);
        login_button->getRenderer()->setBackgroundColorHover(color.bg);
        centerPanel->add(login_button);


        wrongLabel = tgui::Label::create("");
        wrongLabel->setTextSize(17);
        wrongLabel->setPosition({70, 270});
        wrongLabel->getRenderer()->setTextColor(sf::Color(160, 42, 16));
        centerPanel->add(wrongLabel);
        
        auto tryLogin = [this, usernameBox, passwordBox]{
            if (usernameBox->getText().length() == 0 && passwordBox->getText().length() == 0){
                passwordBox->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
                usernameBox->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
            }
            else if(usernameBox->getText().length() == 0){
                usernameBox->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
            }
            else if (passwordBox->getText().length() == 0){
                passwordBox->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
            }
            else {

                std::string username = usernameBox->getText().toStdString();
                username.erase(std::remove_if(username.begin(), username.end(), ::isspace), username.end());

                std::string password = passwordBox->getText().toStdString();
                password.erase(std::remove_if(password.begin(), password.end(), ::isspace), password.end());


                auto loginresult = auth::loginUser(db, username, password);

                std::cout << "status: " << (int)loginresult.status << "\n";
                std::cout << "username vstup: " << username << "\n";
                std::cout << "username vystup: " << loginresult.username << "\n";


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
        };


        bindSoundOnClick(login_button, tryLogin);

        usernameBox->onReturnKeyPress([this, tryLogin]{
            play_button();
            tryLogin();
        });

        passwordBox->onReturnKeyPress([this, tryLogin]{
            play_button();
            tryLogin();
        });


        auto quitButton = tgui::Button::create("Quit");
        quitButton->setPosition({0,5});
        quitButton->setSize({120, 62}); 
        quitButton->setTextSize(30);


        quitButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
        quitButton->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
        quitButton->getRenderer()->setBackgroundColorDown(tgui::Color::Transparent);
        quitButton->getRenderer()->setBackgroundColorDisabled(tgui::Color::Transparent);


        quitButton->getRenderer()->setTextColor(color.text);
        quitButton->getRenderer()->setTextColorHover(color.down);
        quitButton->getRenderer()->setTextColorDown(color.down);
        quitButton->getRenderer()->setTextColorDisabled(tgui::Color(160, 150, 130));

        quitButton->getRenderer()->setBorders(0);

        topBar->add(quitButton);

        bindSoundOnClick(quitButton, [this, color] {

            showOverlay();
            centerPanel->setVisible(false);

            tgui::ChildWindow::Ptr quitLabel = makeQuitLabel();
            std::vector<tgui::Button::Ptr> quitButtons = makeQuitButtons(quitLabel);

            tgui::Button::Ptr yesButton = quitButtons.at(0);
            tgui::Button::Ptr noButton = quitButtons.at(1);

            bindSoundOnClick(yesButton, [this]
            {
                window.close();
            });

            bindSoundOnClick(noButton, [this, quitLabel]
            {
                hideOverlay();
                root->remove(quitLabel);
                centerPanel->setVisible(true);
            });
        });





        bindSoundOnClick(forgotPasswordButton, [this, color]{
            centerPanel->setVisible(false);
            
            auto forgotPasswordPanel = makePanel(Constants::forgotPanelWidth, Constants::forgotPanelHeight);

            auto forgotPasswordLabel = tgui::Label::create("Write email of your account");
            forgotPasswordLabel->setPosition({40, 55});
            forgotPasswordLabel->setTextSize(30);   
            forgotPasswordLabel->getRenderer()->setTextColor(color.text);
            forgotPasswordPanel->add(forgotPasswordLabel);


            auto forgotPasswordBox = tgui::EditBox::create();
            forgotPasswordBox->setDefaultText("email");
            forgotPasswordBox->setPosition({100, 145});
            forgotPasswordBox->setTextSize(20);
            forgotPasswordBox->setSize({450, 42});
            forgotPasswordBox->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
            forgotPasswordBox->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
            forgotPasswordBox->getRenderer()->setTextColor(color.text);
            forgotPasswordBox->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
            forgotPasswordBox->getRenderer()->setBorderColor(sf::Color(0,0,0));
            forgotPasswordBox->getRenderer()->setDefaultTextColor(sf::Color(125, 125, 125));

            forgotPasswordPanel->add(forgotPasswordBox);

            auto emailIcon = tgui::Picture::create("assets/images/email_icon.png");
            emailIcon->setPosition({40, 150});
            emailIcon->setSize({32, 25});
            forgotPasswordPanel->add(emailIcon);




            std::vector<tgui::Button::Ptr> forgotPasswordButtons;


            auto backToLoginButton = makeBackToLoginButton(50, 300);

            auto nextButton = tgui::Button::create("Next");
            nextButton->setPosition({400, 300});


            forgotPasswordButtons.push_back(backToLoginButton);
            forgotPasswordButtons.push_back(nextButton);

            for(auto button : forgotPasswordButtons){
                button->setSize({160, 30}); 
                button->setTextSize(18);
                button->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});
                button->getRenderer()->setBorderColor(color.text);
                button->getRenderer()->setTextColor(color.text);
                button->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
                button->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
                button->getRenderer()->setBackgroundColorDown(tgui::Color::Transparent);
                button->getRenderer()->setBackgroundColorDisabled(tgui::Color::Transparent);
                button->getRenderer()->setTextColorHover(color.down);
                button->getRenderer()->setBorderColorHover(color.down);

                forgotPasswordPanel->add(button);
            }

            bindSoundOnClick(backToLoginButton, [this, forgotPasswordPanel]{
                root->remove(forgotPasswordPanel);
                centerPanel->setVisible(true);
            });

            bindSoundOnClick(nextButton, [this, forgotPasswordBox, color, forgotPasswordPanel]{
                if(forgotPasswordBox->getText().length() == 0){
                    forgotPasswordBox->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
                }                
                else {

                root->remove(forgotPasswordPanel);

                auto code_email_panel = tgui::Panel::create({Constants::forgotPanelWidth, Constants::forgotPanelHeight});
                code_email_panel->setPosition({(getGuiWidth() - Constants::forgotPanelWidth) / 2.f, (getGuiHeight() - Constants::forgotPanelHeight) / 2.f});
                code_email_panel->getRenderer()->setBackgroundColor(color.panel2);
                code_email_panel->getRenderer()->setRoundedBorderRadius(20.0);
                
                root->add(code_email_panel);

                }
            });

            });

        bindSoundOnClick(signUpButton, [this, color]{

            centerPanel->setVisible(false);

            auto signUpPanel = makePanel(Constants::signUpPanelWidth,Constants::signUpPanelHeight);

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

            

            auto backToLogin = makeBackToLoginButton(400, 500);
            signUpPanel->add(backToLogin);


            auto eye = tgui::Picture::create(tgui::Texture("assets/images/eye_off.png", {}, {}, true));
            eye->setPosition({500, 300});
            eye->setSize({32, 32});
            signUpPanel->add(eye);


            bindSoundOnClick(eye, [eye, password_box]{
                if (password_box->getPasswordCharacter() == '*'){
                    eye->getRenderer()->setTexture("assets/images/eye_on.png");
                    password_box->setPasswordCharacter(false);
                }
                else {
                    eye->getRenderer()->setTexture("assets/images/eye_off.png");
                    password_box->setPasswordCharacter('*');
                }
            });

            bindSoundOnClick(backToLogin, [this, signUpPanel]{
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



            wrongLabel = tgui::Label::create("");
            wrongLabel->setTextSize(17);
            wrongLabel->setPosition({90, 420});
            wrongLabel->getRenderer()->setTextColor(sf::Color(160, 42, 16));
            signUpPanel->add(wrongLabel);


        bindSoundOnClick(sign_up_button, [this, username_box, email_box, password_box]{
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

                std::string email = email_box->getText().toStdString();
                std::string password = password_box->getText().toStdString();
                std::string username = username_box->getText().toStdString();

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
        time_label->getRenderer()->setTextColor(color.text);
        
        topBar->add(time_label);

        
    }
