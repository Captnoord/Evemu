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

#ifndef __UTIL__STR_CONV_H__INCL__
#define __UTIL__STR_CONV_H__INCL__

#ifdef MSVC
/*
 * Following code produces lots of these, turning them off
 * as they're meaningless.
 *
 * 'conversion' conversion from 'type1' to 'type2', possible loss of data
 */
#   pragma warning( disable : 4244 )
#endif /* MSVC */

namespace Util
{
    /**
     * @brief Generic string conversion template.
     *
     * Tries to use std::string( const T& ) directly.
     *
     * @param[in] val The value to be converted.
     *
     * @return A string corresponding to @a val.
     */
    template< typename T >
    std::string strFrom( const T& val) { return std::string( val ); }
    /**
     * @brief Generic string conversion template.
     *
     * Tries to use T( const char* ) directly.
     *
     * @param[in] str String to be converted.
     *
     * @return A value corresponding to content of @a str.
     */
    template< typename T >
    T strTo( const char* str ) { return T( str ); }
    /**
     * @brief Generic string conversion template.
     *
     * @see strTo( const char* )
     */
    template< typename T >
    T strTo( const std::string& str ) { return strTo< T >( str.c_str() ); }

    /**
     * @brief Converts boolean to string.
     *
     * @param[in] val Boolean to be converted.
     *
     * @return A string corresponding to @a val.
     */
    template<>
    std::string strFrom< bool >( const bool& val );
    /**
     * @brief Converts string to boolean.
     *
     * @param[in] str String to be converted.
     *
     * @return A boolean corresponding to content of @a str.
     */
    template<>
    bool strTo< bool >( const char* str );

    /**
     * @brief Converts integer to string.
     *
     * @param[in] val Integer to be converted.
     *
     * @return A string corresponding to @a val.
     */
    template<>
    std::string strFrom< int64 >( const int64& val );
    /**
     * @brief Converts string to integer.
     *
     * @param[in] str String to be converted.
     *
     * @return An integer corresponding to content of @a str.
     */
    template<>
    int64 strTo< int64 >( const char* str );

    /**
     * @brief Converts integer to string.
     *
     * @see strFrom< int64 >( const int64& )
     */
    template<>
    inline std::string strFrom< int32 >( const int32& val ) { return strFrom< int64 >( val ); }
    /**
     * @brief Converts string to integer.
     *
     * @see strTo< int64 >( const char* )
     */
    template<>
    inline int32 strTo< int32 >( const char* str ) { return strTo< int64 >( str ); }

    /**
     * @brief Converts integer to string.
     *
     * @see strFrom< int32 >( const int32& )
     */
    template<>
    inline std::string strFrom< int16 >( const int16& val ) { return strFrom< int32 >( val ); }
    /**
     * @brief Converts string to integer.
     *
     * @see strTo< int32 >( const char* )
     */
    template<>
    inline int16 strTo< int16 >( const char* str ) { return strTo< int32 >( str ); }

    /**
     * @brief Converts integer to string.
     *
     * @see strFrom< int16 >( const int16& )
     */
    template<>
    inline std::string strFrom< int8 >( const int8& val ) { return strFrom< int16 >( val ); }
    /**
     * @brief Converts string to integer.
     *
     * @see strTo< int16 >( const char* )
     */
    template<>
    inline int8 strTo< int8 >( const char* str ) { return strTo< int16 >( str ); }

    /**
     * @brief Converts unsigned integer to string.
     *
     * @param[in] val Unsigned integer to be converted.
     *
     * @return A string corresponding to @a val.
     */
    template<>
    std::string strFrom< uint64 >( const uint64& val );
    /**
     * @brief Converts string to unsigned integer.
     *
     * @param[in] str String to be converted.
     *
     * @return An unsigned integer corresponding to content of @a str.
     */
    template<>
    uint64 strTo< uint64 >( const char* str );

    /**
     * @brief Converts unsigned integer to string.
     *
     * @see strFrom< uint64 >( const uint64& )
     */
    template<>
    inline std::string strFrom< uint32 >( const uint32& val ) { return strFrom< uint64 >( val ); }
    /**
     * @brief Converts string to unsigned integer.
     *
     * @see strTo< uint64 >( const char* )
     */
    template<>
    inline uint32 strTo< uint32 >( const char* str ) { return strTo< uint64 >( str ); }

    /**
     * @brief Converts unsigned integer to string.
     *
     * @see strFrom< uint32 >( const uint32& val )
     */
    template<>
    inline std::string strFrom< uint16 >( const uint16& val ) { return strFrom< uint32 >( val ); }
    /**
     * @brief Converts string to unsigned integer.
     *
     * @see strTo< uint32 >( const char* )
     */
    template<>
    inline uint16 strTo< uint16 >( const char* str ) { return strTo< uint32 >( str ); }

    /**
     * @brief Converts unsigned integer to string.
     *
     * @see strFrom< uint16 >( const uint16& val )
     */
    template<>
    inline std::string strFrom< uint8 >( const uint8& val ) { return strFrom< uint16 >( val ); }
    /**
     * @brief Converts string to unsigned integer.
     *
     * @see strTo< uint16 >( const char* )
     */
    template<>
    inline uint8 strTo< uint8 >( const char* str ) { return strTo< uint16 >( str ); }

    /**
     * @brief Converts real number to string.
     *
     * @param[in] val Real number to be converted.
     *
     * @return A string corresponding to @a val.
     */
    template<>
    std::string strFrom< long double >( const long double& val );
    /**
     * @brief Converts string to real number.
     *
     * @param[in] str String to be converted.
     *
     * @return A real number corresponding to content of @a str.
     */
    template<>
    long double strTo< long double >( const char* str );

    /**
     * @brief Converts real number to string.
     *
     * @see strFrom< long double >( const long double& )
     */
    template<>
    inline std::string strFrom< double >( const double& val ) { return strFrom< long double >( val ); }
    /**
     * @brief Converts string to real number.
     *
     * @see strTo< long double >( const char* )
     */
    template<>
    inline double strTo< double >( const char* str ) { return strTo< long double >( str ); }

    /**
     * @brief Converts real number to string.
     *
     * @see strFrom< double >( const double& )
     */
    template<>
    inline std::string strFrom< float >( const float& val ) { return strFrom< double >( val ); }
    /**
     * @brief Converts string to real number.
     *
     * @see strTo< double >( const char* )
     */
    template<>
    inline float strTo< float >( const char* str ) { return strTo< double >( str ); }
}

#ifdef MSVC
/*
 * Turn the warning back to default.
 * See above for details.
 */
#   pragma warning( default : 4244 )
#endif /* MSVC */

#endif /* !__UTIL__STR_CONV_H__INCL__ */
