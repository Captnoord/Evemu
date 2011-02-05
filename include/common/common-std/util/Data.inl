/**
 * @file util/Data.inl
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
/* common::util::Data                                                    */
/*************************************************************************/
inline Data::Data()
: mData( NULL ),
  mSize( 0 )
{
}

template< typename T >
Data::Data( T& val )
: mData( reinterpret_cast< uint8* >( &val ) ),
  mSize( sizeof( T ) )
{
}

template< typename Iter >
Data::Data( Iter first, Iter last )
: mData( reinterpret_cast< uint8* >( &*first ) ),
  mSize( ( last - first ) * sizeof( typename std::iterator_traits< Iter >::value_type ) )
{
}

inline Data::Data( const Data& oth )
: mData( oth.mData ),
  mSize( oth.mSize )
{
}

inline Data::~Data()
{
}

/*************************************************************************/
/* common::util::Data::ConstIterator                                     */
/*************************************************************************/
template< typename T >
Data::ConstIterator< T >::ConstIterator( const Data* data, size_t index )
: mData( data ),
  mIndex( index )
{
}

template< typename T >
Data::ConstIterator< T >::ConstIterator( const ConstIterator& oth )
: mData( oth.mData ),
  mIndex( oth.mIndex )
{
}

template< typename T >
Data::ConstIterator< T >&
Data::ConstIterator< T >::operator=( const ConstIterator& oth )
{
    mData = oth.mData;
    mIndex = oth.mIndex;
    return *this;
}

template< typename T >
typename Data::ConstIterator< T >::const_reference
Data::ConstIterator< T >::operator*() const
{
    // make sure we have valid data source
    assert( mData );
    // make sure we're not going off the bounds
    assert( mData->end< value_type >() > *this );

    // obtain the value and return
    return *reinterpret_cast< const_pointer >( &( mData->mData )[ mIndex ] );
}

template< typename T >
Data::ConstIterator< T >
Data::ConstIterator< T >::operator+( difference_type diff ) const
{
    ConstIterator res( *this );
    return res += diff;
}

template< typename T >
Data::ConstIterator< T >&
Data::ConstIterator< T >::operator+=( difference_type diff )
{
    // turn the diff into byte diff
    const difference_type res = diff * sizeof( value_type );

    // make sure we have a valid data source
    assert( mData );
    // make sure we won't go negative
    assert( 0 <= mIndex + res );
    // make sure we won't go past end
    assert( mIndex + res <= mData->size() );

    // set new index
    mIndex += res;

    return *this;
}

template< typename T >
Data::ConstIterator< T >
Data::ConstIterator< T >::operator++( int )
{
    ConstIterator res( *this );
    ++*this;
    return res;
}

template< typename T >
Data::ConstIterator< T >
Data::ConstIterator< T >::operator-( difference_type diff ) const
{
    ConstIterator res( *this );
    return res -= diff;
}

template< typename T >
Data::ConstIterator< T >
Data::ConstIterator< T >::operator--( int )
{
    ConstIterator res( *this );
    --*this;
    return res;
}

template< typename T >
typename Data::ConstIterator< T >::difference_type
Data::ConstIterator< T >::operator-( const ConstIterator& oth ) const
{
    // make sure we have identical data sources
    assert( oth.mData == mData );
    // return difference in element offset
    return static_cast< difference_type >( mIndex - oth.mIndex )
         / static_cast< difference_type >( sizeof( value_type ) );
}

/*************************************************************************/
/* common::util::Data::Iterator                                          */
/*************************************************************************/
template< typename T >
Data::Iterator< T >::Iterator( Data* data, size_t index )
: Base( data, index )
{
}

template< typename T >
Data::Iterator< T >::Iterator( const Iterator& oth )
: Base( oth )
{
}

template< typename T >
Data::Iterator< T >&
Data::Iterator< T >::operator=( const Iterator& oth )
{
    static_cast< Base& >( *this ) = oth;
    return *this;
}

template< typename T >
template< typename T2 >
Data::Iterator< T2 >
Data::Iterator< T >::as() const
{
    return Iterator< T2 >( const_cast< Data* >( Base::mData ), Base::mIndex );
}

template< typename T >
typename Data::Iterator< T >::reference
Data::Iterator< T >::operator*() const
{
    return const_cast< reference >( *static_cast< const Base& >( *this ) );
}

template< typename T >
Data::Iterator< T >
Data::Iterator< T >::operator+( difference_type diff ) const
{
    Iterator res( *this );
    return res += diff;
}

template< typename T >
Data::Iterator< T >&
Data::Iterator< T >::operator+=( difference_type diff )
{
    static_cast< Base& >( *this ) += diff;
    return *this;
}

template< typename T >
Data::Iterator< T >&
Data::Iterator< T >::operator++()
{
    ++static_cast< Base& >( *this );
    return *this;
}

template< typename T >
Data::Iterator< T >
Data::Iterator< T >::operator++( int )
{
    Iterator res( *this );
    ++*this;
    return res;
}

template< typename T >
Data::Iterator< T >
Data::Iterator< T >::operator-( difference_type diff ) const
{
    Iterator res( *this );
    return res -= diff;
}

template< typename T >
Data::Iterator< T >&
Data::Iterator< T >::operator-=( difference_type diff )
{
    static_cast< Base& >( *this ) -= diff;
    return *this;
}

template< typename T >
Data::Iterator< T >&
Data::Iterator< T >::operator--()
{
    --static_cast< Base& >( *this );
    return *this;
}

template< typename T >
Data::Iterator< T >
Data::Iterator< T >::operator--( int )
{
    Iterator res( *this );
    --*this;
    return res;
}

template< typename T >
typename Data::Iterator< T >::difference_type
Data::Iterator< T >::operator-( const Base& oth ) const
{
    return static_cast< const Base& >( *this ) - oth;
}
