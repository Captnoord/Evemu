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

#ifndef __TIME__TIME_CONST_H__INCL__
#define __TIME__TIME_CONST_H__INCL__

/// Number of nanoseconds in a microsecond.
extern const size_t NSEC_PER_USEC;
/// Number of microseconds in a millisecond.
extern const size_t USEC_PER_MSEC;
/// Number of milliseconds in a second.
extern const size_t MSEC_PER_SEC;
/// Number of seconds in a minute.
extern const size_t SEC_PER_MIN;
/// Number of minutes in an hour.
extern const size_t MIN_PER_HOUR;
/// Number of hours in a day.
extern const size_t HOUR_PER_DAY;
/// Number of days in a month.
extern const size_t DAY_PER_MONTH;
/// Number of months in a year.
extern const size_t MONTH_PER_YEAR;

/// Number of Win32Time-units per microsecond.
extern const size_t WIN32TIME_PER_USEC;
/// Number of seconds between Unix Epoch (January 1, 1970) and Windows Epoch (January 1, 1601).
extern const size_t WIN32TIME_SEC_EPOCH_DIFF;

#endif /* !__TIME__TIME_CONST_H__INCL__ */
