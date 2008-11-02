/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "Common.h"
#include "Log.h"
#include "BufferPool.h"

#ifdef ENABLE_BUFFER_POOLS

size_t BufferPool::BufferBucket::buffer_sizes[BUFFER_BUCKET_COUNT] = {
	20,			// 20 bytes
	50,			// 50 bytes
	100,		// 100 bytes
	200,		// 200 bytes
	500,		// 500 bytes
	1024,		// 1 kbyte
	5120,		// 5 kbyte
	10240,		// 10 kbyte (shouldn't be used much)
};

BufferPool g_bufferPool;

#endif
