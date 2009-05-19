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
	Author:		Captnoord
*/

#ifndef _CONFIGMGR_H
#define _CONFIGMGR_H

/**
 * \class ConfigMgr
 *
 * @brief c++ version of the pyton 'config' class
 *
 * this class acts the same way as the config class in eve python.
 *
 * @author Captnoord
 * @date April 2009
 */
class ConfigMgr : public Singleton<ConfigMgr>
{
public:
	ConfigMgr();
	~ConfigMgr();

	bool init();

	void initServiceInfo();
	void initBulkData();

	/**
	* this function loads the bulk data from bulkdata
	*/
	void loadFromBulk();

	/**
	 * \brief gets the 'initvals' for the client
	 *
	 * no long description.
	 *
	 * @return a PyObject containing the 'initvals' for the client.
	 */
	PyObject * GetInitValsClient();
private:

	PyObject * mInitValsClient;

	PyObject * mMachoNetServiceInfo;

	// keep the amount of none objects to a minimum so share it as much as possible.
	PyBaseNone PyNone;

	// we keep this class here because it contains the static objects we use in the cached objects.
	MarshalStream mMarshalStream;
};
#endif // _CONFIGMGR_H

#define sConfigMgr ConfigMgr::getSingleton()