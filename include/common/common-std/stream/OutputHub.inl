/**
 * @file stream/OutputHub.inl
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

/*************************************************************************/
/* common::stream::OutputHub                                             */
/*************************************************************************/
template< typename T >
OutputHub< T >::~OutputHub()
{
    typename std::set< Target* >::iterator cur, end;
    cur = mTargets.begin();
    end = mTargets.end();
    for(; cur != end; ++cur )
        delete *cur;
}

template< typename T >
Error OutputHub< T >::Write( const Element& e )
{
    typename std::set< Target* >::iterator cur, end;
    cur = mTargets.begin();
    end = mTargets.end();
    for(; cur != end; ++cur )
    {
        const Error err = ( *cur )->Write( e );
        if( ERROR_OK != err )
            return err;
    }

    return ERROR_OK;
}

template< typename T >
Error OutputHub< T >::Write( const Element* ep, size_t count, size_t* countWritten )
{
    typename std::set< Target* >::iterator cur, end;
    cur = mTargets.begin();
    end = mTargets.end();
    for(; cur != end; ++cur )
    {
        const Error err = ( *cur )->Write( ep, count, countWritten );
        if( ERROR_OK != err )
            return err;
    }

    return ERROR_OK;
}

template< typename T >
void OutputHub< T >::add( Target* t )
{
    mTargets.insert( t );
}

template< typename T >
void OutputHub< T >::remove( Target* t )
{
    mTargets.remove( t );
}
