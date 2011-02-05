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

#ifndef __UTIL__CRC32_H__INCL__
#define __UTIL__CRC32_H__INCL__

namespace Util
{
     /**
     * @brief Wrapper class for generating CRC-32 checksums.
     *
     * @author Zhur
     */
    class Crc32
    {
    public:
        /**
         * @brief Generates a CRC-32 checksum.
         *
         * @param[in] buf     Data to compute the checksum for.
         * @param[in] bufsize A length of the data.
         *
         * @return The checksum.
         */
        static uint32 Generate( const uint8* buf, size_t bufsize ) { return Finish( Update( buf, bufsize ) ); }
        /**
         * @brief Generates a CRC-32 checksum, not flipping the final result.
         *
         * @param[in] buf     Data to compute the checksum for.
         * @param[in] bufsize A length of the data.
         *
         * @return The not-flipped checksum.
         */
        static uint32 GenerateNoFlip( const uint8* buf, size_t bufsize ) { return Update( buf, bufsize ); }

    protected:
        /**
         * @brief Generates a CRC-32 checksum.
         *
         * @param[in] buf     Data to compute the checksum for.
         * @param[in] bufsize A length of the data.
         * @param[in] crc32   A starting checksum.
         *
         * @return The checksum.
         */
        static uint32 Update( const uint8* buf, size_t bufsize, uint32 crc32 = 0xFFFFFFFF );
        /**
         * @brief Finishes a CRC-32 computation.
         *
         * @param[in] crc32 A checksum to finish.
         *
         * @return The finished checksum.
         */
        static uint32 Finish( uint32 crc32 ) { return ~crc32; }

        /// Precomputed lookup table used to speed up CRC-32 calculation.
        static const uint32 LOOKUP_TABLE[ 0x100 ];
    };
}

#endif /* !__UTIL__CRC32_H__INCL__ */
