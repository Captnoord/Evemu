/**
 * @file db/Type.h
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
 * @author Bloody.Rabbit
 */

#ifndef __COMMON__DB__TYPE_H__INCL__
#define __COMMON__DB__TYPE_H__INCL__

namespace common
{
    namespace db
    {
        /**
         * @brief Describles a type of DB field.
         *
         * This enum is a subset of OLE DB's DBTYPEENUM.
         *
         * @see http://msdn.microsoft.com/en-us/library/ms711251.aspx
         *
         * @author Bloody.Rabbit, Captnoord
         */
        enum TYPE
        {
            //TYPE_EMPTY        = 0x00, // not supported
            //TYPE_NULL         = 0x01, // not supported
            TYPE_I2             = 0x02,
            TYPE_I4             = 0x03,
            TYPE_R4             = 0x04,
            TYPE_R8             = 0x05,
            TYPE_CY             = 0x06, // money
            //TYPE_DATE         = 0x07, // not supported
            //TYPE_BSTR         = 0x08, // not supported
            //TYPE_IDISPATCH    = 0x09, // not supported
            TYPE_ERROR          = 0x0A,
            TYPE_BOOL           = 0x0B,
            //TYPE_VARIANT      = 0x0C, // not supported
            //TYPE_IUNKNOWN     = 0x0D, // not supported
            //TYPE_DECIMAL      = 0x0E, // not supported
            //TYPE_BLOB         = 0x0F, // not supported
            TYPE_I1             = 0x10,
            TYPE_UI1            = 0x11,
            TYPE_UI2            = 0x12,
            TYPE_UI4            = 0x13,
            TYPE_I8             = 0x14,
            TYPE_UI8            = 0x15,
            TYPE_FILETIME       = 0x40, // 64-bit timestamp
            //TYPE_GUID         = 0x48, // not supported
            TYPE_BYTES          = 0x80,
            TYPE_STR            = 0x81,
            TYPE_WSTR           = 0x82
            //TYPE_NUMERIC      = 0x83, // not supported
            //TYPE_UDT          = 0x84, // not supported
            //TYPE_DBDATE       = 0x85, // not supported
            //TYPE_DBTIME       = 0x86, // not supported
            //TYPE_DBTIMESTAMP  = 0x87, // not supported
            //TYPE_HCHAPTER     = 0x88, // not supported
            //TYPE_PROPVARIANT  = 0x8A, // not supported
            //TYPE_VARNUMERIC   = 0x8B  // not supported
        };

        /**
         * @brief Obtains length of field of given type.
         *
         * @param[in] type Type of fields.
         *
         * @return Length of field (in bits).
         */
        uint8 TYPE_GetSizeBits( TYPE type );

        /**
         * @brief Obtains length of field of given type.
         *
         * @param[in] type Type of fields.
         *
         * @return Length of field (in bytes).
         */
        uint8 TYPE_GetSizeBytes( TYPE type );
    }
}

#endif /* !__COMMON__DB__TYPE_H__INCL__ */
