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

#ifndef __UTIL__STRING_H__INCL__
#define __UTIL__STRING_H__INCL__

#include "util/Buffer.h"

namespace Util
{
    /**
     * @brief Generates random key.
     *
     * @param[in] length Length of key to generate.
     *
     * @return Generated key.
     */
    std::string GenerateKey( size_t length );

    /**
     * @brief Function to convert a vector of numbers to a string containing
     *        string representatives of those numbers.
     *
     * @param[in]  ints     contains the numbers that need to converted.
     * @param[in]  if_empty is the default value added if ints is empty.
     * @param[out] into     contains the string representatives of the numbers.
     */
    void ListToINString( const std::vector< int32 >& ints, std::string& into,
                         const char* if_empty = "" );

    /**
     * @brief Decodes string escapes into actual characters.
     *
     * Based on PyString_DecodeEscape from Python.
     *
     * @param[in]  str  Escaped string.
     * @param[out] into The result will be put here.
     *
     * @retval true  Decode ran OK.
     * @retval false Error occured during decoding.
     */
    bool PyDecodeEscape( const char* str, Buffer& into );

    /**
     * @brief Does search & replace on @a subject.
     *
     * @param[in,out] subject String to be searched.
     * @param[in]     search  String which shall be replaced.
     * @param[in]     replace String with which to replace.
     */
    void SearchReplace( std::string& subject, const std::string& search,
                        const std::string& replace );

    /**
     * @brief Splits arguments.
     *
     * @param[in]  str    The string which contains the arguments.
     * @param[out] into   Vector which receives the arguments.
     * @param[in]  divs   Array of valid divider chars.
     * @param[in]  quotes Array of valid quotation chars.
     *
     * @return true  Splitting OK.
     * @return false Malformed input string <var>str</var>.
     */
    bool SplitArguments( const std::string& str, std::vector< std::string >& into,
                         const char* divs = " \t", const char* quotes = "\"\'" );
    /**
     * @brief Splits path to its components.
     *
     * @param[in]  path Path to split.
     * @param[out] into Vector which receives components of the path.
     * @param[in]  sep  Possible path separators.
     */
    void SplitPath( const std::string& path, std::vector< std::string >& into,
                    const char* sep = "/\\" );

    /**
     * @brief Holds various string-related utilities.
     *
     * @author Bloody.Rabbit
     */
    template< typename T >
    struct String;

    /**
     * @brief Implementation of Util::String for <code>char</code>-based strings.
     *
     * @author Bloody.Rabbit
     */
    template<>
    struct String< char >
    {
        /// std::string to use where you would use NULL for const char*.
        static const std::string NULL_STRING;

        /**
         * @brief Generic string conversion template.
         *
         * Tries to use std::string( const T& ) directly.
         *
         * @param[in] val The value to be converted.
         *
         * @return A string corresponding to <var>val</var>.
         */
        template< typename T >
        static std::string from( const T& val) { return std::string( val ); }

        /**
         * @brief Checks whether character is a number.
         *
         * @param[in] c The character to be checked.
         *
         * @retval true  The character is a number.
         * @retval false The character is not a number.
         */
        static bool isNumber( char c );
        /**
         * @brief Checks whether string is a number.
         *
         * @param[in] str The string to be checked.
         * @param[in] len Length of the string.
         *
         * @retval true  The string is a number.
         * @retval false The string is not a number.
         */
        static bool isNumber( const char* str, size_t len );
        /**
         * @brief Checks whether string is a number.
         *
         * @param[in] str The string to be checked.
         *
         * @retval true  The string is a number.
         * @retval false The string is not a number.
         */
        static bool isNumber( const std::string& str );

        /**
         * @brief Checks whether character is a hexadecimal number.
         *
         * @param[in] c The character to be checked.
         *
         * @retval true  The character is a hexadecimal number.
         * @retval false The character is not a hexadecimal number.
         */
        static bool isHexNumber( char c );
        /**
         * @brief Checks whether string is a hexadecimal number.
         *
         * @param[in] str The string to be checked.
         * @param[in] len Length of the string.
         *
         * @retval true  The string is a hexadecimal number.
         * @retval false The string is not a hexadecimal number.
         */
        static bool isHexNumber( const char* str, size_t len );
        /**
         * @brief Checks whether string is a hexadecimal number.
         *
         * @param[in] str The string to be checked.
         *
         * @retval true  The string is a hexadecimal number.
         * @retval false The string is not a hexadecimal number.
         */
        static bool isHexNumber( const std::string& str );

        /**
         * @brief Checks whether character is printable.
         *
         * @param[in] c The character to be checked.
         *
         * @retval true  The character is printable.
         * @retval false The character is not printable.
         */
        static bool isPrintable( char c );
        /**
         * @brief Checks whether string is printable.
         *
         * @param[in] str String to be checked.
         * @param[in] len Length of string.
         *
         * @retval true  The string is printable.
         * @retval false The string is not printable.
         */
        static bool isPrintable( const char* str, size_t len );
        /**
         * @brief Checks whether string is printable.
         *
         * @param[in] str String to be checked.
         *
         * @retval true  The string is printable.
         * @retval false The string is not printable.
         */
        static bool isPrintable( const std::string& str );

        /**
         * @brief Generic string conversion template.
         *
         * Tries to use T( const char* ) directly.
         *
         * @param[in] str String to be converted.
         *
         * @return A value corresponding to content of <var>str</var>.
         */
        template< typename T >
        static T to( const char* str ) { return T( str ); }
        /**
         * @brief Generic string conversion template.
         *
         * @see to( const char* )
         */
        template< typename T >
        static T to( const std::string& str ) { return to< T >( str.c_str() ); }

        /**
         * @brief toupper() for strings.
         *
         * @param[in]  source Source string.
         * @param[out] target Array which receives result.
         */
        static void toUpper( const char* source, char* target );
        /**
         * @brief tolower() for strings.
         *
         * @param[in]  source Source string.
         * @param[out] target Array which receives result.
         */
        static void toLower( const char* source, char* target );
    };

#   ifdef MSVC
    /*
     * Following code produces lots of these, turning them off
     * as they're meaningless.
     *
     * 'conversion' conversion from 'type1' to 'type2', possible loss of data
     */
#       pragma warning( disable : 4244 )
#   endif /* MSVC */

    /**
     * @brief Converts boolean to string.
     *
     * @param[in] val Boolean to be converted.
     *
     * @return A string corresponding to <var>val</var>.
     */
    template<>
    std::string String< char >::from< bool >( const bool& val );
    /**
     * @brief Converts integer to string.
     *
     * @param[in] val Integer to be converted.
     *
     * @return A string corresponding to <var>val</var>.
     */
    template<>
    std::string String< char >::from< int64 >( const int64& val );
    /**
     * @brief Converts integer to string.
     *
     * @see from< int64 >( const int64& )
     */
    template<>
    inline std::string String< char >::from< int32 >( const int32& val ) { return from< int64 >( val ); }
    /**
     * @brief Converts integer to string.
     *
     * @see from< int32 >( const int32& )
     */
    template<>
    inline std::string String< char >::from< int16 >( const int16& val ) { return from< int32 >( val ); }
    /**
     * @brief Converts integer to string.
     *
     * @see from< int16 >( const int16& )
     */
    template<>
    inline std::string String< char >::from< int8 >( const int8& val ) { return from< int16 >( val ); }
    /**
     * @brief Converts unsigned integer to string.
     *
     * @param[in] val Unsigned integer to be converted.
     *
     * @return A string corresponding to <var>val</var>.
     */
    template<>
    std::string String< char >::from< uint64 >( const uint64& val );
    /**
     * @brief Converts unsigned integer to string.
     *
     * @see from< uint64 >( const uint64& )
     */
    template<>
    inline std::string String< char >::from< uint32 >( const uint32& val ) { return from< uint64 >( val ); }
    /**
     * @brief Converts unsigned integer to string.
     *
     * @see from< uint32 >( const uint32& val )
     */
    template<>
    inline std::string String< char >::from< uint16 >( const uint16& val ) { return from< uint32 >( val ); }
    /**
     * @brief Converts unsigned integer to string.
     *
     * @see from< uint16 >( const uint16& val )
     */
    template<>
    inline std::string String< char >::from< uint8 >( const uint8& val ) { return from< uint16 >( val ); }
    /**
     * @brief Converts real number to string.
     *
     * @param[in] val Real number to be converted.
     *
     * @return A string corresponding to <var>val</var>.
     */
    template<>
    std::string String< char >::from< long double >( const long double& val );
    /**
     * @brief Converts real number to string.
     *
     * @see from< long double >( const long double& )
     */
    template<>
    inline std::string String< char >::from< double >( const double& val ) { return from< long double >( val ); }
    /**
     * @brief Converts real number to string.
     *
     * @see from< double >( const double& )
     */
    template<>
    inline std::string String< char >::from< float >( const float& val ) { return from< double >( val ); }

    /**
     * @brief Converts string to boolean.
     *
     * @param[in] str String to be converted.
     *
     * @return A boolean corresponding to content of <var>str</var>.
     */
    template<>
    bool String< char >::to< bool >( const char* str );
    /**
     * @brief Converts string to integer.
     *
     * @param[in] str String to be converted.
     *
     * @return An integer corresponding to content of <var>str</var>.
     */
    template<>
    int64 String< char >::to< int64 >( const char* str );
    /**
     * @brief Converts string to integer.
     *
     * @see to< int64 >( const char* )
     */
    template<>
    inline int32 String< char >::to< int32 >( const char* str ) { return to< int64 >( str ); }
    /**
     * @brief Converts string to integer.
     *
     * @see to< int32 >( const char* )
     */
    template<>
    inline int16 String< char >::to< int16 >( const char* str ) { return to< int32 >( str ); }
    /**
     * @brief Converts string to integer.
     *
     * @see to< int16 >( const char* )
     */
    template<>
    inline int8 String< char >::to< int8 >( const char* str ) { return to< int16 >( str ); }
    /**
     * @brief Converts string to unsigned integer.
     *
     * @param[in] str String to be converted.
     *
     * @return An unsigned integer corresponding to content of <var>str</var>.
     */
    template<>
    uint64 String< char >::to< uint64 >( const char* str );
    /**
     * @brief Converts string to unsigned integer.
     *
     * @see to< uint64 >( const char* )
     */
    template<>
    inline uint32 String< char >::to< uint32 >( const char* str ) { return to< uint64 >( str ); }
    /**
     * @brief Converts string to unsigned integer.
     *
     * @see to< uint32 >( const char* )
     */
    template<>
    inline uint16 String< char >::to< uint16 >( const char* str ) { return to< uint32 >( str ); }
    /**
     * @brief Converts string to unsigned integer.
     *
     * @see to< uint16 >( const char* )
     */
    template<>
    inline uint8 String< char >::to< uint8 >( const char* str ) { return to< uint16 >( str ); }
    /**
     * @brief Converts string to real number.
     *
     * @param[in] str String to be converted.
     *
     * @return A real number corresponding to content of <var>str</var>.
     */
    template<>
    long double String< char >::to< long double >( const char* str );
    /**
     * @brief Converts string to real number.
     *
     * @see to< long double >( const char* )
     */
    template<>
    inline double String< char >::to< double >( const char* str ) { return to< long double >( str ); }
    /**
     * @brief Converts string to real number.
     *
     * @see to< double >( const char* )
     */
    template<>
    inline float String< char >::to< float >( const char* str ) { return to< double >( str ); }

#   ifdef MSVC
    /*
     * Turn the warning back to default.
     * See above for details.
     */
#       pragma warning( default : 4244 )
#   endif /* MSVC */
}

#endif /* !__UTIL__STRING_H__INCL__ */
