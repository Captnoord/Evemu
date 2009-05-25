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

#include "EvemuPCH.h"
#include "FileModule.h"
// TODO: not in repo
//#include "PyObjectDumper.h"

initialiseSingleton( ConfigMgr );

ConfigMgr::ConfigMgr()
{
    init();
}

ConfigMgr::~ConfigMgr() {}

bool ConfigMgr::init()
{
    loadFromBulk();

    initServiceInfo();
    initBulkData();

    return true;
}

void ConfigMgr::initServiceInfo()
{
    PyDict * serviceInfo = new PyDict();
    /* its a fact that this list should be generated dynamically */
    serviceInfo->set_item("stationSvc",     &PyNone);
    serviceInfo->set_item("objectCaching",  &PyNone);
    serviceInfo->set_item("invbroker",      &PyNone);
    serviceInfo->set_item("map",            &PyNone);
    serviceInfo->set_item("beyonce",        &PyNone);
    serviceInfo->set_item("standing2",      &PyNone);
    serviceInfo->set_item("ram",            &PyNone);
    serviceInfo->set_item("DB",             &PyNone);
    serviceInfo->set_item("posMgr",         &PyNone);
    serviceInfo->set_item("voucher",        &PyNone);
    serviceInfo->set_item("entity",         &PyNone);
    serviceInfo->set_item("damageTracker",  &PyNone);
    serviceInfo->set_item("agentMgr",       &PyNone);
    serviceInfo->set_item("dogmaIM",        &PyNone);
    serviceInfo->set_item("machoNet",       &PyNone);
    serviceInfo->set_item("dungeonExplorationMgr", &PyNone);
    serviceInfo->set_item("watchdog",       &PyNone);
    serviceInfo->set_item("ship",           &PyNone);
    serviceInfo->set_item("DB2",            &PyNone);
    serviceInfo->set_item("market",         &PyNone);
    serviceInfo->set_item("surveilance",    &PyNone);
    serviceInfo->set_item("dungeon",        &PyNone);
    serviceInfo->set_item("aggressionMgr",  &PyNone);
    serviceInfo->set_item("sessionMgr",     &PyNone);
    serviceInfo->set_str ("LSC",            "location");
    serviceInfo->set_item("allianceRegistry",&PyNone);
    serviceInfo->set_item("npcSvc",         &PyNone);
    serviceInfo->set_item("cache",          &PyNone);
    serviceInfo->set_item("character",      &PyNone);
    serviceInfo->set_item("factory",        &PyNone);
    serviceInfo->set_item("facWarMgr",      &PyNone);
    serviceInfo->set_item("corpStationMgr", &PyNone);
    serviceInfo->set_item("authentication", &PyNone);
    serviceInfo->set_str ("station",        "station");
    serviceInfo->set_str ("slash",          "location");
    serviceInfo->set_item("charmgr",        &PyNone);
    serviceInfo->set_item("BSD",            &PyNone);
    serviceInfo->set_item("languageSvc",    &PyNone);
    serviceInfo->set_str ("config",         "locationPreferred");
    serviceInfo->set_item("billingMgr",     &PyNone);
    serviceInfo->set_item("billMgr",        &PyNone);
    serviceInfo->set_item("lookupSvc",      &PyNone);
    serviceInfo->set_item("emailreader",    &PyNone);
    serviceInfo->set_item("lootSvc",        &PyNone);
    serviceInfo->set_item("http",           &PyNone);
    serviceInfo->set_item("gagger",         &PyNone);
    serviceInfo->set_item("dataconfig",     &PyNone);
    serviceInfo->set_item("lien",           &PyNone);
    serviceInfo->set_item("i2",             &PyNone);
    serviceInfo->set_str ("wormholeMgr",    "location");
    serviceInfo->set_item("alert",          &PyNone);
    serviceInfo->set_item("director",       &PyNone);
    serviceInfo->set_item("dogma",          &PyNone);
    serviceInfo->set_item("pathfinder",     &PyNone);
    serviceInfo->set_item("corporationSvc", &PyNone);
    serviceInfo->set_item("clones",         &PyNone);
    serviceInfo->set_item("jumpCloneSvc",   &PyNone);
    serviceInfo->set_item("effectCompiler", &PyNone);
    serviceInfo->set_item("corpmgr",        &PyNone);
    serviceInfo->set_item("warRegistry",    &PyNone);
    serviceInfo->set_item("corpRegistry",   &PyNone);
    serviceInfo->set_str ("account",        "location");
    serviceInfo->set_str ("gangSvc",        "location");
    serviceInfo->set_item("userSvc",        &PyNone);
    serviceInfo->set_item("counter",        &PyNone);
    serviceInfo->set_item("petitioner",     &PyNone);
    serviceInfo->set_str ("keeper",         "solarsystem");
    serviceInfo->set_str ("bookmark",       "location");
    serviceInfo->set_item("jumpbeaconsvc",  &PyNone);
    serviceInfo->set_str ("scanMgr",        "solarsystem");
    serviceInfo->set_item("debug",          &PyNone);
    serviceInfo->set_item("skillMgr",       &PyNone);
    serviceInfo->set_item("onlineStatus",   &PyNone);
    serviceInfo->set_item("LPSvc",          &PyNone);

    /*
    serviceInfo->set_str ("trademgr",       "station");
    serviceInfo->set_item("stationSvc",     &PyNone);
    serviceInfo->set_item("zsystem",        &PyNone);
    serviceInfo->set_item("invbroker",      &PyNone);
    serviceInfo->set_item("droneMgr",       &PyNone);
    serviceInfo->set_item("userSvc",        &PyNone);
    serviceInfo->set_item("map",            &PyNone);
    serviceInfo->set_item("beyonce",        &PyNone);
    serviceInfo->set_item("standing2",      &PyNone);
    serviceInfo->set_item("ram",            &PyNone);
    serviceInfo->set_item("DB",             &PyNone);
    serviceInfo->set_item("posMgr",         &PyNone);
    serviceInfo->set_item("voucher",        &PyNone);
    serviceInfo->set_item("entity",         &PyNone);
    serviceInfo->set_item("damageTracker",  &PyNone);
    serviceInfo->set_item("agentMgr",       &PyNone);
    serviceInfo->set_item("dogmaIM",        &PyNone);
    serviceInfo->set_item("machoNet",       &PyNone);
    serviceInfo->set_str ("dungeonExplorationMgr", &PyNone);
    serviceInfo->set_item("watchdog",       &PyNone);
    serviceInfo->set_item("ship",           &PyNone);
    serviceInfo->set_item("DB2",            &PyNone);
    serviceInfo->set_item("market",         &PyNone);
    serviceInfo->set_item("dungeon",        &PyNone);
    serviceInfo->set_item("npcSvc",         &PyNone);
    serviceInfo->set_item("sessionMgr",     &PyNone);
    serviceInfo->set_str ("LSC",            "location");
    serviceInfo->set_item("allianceRegistry", &PyNone);
    serviceInfo->set_str ("tutorialSvc",    "location");
    serviceInfo->set_str ("bookmark",       "location");
    serviceInfo->set_item("cache",          &PyNone);
    serviceInfo->set_item("character",      &PyNone);
    serviceInfo->set_item("factory",        &PyNone);
    serviceInfo->set_item("facWarMgr",      &PyNone);
    serviceInfo->set_item("corpStationMgr", &PyNone);
    serviceInfo->set_item("authentication", &PyNone);
    serviceInfo->set_item("station",        "station");
    serviceInfo->set_item("effectCompiler", &PyNone);
    serviceInfo->set_str ("slash",          "location");
    serviceInfo->set_item("charmgr",        &PyNone);
    serviceInfo->set_item("BSD",            &PyNone);
    serviceInfo->set_item("reprocessingSvc", &PyNone);
    serviceInfo->set_str ("config",         "locationPreferred");
    serviceInfo->set_item("billingMgr",     &PyNone);
    serviceInfo->set_item("billMgr",        &PyNone);
    serviceInfo->set_item("lookupSvc",      &PyNone);
    serviceInfo->set_item("emailreader",    &PyNone);
    serviceInfo->set_item("lootSvc",        &PyNone);
    serviceInfo->set_item("http",           &PyNone);
    serviceInfo->set_item("repairSvc",      &PyNone);
    serviceInfo->set_item("gagger",         &PyNone);
    serviceInfo->set_item("dataconfig",     &PyNone);
    serviceInfo->set_item("lien",           &PyNone);
    serviceInfo->set_item("i2",             &PyNone);
    serviceInfo->set_str ("wormholeMgr",    "location");
    serviceInfo->set_item("pathfinder",     &PyNone);
    serviceInfo->set_item("alert",          &PyNone);
    serviceInfo->set_item("director",       &PyNone);
    serviceInfo->set_item("dogma",          &PyNone);
    serviceInfo->set_item("aggressionMgr",  &PyNone);
    serviceInfo->set_item("corporationSvc", &PyNone);
    serviceInfo->set_item("certificateMgr", &PyNone);
    serviceInfo->set_item("clones",         &PyNone);
    serviceInfo->set_item("jumpCloneSvc",   &PyNone);
    serviceInfo->set_item("insuranceSvc",   &PyNone);
    serviceInfo->set_item("corpmgr",        &PyNone);
    serviceInfo->set_item("warRegistry",    &PyNone);
    serviceInfo->set_item("corpRegistry",   &PyNone);
    serviceInfo->set_str ("account",        "location");
    serviceInfo->set_str ("gangSvc",        "location");
    serviceInfo->set_item("objectCaching",  &PyNone);
    serviceInfo->set_item("counter",        &PyNone);
    serviceInfo->set_item("petitioner",     &PyNone);
    serviceInfo->set_str ("keeper",         "solarsystem");
    serviceInfo->set_item("LPSvc",          &PyNone);
    serviceInfo->set_item("clientStatsMgr", &PyNone);
    serviceInfo->set_item("jumpbeaconsvc",  &PyNone);
    serviceInfo->set_str ("scanMgr",        "solarsystem");
    serviceInfo->set_str ("contractMgr",    "location");
    serviceInfo->set_item("debug",          &PyNone);
    serviceInfo->set_item("languageSvc",    &PyNone);
    serviceInfo->set_item("skillMgr",       &PyNone);
    serviceInfo->set_item("voiceMgr",       &PyNone);
    serviceInfo->set_item("onlineStatus",   &PyNone);
    serviceInfo->set_item("gangSvcObjectHandler", &PyNone);
    */

    mMachoNetServiceInfo = (PyObject*)serviceInfo;
    sObjectCachingSvc.CacheObject((PyObject*)serviceInfo, "machoNet.serviceInfo");
}

void ConfigMgr::initBulkData()
{
    //QueryResult* result = StaticDatabase.QueryNA("SELECT * FROM invcategories");

    //SafeDelete(result);
}

void ConfigMgr::loadFromBulk()
{
    ReadStream readStream;

    std::string bulkdatapath = Config.GetStringDefault("BulkData","Path","bulkdata");
    std::string bulkdir = bulkdatapath + "/" + "*.cache";

    FileModule bulkdataDir(bulkdir.c_str());// FileModule or FileSystem

    const char* file_path = NULL;
    while ((file_path = bulkdataDir.NextFile()) != NULL)
    {
        // fix path so we can open the file
        std::string bulkdatafile = bulkdatapath + file_path;
        FILE * fp = fopen(bulkdatafile.c_str(), "rb");
        if (fp == NULL)
        {
            sLog.Error("ConfigMgr","Unable to open: %s", file_path);
            return;
        }

        /* for those who don't know, this gets me the file size */
        fseek(fp, 0, SEEK_END);
        size_t fsize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        /* my read stream needs to be the same size as the file if I want to do
           nice things with it.
        */
        if (!readStream.resize(fsize))
        {
            sLog.Error("ConfigMgr", "Unable to allocate enough memory: %s", file_path);
            fclose(fp);
            return;
        }

        readStream.reset();

        size_t readCount = fread(readStream.content(), fsize, 1, fp);
        if ((readCount * fsize) != fsize)
        {
            sLog.Error("ConfigMgr", "Reading error while reading: %s", file_path);
            fclose(fp);
            return;
        }

        fclose(fp);

        PyObject* bulk_obj = mMarshalStream.load(readStream);
        if (bulk_obj == NULL)
        {
            sLog.Error("ConfigMgr", "Parsing: %s resulted in a error", file_path);
            return;
        }

        //CachedObject * henk = (CachedObject *)bulk_obj;

        // TODO: dumper not in repo.
        //Dump(stdout, bulk_obj, 0);
        //int henk  = 3;

        // we remove it for the moment
        //bulk_obj->DecRef();

        /* its important that we check if this call may possible loose some objects */
        mMarshalStream.clear();

        printf("ConfigMgr::walking::file_path: %s\n", file_path);
    }
}
