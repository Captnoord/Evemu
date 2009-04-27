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
	Author:		mmcs
*/

#ifndef __SPACEFORM_H
#define __SPACEFORM_H


#define REGIONSUMEDVAL 1e+7
#define CONSTELLATIONSUMEDVAL 2e+7
#define SOLARSYSTEMSUMEDVAL 3e+7


struct Region {

	struct {
		double x,y,z;
		double xMin,yMin,zMin;
		double xMax,yMax,zMax;
		double radius;
		
		uint32 factionID;	
	} RegionData;

	// vector of constellations id's
	std::vector<uint32> constellations;
};

struct Constellation {

    struct {
    	uint32 regionID;
		double x,y,z;
		double xMin,yMin,zMin;
		double xMax,yMax,zMax;
		double radius;
		
		uint32 factionID;
	} ConstellationData;
	
	// the tasty stuff
	// vector of solarsystems id's
	std::vector<uint32> solarsystems;	
};
#define OBJECT_NAME_MAX 100
struct SolarSystemObject {

	uint32 objectID;
	uint16 typeID;
	uint16 groupID;
	
	double x,y,z;
	double radius;
	
	char objectName[OBJECT_NAME_MAX];
};

struct SolarSystem {

	struct {
		uint32 constellationID;
		// stuff...
		double x,y,z;
		double xMin,yMin,zMin;
		double xMax,yMax,zMax;
		double radius;
		
		double luminosity;
		
		uint32 border;
		uint32 fringe;
		uint32 corridor;
		uint32 hub;
		uint32 international;
		uint32 regional;
		uint32 constellation;
		
		double security;
		uint32 factionID;

		uint32 sunTypeID;
		//char securityClass[3];
	} SolarSystemData;
    
    
    std::map<uint32,SolarSystemObject> solarsystem_objects;
    //MapOctree solarSystemMapOctree;
};

#endif//__SPACEFORM_H
