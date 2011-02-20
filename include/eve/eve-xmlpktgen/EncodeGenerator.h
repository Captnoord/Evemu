/**
 * @file EncodeGenerator.h
 *
 * This file is part of EVEmu: EVE Online Server Emulator.<br>
 * Copyright (C) 2006-2011 The EVEmu Team<br>
 * For the latest information visit <i>http://evemu.org</i>.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA, or go to
 * <i>http://www.gnu.org/copyleft/lesser.txt</i>.
 *
 * @author Zhur
 */

#ifndef __ENCODE_GENERATOR_H__INCL__
#define __ENCODE_GENERATOR_H__INCL__

#include "Generator.h"

class ClassEncodeGenerator
: public Generator
{
public:
    ClassEncodeGenerator( FILE* outputFile = NULL );

protected:
    const char* top() const { return mVariableStack.top().c_str(); }
    void pop() { mVariableStack.pop(); }
    void push(const char *v) { mVariableStack.push( v ); }
    void clear() { while( !mVariableStack.empty() ) pop(); }

    bool ProcessElementDef( const TiXmlElement* field );
    bool ProcessElement( const TiXmlElement* field );
    bool ProcessElementPtr( const TiXmlElement* field );

    bool ProcessRaw( const TiXmlElement* field );
    bool ProcessInt( const TiXmlElement* field );
    bool ProcessLong( const TiXmlElement* field );
    bool ProcessReal( const TiXmlElement* field );
    bool ProcessBool( const TiXmlElement* field );
    bool ProcessNone( const TiXmlElement* field );
    bool ProcessBuffer( const TiXmlElement* field );

    bool ProcessString( const TiXmlElement* field );
    bool ProcessStringInline( const TiXmlElement* field );
    bool ProcessWString( const TiXmlElement* field );
    bool ProcessWStringInline( const TiXmlElement* field );
    bool ProcessToken( const TiXmlElement* field );
    bool ProcessTokenInline( const TiXmlElement* field );

    bool ProcessObject( const TiXmlElement* field );
    bool ProcessObjectInline( const TiXmlElement* field );
    bool ProcessObjectEx( const TiXmlElement* field );

    bool ProcessTuple( const TiXmlElement* field );
    bool ProcessTupleInline( const TiXmlElement* field );
    bool ProcessList( const TiXmlElement* field );
    bool ProcessListInline( const TiXmlElement* field );
    bool ProcessListInt( const TiXmlElement* field );
    bool ProcessListLong( const TiXmlElement* field );
    bool ProcessListStr( const TiXmlElement* field );
    bool ProcessDict( const TiXmlElement* field );
    bool ProcessDictInline( const TiXmlElement* field );
    bool ProcessDictRaw( const TiXmlElement* field );
    bool ProcessDictInt( const TiXmlElement* field );
    bool ProcessDictStr( const TiXmlElement* field );

    bool ProcessSubStreamInline( const TiXmlElement* field );
    bool ProcessSubStructInline( const TiXmlElement* field );

private:
    uint32 mItemNumber;
    std::stack<std::string> mVariableStack;
    const char* mName;
};

#endif /* !__ENCODE_GENERATOR_H__INCL__ */
