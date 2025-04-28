#include <string>

// TODO: 
// add MessageHeader with size and Message with header and byte data
// which will be serialized from and deserialized into json

// COULD:
// add separate MessageSerializer class and here just define the generic message
// add an id to header to differenciate simpler messages without any needed json

namespace core
{
    template <typename Category>
    class MessageHeader
    {
        Category id;
        uint32_t size;
    };

    struct Message
    {
        void* data;
        // better to use sized ints for portability
        uint32_t size = sizeof(uin32_t);

    public:
        void serialize(char* payload)
        {
            char sizeBytes[sizeof size + 1];
            sizeBytes[0] = (size >> 24) & 0xFF;
            sizeBytes[1] = (size >> 16) & 0xFF;
            sizeBytes[2] = (size >> 8) & 0xFF;
            sizeBytes[3] = (size) & 0xFF;
            sizeBytes[4] = '\0';

            realloc(payload, (sizeof sizeBytes) + strlen((char*)body) + 1);
            strcpy(payload, sizeBytes);
            strcat(payload, (char*)body);
        }

        bool deserialize(void* msgBuffer, uint32_t bytes)
        {
            uint32_t sizeOfSize = sizeof size;
            if (sizeOfSize <= bytes)
            {
                return false;
            }

            char* buffer = (char*)msgBuffer;
            memcpy(&size, buffer, sizeOfSize);
            memcpy(&data, (char*)msgBuffer + sizeOfSize, bytes - sizeOfSize);
            return true;
        }
    };
}