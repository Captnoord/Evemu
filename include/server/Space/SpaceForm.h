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

struct Constellation;
struct SolarSystem;

struct Region {
	uint32 regionID;

	std::wstring regionName;

    float x,y,z;
    float xMin,yMin,zMin;
    float xMax,yMax,zMax;
    float radius;
    
    uint32 factionID;	
    
	uint32 constellationSize;
	Constellation *constellations;
}

struct Constellation {
	uint32 constellationID;
	uint32 regionID;

	std::wstring constellationName;

    float x,y,z;
    float xMin,yMin,zMin;
    float xMax,yMax,zMax;
    float radius;
    
    uint32 factionID;

	// the tasty stuff
	uint32 solarsystemSize;
	SolarSystem *solarsystems;	
}

struct SolarSystem {
	uint32 solarsystemID;
	uint32 regionID;
  	uint32 constellationID;

    std::wstring solarSystemName;

	// stuff...
    float x,y,z;
    float xMin,yMin,zMin;
    float xMax,yMax,zMax;
    float radius;
    
    float luminosity;
    
    uint32 border;
    uint32 fringe;
  	uint32 corridor;
  	uint32 hub;
  	uint32 international;
    uint32 regional;
    uint32 constellation;
    
    float security;
    uint32 factionID;

    uint32 sunTypeID;
    char securityClass[2];
    
    MapOctree solarSystemMapOctree;
}

#endif//__SPACEFORM_H
