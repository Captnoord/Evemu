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

#ifndef __COMMON__UTIL__DATA_H__INCL__
#define __COMMON__UTIL__DATA_H__INCL__

namespace common
{
    namespace util
    {
        /**
         * @brief A field of static data.
         *
         * This class is designed to be as good as <code>void*</code>
         * + <code>size_t</code> with some neat stuff as a bonus.
         *
         * This class does NOT care about memory management by design!
         * Do NOT pass it any dynamically-allocated memory or leaks
         * will occur.
         *
         * @see Class Buffer which supports memory management.
         *
         * @author Bloody.Rabbit
         */
        class Data
        {
        public:
            /**
             * @brief A constant iterator over content of Data.
             *
             * @author Bloody.Rabbit
             */
            template< typename T >
            class ConstIterator
            : public std::iterator< std::random_access_iterator_tag, T >
            {
                /// Typedef for our base due to readibility.
                typedef std::iterator< std::random_access_iterator_tag, T > Base;

            public:
                /// Typedef for iterator category.
                typedef typename Base::iterator_category iterator_category;
                /// Typedef for value type.
                typedef typename Base::value_type        value_type;
                /// Typedef for difference type.
                typedef typename Base::difference_type   difference_type;
                /// Typedef for pointer.
                typedef typename Base::pointer           pointer;
                /// Typedef for reference.
                typedef typename Base::reference         reference;

                /// Typedef for const pointer.
                typedef const T* const_pointer;
                /// Typedef for const reference.
                typedef const T& const_reference;

                /**
                 * @brief A default constructor.
                 *
                 * @param[in] data  A data source.
                 * @param[in] index An index in data.
                 */
                ConstIterator( const Data* data = NULL, size_t index = 0 )
                : mData( data ),
                  mIndex( index )
                {
                }
                /**
                 * @brief A copy constructor.
                 *
                 * @param[in] oth An iterator to copy.
                 */
                ConstIterator( const ConstIterator& oth )
                : mData( oth.mData ),
                  mIndex( oth.mIndex )
                {
                }

                /**
                 * @brief A copy operator.
                 *
                 * @param[in] oth An iterator to copy.
                 *
                 * @return A reference to self.
                 */
                ConstIterator& operator=( const ConstIterator& oth )
                {
                    mData = oth.mData;
                    mIndex = oth.mIndex;
                    return *this;
                }

                /**
                 * @brief Converts ConstIterator to another ConstIterator
                 *        with different type.
                 *
                 * @return The new ConstIterator.
                 */
                template< typename T2 >
                ConstIterator< T2 > as() const { return ConstIterator< T2 >( mData, mIndex ); }

                /**
                 * @brief A deference operator.
                 *
                 * @return A reference to the value in the data.
                 */
                const_reference operator*() const
                {
                    // make sure we have valid data source
                    assert( mData );
                    // make sure we're not going off the bounds
                    assert( mData->end< value_type >() > *this );

                    // obtain the value and return
                    return *reinterpret_cast< const_pointer >( &( mData->mData )[ mIndex ] );
                }
                /**
                 * @brief A dereference operator.
                 *
                 * @return A pointer to the value in the data.
                 */
                const_pointer operator->() const { return &**this; }
                /**
                 * @brief A subscript operator.
                 *
                 * @param[in] diff An index to be added to this
                 *                 iterator before dereferencing.
                 *
                 * @return A reference to the value in the data.
                 */
                const_reference operator[]( difference_type diff ) const { return *( *this + diff ); }

                /**
                 * @brief A sum operator.
                 *
                 * @param[in] diff An index to be added to this iterator.
                 *
                 * @return An iterator with the added index.
                 */
                ConstIterator operator+( difference_type diff ) const
                {
                    ConstIterator res( *this );
                    return res += diff;
                }
                /**
                 * @brief An add operator.
                 *
                 * @param[in] diff An index to be added to this iterator.
                 *
                 * @return A reference to self with the added index.
                 */
                ConstIterator& operator+=( difference_type diff )
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
                /**
                 * @brief A preincrement operator.
                 *
                 * @return A reference to self.
                 */
                ConstIterator& operator++() { return *this += 1;  }
                /**
                 * @brief A postincrement operator.
                 *
                 * @return An incremented iterator.
                 */
                ConstIterator operator++( int )
                {
                    ConstIterator res( *this );
                    ++*this;
                    return res;
                }

                /**
                 * @brief A diff interator.
                 *
                 * @param[in] diff An index to substract from this iterator.
                 *
                 * @return A copy of self with the substracted index.
                 */
                ConstIterator operator-( difference_type diff ) const
                {
                    ConstIterator res( *this );
                    return res -= diff;
                }
                /**
                 * @brief A substract operator.
                 *
                 * @param[in] diff An index to substract from this iterator.
                 *
                 * @return A reference to self with the substracted index.
                 */
                ConstIterator& operator-=( difference_type diff ) { return *this += -diff; }
                /**
                 * @brief A predecrement operator.
                 *
                 * @return A reference to self with a decremented index.
                 */
                ConstIterator& operator--() { return *this -= 1; }
                /**
                 * @brief A postdecrement operator.
                 *
                 * @return A copy of self with a decremented index.
                 */
                ConstIterator operator--( int )
                {
                    ConstIterator res( *this );
                    --*this;
                    return res;
                }

                /**
                 * @brief Obtains a difference between two iterators.
                 *
                 * @param[in] oth The other iterator in comparison.
                 *
                 * @return The difference.
                 */
                difference_type operator-( const ConstIterator& oth ) const
                {
                    // make sure we have identical data sources
                    assert( oth.mData == mData );
                    // return difference in element offset
                    return static_cast< difference_type >( mIndex - oth.mIndex )
                         / static_cast< difference_type >( sizeof( value_type ) );
                }

                /**
                 * @brief An equality operator.
                 *
                 * @param[in] oth The other iterator in comparison.
                 *
                 * @retval true  The iterators are equal.
                 * @retval false The iterators are not equal.
                 */
                bool operator==( const ConstIterator& oth ) const { return 0 == ( *this - oth ); }
                /**
                 * @brief An inequality operator.
                 *
                 * @param[in] oth The other iterator in comparison.
                 *
                 * @return true  The iterators are not equal.
                 * @return false The iterators are equal.
                 */
                bool operator!=( const ConstIterator& oth ) const { return  0 != ( *this - oth ); }

                /**
                 * @brief A less-than operator.
                 *
                 * @return true  <var>this</var> is less than <var>oth</var>.
                 * @return false <var>this</var> is greater or equal to <var>oth</var>.
                 */
                bool operator<( const ConstIterator& oth ) const { return 0 > ( *this - oth ); }
                /**
                 * @brief A greater-than operator.
                 *
                 * @param[in] oth The other iterator in comparison.
                 *
                 * @return true  <var>this</var> is greater than <var>oth</var>.
                 * @return false <var>this</var> is less or equal to <var>oth</var>.
                 */
                bool operator>( const ConstIterator& oth ) const { return 0 < ( *this - oth ); }
                /**
                 * @brief A less-or-equal operator.
                 *
                 * @param[in] oth The other iterator in comparison.
                 *
                 * @return true  <var>this</var> is less or equal to <var>oth</var>.
                 * @return false <var>this</var> is greater than <var>oth</var>.
                 */
                bool operator<=( const ConstIterator& oth ) const { return 0 >= ( *this - oth ); }
                /**
                 * @brief A greater-or-equal operator.
                 *
                 * @param[in] oth The other iterator in comparison.
                 *
                 * @return true  <var>this</var> is greater or equal to <var>oth</var>.
                 * @return false <var>this</var> is less than <var>oth</var>.
                 */
                bool operator>=( const ConstIterator& oth ) const { return 0 <= ( *this - oth ); }

            protected:
                /// The data source.
                const Data* mData;
                /// Index in the data, in bytes.
                size_t mIndex;
            };

            /**
             * @brief An iterator over content of Data.
             *
             * @author Bloody.Rabbit
             */
            template< typename T >
            class Iterator
            : public ConstIterator< T >
            {
                /// Typedef for our base due to readibility.
                typedef ConstIterator< T > Base;

            public:
                /// Typedef for iterator category.
                typedef typename Base::iterator_category iterator_category;
                /// Typedef for value type.
                typedef typename Base::value_type        value_type;
                /// Typedef for difference type.
                typedef typename Base::difference_type   difference_type;
                /// Typedef for pointer.
                typedef typename Base::pointer           pointer;
                /// Typedef for const pointer.
                typedef typename Base::const_pointer     const_pointer;
                /// Typedef for reference.
                typedef typename Base::reference         reference;
                /// Typedef for const reference.
                typedef typename Base::const_reference   const_reference;

                /**
                 * @brief A default constructor.
                 *
                 * @param[in] data  A data source.
                 * @param[in] index An index in the data.
                 */
                Iterator( Data* data = NULL, size_t index = 0 )
                : Base( data, index )
                {
                }
                /**
                 * @brief A copy constructor.
                 *
                 * @param[in] oth The iterator to copy.
                 */
                Iterator( const Iterator& oth )
                : Base( oth )
                {
                }

                /**
                 * @brief A copy operator.
                 *
                 * @param[in] oth The iterator to copy.
                 *
                 * @return A reference to self.
                 */
                Iterator& operator=( const Iterator& oth ) { static_cast< Base& >( *this ) = oth; return *this; }

                /**
                 * @brief Converts Iterator to another Iterator
                 *        with different type.
                 *
                 * @return The new Iterator.
                 */
                template< typename T2 >
                Iterator< T2 > as() const { return Iterator< T2 >( const_cast< Data* >( Base::mData ), Base::mIndex ); }

                /**
                 * @brief A dereference operator.
                 *
                 * @return A reference to the value within the data.
                 */
                reference operator*() const { return const_cast< reference >( *static_cast< const Base& >( *this ) ); }
                /**
                 * @brief A dereference operator.
                 *
                 * @return A pointer to the value within the data.
                 */
                pointer operator->() const { return &**this; }
                /**
                 * @brief A subscript operator.
                 *
                 * @param[in] diff An index to be added to <var>this</var>
                 *                 before dereferencing.
                 *
                 * @return A reference to the value within the data.
                 */
                reference operator[]( difference_type diff ) const { return *( *this + diff ); }

                /**
                 * @brief A sum operator.
                 *
                 * @param[in] diff An index to be added to <var>this</var>.
                 *
                 * @return An iterator with the added index.
                 */
                Iterator operator+( difference_type diff ) const
                {
                    Iterator res( *this );
                    return res += diff;
                }
                /**
                 * @brief An add operator.
                 *
                 * @param[in] diff An index to be added to <var>this</var>.
                 *
                 * @return A reference to self with the added index.
                 */
                Iterator& operator+=( difference_type diff ) { static_cast< Base& >( *this ) += diff; return *this; }
                /**
                 * @brief A preincrement operator.
                 *
                 * @return A reference to self with the incremented index.
                 */
                Iterator& operator++() { ++static_cast< Base& >( *this ); return *this; }
                /**
                 * @brief A postincrement operator.
                 *
                 * @return A copy of self with the incremented index.
                 */
                Iterator operator++( int )
                {
                    Iterator res( *this );
                    ++*this;
                    return res;
                }

                /**
                 * @brief A diff operator.
                 *
                 * @param[in] diff An index to substract from <var>this</var>.
                 *
                 * @return A copy of self with the substracted index.
                 */
                Iterator operator-( difference_type diff ) const
                {
                    Iterator res( *this );
                    return res -= diff;
                }
                /**
                 * @brief A substract operator.
                 *
                 * @param[in] diff An index to substract from <var>this</var>.
                 *
                 * @return A reference to self with the substracted index.
                 */
                Iterator& operator-=( difference_type diff ) { static_cast< Base& >( *this ) -= diff; return *this; }
                /**
                 * @brief A predecrement operator.
                 *
                 * @return A reference to self with the decremented index.
                 */
                Iterator& operator--() { --static_cast< Base& >( *this ); return *this; }
                /**
                 * @brief A postdecrement operator.
                 *
                 * @return A copy of self with the decremented index.
                 */
                Iterator operator--( int )
                {
                    Iterator res( *this );
                    --*this;
                    return res;
                }

                /**
                 * @brief Obtains a difference between two iterators.
                 *
                 * @param[in] oth The other iterator.
                 *
                 * @return The difference.
                 */
                difference_type operator-( const Base& oth ) const { return static_cast< const Base& >( *this ) - oth; }
            };

            /**
             * @brief Creates an empty data source.
             */
            Data()
            : mData( NULL ),
              mSize( 0 )
            {
            }
            /**
             * @brief Creates a data source with some given content.
             *
             * The content is given as a single variable.
             *
             * @param[in] val The content.
             */
            template< typename T >
            Data( T& val )
            : mData( reinterpret_cast< uint8* >( &val ) ),
              mSize( sizeof( T ) )
            {
            }
            /**
             * @brief Creates a data source with some given content.
             *
             * The content is determined by iterators <var>first</var>
             * and <var>last</var>.
             *
             * @param[in] first Iterator pointing to first element.
             * @param[in] last  Iterator pointing to element after the last one.
             */
            template< typename Iter >
            Data( Iter first, Iter last )
            : mData( reinterpret_cast< uint8* >( &*first ) ),
              mSize( ( last - first ) * sizeof( typename std::iterator_traits< Iter >::value_type ) )
            {
            }
            /**
             * @brief A copy constructor.
             *
             * @param[in] oth The data to copy.
             */
            Data( const Data& oth )
            : mData( oth.mData ),
              mSize( oth.mSize )
            {
            }
            /**
             * @brief A destructor.
             *
             * Does nothing (no release of memory).
             */
            virtual ~Data()
            {
            }

            /********************************************************************/
            /* Read methods                                                     */
            /********************************************************************/
            /**
             * @brief Obtains a current size of the data.
             *
             * @return Current size, in bytes.
             */
            size_t size() const { return mSize; }

            /**
             * @brief Obtains an iterator to begin.
             *
             * @return An iterator to begin.
             */
            template< typename T >
            Iterator< T > begin() { return Iterator< T >( this, 0 ); }
            /**
             * @brief Obtains a constant iterator to begin.
             *
             * @return A constant iterator to begin.
             */
            template< typename T >
            ConstIterator< T > begin() const { return ConstIterator< T >( this, 0 ); }
            /**
             * @brief Obtains an iterator to end.
             *
             * @return An iterator to end.
             */
            template< typename T >
            Iterator< T > end() { return Iterator< T >( this, size() ); }
            /**
             * @brief Obtains a constant iterator to end.
             *
             * @return A constant iterator to end.
             */
            template< typename T >
            ConstIterator< T > end() const { return ConstIterator< T >( this, size() ); }

            /**
             * @brief Gets element from the data.
             *
             * @param[in] index Index of element in the data.
             *
             * @return Reference to element.
             */
            template< typename T >
            T& Get( size_t index ) { return *( begin< T >() + index ); }
            /**
             * @brief Gets const element from the data.
             *
             * @param[in] index Index of element in the data.
             *
             * @return Const reference to element.
             */
            template< typename T >
            const T& Get( size_t index ) const { return *( begin< T >() + index ); }

            /**
             * @brief Overload of access operator[].
             *
             * @param[in] index Index of byte to be returned.
             *
             * @return Reference to required byte.
             */
            uint8& operator[]( size_t index ) { return Get< uint8 >( index ); }
            /**
             * @brief Overload of const access operator[].
             *
             * @param[in] index Index of byte to be returned.
             *
             * @return Const reference to required byte.
             */
            const uint8& operator[]( size_t index ) const { return Get< uint8 >( index ); }

        protected:
            /// The data; type <code>uint8*</code> to allow convenient access by byte-index.
            uint8* mData;
            /// The amount of valid data at <var>mData</var>, in bytes.
            size_t mSize;
        };
    }
}

#endif /* !__COMMON__UTIL__DATA_H__INCL__ */
