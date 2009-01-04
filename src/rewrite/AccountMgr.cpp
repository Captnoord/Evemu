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
	Author:		Captnoord
*/

#include "EvemuPCH.h"
#include "PasswordModule.h"
#include "AccountMgr.h"

initialiseSingleton( AccountMgr );

AccountMgr::AccountMgr()
{
	Log.Notice("Account manager", "Initializing...");
	_updateAccountInfo();
}

AccountMgr::~AccountMgr()
{
	Log.Notice("Account manager", "Shuting down...");

	mLock.Acquire();
	AccountInfoMapItr Itr = mAccountContainer.begin();
	for (;Itr != mAccountContainer.end(); Itr++)
	{
		AccountInfo *entry = Itr->second;

		SafeDeleteArray(entry->AccountName);
		SafeDeleteArray(entry->AccountPassWord);
		SafeDelete(entry);
	}
	mLock.Release();
}

void AccountMgr::_updateAccountInfo()
{
	QueryResult *result = DynamicDatabase.Query("SELECT * FROM account");
	if(!result)
	{
		sLog.String("Query failed: SELECT * FROM account");
		return;
	}

	do 
	{
		Field *field = result->Fetch();
		uint32 accountId			= field[0].GetUInt32();
		const char *accountName		= field[1].GetString();
		const char *accountPass		= field[2].GetString();
		const char* accountShaHash	= field[3].GetString();
		uint32 accountRole			= field[4].GetUInt32();
		const bool changed			= field[6].GetBool();

		size_t nameLen = strlen(accountName);
		size_t passLen = strlen(accountPass);

		wchar_t * uAccountName = new wchar_t[nameLen+1];
		wchar_t * uAccountPass = new wchar_t[passLen+1];

		mbstowcs(uAccountName, accountName, nameLen); uAccountName[nameLen] = '\0';
		mbstowcs(uAccountPass, accountPass, passLen); uAccountPass[passLen] = '\0';

		_addAccountInfo(accountId, uAccountName, uAccountPass, (uint8*)accountShaHash, accountRole, changed);

		SafeDeleteArray(uAccountName);
		SafeDeleteArray(uAccountPass);

	} while(result->NextRow());

	delete result;
}

void AccountMgr::_addAccountInfo(const uint32 accountId, const wchar_t * accountName, const wchar_t * accountPassWord, const uint8 *shaHash/*size 20 byts*/, const uint32 accountRole, bool changed)
{
	AccountInfo * accountEntry = new AccountInfo;

	accountEntry->AccountId = accountId;
	accountEntry->AccountRole = accountRole;

	size_t aNameLen = wcslen(accountName);
	size_t aPassWordLen = wcslen(accountPassWord);
	size_t aShaHashLen = 20;

	accountEntry->AccountName = new wchar_t[aNameLen];
	accountEntry->AccountPassWord = new wchar_t[aPassWordLen];

	memcpy(accountEntry->AccountName, accountName, (aNameLen+1) * 2);
	memcpy(accountEntry->AccountPassWord, accountPassWord, (aPassWordLen+1) * 2);

	if (shaHash != NULL && *shaHash != '\0' && changed == false)
	{
		memcpy(accountEntry->AccountShaHash, shaHash, aShaHashLen);
	}
	else
	{
		std::string hash;
		PasswordModule::GeneratePassHash(accountName, accountPassWord, hash);
		_updateAccountHash(accountName, accountPassWord, hash);

		memcpy(accountEntry->AccountShaHash, hash.c_str(), aShaHashLen);
	}

	uint32 hashValue = HashUtil::djb2_hash(accountName);

	mAccountContainer.insert(std::make_pair(hashValue, accountEntry));
}

void AccountMgr::_updateAccountHash(const wchar_t * accountName, const wchar_t * accountPassWord, const std::string &shaHash)
{
	std::string hexHash = shaHash;
	hexHash = DynamicDatabase.EscapeString(hexHash);
	bool success = DynamicDatabase.WaitExecute("UPDATE account SET hash = '%s' WHERE username = '%ls' AND password = '%ls'", hexHash.c_str(), accountName, accountPassWord);
	
	if (success == false)
		sLog.Error("AccountMgr", "Failed query: 'UPDATE account SET hash = '%s' WHERE username = '%s' AND password = '%s''", hexHash.c_str(), accountName, accountPassWord);
}

/* todo change this mutex into a read lock */
AccountInfo* AccountMgr::lookupAccount(std::wstring & userName)
{
	mLock.Acquire();
	uint32 hashValue = HashUtil::djb2_hash(userName);

	AccountInfoMapConstItr Itr = mAccountContainer.find(hashValue);
	if (Itr != mAccountContainer.end())
	{
		mLock.Release();
		return Itr->second;
	}
	mLock.Release();
	return NULL;
}
