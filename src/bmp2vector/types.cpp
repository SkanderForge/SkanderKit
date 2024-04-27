#include "../bmp2vector/types.h"

coord toCoord(const std::string& s)
{
    coord c;
    auto pos = s.find(" ");
    auto sx = s.substr(0, pos);

    auto sy = s.substr(pos + 1, s.size() - pos);


    c.x = stoi(sx);
    c.y = stoi(sy);

    return c;
}

std::string toHex(uint u, uint minDigits)
{
    std::vector<char> hexChars = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    std::string hexStringReverse = "";
    std::string hexString = "";

    uint digit = 0;

    do
    {
        digit = u % 16;
        hexStringReverse += hexChars[digit];
        u = u / 16;

    } while (u > 0 || hexStringReverse.size() < minDigits);

    for (int i = hexStringReverse.size() -1; i >= 0; i--)
    {
        hexString += hexStringReverse[i];
    }

    return hexString;

}
