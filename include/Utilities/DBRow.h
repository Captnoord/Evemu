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

#ifndef _DBROW_H
#define _DBROW_H

#if 0
#include "packet_types.h"

//#include "TestCaseMarshal.h"

// header of the dbrow I think
class DBRowDescriptor
{

};

// the row class...
class DBRow
{

};

// a rowset consists of..
/*
*/
class RowSet
{
	/*RowSet()
	{
		PyDictStream mainStream;
		mainStream["header"]; // a PyListStream of strings
		mainStream["RowClass"] << "util.Row"; // a PyStringStream
		mainStream["lines"]; // a PyStreamlist of PyStreamlists
	}*/
};

// container that contains a raw RowSet packet
class RowSetStream// : public PyStream
{
	RowSetStream()// : PyStream()
	{

	}
};

// factory class that builds RowSet packets from database query results
class RowSetFactory
{
public:
	static RowSetStream * BuildRowSetPacket(QueryResult * queryResult)
	{
		/*RowSetStream packet;
		packet._push1('~');
		packet._push4(0);*/
	}
};

/*typedef enum {
	DBTYPE_I1			= 0x10,		// int8								i
	DBTYPE_UI1			= 0x11,		// uint8							u
	DBTYPE_BOOL			= 0x0B,		// bool								b
	DBTYPE_I2			= 0x02,		// int16
	DBTYPE_UI2			= 0x12,		// uint16
	DBTYPE_I4			= 0x03,		// int32
	DBTYPE_UI4			= 0x13,		// uint32
	DBTYPE_R4			= 0x04,		// real 32 bits ( float )			f
	DBTYPE_I8			= 0x14,		// int64
	DBTYPE_R8			= 0x05,		// double							d
	DBTYPE_UI8			= 0x15,		// uint64							
	DBTYPE_CY			= 0x06,		// money -> mostly double			m
	DBTYPE_FILETIME		= 0x40,		// 64 bits t_time					t
	DBTYPE_BYTES		= 0x80,		// raw bytes... "blob"				x
	DBTYPE_STR			= 0x81,		// char * string...					s
	DBTYPE_WSTR			= 0x82		// unicode							v
} DBTYPE;*/

class CRowSetFactory
{
public:
	/** format character string spec:
	  * 'b' == boolean
	    'i' == integer
		'u' == unsigned integer
		'f' == double
		's' == string
		'n' == PyNone

		"isisfff"
	*/

	/** DBRowDescriptor
	The blue.DBRowDescriptor is a Object and as far as I can see its a new object.
	A new object means it isn't a divired Object.
	*/

	class PyDBRowDescriptor// : public PyStream
	{
		PyDBRowDescriptor()// : PyStream()
		{
			//_pushOpcode(Op_PySubStream);
			
			//_push1<uint8>(SubStreamHeaderByte); // begin of stream
			//_push4<uint32>(0);					// we don't support shared objects for the moment
		}
	};

	/**
	 * @brief small function to convert mysql field types to the eve one's
	 *
	 * 
	 *
	 * @param[in] fieldType the mysql field type.
	 * @return the marshal db field type
	 * @todo add DBTYPE_STR
	 */
	static int translateFieldType(int fieldType, uint32 flags)
	{
		DBTYPE retValue = DBTYPE_ERROR;
		// this means the col is a number
		if ((flags & NUM_FLAG) != 0)
		{
			bool unsignedNumber = (flags & UNSIGNED_FLAG) != 0;
			
			switch (fieldType)
			{
			case MYSQL_TYPE_TINY:
				retValue = unsignedNumber ? DBTYPE_UI1 : DBTYPE_I1;
				break;
			case MYSQL_TYPE_SHORT:
				retValue = unsignedNumber ? DBTYPE_UI2 : DBTYPE_I2;
				break;
			case MYSQL_TYPE_LONG:
				retValue = unsignedNumber ? DBTYPE_UI4 : DBTYPE_I4;
				break;
			case MYSQL_TYPE_LONGLONG:
				retValue = unsignedNumber ? DBTYPE_UI8 : DBTYPE_I8;
				break;
			case MYSQL_TYPE_INT24:
				retValue = unsignedNumber ? DBTYPE_UI4 : DBTYPE_I4;
				break;
			default:
				Log.Error("CRowSet header stuff"," unsupported field type: %u ", fieldType);
				assert(false);
				break;
			}
		}
		else
		{
			switch (fieldType)
			{
			case MYSQL_TYPE_FLOAT:
				retValue = DBTYPE_R4;
				break;
			case MYSQL_TYPE_DOUBLE:
				retValue = DBTYPE_R8;
				break;
			case MYSQL_TYPE_DATETIME:
				retValue = DBTYPE_FILETIME;
				break;
			case MYSQL_TYPE_STRING:
			case MYSQL_TYPE_VARCHAR:
			case MYSQL_TYPE_VAR_STRING:
				retValue = DBTYPE_WSTR;
				break;
			default:
				Log.Error("CRowSet header stuff"," unsupported field type: %u ", fieldType);
				assert(false);
				break;
			}
		}
		return retValue;
	}	

	/**
	 * @brief Create a CRowSet object from a query result.
	 *
	 * 
	 *
	 * @param[in] queryResult the query result from the database query.
	 * @param[in] formatString the expected field types.
	 * @return a PyStreamObject that contains the result of the database query in Marshal format.
	 */
	static PyStream * BuildCRowSetPacket(QueryResult * queryResult)
	{
		if (queryResult == NULL)
		{
			// add notification.
			return NULL;
		}

		MySQLQueryResult * workQuery = static_cast<MySQLQueryResult*>(queryResult);

		/* hacky solution, resulting in copying the entire crap */
		//PyStream header;
		//PyListStream list;
		//PyDictStream dict;

		// make sure we have data....
		if (workQuery->GetFieldCount() == 0 || workQuery->GetRowCount() == 0)
		{
			//add notification.
			SafeDelete(workQuery);
			return NULL;
		}

		FieldExtendedInfo * fieldNames = workQuery->FetchExtendedInfo();

		//"blue.DBRowDescriptor"

		// hacking a bit, and using crap scopes to make it a bit easier for me ..... HEADACHE....
		/*PyStream rowDescriptor(workQuery->GetFieldCount() * 64); // a rough estimate
		rowDescriptor._pushOpcode(Op_PyTwoTuple);
		{
			rowDescriptor << "blue.DBRowDescriptor";
			rowDescriptor._pushOpcode(Op_PyOneTuple);
				rowDescriptor._pushOpcode(Op_PyTuple);
				rowDescriptor._pushExSize(workQuery->GetFieldCount());
					for (uint32 i = 0; i < workQuery->GetFieldCount(); i++)
					{
						rowDescriptor._pushOpcode(Op_PyTwoTuple);
							rowDescriptor << fieldNames[i].GetName();
							rowDescriptor << translateFieldType(fieldNames[i].GetType(), fieldNames[i].GetFlags());
					}
		}

		testmarshal(&rowDescriptor);*/

		//ByteBuffer dataCheck;

		//dataCheck.append(rowDescriptor.content(), rowDescriptor.size());
		//dataCheck.LogBuffer();

		
		//do
		//{
		//	Field * rowResult = workQuery->Fetch();

		//} while (workQuery->NextRow());

		SafeDelete(workQuery);
		return NULL;
	}
	
};

#endif


#endif//_DBROW_H

