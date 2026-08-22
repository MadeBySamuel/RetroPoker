#pragma once


namespace Constants {

    extern std::vector<std::string> NAMES;
    inline extern const std::size_t MAX_PEOPLE;
    inline constexpr int START_PLAYER_MONEY = 200;
    inline extern const int AMOUNT_IN_PACK;
    inline constexpr float gap = 40.f;


    namespace Panel {
        inline constexpr float loginWidth = 620.f;
        inline constexpr float loginHeight = 440.f;

        inline constexpr float forgotWidth = 620.f;
        inline constexpr float forgotHeight = 350.f;

        inline constexpr float signUpWidth = 620.f;
        inline constexpr float signUpHeight = 550.f;

        inline constexpr float centerWidth = 900.f;
        inline constexpr float centerHeight = 760.f;

        inline constexpr float rightWidth = 420.f;
        inline constexpr float rightHeight = 760.f;
    }

    namespace Bar{
        inline constexpr float topHeight = 56.f;
        inline constexpr float bottomHeight = 70.f;

        inline constexpr float sideWidth = 280.f;
        inline constexpr float sideHeight = 360.f;
    }

    namespace GMButton{
        inline constexpr float width = 400.f;
        inline constexpr float height = 200.f;
        inline constexpr float posY = 100.f;
        inline constexpr float cardDrawPosX = width + 2 * gap; 

    }


}


