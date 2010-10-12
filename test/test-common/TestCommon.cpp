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
    Author:     Bloody.Rabbit
*/

#include "TestCommonPCH.h"

#include "DataTest.h"
#include "StreamSocketTest.h"

/**
 * @brief Runs all tests.
 *
 * @param[in] xmlFile Where to (optionally) output results.
 *
 * @retval true  Testing successfull.
 * @retval false Testing failed.
 */
bool RunTests( CppUnit::OStream* xmlFile = NULL )
{
    CppUnit::TextTestRunner runner;
    if( NULL != xmlFile )
        runner.setOutputter( new CppUnit::XmlOutputter( &runner.result(), *xmlFile ) );

    runner.addTest( DataTest::suite() );

    runner.addTest( new StreamSocketTest< Net::Ip4 > );

    return runner.run();
}

int main( int argc, char* argv[] )
{
    CppUnit::OStream* xmlFile = ( 1 < argc
                                  ? new CppUnit::OFileStream( argv[1] )
                                  : NULL );

    bool success = RunTests( xmlFile );

    SafeDelete( xmlFile );
    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
