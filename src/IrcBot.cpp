//
// Created by unknown on 10/7/15.
//

#include <sstream>
#include "../include/IrcBot.h"

using namespace std;

IrcBot::IrcBot(string ip, string port, string nick, string ident, string realname) {
    Socket = std::unique_ptr<Sockets>(new Sockets(ip, port)); // -- Create a new socket

    Ip = ip; // -- Assign all of our internal values..
    Port = port;
    Nickname = nick;
    RealName = realname;
    Identification = ident;
    Connected = false;
    InputBuffer = "";
}

vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> elems;

    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }

    return elems;
}

string join(vector<string> input) {
    ostringstream joined;
    copy(input.begin(), input.end(), ostream_iterator<string>(joined, " "));
    return joined.str();
}

bool IrcBot::Connect() {
    if (Connected) // -- If we're already connected, ignore.
        return false;

    bool connectResult;
    connectResult = Socket->Connect(); // -- Try to connect to the irc server.

    if (!connectResult) { // -- Connection failed.
        Connected = false;
        return false;
    }

    Connected = true;

    SendRaw("NICK " + Nickname);
    SendRaw("USER " + Identification + " CBOT CBOT :" + RealName);
    SendRaw("MODE " + Nickname + " +B-x");
    cout << "Connected." << endl;
    return true;
}

void IrcBot::SendRaw(string message) {
    message = message + "\r\n"; // -- Add IRC Ending
    char *msg = (char*) message.c_str(); // -- Convert to char*

    int len;
    len = (int) strlen(msg); // -- Get the length

    Socket->Send(msg, len); // -- Send :)
}

void IrcBot::ReadMessage() { // -- Read a message from IRC.
    if (!Connected)
        return;

    //string ircString(""); // -- The output string
    int readBytes; // -- Number of bytes the socket has read.
    char buffer[512]; // -- The input buffer. (Maximum irc packet size is 512 bytes)

    readBytes = Socket->Read(&buffer, 512); // -- Read one byte.

    if (readBytes == -1) {
        Connected = false; // -- Read error occured.
        return;
    }

    InputBuffer.append(string(buffer, (unsigned long) readBytes).c_str()); // -- Add the byte to the end of our string.
}

void IrcBot::ParseMessage(string rawMessage) {
    string prefix;
    string command;
    vector<string> args;

    if (rawMessage.substr(0 ,1) == ":") {
        prefix = rawMessage.substr(1, rawMessage.find(' ', 0) - 1);
        int inx = (int) rawMessage.find(" ");
        args = split(rawMessage.substr((unsigned int) (inx + 1), rawMessage.length() - (inx - 1)), ' ');
        command = args[0];
        args.erase(args.begin());
    } else {
        command = rawMessage.substr(0, rawMessage.find(' ', 0));
        int inx = (int) rawMessage.find(" ");
        args = split(rawMessage.substr((unsigned int) (inx + 1), rawMessage.length() - (inx - 1)), ' ');
    }

    if (command == "PING") {
        SendRaw("PONG " + args[0]);
        return;
    }

    // 451: Have not registered.
    if (command == "451") {
        SendRaw("NICK " + Nickname);
        SendRaw("USER " + Identification + " CBOT CBOT :" + RealName);
        SendRaw("MODE " + Nickname + " +B-x");
        cout << "=== Registered" << endl;
        return;
    }

    if (command == "307") { // -- Authed message

    }
    if (command == "330") { // -- Authed message

    }
    if (command == "332") { // -- Topic Updated
        cout << " === TOPIC UPDATE" << endl;
    }
    if (command == "353") { // -- Nick list

    }

    if (command == "376") {
        SendRaw("JOIN #test");
        cout << "=== Joined Channel" << endl;
        return;
    }
    if (command == "433") { // -- Username in use..


    }

    if (command == "NOTICE") {
        string message = join(args);
        cout << prefix << " >> " << message << endl;
        return;
    }

    if (command == "NICK") {

    }

    if (command == "PART") {

    }

    if (command == "QUIT") {

    }

    if (command == "TOPIC") {

    }

    if (command == "JOIN") {

    }

    if (command == "PRIVMSG") {
        string channel = args[0];
        args.erase(args.begin());
        string message = join(args);
        string name = prefix.substr(0, prefix.find("!"));
        message = message.substr(1, message.length() - 1);

        if (message.substr(0, 1) == "\1") {
            // CTCP
            if (message.length() >= 8 && message.substr(0, 8) == "\1VERSION") {
                cout << "=== CTCP VERSION RECEIVED ===" << endl;
                SendRaw("NOTICE " + name + " :\1VERSION Umby24 C++ IRC Bot #!#!#!#!\1");
                return;
            }

            if (message.length() >= 5 && message.substr(0, 5) == "\1PING") {
                cout << "=== CTCP PING RECEIVED ===" << endl;
                //SendRaw("NOTICE " + name + " :\1VERSION Umby24 C++ IRC Bot #!#!#!#!");
                return;
            }
            cout << "CTCP..." << endl;
        }

        cout << "[" << channel << "] <" << name << "> " << message << endl;
        return;
    }

    cout << rawMessage << endl;
}

string IrcBot::CurrentMessage() {
    unsigned long endIndex = InputBuffer.find("\r\n");

    if (((long)endIndex) == -1)
        return "";

    string message = InputBuffer.substr(0, endIndex);
    InputBuffer = InputBuffer.substr(endIndex + 2, InputBuffer.length() - (endIndex + 2));

    return message;
}

void IrcBot::ParseLoop() {
    while (Connected) {
        ReadMessage();
        string toParse = CurrentMessage();

        while (toParse != "") {
            ParseMessage(toParse);
            toParse = CurrentMessage();
        }
    }
}
