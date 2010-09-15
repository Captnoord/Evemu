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

#include "CommonPCH.h"

#include "time/Const.h"

/*
 * Even though these values might seem oversimplified, do not
 * alter them! WinTime-related calculations depend on these
 * specific values.
 */
const size_t Time::NSEC_PER_USEC = 1000;
const size_t Time::USEC_PER_MSEC = 1000;
const size_t Time::MSEC_PER_SEC = 1000;
const size_t Time::SEC_PER_MIN = 60;
const size_t Time::MIN_PER_HOUR = 60;
const size_t Time::HOUR_PER_DAY = 24;
const size_t Time::DAY_PER_MONTH = 30;
const size_t Time::MONTH_PER_YEAR = 12;

const size_t Time::NSEC_PER_WINTIME = 100;
const size_t Time::WINTIME_PER_USEC = 10;

const size_t Time::EPOCH_DIFF_SEC = Time::SEC_PER_MIN * Time::MIN_PER_HOUR * Time::HOUR_PER_DAY * 134774;
