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


Login::Login(tgui::Gui& gui, sf::Music& music, sf::RenderWindow& window) : Gui_base(gui, music), window(window) {}

// ??? toto pojde preoč 
void Login::login_screen(){

        if (sqlite3_open("users.db", &db) == SQLITE_OK){
            bool database_created = auth::createUsersTable(db);
        }
        else {
            std::cerr << "Failed to open Database" << sqlite3_errmsg(db) << std::endl;
        }
        
        gui.removeAllWidgets();

        tgui::Font::setGlobalFont("assets/fonts/Shelten.ttf");

        root = tgui::Panel::create({getGuiWidth(), getGuiWidth()});
        root->getRenderer()->setTextureBackground("assets/images/backgrounds/login_background.jpeg");
        gui.add(root);
        topBar = makeBar({0,0});

        volume(topBar, root, mainMenuMusic);
    

        centerPanel = makePanel({Constants::Panel::loginWidth, Constants::Panel::loginHeight}, color.centerpanel, root);

        auto loginTitle = makeLabel("Welcome!", 45, color.text, centerPanel);
        loginTitle->setPosition({55, 35});

        auto loginIcon = makeImage("assets/images/login_icon.png",{55, 147}, {28, 28}, centerPanel);
        auto lockIcon = makeImage("assets/images/lock_icon.png", {52, 222},{34, 34}, centerPanel );


        auto signUpButton = makeButton("Sign up", {400,40}, {170,28}, 20, centerPanel);
        signUpButton->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 0});

        auto forgotPasswordButton = makeButton("Forgot password", {400, 390}, {170, 28}, 18, centerPanel);
        forgotPasswordButton->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});


        auto rememberBox = makeCheckBox({50, 390}, centerPanel);

        auto rememberLabel = makeLabel("Remember me", 18, color.text, centerPanel);
        rememberLabel->setPosition({80, 390});
        rememberLabel->getRenderer()->setBorderColor(color.text);

        bindSoundOnClick(rememberLabel, [this, rememberBox]{
            rememberBox->setChecked(!rememberBox->isChecked());
        });

        rememberLabel->onMouseEnter([this, rememberLabel, rememberBox]{
            rememberLabel->getRenderer()->setTextColor(color.down);
            rememberBox->getRenderer()->setBorderColor(color.down);
            rememberBox->getRenderer()->setCheckColor(color.down);
        });

        rememberLabel->onMouseLeave([this, rememberBox, rememberLabel]{
            rememberLabel->getRenderer()->setTextColor(color.text);
            rememberBox->getRenderer()->setBorderColor(color.text);
            rememberBox->getRenderer()->setCheckColor(color.text);
        });

        rememberLabel->onMousePress([this, rememberLabel,rememberBox]{
            rememberLabel->getRenderer()->setTextColor(color.down);
            rememberBox->getRenderer()->setTextColorHover(color.down);
            rememberBox->getRenderer()->setBorderColorHover(color.down);
            rememberBox->getRenderer()->setCheckColorHover(color.down);
        });

        
        auto usernameBox = makeEditBox("Username", {100,145},  centerPanel);
        auto passwordBox = makeEditBox("Password", {100, 225}, centerPanel);
        passwordBox->setPasswordCharacter('*');

        auto eyeIcon = makeImage(tgui::Texture("assets/images/eye_off.png", {}, {}, true), {500, 225}, {32, 28}, centerPanel);

        bindSoundOnClick(eyeIcon, [eyeIcon, passwordBox]{
            if (passwordBox->getPasswordCharacter() == '*'){
                eyeIcon->getRenderer()->setTexture("assets/images/eye_on.png");
                passwordBox->setPasswordCharacter(false);
            }
            else {
                eyeIcon->getRenderer()->setTexture("assets/images/eye_off.png");
                passwordBox->setPasswordCharacter('*');
            }
        });

        auto loginButton = makeButton("LOGIN", {(Constants::Panel::loginWidth - 500) /2.f, 300}, {500, 60}, 28, centerPanel);

        wrongLabel = makeLabel("", 17, sf::Color(160, 42, 16), centerPanel);
        wrongLabel->setPosition({70, 270});
        
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


        bindSoundOnClick(loginButton, tryLogin);

        usernameBox->onReturnKeyPress([this, tryLogin]{
            play_button();
            tryLogin();
        });

        passwordBox->onReturnKeyPress([this, tryLogin]{
            play_button();
            tryLogin();
        });

        auto quitButton = makeButton("Quit", {0,5}, {120, 62}, 28, topBar);

        bindSoundOnClick(quitButton, [this] {

            showOverlay();
            centerPanel->setVisible(false);

            tgui::ChildWindow::Ptr quitPanel = makePopUpPanel("Do you want to exit?", {420.f, 190.f}, 22, color.down, tgui::Color(253, 253, 253), false);
            tgui::Button::Ptr yesButton = makePopUpButton("Yes", {45.f, 50.f}, {120,50}, 20, quitPanel);
            tgui::Button::Ptr noButton = makePopUpButton("No", {235, 50.f}, {120,50}, 20, quitPanel);


            bindSoundOnClick(yesButton, [this]
            {
                window.close();
            });

            bindSoundOnClick(noButton, [this, quitPanel]
            {
                hideOverlay();
                root->remove(quitPanel);
                centerPanel->setVisible(true);
            });
        });


        bindSoundOnClick(forgotPasswordButton, [this]{
            centerPanel->setVisible(false);
            
            forgotPasswordPanel = makePanel({Constants::Panel::forgotWidth, Constants::Panel::forgotHeight}, color.centerpanel, root);

            auto forgotPasswordLabel = makeLabel("Write email of your account", 30, color.text, forgotPasswordPanel);
            forgotPasswordLabel->setPosition({40, 55});

            auto forgotPasswordBox = makeEditBox("email", {100, 145}, forgotPasswordPanel);
            auto emailIcon = makeImage("assets/images/email_icon.png", {40, 150},{32, 25}, forgotPasswordPanel);
            auto backToLoginButton = makeButton("Back to login", {50, 300}, {160, 30}, 18, forgotPasswordPanel);
            auto nextButton = makeButton("Next", {400,300}, {160, 30}, 18, forgotPasswordPanel );


        
            bindSoundOnClick(backToLoginButton, [this]{
                root->remove(forgotPasswordPanel);
                centerPanel->setVisible(true);
            });

            bindSoundOnClick(nextButton, [this, forgotPasswordBox]{
                if(forgotPasswordBox->getText().length() == 0){
                    forgotPasswordBox->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
                }                
                else {

                root->remove(forgotPasswordPanel);

                auto code_email_panel = tgui::Panel::create({Constants::Panel::forgotWidth, Constants::Panel::forgotHeight});
                code_email_panel->setPosition({(getGuiWidth() - Constants::Panel::forgotWidth) / 2.f, (getGuiHeight() - Constants::Panel::forgotHeight) / 2.f});
                code_email_panel->getRenderer()->setBackgroundColor(color.panel2);
                code_email_panel->getRenderer()->setRoundedBorderRadius(20.0);
                
                root->add(code_email_panel);

                }
            });

            });

        bindSoundOnClick(signUpButton, [this]{

            centerPanel->setVisible(false);
            
            signUpPanel = makePanel({Constants::Panel::signUpWidth,Constants::Panel::signUpHeight}, color.centerpanel, root);
            
            auto loginTitle = makeLabel("Sign up", 45, color.text, signUpPanel);
            
            loginTitle->setPosition({40, 30});

            auto usernameBox = makeEditBox("username", {100,145}, signUpPanel);
            auto emailBox = makeEditBox("email",{100, 225}, signUpPanel);
            auto passwordBox = makeEditBox("password", {100,305}, signUpPanel);

            auto usernameIcon = makeImage("assets/images/login_icon.png", {50, 150}, {32, 28}, signUpPanel);

            auto emailIcon = makeImage("assets/images/email_icon.png", {50, 230}, {32, 24}, signUpPanel);

            auto passwordIcon = makeImage("assets/images/lock_icon.png", {50,310}, {32, 28}, signUpPanel);
            
            auto backToLoginButton = makeButton("Back to login", {400,40}, {170,28}, 20, signUpPanel);
            backToLoginButton->getRenderer()->setBorders({0,0,0,0});

            auto eyeIcon = makeImage(tgui::Texture("assets/images/eye_off.png", {}, {}, true), {500, 300}, {32, 28}, signUpPanel);

            bindSoundOnClick(eyeIcon, [eyeIcon, passwordBox]{
                if (passwordBox->getPasswordCharacter() == '*'){
                    eyeIcon->getRenderer()->setTexture("assets/images/eye_on.png");
                    passwordBox->setPasswordCharacter(false);
                }
                else {
                    eyeIcon->getRenderer()->setTexture("assets/images/eye_off.png");
                    passwordBox->setPasswordCharacter('*');
                }
            });

            bindSoundOnClick(backToLoginButton, [this]{
                root->remove(signUpPanel);
                centerPanel->setVisible(true);
            });

        auto signUpButton = makeButton("Create account", {(Constants::Panel::signUpWidth - 500) / 2.f, 420}, {500, 60}, 28, signUpPanel);


        wrongLabel = tgui::Label::create("");
        wrongLabel->setTextSize(17);
        wrongLabel->setPosition({90, 420});
        wrongLabel->getRenderer()->setTextColor(sf::Color(160, 42, 16));
        signUpPanel->add(wrongLabel);


        bindSoundOnClick(signUpButton, [this, usernameBox, emailBox, passwordBox]{
            if (usernameBox->getText().length() == 0 && passwordBox->getText().length() == 0 && usernameBox->getText().length() == 0){
                emailBox->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
                passwordBox->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
                usernameBox->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
            }
            else if(usernameBox->getText().length() == 0){
                usernameBox->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
            }
            else if (passwordBox->getText().length() == 0){
                passwordBox->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
            }
            else if (emailBox->getText().length() == 0){
                emailBox->getRenderer()->setDefaultTextColor(sf::Color(160, 42, 16));
            }

            else{

                std::string email = emailBox->getText().toStdString();
                std::string password = passwordBox->getText().toStdString();
                std::string username = usernameBox->getText().toStdString();

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

        timeLabel = tgui::Label::create("");
        timeLabel->setTextSize(30);
        timeLabel->setPosition({getGuiWidth() - 125.f, 10});
        timeLabel->getRenderer()->setTextColor(color.text);
        
        topBar->add(timeLabel);

        
    }
