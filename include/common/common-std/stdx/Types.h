/**
 * @file stdx/Types.h
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

#ifndef __COMMON__STDX__TYPES_H__INCL__
#define __COMMON__STDX__TYPES_H__INCL__

/*
 * u?int(8|16|32|64)
 */
#ifdef MSVC
typedef   signed __int8   int8;
typedef unsigned __int8  uint8;
typedef   signed __int16  int16;
typedef unsigned __int16 uint16;
typedef   signed __int32  int32;
typedef unsigned __int32 uint32;
typedef   signed __int64  int64;
typedef unsigned __int64 uint64;
#else /* !MSVC */
typedef  int8_t   int8;
typedef uint8_t  uint8;
typedef  int16_t  int16;
typedef uint16_t uint16;
typedef  int32_t  int32;
typedef uint32_t uint32;
typedef  int64_t  int64;
typedef uint64_t uint64;
#endif /* !MSVC */

/*
 * MSVC:
 *   PRI[diouxX](8|16|32|64)
 *   SCN[diouxX](8|16|32|64)
 */
#ifdef MSVC
#   define PRId8 "hhd"
#   define PRIi8 "hhi"
#   define PRIo8 "hho"
#   define PRIu8 "hhu"
#   define PRIx8 "hhx"
#   define PRIX8 "hhX"

#   define PRId16 "hd"
#   define PRIi16 "hi"
#   define PRIo16 "ho"
#   define PRIu16 "hu"
#   define PRIx16 "hx"
#   define PRIX16 "hX"

#   define PRId32 "I32d"
#   define PRIi32 "I32i"
#   define PRIo32 "I32o"
#   define PRIu32 "I32u"
#   define PRIx32 "I32x"
#   define PRIX32 "I32X"

#   define PRId64 "I64d"
#   define PRIi64 "I64i"
#   define PRIo64 "I64o"
#   define PRIu64 "I64u"
#   define PRIx64 "I64x"
#   define PRIX64 "I64X"

#   define SCNd8 "hhd"
#   define SCNi8 "hhi"
#   define SCNo8 "hho"
#   define SCNu8 "hhu"
#   define SCNx8 "hhx"

#   define SCNd16 "hd"
#   define SCNi16 "hi"
#   define SCNo16 "ho"
#   define SCNu16 "hu"
#   define SCNx16 "hx"

#   define SCNd32 "I32d"
#   define SCNi32 "I32i"
#   define SCNo32 "I32o"
#   define SCNu32 "I32u"
#   define SCNx32 "I32x"

#   define SCNd64 "I64d"
#   define SCNi64 "I64i"
#   define SCNo64 "I64o"
#   define SCNu64 "I64u"
#   define SCNx64 "I64x"
#endif /* MSVC */

#endif /* !__COMMON__STDX__TYPES_H__INCL__ */
