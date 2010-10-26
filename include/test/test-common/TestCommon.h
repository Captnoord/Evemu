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

#ifndef __TEST_COMMON_H__INCL__
#define __TEST_COMMON_H__INCL__

/*
 * Static includes specific to test-common.
 */

/*************************************************************************/
/* dep includes                                                          */
/*************************************************************************/
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/XmlOutputter.h>

/*************************************************************************/
/* common includes                                                       */
/*************************************************************************/
#include "net/Ip4.h"
#include "net/StreamSocket.h"

#include "std/File.h"

#include "util/Buffer.h"
#include "util/Data.h"

#endif /* !__TEST_COMMON_H__INCL__ */
