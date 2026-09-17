#pragma once

#include <array>

struct TableSeat {
    float leftX;
    float topY;
    float cardW;
    float cardH;
    float gap;
};

struct TablePanel {
    float x;
    float y;
};

namespace TableLayout {
constexpr TableSeat HUMAN = {0.438f, 0.78f, 0.06f, 0.145f, 0.0042f};

constexpr std::array<TableSeat, 8> OPPONENT_SEATS = {{
    {0.205f, 0.82, 0.0418f, 0.1062f, 0.0040f}, 
    {0.036f, 0.675, 0.0418f, 0.1062f, 0.0040f}, 
    {0.0418f, 0.2454f, 0.0418f, 0.1062f, 0.0040f}, 
    {0.2895f, 0.1028f, 0.0438f, 0.1049f, 0.0040f}, 
    {0.6222f, 0.1028f, 0.0438f, 0.1049f, 0.0040f}, 
    {0.8762f, 0.2454f, 0.0418f, 0.1062f, 0.0040f}, 
    {0.874f, 0.675, 0.0418f, 0.1062f, 0.0040f}, 
    {0.7060f, 0.8190f, 0.0402f, 0.0960f, 0.0042f}, 

}};

constexpr TableSeat COMMUNITY = {0.3044f, 0.4207f, 0.070f, 0.16f, 0.0054f};

constexpr std::array<TablePanel, 8> OPPONENT_TURNS = {{
    {0.2, 0.77f},
    {0.029, 0.63},
    {0.035, 0.2f},
    {0.285f, 0.06f},
    {0.617f, 0.06f},
    {0.869f, 0.2f},
    {0.867f, 0.632},
    {0.698f, 0.77f},
}};

constexpr TablePanel PLAYER_BET = {0.46f, 0.70f};

constexpr std::array<TablePanel, 8> OPPONENTS_BET = {{
    {0.248, 0.7f},
    {0.17, 0.64},
    {0.17, 0.35f},
    {0.327f, 0.28f},
    {0.66f, 0.28f},
    {0.82f, 0.35f},
    {0.82f, 0.632},
    {0.742f, 0.7f},
}};

} 
