#include <vector>

// TODO: 
// add MessageHeader with size and Message with header and byte data
// which will be serialized from and deserialized into json

// COULD:
// add separate MessageSerializer class and here just define the generic message
// add an id to header to differenciate simpler messages without any needed json

namespace core::messages
{
    enum Category : uint32_t
    {
        NOTIFICATION,
        CHAT_MESSAGE,
    };

    // header will have to be of a constant size, with basic types and no buffers
    struct MessageHeader
    {
        Category category;
        uint64_t size;
    };

    struct Message
    {
        MessageHeader header;
        // this will be a json perhaps
        std::vector<uint8_t> body;

        template <typename T, typename ...Args>
        friend void addData(T const& data, Args const& ...args)
        {
            addData(data);
            addData(args);
        }

        template <typename T>
        friend void addData(T const& data)
        {
            size_t bodySize = body.size();
            body.resize(body.size() + sizeof(T));
            // add at the end of buffer
            std::memcpy(body.data() + bodySize, &data, sizeof(T));
            // update size in header
            header.size = body.size();
        }

        template <typename T, typename ...Args>
        friend void retrieveData(T& data, Args& ...args)
        {
            retrieveData(data);
            retrieveData(args);
        }

        template <typename T>
        friend void retrieveData(T&data)
        {
            size_t bodySize = body.size() - sizeof(T);
            // add at the end of buffer
            std::memcpy(&data, body.data() + bodySize, sizeof(T));
            body.resize(bodySize);
            // update size in header
            header.size = body.size();
        }
    };
}