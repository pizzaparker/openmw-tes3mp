//
// Created by koncord on 13.01.16.
//

#include "PacketDynamicStats.hpp"
#include <components/openmw-mp/NetworkMessages.hpp>

using namespace mwmp;

PacketDynamicStats::PacketDynamicStats(RakNet::RakPeerInterface *peer) : PlayerPacket(peer)
{
    packetID = ID_GAME_DYNAMICSTATS;
}

void PacketDynamicStats::Packet(RakNet::BitStream *bs, BasePlayer *player, bool send)
{
    PlayerPacket::Packet(bs, player, send);
    RW(player->creatureStats.mDynamic[0], send); // health
    RW(player->creatureStats.mDynamic[1], send); // magic
    RW(player->creatureStats.mDynamic[2], send); // fatigue
}