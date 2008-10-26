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

#ifdef ASCENT_FRAMEWORK_ENABLE_OPENSSL

#include "WowCrypt.h"
#include "Sha1.h"

/* 2.4.3 encryption */
uint8 g_243buf1[64];
uint8 g_243buf2[64];
static const uint8 g_243seed[16] = { 0x38, 0xA7, 0x83, 0x15, 0xF8, 0x92, 0x25, 0x30, 0x71, 0x98, 0x67, 0xB1, 0x8C, 0x4, 0xE2, 0xAA };

void WowCrypt::GlobalInit()
{
	uint32 i;
	memset(g_243buf1, 0x36, 64);
	memset(g_243buf2, 0x5c, 64);

	for( i = 0; i < 16; ++i )
	{
		g_243buf1[i] ^= g_243seed[i];
		g_243buf2[i] ^= g_243seed[i];
	}
}

WowCrypt::WowCrypt()
{
	_initialized = false;
	_key = NULL;
}

void WowCrypt::Init()
{
	_send_i = _send_j = _recv_i = _recv_j = 0;
	_initialized = true;
}

void WowCrypt::DecryptRecv(uint8 *data, size_t len)
{
	if (!_initialized) return;
	if (len < CRYPTED_RECV_LEN) return;
	uint8 x;

	for (size_t t = 0; t < CRYPTED_RECV_LEN; t++) {
		_recv_i %= _keyLen;
		x = (data[t] - _recv_j) ^ _key[_recv_i];
		++_recv_i;
		_recv_j = data[t];
		data[t] = x;
	}
}

void WowCrypt::DecryptSixRecv(uint8 *data)
{
    if (!_initialized) return;
    
    uint8 x, KeySize = (uint8)_keyLen;

    // 0
    _recv_i %= KeySize;
    x = (data[0] - _recv_j) ^ _key[_recv_i];
    ++_recv_i;
    _recv_j = data[0];
    data[0] = x;

    // 1
    _recv_i %= KeySize;
    x = (data[1] - _recv_j) ^ _key[_recv_i];
    ++_recv_i;
    _recv_j = data[1];
    data[1] = x;

    // 2
    _recv_i %= KeySize;
    x = (data[2] - _recv_j) ^ _key[_recv_i];
    ++_recv_i;
    _recv_j = data[2];
    data[2] = x;

    // 3
    _recv_i %= KeySize;
    x = (data[3] - _recv_j) ^ _key[_recv_i];
    ++_recv_i;
    _recv_j = data[3];
    data[3] = x;

    // 4
    _recv_i %= KeySize;
    x = (data[4] - _recv_j) ^ _key[_recv_i];
    ++_recv_i;
    _recv_j = data[4];
    data[4] = x;

    // 5
    _recv_i %= KeySize;
    x = (data[5] - _recv_j) ^ _key[_recv_i];
    ++_recv_i;
    _recv_j = data[5];
    data[5] = x;
}

void WowCrypt::EncryptSend(uint8 *data, size_t len)
{
	if (!_initialized) return;
	if (len < CRYPTED_SEND_LEN) return;

	for (size_t t = 0; t < CRYPTED_SEND_LEN; t++) {
		_send_i %= _keyLen;
		data[t] = _send_j = (data[t] ^ _key[_send_i]) + _send_j;
        ++_send_i;
	}
}

void WowCrypt::EncryptFourSend(uint8 * data)
{
    if (!_initialized) return;

    uint8 KeySize = (uint8)_keyLen;

    _send_i %= KeySize;
    data[0] = _send_j = (data[0] ^ _key[_send_i]) + _send_j;
    ++_send_i;

    _send_i %= KeySize;
    data[1] = _send_j = (data[1] ^ _key[_send_i]) + _send_j;
    ++_send_i;

    _send_i %= KeySize;
    data[2] = _send_j = (data[2] ^ _key[_send_i]) + _send_j;
    ++_send_i;

    _send_i %= KeySize;
    data[3] = _send_j = (data[3] ^ _key[_send_i]) + _send_j;
    ++_send_i;
}

void WowCrypt::SetKey(uint8 *key)
{
	Sha1Hash h1, h2;
	h1.UpdateData(g_243buf1, 64);
	h1.UpdateData(key, 40);
	h1.Finalize();
	h2.UpdateData(g_243buf2, 64);
	h2.UpdateData(h1.GetDigest(), 20);
	h2.Finalize();

	_key = new uint8[20];
	_keyLen = 20;
	memcpy(_key, h2.GetDigest(), 20);
}

WowCrypt::~WowCrypt()
{
	if( _key != NULL )
	{
		delete [] _key;
		//_key = NULL;
	}
}

#endif//ASCENT_FRAMEWORK_ENABLE_OPENSSL