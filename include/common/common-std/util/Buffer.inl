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

/*************************************************************************/
/* common::util::Buffer                                                  */
/*************************************************************************/
inline Buffer::Buffer( size_t len, const uint8& fill )
: mCapacity( 0 )
{
    // Resize the buffer
    Resize< uint8 >( len, fill );
}

template< typename Iter >
Buffer::Buffer( Iter first, Iter last )
: mCapacity( 0 )
{
    // Assign the content
    Assign( first, last );
}

inline Buffer::Buffer( const Buffer& oth )
: mCapacity( 0 )
{
    // Assign the content
    *this = oth;
}

inline Buffer::~Buffer()
{
    // Release the buffer
    util::safeFree( mData );
}

template< typename T >
void Buffer::Append( const T& value )
{
    // we wish to append to the end
    const Iterator< T > index = end< T >();

    // make enough room; we're going to fill the gap immediately
    _ResizeAt< T >( index, 1 );

    // assign the value, filling the gap
    *index = value;
}

template< typename Iter >
void Buffer::Append( Iter first, Iter last )
{
    // for readability
    typedef typename std::iterator_traits< Iter >::value_type T;

    // we wish to append to the end
    const Iterator< T > index = end< T >();

    // make enough room; we're going to fill the gap immediately
    _ResizeAt< T >( index, last - first );

    // assign the value, filling the gap
    std::copy( first, last, index );
}

template< typename T >
void Buffer::Assign( const T& value )
{
    // we wish to assign to beginning
    const Iterator< T > index = begin< T >();

    // do we have enough space?
    if( end< T >() > index )
    {
        // yes, we do: assign the value
        *index = value;

        // shrink the buffer; no gap will be created
        _ResizeAt< T >( index, 1 );
    }
    else
    {
        // no, we don't: make enough room; we're going to fill the gap immediately
        _ResizeAt< T >( index, 1 );

        // assign the value, filling the gap
        *index = value;
    }
}

template< typename Iter >
void Buffer::Assign( Iter first, Iter last )
{
    // for readability
    typedef typename std::iterator_traits< Iter >::value_type T;

    // we wish to assign to beginning
    const Iterator< T > index = begin< T >();

    // do we have enough space?
    if( last - first <= end< T >() - index )
    {
        // yes, we do: assign the value
        std::copy( first, last, index );

        // shrink the buffer; no gap will be created
        _ResizeAt< T >( index, last - first );
    }
    else
    {
        // no, we don't: make enough room; we're going to fill the gap immediately
        _ResizeAt< T >( index, last - first );

        // assign the value, filling the gap
        std::copy( first, last, index );
    }
}

template< typename T >
Buffer& Buffer::operator<<( const T& value )
{
    // append the value
    Append< T >( value );
    // return ourselves
    return *this;
}

template< typename T >
Buffer& Buffer::operator=( const T& value )
{
    // assign the value
    Assign< T >( value );
    // return ourselves
    return *this;
}

inline Buffer& Buffer::operator=( const Buffer& value )
{
    // assign new content
    Assign< ConstIterator< uint8 > >( value.begin< uint8 >(), value.end< uint8 >() );
    // return ourselves
    return *this;
}

template< typename T >
void Buffer::Reserve( size_t requiredCount )
{
    // reserve at beginning
    ReserveAt< T >( begin< T >(), requiredCount );
}

template< typename T >
void Buffer::ReserveAt( ConstIterator< T > index, size_t requiredCount )
{
    // make sure we're not going off the bounds
    assert( index <= end< T >() );

    // turn Iterator into byte offset
    const size_t _index = index.template as< uint8 >() - begin< uint8 >();
    // obtain required size in bytes
    const size_t _requiredSize = requiredCount * sizeof( T );

    // reallocate if necessary
    if( _index + _requiredSize > capacity() )
        _Reallocate( _index + _requiredSize );
}

template< typename T >
void Buffer::Resize( size_t requiredCount, const T& fill )
{
    // Resize at beginning
    ResizeAt< T >( begin< T >(), requiredCount, fill );
}

template< typename T >
void Buffer::ResizeAt( ConstIterator< T > index, size_t requiredCount,
                       const T& fill )
{
    // make sure we're not going off the bounds
    assert( index <= end< T >() );

    // keep old size
    const size_t _oldSize = size();
    // do actual resize
    _ResizeAt< T >( index, requiredCount );

    // turn Iterator into byte offset
    const size_t _index = index.template as< uint8 >() - begin< uint8 >();
    // obtain required size in bytes
    const size_t _requiredSize = requiredCount * sizeof( T );

    // has a gap been created?
    if( _index + _requiredSize > _oldSize )
        // fill it with the fill
        std::fill_n( reinterpret_cast< T* >( &mData[ _oldSize ] ),
                     _index + _requiredSize - _oldSize,
                     fill );
}

template< typename T >
void Buffer::_Resize( size_t requiredCount )
{
    // resize at beginning
    _ResizeAt< T >( begin< T >(), requiredCount );
}

template< typename T >
void Buffer::_ResizeAt( ConstIterator< T > index, size_t requiredCount )
{
    // make sure we're not going off the bounds
    assert( index <= end< T >() );

    // turn index into byte offset
    const size_t _index = index.template as< uint8 >() - begin< uint8 >();
    // obtain required size in bytes
    const size_t _requiredSize = requiredCount * sizeof( T );

    // reallocate
    _Reallocate( _index + _requiredSize );
    // set new size
    mSize = _index + _requiredSize;
}

inline void Buffer::_Reallocate( size_t requiredSize )
{
    // calculate new capacity for required size
    size_t newCapacity = _CalcBufferCapacity( capacity(), requiredSize );
    // make sure new capacity is bigger than required size
    assert( requiredSize <= newCapacity );

    // has the capacity changed?
    if( newCapacity != capacity() )
    {
        // reallocate
        mData = reinterpret_cast< uint8* >( ::realloc( mData, newCapacity ) );
        // set new capacity
        mCapacity = newCapacity;
    }
}

inline size_t Buffer::_CalcBufferCapacity( size_t currentCapacity,
                                           size_t requiredSize )
{
    size_t newCapacity = 0;

    // if more than 0x100 bytes required, return next power of 2
    if( 0x100 < requiredSize )
        newCapacity = (size_t)npowof2( requiredSize );
    // else if non-zero, return 0x100 bytes
    else if( 0 < requiredSize )
        newCapacity = 0x100;
    // else return 0 bytes

    /* if current capacity is sufficient and at the same time smaller
       than the new capacity, return current one ... saves resources */
    if( requiredSize <= currentCapacity && currentCapacity < newCapacity )
        return currentCapacity;
    else
        return newCapacity;
}
