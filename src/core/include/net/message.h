#pragma once 

#include <vector>

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
        NOTIFICATION,
        CHAT_MESSAGE,
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
        void addData(T const& data, Args const& ...args)
        {
            addData(data);
            addData(args...);
        }

        // COULD: 
        // add chunks of data with fixed size everytime we resize and check if the current 
        // capacity allows adding to the buffer before resizing it
        template <typename T>
        void addData(T const& data)
        {
            std::size_t bodySize = body.size();
            body.resize(body.size() + sizeof(T));
            // add at the end of buffer
            std::memcpy(body.data() + bodySize, &data, sizeof(T));
            // update size in header
            header.size = body.size();
        }

        void addBuffer(const void* buffer, std::size_t bufSize)
        {
            std::size_t bodySize = body.size();
            body.resize(body.size() + bufSize);
            std::memcpy(body.data() + bodySize, buffer, bufSize);
            header.size = body.size();

            // store size so that the receiver can know the buffer size
            addData<std::size_t>(bufSize);
        }

        template <typename T, typename ...Args>
        void retrieveData(T& data, Args& ...args)
        {
            retrieveData(data);
            retrieveData(args...);
        }

        template <typename T>
        void retrieveData(T& data)
        {
            std::size_t currBodySize = body.size();
            if (currBodySize <= 0)
            {
                return;
            }
            std::size_t bodyDataLocation = currBodySize - sizeof(T);
            std::memcpy(&data, body.data() + bodyDataLocation, sizeof(T));
            body.resize(bodyDataLocation);
            header.size = body.size();
        }

        void retrieveBuffer(void* buffer)
        {
            std::size_t currBodySize = body.size();
            if (currBodySize <= 0)
            {
                return;
            }
            // retrieve size first
            std::size_t bufSize = 0;
            retrieveData<std::size_t>(bufSize);
            if (bufSize <= 0)
            {
                // there's actually no buffer?
                return;
            }

            std::size_t bodyBufferLocation = body.size() - bufSize;
            std::memcpy(buffer, body.data() + bodyBufferLocation, bufSize);
            body.resize(bodyBufferLocation);
            header.size = body.size();
        }
    };

    struct ExampleMsgData
    {
        int value = 0;
        float value2 = 0;
        std::string strValue;

        void serializeMsgInto(Message& msg)
        {
            msg.addData(value, value2);
            msg.addBuffer(strValue.c_str(), strValue.size() + 1);
        }
        void deserializeMsgFrom(Message& msg)
        {
            msg.retrieveBuffer(strValue.data());
            msg.retrieveData(value2, value);
        }
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

        void serializeInto(Message& msg) const override
        {
            msg.addBuffer(text.c_str(), text.length() + 1);
            msg.addBuffer(username.c_str(), username.length() + 1);
            msg.addData(hostID);
        }
        void deserializeFrom(Message& msg) override
        {
            msg.retrieveData(hostID);
            msg.retrieveBuffer(username.data());
            msg.retrieveBuffer(text.data());
        }
    };

    struct ConnectionEstablished : public MessagePack
    {
        uint32_t hostID;

        void serializeInto(Message& msg) const override
        {
            msg.addData(hostID);
        }

        void deserializeFrom(Message& msg) override
        {
            msg.retrieveData(hostID);
        }
    };
}