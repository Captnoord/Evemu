/**
 * @file time/Tm.h
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

#ifndef __COMMON__TIME__TM_H__INCL__
#define __COMMON__TIME__TM_H__INCL__

namespace common
{
    namespace time
    {
        /**
         * @brief A wrapper around <code>struct tm</code>.
         *
         * @author Bloody.Rabbit
         */
        class Tm
        {
        public:
            /**
             * @brief A default constructor.
             */
            Tm();
            /**
             * @brief A primary constructor.
             *
             * @param[in] t A <code>tm</code>.
             */
            Tm( const tm& t );
            /**
             * @brief A conversion operator.
             *
             * @param[in] t A <code>time_t</code> value to convert.
             */
            Tm( time_t t );

            /**
             * @brief A copy operator.
             *
             * @param[in] oth An object to copy.
             */
            Tm( const Tm& oth );

            /// Obtains the stored <code>tm</code>.
            const tm& t() const { return mTm; }

            /// Obtains a number of seconds after the minute.
            int sec() const { return t().tm_sec; }
            /// Obtains a number of minutes after the hour.
            int min() const { return t().tm_min; }
            /// Obtains a number of hours since midnight.
            int hour() const { return t().tm_hour; }

            /// Obtains a day of the month.
            int mday() const { return t().tm_mday; }
            /// Obtains a number of days since Sunday.
            int wday() const { return t().tm_wday; }
            /// Obtains a number of days since January 1.
            int yday() const { return t().tm_yday; }

            /// Obtains a number of months since January.
            int mon() const { return t().tm_mon; }
            /// Obtains a number of years since 1900.
            int year() const { return t().tm_year; }

            /// Obtains the daylight saving time flag.
            int isDst() const { return t().tm_isdst; }

        protected:
            /// The stored <code>tm</code>.
            tm mTm;
        };
    }
}

#endif /* !__COMMON__TIME__TM_H__INCL__ */
