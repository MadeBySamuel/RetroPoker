#pragma once


namespace Constants {

    extern std::vector<std::string> NAMES;
    inline extern const std::size_t MAX_PEOPLE;
    inline constexpr int START_PLAYER_MONEY = 200;
    inline extern const int AMOUNT_IN_PACK;
    inline constexpr float gap = 40.f;


    // Ratios are based on the original 1920 x 1080 layout.
    namespace Panel {
        inline constexpr float loginWidthRatio = 0.322917f;
        inline constexpr float loginHeightRatio = 0.407407f;

        inline constexpr float forgotWidthRatio = 0.322917f;
        inline constexpr float forgotHeightRatio = 0.324074f;

        inline constexpr float signUpWidthRatio = 0.322917f;
        inline constexpr float signUpHeightRatio = 0.509259f;

        inline constexpr float centerWidthRatio = 0.468750f;
        inline constexpr float centerHeightRatio = 0.703704f;

        inline constexpr float rightWidthRatio = 0.218750f;
        inline constexpr float rightHeightRatio = 0.703704f;
    }

    namespace Bar {
        inline constexpr float topHeightRatio = 0.051852f;
        inline constexpr float bottomHeightRatio = 0.064815f;

        inline constexpr float sideWidthRatio = 0.145833f;
        inline constexpr float sideHeightRatio = 0.333333f;
    }
    

    namespace GMButton{
        inline constexpr float widthRatio = 0.434783f;
        inline constexpr float heightRatio = 0.303030f;
        inline constexpr float leftXRatio = 0.043478f;
        inline constexpr float rightXRatio = 0.521739f;
        inline constexpr float topYRatio = 0.151515f;

    }


}

