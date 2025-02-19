/// Uncomment due to minimalize usage of Qt framework in the project
//#define MINIMUM_USAGE_OF_QT_FRAMEWORK

#ifndef GLOBAL_H
#define GLOBAL_H
#include <string>

static const std::string g_WrongInputMsg("Unrecognized input type");
static const std::string g_ChannelOutOfRangeMsg("Channel value is out of range ");
static const std::string g_WrongChannelAmountMsg("Channels amount is incorrect");
static const std::string g_WrongPieSettingsMsg("Amount of entries is not possible to visualize within given settings");

static std::string toLowerString(const std::string& text)
{
    std::string result = text;
    for (char& c : result)
        c = tolower(c);

    return result;
}

#endif // GLOBAL_H