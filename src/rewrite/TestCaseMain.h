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