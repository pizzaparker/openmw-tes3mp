#ifndef OPENMW_ACTORPROCESSOR_HPP
#define OPENMW_ACTORPROCESSOR_HPP


#include <components/openmw-mp/Base/BasePacketProcessor.hpp>
#include <components/openmw-mp/Packets/BasePacket.hpp>
#include <components/openmw-mp/Packets/Actor/ActorPacket.hpp>
#include <components/openmw-mp/NetworkMessages.hpp>
#include "Players.hpp"

namespace mwmp
{
    class ActorProcessor : public BasePacketProcessor<ActorProcessor>
    {
    public:

        virtual void Do(ActorPacket &packet, const std::shared_ptr<Player> &player, BaseActorList &actorList);

        static bool Process(RakNet::Packet &packet, BaseActorList &actorList) noexcept;
    };
}

#endif //OPENMW_ACTORPROCESSOR_HPP
