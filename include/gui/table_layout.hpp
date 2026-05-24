#pragma once

#include <array>

struct TableSeat {
    float leftX;
    float topY;
    float cardW;
    float cardH;
    float gap;
};

namespace TableLayout {

constexpr TableSeat HUMAN = {0.4599f, 0.8160f, 0.0402f, 0.0960f, 0.0042f};

constexpr std::array<TableSeat, 5> OPPONENT_SEATS = {{
    {0.0418f, 0.2454f, 0.0418f, 0.1062f, 0.0040f}, // left middle
    {0.2895f, 0.1028f, 0.0438f, 0.1049f, 0.0040f}, // top left
    {0.6232f, 0.1028f, 0.0438f, 0.1049f, 0.0040f}, // top right
    {0.8792f, 0.2454f, 0.0418f, 0.1062f, 0.0040f}, // right middle
    {0.7060f, 0.8160f, 0.0402f, 0.0960f, 0.0042f}, // bottom right
}};

constexpr TableSeat COMMUNITY = {0.3044f, 0.4307f, 0.070f, 0.1362f, 0.0054f};

} 
