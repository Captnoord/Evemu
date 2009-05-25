/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of EVEmu: EVE Online Server Emulator
    Copyright 2006 - 2009 The EVEmu Team
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
    Author:     Captnoord
*/

#ifndef _ACCOUNT_MGR_H
#define _ACCOUNT_MGR_H

// minimal account info struct.
struct AccountInfo
{
    uint32      AccountId;
    wchar_t *   AccountName;
    wchar_t *   AccountPassWord;
    uint8       AccountShaHash[20];
    uint32      AccountRole;        /* actually is a 64 bits value, but mainly no normal user will ever get to the point he gets that flag. */
};

/**
 * \class AccountMgr
 *
 * @brief a small account info tracker singleton
 *
 * a small account info tracker singleton, to calc pass hashes, allow lookups and much more.
 *
 * @author Captnoord
 * @date January 2009
 * @todo add on the fly check if people have changed there password. Add the password check on shutdown to make sure.
 * @note there is a possibility that this class doesn't work the way people want it. Its not complete yet. WIll come in time.
 */
class AccountMgr : public Singleton<AccountMgr>
{
public:
    AccountMgr();
    ~AccountMgr();

    AccountInfo* lookupAccount(std::wstring & userName);

private:
    // go through all the accounts that don't have a pass generated or has the changed flag on.
    void OnAccountEvent();
    void _updateAccountInfo();
    void _addAccountInfo(const uint32 accountId, const wchar_t * accountName, const wchar_t * accountPassWord, const uint8 *shaHash/*size 20 byts*/, const uint32 accountRole, bool changed);
    void _updateAccountHash(const wchar_t * accountName, const wchar_t * accountPassWord, const std::string &shaHash);


    typedef std::tr1::unordered_map<uint32, AccountInfo*>   AccountInfoMap;
    typedef AccountInfoMap::iterator                        AccountInfoMapItr;
    typedef AccountInfoMap::const_iterator                  AccountInfoMapConstItr;

    AccountInfoMap mAccountContainer;
    Mutex mLock;
};

#endif//_ACCOUNT_MGR_H

#define sAccountMgr AccountMgr::getSingleton()
