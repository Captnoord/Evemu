/*  EVEmu: EVE Online Server Emulator

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY except by those people which sell it, which
  are required to give you total support for your newly bought product;
  without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
	
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#include "TutorialService.h"
#include "../common/logsys.h"
#include "../common/PyRep.h"
#include "../common/PyPacket.h"
#include "../Client.h"
#include "../PyServiceCD.h"
#include "../PyServiceMgr.h"

#include "../packets/Tutorial.h"
#include "../packets/General.h"

PyCallable_Make_InnerDispatcher(TutorialService)

TutorialService::TutorialService(PyServiceMgr *mgr, DBcore *dbc)
: PyService(mgr, "tutorialSvc"),
  m_dispatch(new Dispatcher(this)),
  m_db(dbc)
{
	_SetCallDispatcher(m_dispatch);

	//PyCallable_REG_CALL(TutorialService, GetTutorialInfo)
	PyCallable_REG_CALL(TutorialService, GetTutorials)
	PyCallable_REG_CALL(TutorialService, GetCriterias)
	PyCallable_REG_CALL(TutorialService, GetCategories)
	PyCallable_REG_CALL(TutorialService, GetContextHelp)
	PyCallable_REG_CALL(TutorialService, GetCharacterTutorialState)
}

TutorialService::~TutorialService() {
	delete m_dispatch;
}

/*
PyCallResult TutorialService::Handle_GetTutorialInfo(PyCallArgs &call) {
	Call_GetTutorialInfo args;
	if(!args.Decode(&call.tuple)) {
		codelog(CLIENT__ERROR, "Can't parse args.");
		return(NULL);
	}

	Rsp_GetTutorialInfo rsp;

	rsp.pagecriterias = m_db.GetPageCriterias(args.tutorialID);
	if(rsp.pagecriterias == NULL) {
		codelog(SERVICE__ERROR, "An error occured while getting pagecriterias for tutorial %lu.", args.tutorialID);
		return(NULL);
	}

	rsp.pages = m_db.GetPages(args.tutorialID);
	if(rsp.pages == NULL) {
		codelog(SERVICE__ERROR, "An error occured while getting pages for tutorial %lu.", args.tutorialID);
		return(NULL);
	}

	rsp.tutorial = m_db.GetTutorial(args.tutorialID);
	if(rsp.tutorial == NULL) {
		codelog(SERVICE__ERROR, "An error occured while getting tutorial %lu.", args.tutorialID);
		return(NULL);
	}

	rsp.criterias = m_db.GetTutorialCriterias(args.tutorialID);
	if(rsp.criterias == NULL) {
		codelog(SERVICE__ERROR, "An error occured while getting criterias for tutorial %lu.", args.tutorialID);
		return(NULL);
	}

	return(rsp.Encode());
}
*/

PyCallResult TutorialService::Handle_GetTutorials(PyCallArgs &call) {
	//return(m_db.GetAllTutorials());

	PyRepDict *d = new PyRepDict;

	PyRepTuple *args = new PyRepTuple(1);
	PyRepTuple *col_list = new PyRepTuple(5);
	args->items[0] = col_list;

	PyRepTuple *col;
	col = new PyRepTuple(2);
	col->items[0] = new PyRepString("tutorialID");
	col->items[1] = new PyRepInteger(DBTYPE_I4);
	col_list->items[0] = col;

	col = new PyRepTuple(2);
	col->items[0] = new PyRepString("tutorialName");
	col->items[1] = new PyRepInteger(DBTYPE_STR);
	col_list->items[1] = col;

	col = new PyRepTuple(2);
	col->items[0] = new PyRepString("nextTutorialID");
	col->items[1] = new PyRepInteger(DBTYPE_I4);
	col_list->items[2] = col;

	col = new PyRepTuple(2);
	col->items[0] = new PyRepString("categoryID");
	col->items[1] = new PyRepInteger(DBTYPE_I4);
	col_list->items[3] = col;

	col = new PyRepTuple(2);
	col->items[0] = new PyRepString("dataID");
	col->items[1] = new PyRepInteger(DBTYPE_I4);
	col_list->items[4] = col;

	d->add("header", new PyRepPackedObject1("blue.DBRowDescriptor", args));

	return(new PyRepPackedObject2("dbutil.CRowset", NULL, d));
}

PyCallResult TutorialService::Handle_GetCriterias(PyCallArgs &call) {
	return(m_db.GetAllCriterias());
}

PyCallResult TutorialService::Handle_GetCategories(PyCallArgs &call) {
	_log(SERVICE__WARNING, "%s: GetCategories unimplemented.", GetName());

	util_Rowset res;

	res.header.push_back("categoryID");
	res.header.push_back("categoryName");
	res.header.push_back("description");
	res.header.push_back("dataID");	//TODO: what is this?

	//TODO: save this crap into DB
	PyRepList *line = new PyRepList;
	line->add(new PyRepInteger(1));
	line->add(new PyRepString("Beginner Tutorials"));
	line->add(new PyRepString("Here are the basic tutorials for your first days in EVE!"));
	line->add(new PyRepInteger(2386927));
	res.lines.add(line);

	line = new PyRepList;
	line->add(new PyRepInteger(2));
	line->add(new PyRepString("Intermediate Tutorials"));
	line->add(new PyRepString("Under this category fall all advanced tutorials"));
	line->add(new PyRepInteger(2387321));
	res.lines.add(line);

	line = new PyRepList;
	line->add(new PyRepInteger(3));
	line->add(new PyRepString("Advanced Tutorials"));
	line->add(new PyRepString(""));
	line->add(new PyRepInteger(2388093));
	res.lines.add(line);

	line = new PyRepList;
	line->add(new PyRepInteger(4));
	line->add(new PyRepString("Informative Tutorials"));
	line->add(new PyRepString(""));
	line->add(new PyRepInteger(2388094));
	res.lines.add(line);

	return(res.Encode());
}

PyCallResult TutorialService::Handle_GetContextHelp(PyCallArgs &call) {
	//unimplemented
	return(new PyRepList());
}

PyCallResult TutorialService::Handle_GetCharacterTutorialState(PyCallArgs &call) {
	util_Rowset rs;

	rs.header.push_back("characterID");
	rs.header.push_back("tutorialID");
	rs.header.push_back("pageID");
	rs.header.push_back("eventTypeID");

	return(rs.Encode());
}
























