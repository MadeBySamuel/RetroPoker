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
tgui::Button::Ptr Login::makeBackToLoginButton(float x, float y){

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
        
        gui.removeAllWidgets();

        tgui::Font::setGlobalFont("assets/fonts/Shelten.ttf");

        root = tgui::Panel::create({getGuiWidth(), getGuiWidth()});
        root->getRenderer()->setTextureBackground("assets/images/backgrounds/login_background.jpeg");
        gui.add(root);
        topBar = makeBar();

        volume(topBar, root, music);
    

//        color.centerpanel


        centerPanel = makePanel({Constants::loginPanelWidth, Constants::loginPanelHeight}, color.centerpanel);

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


        auto signUpButton = makeButton("Sign up", {400,40}, {170,28}, 20, centerPanel);
        signUpButton->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 0});

        auto forgotPasswordButton = makeButton("Forgot password", {400, 390}, {170, 28}, 18, centerPanel);
        forgotPasswordButton->getRenderer()->setBorders(tgui::Borders{0, 0, 0, 2});


        auto rememberBox = makeCheckBox({50, 390}, centerPanel);

        auto rememberLabel = tgui::Label::create("Remember me");
        rememberLabel->setPosition({80, 390});
        rememberLabel->setTextSize(18);
        rememberLabel->getRenderer()->setTextColor(color.text);
        rememberLabel->getRenderer()->setBorderColor(color.text);
        centerPanel->add(rememberLabel);

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

        auto loginButton = makeButton("LOGIN", {70, 300}, {500, 60}, 28, centerPanel);


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

            tgui::ChildWindow::Ptr quitPanel = makePopUpPanel("Do you want to exit?", {420.f, 190.f}, 22, color.down, tgui::Color(253, 253, 253));
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
            
            auto forgotPasswordPanel = makePanel({Constants::forgotPanelWidth, Constants::forgotPanelHeight}, color.centerpanel );

            auto forgotPasswordLabel = tgui::Label::create("Write email of your account");
            forgotPasswordLabel->setPosition({40, 55});
            forgotPasswordLabel->setTextSize(30);   
            forgotPasswordLabel->getRenderer()->setTextColor(color.text);
            forgotPasswordPanel->add(forgotPasswordLabel);

            auto forgotPasswordBox = makeEditBox("email", {100, 145}, forgotPasswordPanel);

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

            bindSoundOnClick(nextButton, [this, forgotPasswordBox, forgotPasswordPanel]{
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

        bindSoundOnClick(signUpButton, [this]{

            centerPanel->setVisible(false);
            auto signUpPanel = makePanel({Constants::signUpPanelWidth,Constants::signUpPanelHeight}, color.centerpanel);
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
