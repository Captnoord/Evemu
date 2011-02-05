/**
 * @file time/Msec.h
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

#ifndef __COMMON__TIME__MSEC_H__INCL__
#define __COMMON__TIME__MSEC_H__INCL__

namespace common
{
    namespace time
    {
        class Timeval;
        class WinTime;

#   ifndef WIN32
        class Timespec;
#   endif /* !WIN32 */

        /**
         * @brief A wrapper around <code>size_t</code> as a count of milliseconds.
         *
         * @author Bloody.Rabbit
         */
        class Msec
        {
        public:
            /// A millisecond in Msec.
            static const Msec MSEC;
            /// A second in Msec.
            static const Msec SEC;
            /// A minute in Msec.
            static const Msec MIN;
            /// An hour in Msec.
            static const Msec HOUR;
            /// A day in Msec.
            static const Msec DAY;
            /// A month in Msec.
            static const Msec MONTH;
            /// A year in Msec.
            static const Msec YEAR;

            /**
             * @brief A default constructor.
             *
             * @param[in] msec A number of milliseconds.
             */
            Msec( size_t msec = 0 );

            /**
             * @brief A copy constructor.
             *
             * @param[in] oth The object to copy.
             */
            Msec( const Msec& oth );
#       ifndef WIN32
            /**
             * @brief A conversion constructor.
             *
             * @param[in] ts Timespec to be converted.
             */
            Msec( const Timespec& ts );
#       endif /* !WIN32 */
            /**
             * @brief A conversion constructor.
             *
             * @param[in] tv Timeval to be converted.
             */
            Msec( const Timeval& tv );
            /**
             * @brief A conversion constructor.
             *
             * @param[in] time WinTime to be converted.
             */
            Msec( const WinTime& time );

            /// Obtains the stored number of milliseconds.
            size_t count() const { return mMsec; }

            /// An equivalency operator.
            bool operator==( const Msec& oth ) const;
            /// A non-equivalency operator.
            bool operator!=( const Msec& oth ) const;
            /// A less-than operator.
            bool operator<( const Msec& oth ) const;
            /// A less-or-equal operator.
            bool operator<=( const Msec& oth ) const;
            /// A greater-than operator.
            bool operator>( const Msec& oth ) const;
            /// A greater-or-equal operator.
            bool operator>=( const Msec& oth ) const;

            /// A sum operator.
            Msec operator+( const Msec& oth ) const;
            /// A difference operator.
            Msec operator-( const Msec& oth ) const;
            /// A multiply operator.
            Msec operator*( size_t ratio ) const;
            /// A division operator.
            Msec operator/( size_t ratio ) const;
            /// A remainder operator.
            Msec operator%( size_t ratio ) const;

            /// An assignment operator.
            Msec& operator=( const Msec& oth );
            /// An add operator.
            Msec& operator+=( const Msec& oth );
            /// A substract operator.
            Msec& operator-=( const Msec& oth );
            /// An expand operator.
            Msec& operator*=( size_t ratio );
            /// A reduce operator.
            Msec& operator/=( size_t ratio );
            /// A remainder-assign operator.
            Msec& operator%=( size_t ratio );

        protected:
            /// The count of milliseconds.
            size_t mMsec;
        };
    }
}

#endif /* !__COMMON__TIME__MSEC_H__INCL__ */
