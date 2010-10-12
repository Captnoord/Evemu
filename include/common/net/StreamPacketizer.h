/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of EVEmu: EVE Online Server Emulator
    Copyright 2006 - 2008 The EVEmu Team
    For the latest information visit http://evemu.mmoforge.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
    Author:     Zhur
*/

#ifndef __NET__STREAM_PACKETIZER_H__INCL__
#define __NET__STREAM_PACKETIZER_H__INCL__

#include "util/Buffer.h"

namespace Net
{
    /**
     * @brief Turns a stream into packets.
     *
     * @author Bloody.Rabbit
     */
    class StreamPacketizer
    {
    public:
        /**
         * @brief A destructor.
         *
         * Performs cleanup by calling ClearBuffers.
         */
        ~StreamPacketizer();

        /**
         * @brief Inputs new data.
         *
         * @param[in] data The data.
         */
        void InputData( const Util::Data& data );
        /**
         * @brief Processes input data into packets.
         */
        void Process();

        /**
         * @brief Obtains a next packet in the queue.
         *
         * @return The packet.
         */
        Util::Buffer* PopPacket();

        /**
         * @brief Deletes all remaining packets.
         */
        void ClearBuffers();

    protected:
        /// Unprocessed or uncomplete packet data.
        Util::Buffer mBuffer;

        /// A queue of processed packets.
        std::queue< Util::Buffer* > mPackets;
    };
}

#endif /* !__NET__STREAM_PACKETIZER_H__INCL__ */
