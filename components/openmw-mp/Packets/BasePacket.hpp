#ifndef OPENMW_BASEPACKET_HPP
#define OPENMW_BASEPACKET_HPP

#include <string>
#include <RakNetTypes.h>
#include <BitStream.h>
#include <PacketPriority.h>


namespace mwmp
{
    class BasePacket
    {
    public:
        explicit BasePacket(RakNet::RakPeerInterface *peer);

        virtual ~BasePacket() = default;

        virtual void Packet(RakNet::BitStream *bs, bool send);
        virtual uint32_t Send(bool toOtherPlayers = true);
        virtual uint32_t Send(RakNet::AddressOrGUID destination);
        virtual void Read();

        void setGUID(RakNet::RakNetGUID guid);
        RakNet::RakNetGUID getGUID();

        void SetReadStream(RakNet::BitStream *bitStream);
        void SetSendStream(RakNet::BitStream *bitStream);
        void SetStreams(RakNet::BitStream *inStream, RakNet::BitStream *outStream);
        virtual uint32_t RequestData(RakNet::RakNetGUID guid);

        static inline size_t headerSize()
        {
            return static_cast<size_t>(1 + RakNet::RakNetGUID::size()); // packetID + RakNetGUID (uint64_t)
        }

        unsigned char GetPacketID() const
        {
            return packetID;
        }

    protected:
        template<class templateType>
        void RW(templateType &data, unsigned int size, bool write)
        {
            if (write)
                bs->Write(data, size);
            else
                bs->Read(data, size);
        }

        template<class templateType>
        void RW(templateType &data, bool write, bool compress = 0)
        {
            if (write)
            {
                if (compress)
                    bs->WriteCompressed(data);
                else
                    bs->Write(data);
            }
            else
            {
                if (compress)
                    bs->ReadCompressed(data);
                else
                    bs->Read(data);
            }
        }

        void RW(bool &data, bool write)
        {
            if (write)
                bs->Write(data);
            else
                bs->Read(data);
        }

        void RW(std::string &str, bool write, bool compress = false)
        {
            if (write)
            {
                if (compress)
                    RakNet::RakString::SerializeCompressed(str.c_str(), bs);
                else
                {
                    RakNet::RakString rstr;
                    rstr = str.c_str();
                    bs->Write(rstr);
                }
            }
            else
            {
                RakNet::RakString rstr;
                if (compress)
                    rstr.DeserializeCompressed(bs);
                else
                    bs->Read(rstr);
                str = rstr.C_String();
            }
        }

    protected:
        uint8_t packetID;
        PacketReliability reliability;
        PacketPriority priority;
        char orderChannel;
        RakNet::BitStream *bsRead, *bsSend, *bs;
        RakNet::RakPeerInterface *peer;
        RakNet::RakNetGUID guid;
    };
}

#endif //OPENMW_BASEPACKET_HPP
