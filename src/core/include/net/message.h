#pragma once 

#include <vector>
#include <string>

// TODO: 
// add MessageHeader with size and Message with header and byte data
// which can be serialized from and deserialized into types and json
// message implementations should have the serialization logic and should
// be interacted with by the application high level logic instead of raw data (ideally)

// COULD:
// add separate MessageSerializer class and here just define the generic message
// add an id to header to differenciate simpler messages without any needed json

// COULD: 
// store byte buffers in blocks of arranged size and store a smaller int with the amount of blocks to read.
// also should add a character like '\0' to the end of the written buffer to know that its the buffer end and 
// then try to find it from the front since it should be within the last block
// inline const uint16_t kBufBlockSize = 32u;


// TODO: 
// figure out another implementation without std::vector<uint8_t> as body buffer


namespace core::messages
{
    enum MessageID : uint32_t
    {
        CONNECTION_ESTABLISHED,
        SERVER_DATA, // send important data back to a connected host, after receiving username
        HOST_CONNECTION,
        HOST_DISCONNECTED,
        CHAT_MESSAGE,
        NOTIFICATION, // dont use yet
    };

    // header will have to be of a constant size, with basic types and no buffers
    struct MessageHeader
    {
        MessageID id;
        std::size_t size;
    };

    struct Message
    {
        MessageHeader header;
        std::vector<uint8_t> body;

    public:
        template <typename T, typename ...Args>
        void addData(T const& data, Args const& ...args);
        template <typename T>
        void addData(T const& data);
        void addBuffer(const void* buffer, std::size_t bufSize);

        template <typename T, typename ...Args>
        void retrieveData(T& data, Args& ...args);
        template <typename T>
        void retrieveData(T& data);
        void retrieveBuffer(void* buffer, uint32_t bufSize);

        void cleanup();
    };

    struct MessagePack
    {
        virtual ~MessagePack() = default;
        virtual void serializeInto(Message& msg) const = 0;
        virtual void deserializeFrom(Message& msg) = 0;
    };

    struct ChatMessage : public MessagePack
    {
        std::string text;
        std::string username;
        uint32_t hostID;

        void serializeInto(Message& msg) const override;
        void deserializeFrom(Message& msg) override;
    };

    struct HostConnection : public MessagePack
    {
        std::string username;
        uint32_t hostID;

        void serializeInto(Message& msg) const override;
        void deserializeFrom(Message& msg) override;
    };

    struct ConnectionEstablished : public MessagePack
    {
        std::string username;

        void serializeInto(Message& msg) const override;
        void deserializeFrom(Message& msg) override;
    };

    struct ServerData : public MessagePack
    {
        uint32_t hostID;

        void serializeInto(Message& msg) const override;
        void deserializeFrom(Message& msg) override;
    };

    struct HostDisconnected : public MessagePack
    {
        uint32_t hostID;

        void serializeInto(Message& msg) const override;
        void deserializeFrom(Message& msg) override;
    };
}