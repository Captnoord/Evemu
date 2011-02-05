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

#ifndef __COMMON__TIME__TIMEVAL_H__INCL__
#define __COMMON__TIME__TIMEVAL_H__INCL__

#include "time/Utils.h"

namespace common
{
    namespace time
    {
        class Msec;
        class WinTime;

#   ifndef WIN32
        class Timespec;
#   endif /* !WIN32 */

        /**
         * @brief A wrapper around <code>struct timeval</code>.
         *
         * @author Bloody.Rabbit
         */
        class Timeval
        {
        public:
            /// A microsecond in Timeval.
            static const Timeval USEC;
            /// A millisecond in Timeval.
            static const Timeval MSEC;
            /// A second in Timeval.
            static const Timeval SEC;
            /// A minute in Timeval.
            static const Timeval MIN;
            /// An hour in Timeval.
            static const Timeval HOUR;
            /// A day in Timeval.
            static const Timeval DAY;
            /// A month in Timeval.
            static const Timeval MONTH;
            /// A year in Timeval.
            static const Timeval YEAR;

            /**
             * @brief A primary constructor.
             *
             * @param[in] sec  The number of seconds.
             * @param[in] usec The number of microseconds.
             */
            Timeval( time_t sec = 0, useconds_t usec = 0 );
            /**
             * @brief A primary constructor.
             *
             * @param[in] tv The <code>timeval</code>.
             */
            Timeval( const timeval& tv );

            /**
             * @brief A conversion constructor.
             *
             * @param[in] msec Msec to be converted.
             */
            Timeval( const Msec& msec );
#       ifndef WIN32
            /**
             * @brief A conversion operator.
             *
             * @param[in] ts Timespec to be converted.
             */
            Timeval( const Timespec& ts );
#       endif /* !WIN32 */
            /**
             * @brief A copy constructor.
             *
             * @param[in] oth Timeval to be copied.
             */
            Timeval( const Timeval& oth );
            /**
             * @brief A conversion constructor.
             *
             * @param[in] time WinTime to be converted.
             */
            Timeval( const WinTime& time );

            /// Obtains the stored <code>timeval</code>.
            const timeval& tv() const { return mTimeval; }

            /// Obtains the stored number of seconds.
            time_t sec() const { return tv().tv_sec; }
            /// Obtains the stored number of microseconds.
            useconds_t usec() const { return tv().tv_usec; }

            /// An equivalency operator.
            bool operator==( const Timeval& oth ) const;
            /// A non-equivalency operator.
            bool operator!=( const Timeval& oth ) const;
            /// A less-than operator.
            bool operator<( const Timeval& oth ) const;
            /// A less-or-equal operator.
            bool operator<=( const Timeval& oth ) const;
            /// A greater-than operator.
            bool operator>( const Timeval& oth ) const;
            /// A greater-or-equal operator.
            bool operator>=( const Timeval& oth ) const;

            /// A sum operator.
            Timeval operator+( const Timeval& oth ) const;
            /// A difference operator.
            Timeval operator-( const Timeval& oth ) const;
            /// A multiply operator.
            Timeval operator*( size_t ratio ) const;
            /// A division operator.
            Timeval operator/( size_t ratio ) const;
            /// A remainder operator.
            Timeval operator%( size_t ratio ) const;

            /// An assignment operator.
            Timeval& operator=( const Timeval& oth );
            /// An add operator.
            Timeval& operator+=( const Timeval& oth );
            /// A substract operator.
            Timeval& operator-=( const Timeval& oth );
            /// An expand operator.
            Timeval& operator*=( size_t ratio );
            /// A reduce operator.
            Timeval& operator/=( size_t ratio );
            /// A remainder-assign operator.
            Timeval& operator%=( size_t ratio );

        protected:
            /// The <code>timeval</code> itself.
            timeval mTimeval;
        };
    }
}

#endif /* !__COMMON__TIME__TIMEVAL_H__INCL__ */
