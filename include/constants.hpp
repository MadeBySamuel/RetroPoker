#pragma once


namespace Constants {
    extern std::vector<std::string> NAMES;
    inline extern const std::size_t MAX_PEOPLE;

    inline constexpr int START_PLAYER_MONEY = 200;

    inline extern const int AMOUNT_IN_PACK;

    inline constexpr float topBarHeight = 56.f;
    inline constexpr float loginPanelWidth = 620.f;
    inline constexpr float loginPanelHeight = 440.f;
    inline constexpr float forgotPanelWidth = 620.f;
    inline constexpr float forgotPanelHeight = 350.f;
    inline constexpr float signUpPanelWidth = 620.f;
    inline constexpr float signUpPanelHeight = 550.f;

    inline constexpr float sideBarWidth = 280.f;
    inline constexpr float sideBarHeight = 360.f;
    inline constexpr float centerPanelWidth = 900.f;
    inline constexpr float centerPanelHeight = 760.f;
    inline constexpr float rightPanelWidth = 420.f;
    inline constexpr float rightPanelHeight = 760.f;
    
    inline constexpr float buttonWidth = 400.f;
    inline constexpr float buttonHeight = 200.f;


    inline constexpr float gap = 40.f;
    inline constexpr float totalWidth = gap + buttonWidth + gap + buttonWidth + gap;

}
