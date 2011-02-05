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

#ifndef __TIME__TIMESPEC_H__INCL__
#define __TIME__TIMESPEC_H__INCL__

namespace Time
{
#   ifndef WIN32
    class Msec;
    class Timeval;
    class WinTime;

    /**
     * @brief A wrapper around <code>struct timespec</code>.
     *
     * @author Bloody.Rabbit
     */
    class Timespec
    {
    public:
        /// A nanosecond in Timespec.
        static const Timespec NSEC;
        /// A microsecond in Timespec.
        static const Timespec USEC;
        /// A millisecond in Timespec.
        static const Timespec MSEC;
        /// A second in Timespec.
        static const Timespec SEC;
        /// A minute in Timespec.
        static const Timespec MIN;
        /// An hour in Timespec.
        static const Timespec HOUR;
        /// A day in Timespec.
        static const Timespec DAY;
        /// A month in Timespec.
        static const Timespec MONTH;
        /// A year in Timespec.
        static const Timespec YEAR;

        /**
         * @brief A primary constructor.
         *
         * @param[in] sec  The number of seconds.
         * @param[in] nsec The number of nanoseconds.
         */
        Timespec( time_t sec = 0, long nsec = 0 );
        /**
         * @brief A primary constructor.
         *
         * @param[in] ts The <code>timespec</code>.
         */
        Timespec( const timespec& ts );

        /**
         * @brief A conversion constructor.
         *
         * @param[in] msec Msec to be converted.
         */
        Timespec( const Msec& msec );
        /**
         * @brief A copy constructor.
         *
         * @param[in] oth Timespec to copy.
         */
        Timespec( const Timespec& oth );
        /**
         * @brief A conversion constructor.
         *
         * @param[in] tv Timeval to be converted.
         */
        Timespec( const Timeval& tv );
        /**
         * @brief A conversion constructor.
         *
         * @param[in] time WinTime to be converted.
         */
        Timespec( const WinTime& time );

        /// Obtains the stored <code>timespec</code>.
        const timespec& ts() const { return mTimespec; }

        /// The stored number of seconds.
        time_t sec() const { return ts().tv_sec; }
        /// The stored number of nanoseconds.
        long nsec() const { return ts().tv_nsec; }

        /// An equivalency operator.
        bool operator==( const Timespec& oth ) const;
        /// A non-equivalency operator.
        bool operator!=( const Timespec& oth ) const;
        /// A less-than operator.
        bool operator<( const Timespec& oth ) const;
        /// A less-or-equal operator.
        bool operator<=( const Timespec& oth ) const;
        /// A greater-than operator.
        bool operator>( const Timespec& oth ) const;
        /// A greater-or-equal operator.
        bool operator>=( const Timespec& oth ) const;

        /// A sum operator.
        Timespec operator+( const Timespec& oth ) const;
        /// A difference operator.
        Timespec operator-( const Timespec& oth ) const;
        /// A multiply operator.
        Timespec operator*( size_t ratio ) const;
        /// A division operator.
        Timespec operator/( size_t ratio ) const;
        /// A remainder operator.
        Timespec operator%( size_t ratio ) const;

        /// An assignment operator.
        Timespec& operator=( const Timespec& oth );
        /// An add operator.
        Timespec& operator+=( const Timespec& oth );
        /// A substract operator.
        Timespec& operator-=( const Timespec& oth );
        /// An expand operator.
        Timespec& operator*=( size_t ratio );
        /// A reduce operator.
        Timespec& operator/=( size_t ratio );
        /// A remainder-assign operator.
        Timespec& operator%=( size_t ratio );

    protected:
        /// The <code>timespec</code>.
        timespec mTimespec;
    };
#   endif /* !WIN32 */
}

#endif /* !__TIME__TIMESPEC_H__INCL__ */
