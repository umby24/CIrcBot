#include <iostream>
#include "include/IrcBot.h"

using namespace std;

int main()
{
    IrcBot bot("irc.esper.net", "6667", "UmbyCBot", "umbyCBot", "umby24");
    cout << "Connecting.."<< endl;
    bot.Connect();
    cout << "Beginning parse loop"<< endl;
    bot.ParseLoop();
    cout << "=== Ended ===" << endl;
}
