#include <components/openmw-mp/NetworkMessages.hpp>
#include "PacketPlayerSpellbook.hpp"

using namespace std;
using namespace mwmp;

PacketPlayerSpellbook::PacketPlayerSpellbook(RakNet::RakPeerInterface *peer) : PlayerPacket(peer)
{
    packetID = ID_PLAYER_SPELLBOOK;
}

void PacketPlayerSpellbook::Packet(RakNet::BitStream *bs, bool send)
{
    PlayerPacket::Packet(bs, send);

    RW(player->spellbookChanges.action, send);

    uint32_t changesCount;
    
    if (send)
        changesCount = static_cast<uint32_t>(player->spellbookChanges.spells.size());

    RW(changesCount, send);

    if (!send)
    {
        player->spellbookChanges.spells.clear();
        player->spellbookChanges.spells.resize(changesCount);
    }

    for (auto &&spell : player->spellbookChanges.spells)
    {
        RW(spell.mId, send, true);
    }
}
