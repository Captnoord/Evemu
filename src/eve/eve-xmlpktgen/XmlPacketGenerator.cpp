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
    Author:     Zhur
*/

#include "EVEXmlPktGenPCH.h"

#include "XmlPacketGenerator.h"

/************************************************************************/
/* XmlPacketGenerator                                                   */
/************************************************************************/
const char* const XmlPacketGenerator::FILE_COMMENT =
"/*  EVEmu: EVE Online Server Emulator\n"
"  \n"
"  **************************************************************\n"
"  This file is automatically generated, DO NOT EDIT IT DIRECTLY.\n"
"  **************************************************************\n"
"  \n"
"  (If you need to customize an object, you must copy that object\n"
"  into another source file, and give up the ability to generate it)\n"
"  \n"
"  \n"
"  This program is free software; you can redistribute it and/or modify\n"
"  it under the terms of the GNU General Public License as published by\n"
"  the Free Software Foundation; version 2 of the License.\n"
"  \n"
"  This program is distributed in the hope that it will be useful,\n"
"  but WITHOUT ANY WARRANTY except by those people which sell it, which\n"
"  are required to give you total support for your newly bought product;\n"
"  without even the implied warranty of MERCHANTABILITY or FITNESS FOR\n"
"  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n"
"  \n"
"  You should have received a copy of the GNU General Public License\n"
"  along with this program; if not, write to the Free Software\n"
"  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA\n"
"*/";

std::string XmlPacketGenerator::FNameToDef( const char* buf )
{
    std::string res;
    res += "__";

    for(; '\0' != *buf; ++buf )
    {
        if( !Util::String< char >::isPrintable( *buf )
            || *buf == '/'
            || *buf == '\\'
            || *buf == ':'
            || *buf == '.'
            || *buf == '-' )
        {
            res += '_';
        }
        else
        {
            res += ::toupper( *buf );
        }
    }

    res += "__";
    return res;
}

XmlPacketGenerator::XmlPacketGenerator( const char* header, const char* source )
: mHeaderFile( NULL ),
  mHeaderFileName( header ),
  mSourceFile( NULL ),
  mSourceFileName( source )
{
    AddMemberParser( "elements",   &XmlPacketGenerator::ParseElements );
    AddMemberParser( "include",    &XmlPacketGenerator::ParseInclude );
    AddMemberParser( "elementDef", &XmlPacketGenerator::ParseElementDef );
}

XmlPacketGenerator::~XmlPacketGenerator()
{
    // Close the files
    SetHeaderFile( "" );
    SetSourceFile( "" );
}

bool XmlPacketGenerator::ParseElements( const TiXmlElement* field )
{
    if( !OpenFiles() )
    {
        sLog.Error( "XmlPacketGenerator", "Unable to open output files: %s.", strerror( errno ) );
        return false;
    }

    const std::string def = FNameToDef( mHeaderFileName.c_str() );

    //headers:
    fprintf( mHeaderFile,
        "%s\n"
        "\n"
        "#ifndef %s\n"
        "#define %s\n"
        "\n"
        "#include \"python/PyVisitor.h\"\n"
        "#include \"python/PyRep.h\"\n"
        "\n",
        FILE_COMMENT,
        def.c_str(),
        def.c_str()
    );
    fprintf( mSourceFile,
        "%s\n"
        "\n"
        "#include \"EVECommonPCH.h\"\n"
        "\n"
        "#include \"%s\"\n"
        "\n",
        FILE_COMMENT,
        mHeaderFileName.c_str()
    );

    //content
    bool res = ParseElementChildren( field );

    //footers:
    fprintf( mHeaderFile,
        "#endif /* !%s */\n"
        "\n",
        def.c_str()
    );

    return res;
}

bool XmlPacketGenerator::ParseInclude( const TiXmlElement* field )
{
    const char* file = field->Attribute( "file" );
    if( file == NULL )
    {
        sLog.Error( "XmlPacketGenerator", "field at line %d is missing the file attribute, skipping.", field->Row() );
        return false;
    }

    fprintf( mHeaderFile,
        "#include \"%s\"\n"
        "\n",
        file
    );
    return true;
}

bool XmlPacketGenerator::ParseElementDef( const TiXmlElement* field )
{
    bool res = ( mClone.ParseElement( field )
                 && mConstruct.ParseElement( field )
                 && mDecode.ParseElement( field )
                 && mDestruct.ParseElement( field )
                 && mDump.ParseElement( field )
                 && mEncode.ParseElement( field )
                 && mHeader.ParseElement( field ) );

    return res;
}

void XmlPacketGenerator::SetHeaderFile( const char* header )
{
    if( mHeaderFileName != header )
    {
        if( NULL != mHeaderFile )
        {
            fclose( mHeaderFile );
            mHeaderFile = NULL;

            // propagate the change to the generators
            mHeader.SetOutputFile( NULL );
        }

        mHeaderFileName = header;
    }
}

void XmlPacketGenerator::SetSourceFile( const char* source )
{
    if( mSourceFileName != source )
    {
        if( NULL != mSourceFile )
        {
            fclose( mSourceFile );
            mSourceFile = NULL;

            // propagate the change to the generators
            mClone.SetOutputFile( NULL );
            mConstruct.SetOutputFile( NULL );
            mDecode.SetOutputFile( NULL );
            mDestruct.SetOutputFile( NULL );
            mDump.SetOutputFile( NULL );
            mEncode.SetOutputFile( NULL );
        }

        mSourceFileName = source;
    }
}

bool XmlPacketGenerator::OpenFiles()
{
    bool res = true;

    if( !mHeaderFileName.empty() && ( NULL == mHeaderFile ) )
    {
        mHeaderFile = fopen( mHeaderFileName.c_str(), "w" );

        if( NULL == mHeaderFile )
            res = false;
        else
        {
            // propagate the change to the generators
            mHeader.SetOutputFile( mHeaderFile );
        }
    }

    if( !mSourceFileName.empty() && ( NULL == mSourceFile ) )
    {
        mSourceFile = fopen( mSourceFileName.c_str(), "w" );

        if( NULL == mSourceFile )
            res = false;
        else
        {
            // propagate the change to the generators
            mClone.SetOutputFile( mSourceFile );
            mConstruct.SetOutputFile( mSourceFile );
            mDecode.SetOutputFile( mSourceFile );
            mDestruct.SetOutputFile( mSourceFile );
            mDump.SetOutputFile( mSourceFile );
            mEncode.SetOutputFile( mSourceFile );
        }
    }

    return res;
}
