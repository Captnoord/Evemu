/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// Ascent Configuration Header File
// Feel free to customize to your needs.

#ifndef __ASCENTCONFIG_H
#define __ASCENTCONFIG_H

/**
 * DATABASE LAYER SET UP
 */

/**
 * Enable/disable database backends.
 * Currently supported, MySQL, (under development): PostgreSQL, SQLite
 */
//#define NO_DBLAYER_SQL

#if !defined(NO_DBLAYER_SQL)
	//#define ENABLE_DATABASE_MYSQL 1
	//#define ENABLE_DATABASE_POSTGRES 1
	//#define ENABLE_DATABASE_SQLITE 1
#endif

/**
* OPENSSL SET UP
*/

//#define ASCENT_FRAMEWORK_ENABLE_OPENSSL

// broken atm
// TODO fix this
//define ASCENT_FRAMEWORK_ENABLE_CRASH_HANDLER

/**
* CONSOLE OUTPUT
*/

/**
* Enable/disable console output
* disabling console output will make sure its possible to run the applications with as much speed as possible
*/
#define ENABLE_CONSOLE_LOG 1

// log is multi threaded

#define LOG_USE_MUTEX 1

#endif		// __ASCENTCONFIG_H

