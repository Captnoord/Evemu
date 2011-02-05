/**
 * @file net/StreamPacketizer.h
 *
 * This file is part of EVEmu: EVE Online Server Emulator.<br>
 * Copyright (C) 2006-2011 The EVEmu Team<br>
 * For the latest information visit <i>http://evemu.org</i>.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA, or go to
 * <i>http://www.gnu.org/copyleft/lesser.txt</i>.
 *
 * @author Zhur
 */

#ifndef __COMMON__NET__STREAM_PACKETIZER_H__INCL__
#define __COMMON__NET__STREAM_PACKETIZER_H__INCL__

namespace common
{
    namespace net
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
            void InputData( const util::Data& data );
            /**
             * @brief Processes input data into packets.
             */
            void Process();

            /**
             * @brief Obtains a next packet in the queue.
             *
             * @return The packet.
             */
            util::Buffer* PopPacket();

            /**
             * @brief Deletes all remaining packets.
             */
            void ClearBuffers();

        protected:
            /// Unprocessed or uncomplete packet data.
            util::Buffer mBuffer;

            /// A queue of processed packets.
            std::queue< util::Buffer* > mPackets;
        };
    }
}

#endif /* !__COMMON__NET__STREAM_PACKETIZER_H__INCL__ */
