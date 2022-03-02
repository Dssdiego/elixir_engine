//
// Created by Diego S. Seabra on 02/03/22.
//

#ifndef VULKAN_ENGINE_CARDGAME_H
#define VULKAN_ENGINE_CARDGAME_H

enum Suit {
    CLUBS, // ♣
    DIAMONDS, // ♦
    HEARTS, // ♥
    SPADES // ♠
};

struct Card {
    uint32_t value;
    Suit suit;
    std::string textureName;
};

//std::array<Card, 32> cards =
//{
//        Card { 1, Suit::CLUBS, "ace_clubs.png" },
//        Card { 2, Suit::CLUBS, "2_clubs.png" },
//        Card { 3, Suit::CLUBS, "3_clubs.png" },
//        Card { 4, Suit::CLUBS, "4_clubs.png" },
//        Card { 5, Suit::CLUBS, "5_clubs.png" },
//        Card { 6, Suit::CLUBS, "6_clubs.png" },
//        Card { 7, Suit::CLUBS, "7_clubs.png" },
//        Card { 8, Suit::CLUBS, "8_clubs.png" },
//        Card { 9, Suit::CLUBS, "9_clubs.png" },
//        Card { 10, Suit::CLUBS, "10_clubs.png" },
//        Card { 11, Suit::CLUBS, "jack_clubs.png" },
//        Card { 12, Suit::CLUBS, "queen_clubs.png" },
//        Card { 13, Suit::CLUBS, "king_clubs.png" },
//
//        Card { 1, Suit::SPADES, "ace_spades.png" },
//        Card { 2, Suit::SPADES, "2_spades.png" },
//        Card { 3, Suit::SPADES, "3_spades.png" },
//        Card { 4, Suit::SPADES, "4_spades.png" },
//        Card { 5, Suit::SPADES, "5_spades.png" },
//        Card { 6, Suit::SPADES, "6_spades.png" },
//        Card { 7, Suit::SPADES, "7_spades.png" },
//        Card { 8, Suit::SPADES, "8_spades.png" },
//        Card { 9, Suit::SPADES, "9_spades.png" },
//        Card { 10, Suit::SPADES, "10_spades.png" },
//        Card { 11, Suit::SPADES, "jack_spades.png" },
//        Card { 12, Suit::SPADES, "queen_spades.png" },
//        Card { 13, Suit::SPADES, "king_spades.png" },
//};

std::array<std::string, 3> backs = {
       "back_black.png",
       "back_blue.png",
       "back_red.png",
};

void loadAssets()
{
//    std::cout << "Play Game: " << cards.data() << std::endl;
}

void loadCards()
{

}

#endif //VULKAN_ENGINE_CARDGAME_H
