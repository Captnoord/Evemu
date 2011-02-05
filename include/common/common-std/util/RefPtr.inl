/**
 * @file util/RefPtr.inl
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
/* common::util::RefObject                                               */
/*************************************************************************/
inline RefObject::RefObject( size_t refCount )
: mRefCount( refCount )
{
}

inline RefObject::~RefObject()
{
    assert( 0 == mRefCount );
}

inline void RefObject::IncRef() const
{
    ++mRefCount;
}

inline void RefObject::DecRef() const
{
    assert( 0 < mRefCount );
    --mRefCount;

    if( 0 == mRefCount )
        delete this;
}

/*************************************************************************/
/* common::util::RefPtr                                                  */
/*************************************************************************/
template< typename T >
template< typename T2 >
RefPtr< T > RefPtr< T >::StaticCast( const RefPtr< T2 >& oth )
{
    return RefPtr( static_cast< T* >( oth.get() ) );
}

template< typename T >
RefPtr< T >::RefPtr( T* p )
: mPtr( p )
{
    if( *this )
        (*this)->IncRef();
}

template< typename T >
RefPtr< T >::RefPtr( const RefPtr& oth )
: mPtr( oth.get() )
{
    if( *this )
        (*this)->IncRef();
}

template< typename T >
template< typename T2 >
RefPtr< T >::RefPtr( const RefPtr< T2 >& oth )
: mPtr( oth.get() )
{
    if( *this )
        (*this)->IncRef();
}

template< typename T >
RefPtr< T >::~RefPtr()
{
    if( *this )
        (*this)->DecRef();
}

template< typename T >
RefPtr< T >& RefPtr< T >::operator=( const RefPtr& oth )
{
    if( *this )
        (*this)->DecRef();

    mPtr = oth.get();

    if( *this )
        (*this)->IncRef();

    return *this;
}

template< typename T >
template< typename T2 >
RefPtr< T >& RefPtr< T >::operator=( const RefPtr< T2 >& oth )
{
    if( *this )
        (*this)->DecRef();

    mPtr = oth.get();

    if( *this )
        (*this)->IncRef();

    return *this;
}
