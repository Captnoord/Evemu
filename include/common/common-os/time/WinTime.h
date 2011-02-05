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

#ifndef __COMMON__TIME__WIN_TIME_H__INCL__
#define __COMMON__TIME__WIN_TIME_H__INCL__

namespace common
{
    namespace time
    {
        class Msec;
        class Timeval;

#   ifndef WIN32
        class Timespec;
#   endif /* !WIN32 */

        /**
         * @brief A wrapper around <code>uint64</code> as a Windows time.
         *
         * @author Bloody.Rabbit
         */
        class WinTime
        {
        public:
            /// A microsecond in WinTime.
            static const WinTime USEC;
            /// A millisecond in WinTime.
            static const WinTime MSEC;
            /// A second in WinTime.
            static const WinTime SEC;
            /// A minute in WinTime.
            static const WinTime MIN;
            /// An hour in WinTime.
            static const WinTime HOUR;
            /// A day in WinTime.
            static const WinTime DAY;
            /// A month in WinTime.
            static const WinTime MONTH;
            /// A year in WinTime.
            static const WinTime YEAR;

            /**
             * @brief A primary constructor.
             *
             * @param[in] time The Windows time.
             */
            WinTime( uint64 time = 0 );

            /**
             * @brief A conversion constructor.
             *
             * @param[in] msec Msec to be converted.
             */
            WinTime( const Msec& msec );
#       ifndef WIN32
            /**
             * @brief A conversion operator.
             *
             * @param[in] ts Timespec to be converted.
             */
            WinTime( const Timespec& ts );
#       endif /* !WIN32 */
            /**
             * @brief A conversion constructor.
             *
             * @param[in] tv Timeval to be converted.
             */
            WinTime( const Timeval& tv );
            /**
             * @brief A copy constructor.
             *
             * @param[in] oth WinTime to be copied.
             */
            WinTime( const WinTime& oth );

            /// Obtains the stored time.
            uint64 count() const { return mTime; }

            /// An equivalency operator.
            bool operator==( const WinTime& oth ) const;
            /// A non-equivalency operator.
            bool operator!=( const WinTime& oth ) const;
            /// A less-than operator.
            bool operator<( const WinTime& oth ) const;
            /// A less-or-equal operator.
            bool operator<=( const WinTime& oth ) const;
            /// A greater-than operator.
            bool operator>( const WinTime& oth ) const;
            /// A greater-or-equal operator.
            bool operator>=( const WinTime& oth ) const;

            /// A sum operator.
            WinTime operator+( const WinTime& oth ) const;
            /// A difference operator.
            WinTime operator-( const WinTime& oth ) const;
            /// A multiply operator.
            WinTime operator*( size_t ratio ) const;
            /// A division operator.
            WinTime operator/( size_t ratio ) const;
            /// A remainder operator.
            WinTime operator%( size_t ratio ) const;

            /// An assignment operator.
            WinTime& operator=( const WinTime& oth );
            /// An add operator.
            WinTime& operator+=( const WinTime& oth );
            /// A substract operator.
            WinTime& operator-=( const WinTime& oth );
            /// An expand operator.
            WinTime& operator*=( size_t ratio );
            /// A reduce operator.
            WinTime& operator/=( size_t ratio );
            /// A remainder-assign operator.
            WinTime& operator%=( size_t ratio );

        protected:
            /// The time itself.
            uint64 mTime;
        };
    }
}

#endif /* !__COMMON__TIME__WIN_TIME_H__INCL__ */
