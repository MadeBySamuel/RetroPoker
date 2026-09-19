#include <algorithm>
#include <array>
#include <map>
#include <vector>

#include "hand_evaluator.hpp"

namespace {

int packRanks(const std::vector<int>& ranks)
{
    int value = 0;
    for (const int rank : ranks) {
        value = value * 15 + rank;
    }
    return value;
}

int findStraightHigh(const std::array<int, 15>& rankCount)
{
    for (int high = 14; high >= 5; --high) {
        bool hasStraight = true;
        for (int rank = high; rank > high - 5; --rank) {
            if (rankCount[rank] == 0) {
                hasStraight = false;
                break;
            }
        }
        if (hasStraight) {
            return high;
        }
    }

    if (rankCount[14] && rankCount[2] && rankCount[3] && rankCount[4] && rankCount[5]) {
        return 5;
    }
    return 0;
}

std::vector<int> ranksDescending(const std::array<int, 15>& rankCount, int excludedRank = -1)
{
    std::vector<int> ranks;
    for (int rank = 14; rank >= 2; --rank) {
        if (rank != excludedRank && rankCount[rank] > 0) {
            ranks.push_back(rank);
        }
    }
    return ranks;
}

} // namespace

HandScore HandEvaluator::evaluate(const std::vector<Card>& cards, const std::string& playerName)
{
    std::array<int, 15> rankCount{};
    std::map<std::string, std::vector<int>> suitRanks;

    for (const Card& card : cards) {
        if (card.rank < 2 || card.rank > 14) {
            continue;
        }
        ++rankCount[card.rank];
        suitRanks[card.name].push_back(card.rank);
    }

    for (auto& [suit, ranks] : suitRanks) {
        std::sort(ranks.begin(), ranks.end(), std::greater<int>());
    }

    for (const auto& [suit, ranks] : suitRanks) {
        if (ranks.size() < 5) {
            continue;
        }

        std::array<int, 15> suitedRankCount{};
        for (const int rank : ranks) {
            suitedRankCount[rank] = 1;
        }
        const int straightFlushHigh = findStraightHigh(suitedRankCount);
        if (straightFlushHigh == 14) {
            return {9, packRanks({14}), playerName};
        }
        if (straightFlushHigh != 0) {
            return {8, packRanks({straightFlushHigh}), playerName};
        }
    }

    int fourKind = 0;
    for (int rank = 14; rank >= 2; --rank) {
        if (rankCount[rank] == 4) {
            fourKind = rank;
            break;
        }
    }
    if (fourKind != 0) {
        return {7, packRanks({fourKind, ranksDescending(rankCount, fourKind).front()}), playerName};
    }

    std::vector<int> trips;
    std::vector<int> pairs;
    for (int rank = 14; rank >= 2; --rank) {
        if (rankCount[rank] >= 3) {
            trips.push_back(rank);
        }
        if (rankCount[rank] >= 2) {
            pairs.push_back(rank);
        }
    }
    if (!trips.empty()) {
        const int trip = trips.front();
        int pair = 0;
        for (const int rank : pairs) {
            if (rank != trip) {
                pair = rank;
                break;
            }
        }
        if (pair != 0) {
            return {6, packRanks({trip, pair}), playerName};
        }
    }

    for (const auto& [suit, ranks] : suitRanks) {
        if (ranks.size() >= 5) {
            return {5, packRanks({ranks[0], ranks[1], ranks[2], ranks[3], ranks[4]}), playerName};
        }
    }

    const int straightHigh = findStraightHigh(rankCount);
    if (straightHigh != 0) {
        return {4, packRanks({straightHigh}), playerName};
    }

    if (!trips.empty()) {
        const int trip = trips.front();
        const auto kickers = ranksDescending(rankCount, trip);
        return {3, packRanks({trip, kickers[0], kickers[1]}), playerName};
    }

    if (pairs.size() >= 2) {
        const auto kickers = ranksDescending(rankCount, pairs[0]);
        const int kicker = *std::find_if(kickers.begin(), kickers.end(), [&pairs](int rank) {
            return rank != pairs[1];
        });
        return {2, packRanks({pairs[0], pairs[1], kicker}), playerName};
    }

    if (!pairs.empty()) {
        const int pair = pairs.front();
        const auto kickers = ranksDescending(rankCount, pair);
        return {1, packRanks({pair, kickers[0], kickers[1], kickers[2]}), playerName};
    }

    const auto highCards = ranksDescending(rankCount);
    std::vector<int> bestHighCards;
    for (std::size_t i = 0; i < highCards.size() && i < 5; ++i) {
        bestHighCards.push_back(highCards[i]);
    }
    return {0, packRanks(bestHighCards), playerName};
}
