/**
 * @file util/String.cpp
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

#include "util/String.h"

using namespace common;
using namespace common::util;

/*************************************************************************/
/* common::util                                                          */
/*************************************************************************/
std::string util::GenerateKey( size_t length )
{
    static const char CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static const size_t CHARS_COUNT = sizeof( CHARS ) / sizeof( char );

    std::string key;

    for(; 0 < length; --length)
        key += CHARS[ MakeRandomInt( 0, CHARS_COUNT - 1 ) ];

    return key;
}

void util::ListToINString( const std::vector< int32 >& ints, std::string& into,
                           const char* if_empty )
{
    if( ints.empty() )
    {
        into = if_empty;
        return;
    }

    /*
     * Some small theory about numbers to strings
     *
     * the max size of a number converted to
     * a string is:
     * uint32 -1 results in
     * "4294967295" which is 10 characters.
     */
    size_t format_index = into.size();
    into.resize( format_index + ints.size() * ( 10 + 1 ) );

    std::vector<int32>::const_iterator cur, end;
    cur = ints.begin();
    end = ints.end();
    for(; cur != end; ++cur)
    {
        if( ( cur + 1 ) != end )
            format_index += snprintf( &into[ format_index ], 12, "%d,", *cur );
        else
            // last value to be printed
            format_index += snprintf( &into[ format_index ], 11, "%d", *cur );
    }
}

bool util::PyDecodeEscape( const char* str, util::Buffer& into )
{
    const size_t len = strlen( str );
    const char* const end = str + len;

    while( str < end )
    {
        if( *str != '\\' )
        {
            into.Append< char >( *str++ );
            continue;
        }

        if( ++str == end )
            //ended with a \ char
            return false;

        int c;
        switch( *str++ )
        {
            /* XXX This assumes ASCII! */
            case '\n':                                break; /* ? */
            case '\\': into.Append< char >( '\\' );   break;
            case '\'': into.Append< char >( '\'' );   break;
            case '\"': into.Append< char >( '\"' );   break;
            case 'b':  into.Append< char >( '\b' );   break;
            case 'f':  into.Append< char >( '\014' ); break; /* FF */
            case 't':  into.Append< char >( '\t' );   break;
            case 'n':  into.Append< char >( '\n' );   break;
            case 'r':  into.Append< char >( '\r' );   break;
            case 'v':  into.Append< char >( '\013' ); break; /* VT */
            case 'a':  into.Append< char >( '\007' ); break; /* BEL, not classic C */

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                c = str[-1] - '0';
                if( '0' <= *str && *str <= '7' )
                {
                    c = ( c << 3 ) + *str++ - '0';
                    if( '0' <= *str && *str <= '7' )
                        c = ( c << 3 ) + *str++ - '0';
                }
                into.Append< uint8 >( c );
                break;

        case 'x':
            if( isxdigit( str[0] ) && isxdigit( str[1] ) )
            {
                unsigned int x = 0;
                c = *str++;

                if( isdigit(c) )
                    x = c - '0';
                else if( islower(c) )
                    x = 10 + c - 'a';
                else
                    x = 10 + c - 'A';

                x = x << 4;
                c = *str++;

                if( isdigit(c) )
                    x += c - '0';
                else if( islower(c) )
                    x += 10 + c - 'a';
                else
                    x += 10 + c - 'A';

                into.Append< uint8 >( x );
                break;
            }
            //"invalid \\x escape");
            return false;

        default:
            return false;
        }
    }

    return true;
}

void util::SearchReplace( std::string& subject, const std::string& search,
                          const std::string& replace )
{
    std::string::size_type pos = 0;
    while( std::string::npos != ( pos = subject.find( search, pos ) ) )
    {
        subject.replace( pos, search.length(), replace );
        pos += replace.length();
    }
}

bool util::SplitArguments( const std::string& str, std::vector< std::string >& into,
                           const char* divs, const char* quotes )
{
    size_t baseIndex = 0;

    while( std::string::npos != baseIndex )
    {
        size_t divIndex = str.find_first_of( divs, baseIndex );
        size_t quoteIndex = str.find_first_of( quotes, baseIndex );

        while( quoteIndex < divIndex )
        {
            size_t otherQuoteIndex = str.find_first_of( quotes, quoteIndex + 1 );
            if( std::string::npos == otherQuoteIndex )
                // unpaired quote
                return false;

            divIndex = str.find_first_of( divs, otherQuoteIndex + 1 );
            quoteIndex = str.find_first_of( quotes, otherQuoteIndex + 1 );
        }

        into.push_back( str.substr( baseIndex, divIndex ) );
        baseIndex = str.find_first_not_of( divs, divIndex );
    }

    return true;
}

void util::SplitPath( const std::string& path, std::vector< std::string >& into,
                      const char* sep )
{
    size_t baseIndex = 0;

    while( std::string::npos != baseIndex )
    {
        size_t sepIndex = path.find_first_of( sep, baseIndex );

        into.push_back( path.substr( baseIndex, sepIndex ) );
        baseIndex = path.find_first_not_of( sep, sepIndex );
    }
}

/*************************************************************************/
/* common::util::String< char >                                          */
/*************************************************************************/
const std::string String< char >::NULL_STRING = "NULL";

template<>
std::string String< char >::from< bool >( const bool& val )
{
    return val ? "true" : "false";
}

template<>
std::string String< char >::from< int64 >( const int64& val )
{
    std::string str;
    ::sprintf( str, "%"PRId64, val );
    return str;
}

template<>
std::string String< char >::from< uint64 >( const uint64& val )
{
    std::string str;
    ::sprintf( str, "%"PRIu64, val );
    return str;
}

template<>
std::string String< char >::from< long double >( const long double& val )
{
    std::string str;
    ::sprintf( str, "%Lf", val );
    return str;
}

bool String< char >::isNumber( char c )
{
    return 0 != ::isdigit( c );
}

bool String< char >::isNumber( const char* str, size_t len )
{
    // skip sign if there is one
    if( 1 >= len )
    {
        if(    '-' == str[0]
            || '+' == str[0] )
        {
            str += 1;
            len -= 1;
        }
    }

    if( 0 == len )
        return false;

    bool seenDec = false;
    for(; len > 0; ++str, --len)
    {
        if( !isNumber( *str ) )
        {
            if( !seenDec && '.' == *str )
                seenDec = true;
            else
                return false;
        }
    }

    return true;
}

bool String< char >::isNumber( const std::string& str )
{
    return isNumber( str.c_str(), str.length() );
}

bool String< char >::isHexNumber( char c )
{
    return 0 != ::isxdigit( c );
}

bool String< char >::isHexNumber( const char* str, size_t len )
{
    // skip sign if there is one
    if( 0 < len )
    {
        if(    '-' == str[0]
            || '+' == str[0] )
        {
            str += 1;
            len -= 1;
        }
    }

    // skip "0x" or "0X" prefix if there is one
    if( 1 < len )
    {
        if(    '0' == str[0]
            && (    'x' == str[1]
                 || 'X' == str[1] ) )
        {
            str += 2;
            len -= 2;
        }
    }

    if( 0 == len )
        return false;

    for(; 0 < len; ++str, --len)
    {
        if( !isHexNumber( *str ) )
            return false;
    }

    return true;
}

bool String< char >::isHexNumber( const std::string& str )
{
    return isHexNumber( str.c_str(), str.length() );
}

bool String< char >::isPrintable( char c )
{
    // They seem to expect it unsigned ...
    const unsigned char _c = c;

    return ( ::isgraph( _c ) || ::isspace( _c ) );
}

bool String< char >::isPrintable( const char* str, size_t len )
{
    for(; 0 < len; ++str, --len)
    {
        if( !isPrintable( *str ) )
            return false;
    }

    return true;
}

bool String< char >::isPrintable( const std::string& str )
{
    return isPrintable( str.c_str(), str.size() );
}

template<>
bool String< char >::to< bool >( const char* str )
{
    if( !::strcasecmp( str, "true" ) )
        return true;
    else if( !::strcasecmp( str, "false" ) )
        return false;
    else if( !::strcasecmp( str, "yes" ) )
        return true;
    else if( !::strcasecmp( str, "no" ) )
        return false;
    else if( !::strcasecmp( str, "y" ) )
        return true;
    else if( !::strcasecmp( str, "n" ) )
        return false;
    else if( !::strcasecmp( str, "on" ) )
        return true;
    else if( !::strcasecmp( str, "off" ) )
        return false;
    else if( !::strcasecmp( str, "enable" ) )
        return true;
    else if( !::strcasecmp( str, "disable" ) )
        return false;
    else if( !::strcasecmp( str, "enabled" ) )
        return true;
    else if( !::strcasecmp( str, "disabled" ) )
        return false;
    else if( 0 != to< int >( str ) )
        return true;
    else
        return false;
}

template<>
int64 String< char >::to< int64 >( const char* str )
{
    int64 v = 0;
    ::sscanf( str, "%"SCNd64, &v );
    return v;
}

template<>
uint64 String< char >::to< uint64 >( const char* str )
{
    uint64 v = 0;
    ::sscanf( str, "%"SCNu64, &v );
    return v;
}

template<>
long double String< char >::to< long double >( const char* str )
{
    long double v = 0.0;
    ::sscanf( str, "%Lf", &v );
    return v;
}

void String< char >::toUpper( const char* source, char* target )
{
    if( target )
    {
        for(; '\0' != *source; ++target, ++source )
            *target = ::toupper( *source );

        *target = '\0';
    }
}

void String< char >::toLower( const char* source, char* target )
{
    if( target )
    {
        for(; '\0' != *source; ++target, ++source )
            *target = ::tolower( *source );

        *target = '\0';
    }
}
