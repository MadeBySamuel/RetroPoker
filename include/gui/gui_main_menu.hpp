#pragma once

#include <functional>
#include <string>
#include <vector>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "gui_utils.hpp"
#include "campaign.hpp"

class MainMenu : public Gui_base {

    private:
        sf::RenderWindow& window;
        tgui::Panel::Ptr topBar;
        tgui::Label::Ptr timeLabel;
        std::string last_time_text;

    public:
        MainMenu(tgui::Gui& gui, sf::Music& music, sf::RenderWindow& window);
        std::function<void()> onLogout;
        std::function<void(int, int)> onGame;
        std::function<void(int)> onCampaign;
        std::function<void()> onResetCampaign;
        campaign::Profile campaignProfile;
        std::string campaignSaveError;
        std::string loggedUsername;

        void gameplay_settings();

        void showMainMenu();
        void showCampaign();
        void handleGameMenuEscape();
        void handleGameSettingsEscape();
        void game_menu(std::vector<tgui::Button::Ptr> buttons);
};
