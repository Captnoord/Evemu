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
    Authors:        Captnoord, mmcs
*/

/* Small Todo and note space
   - check http://nedprod.com/programs/portable/nedmalloc/index.html as the we allocate and deallocate very very often due to
     the design
   - Proxy node id is static.
*/

#include "EvemuPCH.h"

initialiseSingleton( Space );

Space::Space() : mAcceptedConnections(0), mAuthorizedConnections(0), mUserIdGeneratorNumber(1), mStartTime(0), mProxyNodeID(666) { }

bool Space::Load_SpaceForm_fromDB(Database *mdb)
{
    /*QueryResult *result = NULL;
    Field *field = NULL;
    uint32 regionID,constellationID,solarsystemID;
    regionID = constellationID = solarsystemID = 0;

    printf("geting max regionID.\n");
    // get max regionID
    result = mdb.Query("select max(regionID) from mapRegions");
    if(!result)
    {
        sLog.String("Query failed: select max(regionID) from mapRegions");
        return false;
    }

    field = result->Fetch();
    m_regionsSize = field[0].GetUInt32() - REGIONSUMEDVAL + 1;

    m_regions = new Region[m_regionsSize];

    for(int i = 0; i < m_regionsSize ; ++i)
        memset(&m_regions.RegionData,0,sizeof(m_regions.RegionData));

    printf("geting region data.\n");
    // get region data
    delete result;
    result = mdb.Query("select regionID,x,y,z,xMin,yMin,zMin,xMax,yMax,zMax,radius,factionID from mapRegions");
    if(!result)
    {
        sLog.String("Query failed: select regionID,x,y,z,xMin,yMin,zMin,xMax,yMax,zMax,radius,factionID from mapRegions");
        return false;
    }

    do
    {
        field = result->Fetch();
        regionID = field[0].GetUInt32() - REGIONSUMEDVAL;
        m_regions[regionID].RegionData.x =  field[1].GetDouble();
        m_regions[regionID].RegionData.y =  field[2].GetDouble();
        m_regions[regionID].RegionData.z =  field[3].GetDouble();
        m_regions[regionID].RegionData.xMin =  field[4].GetDouble();
        m_regions[regionID].RegionData.yMin =  field[5].GetDouble();
        m_regions[regionID].RegionData.zMin =  field[6].GetDouble();
        m_regions[regionID].RegionData.xMax =  field[7].GetDouble();
        m_regions[regionID].RegionData.yMax =  field[8].GetDouble();
        m_regions[regionID].RegionData.zMax =  field[9].GetDouble();
        m_regions[regionID].RegionData.radius =  field[10].GetDouble();
        m_regions[regionID].RegionData.factionID =  field[11].GetUInt32();

    } while(result->NextRow());

    printf("geting max constellationID.\n");
    // get max constellationID
    delete result;
    result = mdb.Query("select max(constellationID) from mapConstellations");
    if(!result)
    {
        sLog.String("Query failed: select max(constellationID) from mapConstellations");
        return false;
    }

    field = result->Fetch();
    m_constellationsSize = field[0].GetUInt32() - CONSTELLATIONSUMEDVAL + 1;

    m_constellations = new Constellation[m_constellationsSize];

    for(int i = 0; i < m_constellationsSize ; ++i)
        memset(&m_constellations.ConstellationData,0,sizeof(m_constellations.ConstellationData));

    printf("geting constellation data.\n");
    // get constellation data
    delete result;
    result = mdb.Query("select constellationID,regionID,x,y,z,xMin,yMin,zMin,xMax,yMax,zMax,radius,factionID from mapConstellations");
    if(!result)
    {
        sLog.String("Query failed: select constellationID,regionID,x,y,z,xMin,yMin,zMin,xMax,yMax,zMax,radius,factionID from mapConstellations");
        return false;
    }

    do
    {
        field = result->Fetch();
        constellationID = field[0].GetUInt32() - CONSTELLATIONSUMEDVAL;
        regionID = field[1].GetUInt32() - REGIONSUMEDVAL;

        m_regions[regionID].constellations.push_back(constellationID);

        m_constellations[constellationID].ConstellationData.regionID = regionID;
        m_constellations[constellationID].ConstellationData.x =  field[2].GetDouble();
        m_constellations[constellationID].ConstellationData.y =  field[3].GetDouble();
        m_constellations[constellationID].ConstellationData.z =  field[4].GetDouble();
        m_constellations[constellationID].ConstellationData.xMin =  field[5].GetDouble();
        m_constellations[constellationID].ConstellationData.yMin =  field[6].GetDouble();
        m_constellations[constellationID].ConstellationData.zMin =  field[7].GetDouble();
        m_constellations[constellationID].ConstellationData.xMax =  field[8].GetDouble();
        m_constellations[constellationID].ConstellationData.yMax =  field[9].GetDouble();
        m_constellations[constellationID].ConstellationData.zMax =  field[10].GetDouble();
        m_constellations[constellationID].ConstellationData.radius =  field[11].GetDouble();
        m_constellations[constellationID].ConstellationData.factionID =  field[12].GetUInt32();

    } while(result->NextRow());

    printf("geting max solarsystemID.\n");
    // get max solarsystemID
    delete result;
    result = mdb.Query("select max(solarsystemID) from mapSolarSystems");
    if(!result)
    {
        sLog.String("Query failed: select max(solarsystemID) from mapSolarSystems");
        return false;
    }

    field = result->Fetch();
    uint32 m_solarsystemsSize = field[0].GetUInt32() - SOLARSYSTEMSUMEDVAL + 1;

    m_solarsystems = new SolarSystem[m_solarsystemsSize];

    for(int i = 0; i < m_solarsystemsSize ; ++i)
        memset(&m_solarsystems.SolarSystemData,0,sizeof(m_solarsystems.SolarSystemData));

    printf("geting solarsystem data.\n");
    // get solarsystem data
    delete result;
    result = mdb.Query("select solarsystemID,constellationID,x,y,z,xMin,yMin,zMin,xMax,yMax,zMax,radius,luminosity,border,fringe,corridor,hub,international,regional,constellation,security,factionID,sunTypeID,securityClass from mapSolarSystems");
    if(!result)
    {
        sLog.String("Query failed: select solarsystemID,constellationID,x,y,z,xMin,yMin,zMin,xMax,yMax,zMax,radius,luminosity,border,fringe,corridor,hub,international,regional,constellation,security,factionID,sunTypeID,securityClass from mapSolarSystems");
        return false;
    }

    do
    {
        field = result->Fetch();

        solarsystemID = field[0].GetUInt32() - SOLARSYSTEMSUMEDVAL;
        constellationID = field[1].GetUInt32() - CONSTELLATIONSUMEDVAL;

        m_constellations[constellationID].solarsystems.push_back(solarsystemID);

        m_solarsystems[solarsystemID].SolarSystemData.constellationID = constellationID;
        m_solarsystems[solarsystemID].SolarSystemData.x =  field[2].GetDouble();
        m_solarsystems[solarsystemID].SolarSystemData.y =  field[3].GetDouble();
        m_solarsystems[solarsystemID].SolarSystemData.z =  field[4].GetDouble();
        m_solarsystems[solarsystemID].SolarSystemData.xMin =  field[5].GetDouble();
        m_solarsystems[solarsystemID].SolarSystemData.yMin =  field[6].GetDouble();
        m_solarsystems[solarsystemID].SolarSystemData.zMin =  field[7].GetDouble();
        m_solarsystems[solarsystemID].SolarSystemData.xMax =  field[8].GetDouble();
        m_solarsystems[solarsystemID].SolarSystemData.yMax =  field[9].GetDouble();
        m_solarsystems[solarsystemID].SolarSystemData.zMax =  field[10].GetDouble();
        m_solarsystems[solarsystemID].SolarSystemData.radius =  field[11].GetDouble();
        m_solarsystems[solarsystemID].SolarSystemData.luminosity =  field[12].GetDouble();

        m_solarsystems[solarsystemID].SolarSystemData.border =  field[13].GetUInt32();
        m_solarsystems[solarsystemID].SolarSystemData.fringe =  field[14].GetUInt32();
        m_solarsystems[solarsystemID].SolarSystemData.corridor =  field[15].GetUInt32();
        m_solarsystems[solarsystemID].SolarSystemData.hub =  field[16].GetUInt32();
        m_solarsystems[solarsystemID].SolarSystemData.international =  field[17].GetUInt32();
        m_solarsystems[solarsystemID].SolarSystemData.regional =  field[18].GetUInt32();
        m_solarsystems[solarsystemID].SolarSystemData.constellation =  field[19].GetUInt32();

        m_solarsystems[solarsystemID].SolarSystemData.security =  field[20].GetDouble();

        m_solarsystems[solarsystemID].SolarSystemData.factionID =  field[21].GetUInt32();
        m_solarsystems[solarsystemID].SolarSystemData.sunTypeID =  field[22].GetUInt32();

        //m_solarsystems[solarsystemID].SolarSystemData.securityClass =  field[23].GetString();

    } while(result->NextRow());

    delete result;

    // TODO: for each solarsystem whe got to cache our map objects :x
    // whe may not need to do this every reboot, if octree caching is implemented, just at first server run.
*/
    return true;
}

void Space::Delete_SpaceForm()
{
    delete[] m_regions;
    delete[] m_constellations;

    m_regions = NULL;
    m_constellations = NULL;
}

EveClientSession* Space::FindSession(uint32 id)
{
    m_sessionlock.AcquireReadLock();
    EveClientSession * ret = 0;
    SessionMapConstItr itr = m_sessions.find(id);

    if(itr != m_sessions.end())
        ret = itr->second;

    m_sessionlock.ReleaseReadLock();
    return ret;
}

void Space::RemoveSession(uint32 id)
{
    m_sessionlock.AcquireWriteLock();

    SessionMapItr itr = m_sessions.find(id);

    if(itr != m_sessions.end())
    {
        delete itr->second;
        m_sessions.erase(itr);
    }

    m_sessionlock.ReleaseWriteLock();
}

void Space::AddSession(EveClientSession* session)
{
    if(session == NULL)
        return;

    m_sessionlock.AcquireWriteLock();

    ASSERT(session);
    //m_sessions[session->GetUserId()] = session;

    //if(m_sessions.size() >  PeakSessionCount)
    //  PeakSessionCount = (uint32)m_sessions.size();

    m_sessionlock.ReleaseWriteLock();
}

void Space::AddGlobalSession(EveClientSession *session)
{
    if(session == NULL)
        return;

    SessionsMutex.Acquire();
    Sessions.insert(session);
    SessionsMutex.Release();
}

void Space::RemoveGlobalSession(EveClientSession *session)
{
    SessionsMutex.Acquire();
    Sessions.erase(session);
    SessionsMutex.Release();
}

void Space::DeleteSession(EveClientSession *session)
{
    m_sessionlock.AcquireWriteLock();
    // remove from big map
    //m_sessions.erase(session->GetUserId());

    m_sessionlock.ReleaseWriteLock();

    // delete us
    session->Delete();
}

void Space::Update(time_t diff)
{

}

void Space::UpdateSessions(time_t diff)
{
    if (Sessions.empty() == true)
        return;

    SessionSet::iterator itr = Sessions.begin();
    SessionSet::iterator it2;
    EveClientSession *session;
    int result;
    for(; itr != Sessions.end();)
    {
        session = (*itr);
        it2 = itr;
        ++itr;

        if((result = session->Update()))
        {
            if(result == 1)
            {
                // complete deletion
                DeleteSession(session);
            }
            Sessions.erase(it2);

            /*  this is part of the hack described above.......
                Note: reason: there is no use in updating a client that has been deleted
            */
            continue;
        }

        /* THIS IS A HACK.... player updates should be done in a quad tree style of grid.... resulting in not updating players
           and sending packets to players that we can't even see..... what so ever...
        */
        //session->_client->Update();
    }
}

/************************************************************************/
/* Server statistics                                                    */
/************************************************************************/
void Space::OnClientDisconnect()
{
    if (mAcceptedConnections > 0)
    {
        mAcceptedConnections--;
        return;
    }

    ASCENT_ASSERT(mAcceptedConnections > 0);
}

void Space::OnClientConnect()
{
    mAcceptedConnections++;
}
