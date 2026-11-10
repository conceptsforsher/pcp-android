#include "pcp/sas_emoji_utils.hpp"
#include <sstream>
namespace pcp {
static const std::vector<SasEmojiData> emojis = {
    {"🐶","Dog"},{"🐱","Cat"},{"🦁","Lion"},{"🐎","Horse"},{"🦄","Unicorn"},
    {"🐷","Pig"},{"🐘","Elephant"},{"🐰","Rabbit"},{"🐼","Panda"},{"🐓","Rooster"},
    {"🐧","Penguin"},{"🐢","Turtle"},{"🐙","Octopus"},{"🦋","Butterfly"},{"🌹","Rose"},
    {"🍒","Cherry"},{"🍌","Banana"},{"🍎","Apple"},{"🍓","Strawberry"},{"🌽","Corn"},
    {"🍕","Pizza"},{"🎂","Cake"},{"🍺","Beer"},{"☕","Coffee"},{"🎸","Guitar"},
    {"✈️","Airplane"},{"🚀","Rocket"},{"🏖️","Beach"},{"🏔️","Mountain"},{"🌪️","Tornado"},
    {"🗼","Tokyo Tower"},{"🗽","Statue of Liberty"},{"🎡","Ferris Wheel"},{"🔑","Key"},
    {"🔨","Hammer"},{"📞","Telephone"},{"✂️","Scissors"},{"🕯️","Candle"},{"📌","Pushpin"},
    {"🔫","Pistol"},{"⚓","Anchor"},{"⚡","Lightning"},{"🤖","Robot"},{"🎓","Graduation"},
    {"🔔","Bell"},{"✏️","Pencil"},{"📏","Ruler"},{"🔧","Wrench"},{"🎵","Music"}
};
std::vector<SasEmojiData> getSasEmojiList() { return emojis; }
SasEmojiData getSasEmojiByIndex(int i) { return emojis[i % emojis.size()]; }
std::string formatSasEmojiDisplay(const std::vector<int>& indices) {
    std::ostringstream os;
    for (size_t i = 0; i < indices.size(); i++) {
        if (i > 0 && i % 7 == 0) os << "\n";
        os << getSasEmojiByIndex(indices[i]).emoji << " ";
    }
    return os.str();
}
}
