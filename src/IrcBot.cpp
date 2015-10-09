//
// Created by unknown on 10/7/15.
//

#include <sstream>
#include "../include/IrcBot.h"

IrcBot::IrcBot(std::string ip, std::string port, std::string nick, std::string ident, std::string realname) {
    Socket = new Sockets(ip, port);

    Ip = ip;
    Port = port;
    Nickname = nick;
    RealName = realname;
    Identification = ident;
    Connected = false;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;

    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }

    return elems;
}

std::string join(std::vector<std::string> input) {
    std::ostringstream joined;
    std::copy(input.begin(), input.end(), std::ostream_iterator<std::string>(joined, " "));
    return joined.str();
}

bool IrcBot::Connect() {
    std::cout << "irc connect" << std::endl;

    if (Connected)
        return false;

    bool connectResult;
    connectResult = Socket->Connect();

    if (!connectResult) {
        Connected = false;
        return false;
    }

    Connected = true;

    SendRaw("NICK " + Nickname);
    SendRaw("USER " + Identification + " CBOT CBOT :" + RealName);
    SendRaw("MODE " + Nickname + " +B-x");
    std::cout << "Connected." << std::endl;
    return true;
}

void IrcBot::SendRaw(std::string message) {
    message = message + "\r\n"; // -- Add IRC Ending
    char *msg = (char*) message.c_str(); // -- Convert to char*

    int len;
    len = (int) strlen(msg); // -- Get the length

    Socket->Send(msg, len); // -- Send :)
}

std::string IrcBot::ReadMessage() {
    std::string ircString("");
    int readBytes;
    char buffer;

    do {
        readBytes = Socket->Read(&buffer, 1);

        if (readBytes == -1) {
            Connected = false; // -- Read error occured.
            return ircString;
        }

        ircString.append(std::string(1, buffer).c_str());

        if ((ircString.length() > 2 && std::string(ircString.end()-2, ircString.end()) == "\r\n"))
            break;
    } while(readBytes);

    /*
    while (ircLast != "\r\n") {
        readBytes = Socket.Read(buffer, 1);

        if (readBytes == -1) {
            Connected = false; // -- Read error occured.
            return ircString;
        }
        ircString.append(buffer); //ircString = ircString + buffer;
        ircLast = std::string(ircString.end()-2, ircString.end());
        //ircLast = ircLast.substr(1, 1) + buffer;
        free(buffer);
        buffer = new char[1];
    }
     */

    //free(buffer);
    return  ircString;
}

void IrcBot::ParseLoop() {
    while (Connected) {
        std::string rawMessage;
        std::string prefix;
        std::string command;
        std::vector<std::string> args;

        rawMessage = ReadMessage();

        if (rawMessage.substr(0 ,1) == ":") {
            prefix = rawMessage.substr(1, rawMessage.find(' ', 0) - 1);
            int inx = rawMessage.find(" ");
            args = split(rawMessage.substr((unsigned int) (inx + 1), rawMessage.length() - (inx - 1)), ' ');
            command = args[0];
            args.erase(args.begin());
        } else {
            command = rawMessage.substr(0, rawMessage.find(' ', 0));
            int inx = rawMessage.find(" ");
            args = split(rawMessage.substr((unsigned int) (inx + 1), rawMessage.length() - (inx - 1)), ' ');
            std::cout << command << std::endl;
        }

        if (command == "PING") {
            SendRaw("PONG " + args[0]);
            continue;
        }

        std::cout << "Command: " << command << std::endl;

        // 451: Have not registered.
        if (command == "451") {
            SendRaw("NICK " + Nickname);
            SendRaw("USER " + Identification + " CBOT CBOT :" + RealName);
            SendRaw("MODE " + Nickname + " +B-x");
            std::cout << "Registered" << std::endl;
            continue;
        }

        if (command == "NOTICE") {
            std::string message = join(args);
            std::cout << prefix << ">> " << message << std::endl;
            continue;
        }

        if (command == "376") {
            SendRaw("JOIN #test");
            SendRaw("PRIVMSG #test :I am a test C++ bot!");
            continue;
        }

        std::cout << "RAW: " << rawMessage << std::endl;
        //std::cout << "Args:";
        //for(std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it) {
        //     std::cout << *it << std::endl;
        //}
    }
}
