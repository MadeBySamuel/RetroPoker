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

void Login::login_screen(){


        const float guiW = getGuiWidth();
        const float guiH = getGuiHeight();

        const float loginW = Constants::Panel::loginWidthRatio * guiW;
        const float loginH = Constants::Panel::loginHeightRatio * guiH;


        if (sqlite3_open("users.db", &db) == SQLITE_OK){
            bool database_created = auth::createUsersTable(db);
        }
        else {
            std::cerr << "Failed to open Database" << sqlite3_errmsg(db) << std::endl;
        }
        
        gui.removeAllWidgets();

        tgui::Font::setGlobalFont("assets/fonts/Shelten.ttf");

        root = tgui::Panel::create({guiW, guiH});
        root->getRenderer()->setTextureBackground("assets/images/backgrounds/login_background.jpeg");
        gui.add(root);
        topBar = makeBar({0,0});

        volume(topBar, root, mainMenuMusic);
    

        centerPanel = makePanel({loginW, loginH}, color.centerpanel, root);

        auto loginTitle = makeLabel("Welcome!", 45, color.text, centerPanel);
        loginTitle->setPosition({loginW * 0.088710f, loginH * 0.079545f});

        auto loginIcon = makeImage("assets/images/login_icon.png", {loginW * 0.088710f, loginH * 0.334091f}, {loginW * 0.045161f, loginH * 0.063636f}, centerPanel);
        auto lockIcon = makeImage("assets/images/lock_icon.png", {loginW * 0.083871f, loginH * 0.504545f}, {loginW * 0.054839f, loginH * 0.077273f}, centerPanel);


        auto signUpButton = makeButton("Sign up", {loginW * 0.645161f, loginH * 0.090909f}, {loginW * 0.274194f, loginH * 0.063636f}, 20, centerPanel);
        signUpButton->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 0});

        auto forgotPasswordButton = makeButton("Forgot password", {loginW * 0.645161f, loginH * 0.886364f}, {loginW * 0.274194f, loginH * 0.063636f}, 18, centerPanel);
        forgotPasswordButton->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});


        auto rememberBox = makeCheckBox({loginW * 0.080645f, loginH * 0.886364f}, centerPanel);

        auto rememberLabel = makeLabel("Remember me", 18, color.text, centerPanel);
        rememberLabel->setPosition({loginW * 0.129032f, loginH * 0.886364f});
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

        
        auto usernameBox = makeEditBox("Username", {loginW * 0.161290f, loginH * 0.329545f}, centerPanel);
        auto passwordBox = makeEditBox("Password", {loginW * 0.161290f, loginH * 0.511364f}, centerPanel);
        passwordBox->setPasswordCharacter('*');

        auto eyeIcon = makeImage(tgui::Texture("assets/images/eye_off.png", {}, {}, true), {loginW * 0.806452f, loginH * 0.511364f}, {loginW * 0.051613f, loginH * 0.063636f}, centerPanel);

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

        auto loginButton = makeButton("LOGIN", {loginW * 0.096774f, loginH * 0.681818f}, {loginW * 0.806452f, loginH * 0.136364f}, 28, centerPanel);

        wrongLabel = makeLabel("", 17, sf::Color(160, 42, 16), centerPanel);
        wrongLabel->setPosition({loginW * 0.112903f, loginH * 0.613636f});
        
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

        auto quitButton = makeButton("Quit", {0.f, topBar->getSize().y * 0.09f}, {guiW * 0.062500f, topBar->getSize().y * 0.90f}, 28, topBar);

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

            const float forgotW = Constants::Panel::forgotWidthRatio * getGuiWidth();
            const float forgotH = Constants::Panel::forgotHeightRatio * getGuiHeight();
            forgotPasswordPanel = makePanel({forgotW, forgotH}, color.centerpanel, root);

            auto forgotPasswordLabel = makeLabel("Write email of your account", 30, color.text, forgotPasswordPanel);
            forgotPasswordLabel->setPosition({forgotW * 0.064516f, forgotH * 0.157143f});

            auto forgotPasswordBox = makeEditBox("email", {forgotW * 0.161290f, forgotH * 0.414286f}, forgotPasswordPanel);
            auto emailIcon = makeImage("assets/images/email_icon.png", {forgotW * 0.064516f, forgotH * 0.428571f}, {forgotW * 0.051613f, forgotH * 0.071429f}, forgotPasswordPanel);
            auto backToLoginButton = makeButton("Back to login", {forgotW * 0.080645f, forgotH * 0.857143f}, {forgotW * 0.258065f, forgotH * 0.085714f}, 18, forgotPasswordPanel);
            auto nextButton = makeButton("Next", {forgotW * 0.645161f, forgotH * 0.857143f}, {forgotW * 0.258065f, forgotH * 0.085714f}, 18, forgotPasswordPanel);


        
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

                const float forgotW = Constants::Panel::forgotWidthRatio * getGuiWidth();
                const float forgotH = Constants::Panel::forgotHeightRatio * getGuiHeight();
                auto code_email_panel = tgui::Panel::create({forgotW, forgotH});
                code_email_panel->setPosition({(getGuiWidth() - forgotW) / 2.f, (getGuiHeight() - forgotH) / 2.f});
                code_email_panel->getRenderer()->setBackgroundColor(color.panel2);
                code_email_panel->getRenderer()->setRoundedBorderRadius(20.0);
                
                root->add(code_email_panel);

                }
            });

            });

        bindSoundOnClick(signUpButton, [this]{

            centerPanel->setVisible(false);

            const float signUpW = Constants::Panel::signUpWidthRatio * getGuiWidth();
            const float signUpH = Constants::Panel::signUpHeightRatio * getGuiHeight();
            signUpPanel = makePanel({signUpW, signUpH}, color.centerpanel, root);
            
            auto loginTitle = makeLabel("Sign up", 45, color.text, signUpPanel);
            
            loginTitle->setPosition({signUpW * 0.064516f, signUpH * 0.054545f});

            auto usernameBox = makeEditBox("username", {signUpW * 0.161290f, signUpH * 0.263636f}, signUpPanel);
            auto emailBox = makeEditBox("email", {signUpW * 0.161290f, signUpH * 0.409091f}, signUpPanel);
            auto passwordBox = makeEditBox("password", {signUpW * 0.161290f, signUpH * 0.554545f}, signUpPanel);

            auto usernameIcon = makeImage("assets/images/login_icon.png", {signUpW * 0.080645f, signUpH * 0.272727f}, {signUpW * 0.051613f, signUpH * 0.050909f}, signUpPanel);

            auto emailIcon = makeImage("assets/images/email_icon.png", {signUpW * 0.080645f, signUpH * 0.418182f}, {signUpW * 0.051613f, signUpH * 0.043636f}, signUpPanel);

            auto passwordIcon = makeImage("assets/images/lock_icon.png", {signUpW * 0.080645f, signUpH * 0.563636f}, {signUpW * 0.051613f, signUpH * 0.050909f}, signUpPanel);
            
            auto backToLoginButton = makeButton("Back to login", {signUpW * 0.645161f, signUpH * 0.072727f}, {signUpW * 0.274194f, signUpH * 0.050909f}, 20, signUpPanel);
            backToLoginButton->getRenderer()->setBorders({0,0,0,0});

            auto eyeIcon = makeImage(tgui::Texture("assets/images/eye_off.png", {}, {}, true), {signUpW * 0.806452f, signUpH * 0.545455f}, {signUpW * 0.051613f, signUpH * 0.050909f}, signUpPanel);

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

        auto signUpButton = makeButton("Create account", {signUpW * 0.096774f, signUpH * 0.763636f}, {signUpW * 0.806452f, signUpH * 0.109091f}, 28, signUpPanel);


        wrongLabel = tgui::Label::create("");
        wrongLabel->setTextSize(17);
        wrongLabel->setPosition({signUpW * 0.145161f, signUpH * 0.763636f});
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

    }
