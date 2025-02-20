#ifndef API_STRUCTS_H
#define API_STRUCTS_H

#include "nlohmann/json.hpp"
#include <iostream>

#define GET_TO(varname) if (json.contains(#varname) && !json.at(#varname).is_null()) { \
    json.at(#varname).get_to(varname); \
}

#define GET_LIST(type, name) if (json.contains(#name) && !json.at(#name).is_null()) { \
    std::vector<nlohmann::json> data = json.at(#name); \
    for (nlohmann::json& item : data) { \
        name.push_back(type(item)); \
    } \
}

#define GET_OBJ(type, name) if (json.contains(#name) && !json.at(#name).is_null()) { \
nlohmann::json data = json.at(#name); \
    if (data.is_object()) name = type(data); \
    else std::cout << #name << " is in fact " << data << std::endl; \
}

#define GET_ID json.at("_id").get_to(id);

// I hope to remove it somewhen
// Currently it won't compile
// unless there is a constructor
// without arguments
#define NO_ARGS_CONSTRUCTOR(strct) strct() { /* std::cerr << #strct << "() without args triggered!!!" << std::endl; */ }

using channel_id = std::string;
using message_id = std::string;
using user_id = std::string;
using server_id = std::string;
using file_id = std::string;

struct File {
    file_id id;
    std::string tag;
    std::string filename;
    long size;

    NO_ARGS_CONSTRUCTOR(File)

    File(nlohmann::json& json) {
        GET_ID
        GET_TO(tag)
        GET_TO(filename)
        GET_TO(size)
    }

    std::string url() {
        if (id.empty()) return "";
        return "https://autumn.revolt.chat/" +tag+ "/" +id+ "/original";
    }

    std::string static_image_url() {
        if (id.empty()) return "";
        return "https://autumn.revolt.chat/" +tag+ "/" +id;
    }
};

struct Message {
    message_id id;
    channel_id channel;
    user_id author;
    std::string content;

    NO_ARGS_CONSTRUCTOR(Message)

    Message(nlohmann::json& json) {
        GET_ID
        GET_TO(channel)
        GET_TO(author)
        GET_TO(content)
    }

};

struct User {
    user_id id;
    std::string username;
    std::string discriminator;
    std::string display_name = "";
    File avatar;

    NO_ARGS_CONSTRUCTOR(User)

    User(nlohmann::json& json) {
        GET_ID
        GET_TO(username)
        GET_TO(discriminator)
        GET_TO(display_name)
        GET_OBJ(File, avatar)
    }

    std::string display() {
        return display_name.empty() ? username : display_name;
    }
};

struct Channel {
    channel_id id;
    std::string channel_type;
    server_id server;
    std::string name;
    std::string description;

    NO_ARGS_CONSTRUCTOR(Channel)

    Channel(nlohmann::json& json) {
        GET_ID
        GET_TO(channel_type)
        GET_TO(server)
        GET_TO(name)
        GET_TO(description)
    }
};

struct Server {
    server_id id;
    user_id owner;
    std::string name;
    std::string description;
    std::vector<channel_id> channels;
    File icon;

    NO_ARGS_CONSTRUCTOR(Server)

    Server(nlohmann::json& json) {
        GET_ID
		GET_TO(owner)
		GET_TO(name)
		GET_TO(description)
        GET_TO(channels)
        GET_OBJ(File, icon)
    }
};

struct ReadyData {
    std::vector<User> users;
    std::vector<Server> servers;
    std::vector<Channel> channels;

    NO_ARGS_CONSTRUCTOR(ReadyData)

    ReadyData(nlohmann::json& json) {
        GET_LIST(User, users)
        GET_LIST(Server, servers)
        GET_LIST(Channel, channels)
    }
};


#endif // API_STRUCTS_H
