#ifndef CACHE_H
#define CACHE_H

#include "api_structs.h"
#include <optional>

class Cache {
    friend class Client;

    public:
        #define LOOKUP(type, name, variable) \
        std::optional<type> name(std::string id) { \
            auto it = variable.find(id); \
            if (it != variable.end()) { \
                return it->second; \
            } \
            return std::nullopt; \
        }


        std::unordered_map<user_id, User> users;
        std::unordered_map<message_id, Message> messages;
        std::unordered_map<channel_id, Channel> channels;
        std::unordered_map<server_id, Server> servers;

        user_id self;

        LOOKUP(User, user, users)
        LOOKUP(Channel, channel, channels)
        LOOKUP(Server, server, servers)
        LOOKUP(Message, message, messages)

        std::vector<Message> channelMessages(channel_id channelId) {
            std::vector<Message> msgs;
                for (const auto& message : messages) {
                if (message.second.channel == channelId) {
                    msgs.push_back(message.second);
                }
            }
            return msgs;
        }
};

#endif // CACHE_H
