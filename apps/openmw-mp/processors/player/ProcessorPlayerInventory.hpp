//
// Created by koncord on 01.04.17.
//

#ifndef OPENMW_PROCESSORPLAYERINVENTORY_HPP
#define OPENMW_PROCESSORPLAYERINVENTORY_HPP

#include "../PlayerProcessor.hpp"

namespace mwmp
{
    class ProcessorPlayerInventory final: public PlayerProcessor
    {
    public:
        ProcessorPlayerInventory()
        {
            BPP_INIT(ID_PLAYER_INVENTORY)
        }

        void Do(PlayerPacket &packet, const std::shared_ptr<Player> &player) override
        {
            DEBUG_PRINTF(strPacketID.c_str());

            Networking::get().getState().getEventCtrl().Call<CoreEvent::ON_PLAYER_INVENTORY>(player.get());
        }
    };
}

#endif //OPENMW_PROCESSORPLAYERINVENTORY_HPP
