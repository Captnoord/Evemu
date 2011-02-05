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

#ifndef __COMMON__GA__UTILS_H__INCL__
#define __COMMON__GA__UTILS_H__INCL__

namespace common
{
    /**
     * @brief Various utilities related to the Gangsta wrapper library.
     */
    namespace ga
    {
        /**
         * @brief A point in 3D space.
         *
         * @author Zhur
         */
        class Point
        : public GaVec3
        {
        public:
            /**
             * @brief A default constructor.
             */
            Point()
            : GaVec3()
            {
            }
            /**
             * @brief A cube-like constructor.
             *
             * @param[in] v A value of coordinate for every axis.
             */
            Point( GaFloat v )
            : GaVec3( v )
            {
            }
            /**
             * @brief A constructor with a coordinate array.
             *
             * @param[in] v An array of coordinates; the first value in
             *              the array is for X, the second one for Y and
             *              the last one for Z.
             */
            Point( const GaFloat* v )
            : GaVec3( v )
            {
            }
            /**
             * @brief A constructor with 3 value for earch coordinate.
             *
             * @param[in] X A value of coordinate for X axis.
             * @param[in] Y A value of coordinate for Y axis.
             * @param[in] Z A value of coordinate for Z axis.
             */
            Point( GaFloat X, GaFloat Y, GaFloat Z )
            : GaVec3( X, Y, Z )
            {
            }
            /**
             * @brief A constructor with a vector.
             *
             * @param[in] oth The vector the target of which
             *                is to be assigned.
             */
            Point( const GaVec3& oth )
            : GaVec3( oth )
            {
            }
            /**
             * @brief A copy constructor.
             *
             * @param[in] oth The point to copy.
             */
            Point( const Point& oth )
            : GaVec3( oth )
            {
            }
        };

        /**
         * @brief A vector in 3D space.
         *
         * @author Zhur
         */
        class Vector
        : public GaVec3
        {
        public:
            /**
             * @brief A default constructor.
             */
            Vector()
            : GaVec3()
            {
            }
            /**
             * @brief A cube-like constructor.
             *
             * @param[in] v A value of coordinate for every axis.
             */
            Vector( GaFloat v )
            : GaVec3( v )
            {
            }
            /**
             * @brief A constructor with a coordinate array.
             *
             * @param[in] v An array of coordinates; the first value in
             *              the array is for X, the second one for Y and
             *              the last one for Z.
             */
            Vector( const GaFloat* v )
            : GaVec3( v )
            {
            }
            /**
             * @brief A constructor with 3 value for earch coordinate.
             *
             * @param[in] X A value of coordinate for X axis.
             * @param[in] Y A value of coordinate for Y axis.
             * @param[in] Z A value of coordinate for Z axis.
             */
            Vector( GaFloat X, GaFloat Y, GaFloat Z )
            : GaVec3( X, Y, Z )
            {
            }
            /**
             * @brief A constructor with a point.
             *
             * @param[in] oth Where the vector shall point.
             */
            Vector( const Point& oth )
            : GaVec3( oth )
            {
            }
            /**
             * @brief A constructor with two points.
             *
             * @param[in] from An origin of the vector.
             * @param[in] to   Where the vector shall point.
             */
            Vector( const Point& from, const Point& to )
            : GaVec3( ( to.x - from.x ),
                      ( to.y - from.y ),
                      ( to.z - from.z ) )
            {
            }
            /**
             * @brief A copy constructor.
             *
             * @param[in] oth The vector to copy.
             */
            Vector( const GaVec3& oth )
            : GaVec3( oth )
            {
            }
        };
    }
}

#endif /* !__COMMON__GA__UTILS_H__INCL__ */
