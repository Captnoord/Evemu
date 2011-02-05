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

#include "CommonStd.h"

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
const size_t time::NSEC_PER_USEC = 1000;
const size_t time::USEC_PER_MSEC = 1000;
const size_t time::MSEC_PER_SEC = 1000;
const size_t time::SEC_PER_MIN = 60;
const size_t time::MIN_PER_HOUR = 60;
const size_t time::HOUR_PER_DAY = 24;
const size_t time::DAY_PER_MONTH = 30;
const size_t time::MONTH_PER_YEAR = 12;

const size_t time::NSEC_PER_WINTIME = 100;
const size_t time::WINTIME_PER_USEC = 10;

const size_t time::EPOCH_DIFF_SEC = time::SEC_PER_MIN * time::MIN_PER_HOUR * time::HOUR_PER_DAY * 134774;
