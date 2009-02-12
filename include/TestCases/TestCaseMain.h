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

#ifndef _TEST_CASE_MAIN_H
#define _TEST_CASE_MAIN_H

/**
 * @brief simple test case to test CRowSets
 *
 * Its based on content on the database.
 *
 */
static bool testCRowSet01()
{
	QueryResult * queryResult = StaticDatabase.QueryNA("SELECT reactionTypeID,input,typeID,quantity FROM invTypeReactions");
	if (queryResult == NULL)
	{
		Log.Error("TEST_CASE","crowset sql query sucks bigtime");
		return false;
	}

	PyStream * test = CRowSetFactory::BuildCRowSetPacket(queryResult);
	if (test == NULL)
		return false;
	delete test;
	return true;
}

static bool testUnicodeFunctions()
{
	const char* testString = "Ome henk is een hele fijne vent, wel doet hij soms een beetje raar.\nOok houd hij erg van pinda rotsjes.\n";
	PyUnicodeUCS2* str = PyUnicodeUCS2_DecodeUTF8(testString, strlen(testString));

}


class TestCast
{
public:
	static void DoTests()
	{
		testCRowSet01();
	}
};


#endif