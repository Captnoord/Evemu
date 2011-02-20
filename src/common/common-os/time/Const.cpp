/**
 * @file time/Const.cpp
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
 * @author Bloody.Rabbit
 */

#include "CommonOs.h"

#include "time/Const.h"

using namespace common;
using namespace common::time;

/*************************************************************************/
/* common::time                                                          */
/*************************************************************************/
/*
 * Even though these values might seem oversimplified, do not
 * alter them! WinTime-related calculations depend on these
 * specific values.
 */
const unsigned int time::NSEC_PER_USEC = 1000;
const unsigned int time::USEC_PER_MSEC = 1000;
const unsigned int time::MSEC_PER_SEC = 1000;
const unsigned int time::SEC_PER_MIN = 60;
const unsigned int time::MIN_PER_HOUR = 60;
const unsigned int time::HOUR_PER_DAY = 24;
const unsigned int time::DAY_PER_MONTH = 30;
const unsigned int time::MONTH_PER_YEAR = 12;

const unsigned int time::NSEC_PER_WINTIME = 100;
const unsigned int time::WINTIME_PER_USEC = 10;

const unsigned long long time::EPOCH_DIFF_SEC = time::SEC_PER_MIN * time::MIN_PER_HOUR * time::HOUR_PER_DAY * 134774ULL;
