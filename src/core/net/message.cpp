#include <net/message.h>

#include <iostream>


namespace core::messages
{
    // COULD: 
    // add chunks of data with fixed size everytime we resize and check if the current 
    // capacity allows adding to the buffer before resizing it
    template <typename T, typename ...Args>
    void Message::addData(T const& data, Args const& ...args)
    {
        addData(data);
        addData(args...);
    }

    template <typename T>
    void Message::addData(T const& data)
    {
        static_assert(std::is_standard_layout<T>::value, "Data is too complex to be pushed into vector");

        std::size_t bodySize = body.size();
        body.resize(body.size() + sizeof(T));
        // add at the end of body
        std::memcpy(body.data() + bodySize, &data, sizeof(T));
        // update size in header
        header.size = body.size();
    }

    void Message::addBuffer(const void* buffer, std::size_t bufSize)
    {
        std::size_t bodySize = body.size();
        body.resize(body.size() + bufSize);
        std::memcpy(body.data() + bodySize, buffer, bufSize);
        header.size = body.size();

        // store size so that the receiver can know the buffer size
        addData<uint32_t>(static_cast<uint32_t>(bufSize));
    }

    template <typename T, typename ...Args>
    void Message::retrieveData(T& data, Args& ...args)
    {
        retrieveData(data);
        retrieveData(args...);
    }

    template <typename T>
    void Message::retrieveData(T& data)
    {
        static_assert(std::is_standard_layout<T>::value, "Data is too complex to be read from vector");

        if (body.size() < sizeof(T)) {
            return;
        }

        std::size_t dataLocation = body.size() - sizeof(T);
        std::memcpy(&data, body.data() + dataLocation, sizeof(T));
        body.resize(dataLocation);
        header.size = body.size();
    }

    void Message::retrieveBuffer(void* buffer, uint32_t bufSize)
    {
        std::size_t currBodySize = body.size();
        if (currBodySize <= 0)
        {
            return;
        }

        std::size_t bufferLocation = body.size() - static_cast<std::size_t>(bufSize);
        std::memcpy(buffer, body.data() + bufferLocation, bufSize);
        body.resize(bufferLocation);
        header.size = body.size();
    }

    void Message::cleanup()
    {
        body.clear();
        body.resize(0);
        header.size = 0;
    }

    void ChatMessage::serializeInto(Message& msg) const
    {
        msg.header.id = MessageID::CHAT_MESSAGE;
        msg.addData(hostID);
        msg.addBuffer(text.c_str(), text.length() + 1);
        msg.addBuffer(username.c_str(), username.length() + 1);
    }

    void ChatMessage::deserializeFrom(Message& msg)
    {
        uint32_t userSize;
        msg.retrieveData(userSize);
        username.resize(userSize);
        msg.retrieveBuffer(username.data(), userSize);

        uint32_t textSize;
        msg.retrieveData(textSize);
        text.resize(textSize);
        msg.retrieveBuffer(text.data(), textSize);

        msg.retrieveData(hostID);
    }


    void HostConnection::serializeInto(Message& msg) const
    {
        msg.header.id = MessageID::HOST_CONNECTION;
        msg.addData(hostID);
        msg.addBuffer(username.c_str(), username.length() + 1);
    }

    void HostConnection::deserializeFrom(Message& msg)
    {
        uint32_t userSize;
        msg.retrieveData(userSize);
        username.resize(userSize);
        msg.retrieveBuffer(username.data(), userSize);

        msg.retrieveData(hostID);
    }


    void ConnectionEstablished::serializeInto(Message& msg) const
    {
        msg.header.id = MessageID::CONNECTION_ESTABLISHED;
        msg.addBuffer(username.c_str(), username.length() + 1);
    }

    void ConnectionEstablished::deserializeFrom(Message& msg)
    {
        uint32_t userSize;
        msg.retrieveData(userSize);
        username.resize(userSize);
        msg.retrieveBuffer(username.data(), userSize);
    }


    void HostData::serializeInto(Message& msg) const
    {
        msg.header.id = MessageID::HOST_DATA;
        msg.addData(hostID);
    }

    void HostData::deserializeFrom(Message& msg)
    {
        msg.retrieveData(hostID);
    }
}