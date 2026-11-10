#include <nlohmann/json.hpp>
#pragma once
#include <string>
#include <vector>

namespace pcp {

enum class SasMethod { EMOJI, DECIMAL };

struct SasEmoji {
    std::string emoji;
    std::string description;
};

const std::vector<SasEmoji> SAS_EMOJIS = {
    {"🐶","Dog"},{"🐱","Cat"},{"🦁","Lion"},{"🐎","Horse"},{"🦄","Unicorn"},
    {"🐷","Pig"},{"🐘","Elephant"},{"🐰","Rabbit"},{"🐼","Panda"},{"🐓","Rooster"},
    {"🐧","Penguin"},{"🐢","Turtle"},{"🐙","Octopus"},{"🦋","Butterfly"},{"🌹","Rose"},
    {"🍒","Cherry"},{"🍌","Banana"},{"🍎","Apple"},{"🍓","Strawberry"},{"🌽","Corn"},
    {"🍕","Pizza"},{"🎂","Cake"},{"🍺","Beer"},{"☕","Coffee"},{"🎸","Guitar"},
    {"✈️","Airplane"},{"🚀","Rocket"},{"🏖️","Beach"},{"🏔️","Mountain"},{"🌪️","Tornado"},
    {"🗼","Tokyo Tower"},{"🗽","Statue of Liberty"},{"🎡","Ferris Wheel"},{"🔑","Key"},
    {"🔨","Hammer"},{"📞","Telephone"},{"✂️","Scissors"},{"🕯️","Candle"},{"📌","Pushpin"},
    {"🔫","Pistol"},{"⚓","Anchor"},{"⚡","Lightning"},{"🤖","Robot"},{"🎓","Graduation"},
    {"🔔","Bell"},{"✏️","Pencil"},{"📏","Ruler"},{"🔧","Wrench"},{"🎵","Music"},
    {"🎅","Santa"},{"🐳","Whale"},{"🐊","Crocodile"},{"🐮","Cow"},{"🐸","Frog"},
    {"🐙","Octopus"},{"🦋","Butterfly"},{"🌻","Sunflower"},{"🍇","Grapes"},{"🥕","Carrot"}
};

// Get emoji at SAS verification index
SasEmoji getSasEmoji(int index);

// Get decimal comparison values for SAS
std::vector<int> getSasDecimals(const std::string& sasBytes, int count = 3);

// Format SAS emoji list for display
std::string formatSasEmojis(const std::vector<int>& indices);

// Format SAS decimals for display
std::string formatSasDecimals(const std::vector<int>& decimals);

// Extract SAS bytes from key verification event
std::string extractSasBytes(const std::string& verificationJson);

} // namespace pcp
