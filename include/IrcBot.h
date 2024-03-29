//
// Created by unknown on 10/7/15.
//

#ifndef IRCSOCKETS_IRCBOT_H
#define IRCSOCKETS_IRCBOT_H
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <memory>

#ifdef __linux__
#include "LinuxSockets.h"
#else
#include "WindowsSockets.h"
#endif

class IrcBot {
public:
    //IrcBot(std::string ip, std::string port, std::string nick, std::string ident, std::string realname);

    IrcBot(std::string ip, std::string port, std::string nick, std::string ident, std::string realname);

    bool Connect();
    void ParseLoop();
    void SendRaw(std::string message);
private:
    std::unique_ptr<Sockets> Socket;
    std::string InputBuffer;
    std::string Ip;
    std::string Port;
    bool Connected;
    std::string Nickname;
    std::string Identification;
    std::string RealName;


    void ReadMessage();

    std::string CurrentMessage();

    void ParseMessage(std::string rawMessage);
};


#endif //IRCSOCKETS_IRCBOT_H
