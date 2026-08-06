#pragma once

#include <core/types.hpp>
#include <core/memory.hpp>


///////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class List
{
	public:

		void invert_data( isize capacity_bytes )
		{
			void* new_data = malloc( capacity_bytes );

			memcpy( new_data, data, capacity );
			free( data );

			data = new_data;
			capacity = capacity_bytes;

		}

		void append( T value )
		{
			if ( capacity < ( size() + 1 ) * sizeof( T ) )
			{
				isize new_capacity = capacity * 2;
				void* new_data = malloc( new_capacity );

				memcpy( new_data, data, capacity );
				free( data );

				data = new_data;
				capacity = new_capacity;
			}

			T* values = static_cast<T*>( data );
			values[ size() ] = value;
		}

		NO_DISCARD T get( u64 index ) const
		{
			if ( index >= size() ) return T{};

			T* values = static_cast<T*>( data );
			return values[ index ];
		}

		NO_DISCARD u64 size() const
		{
			return capacity / sizeof( T );
		}

		void clear()
		{
			if ( data ) free( data );
			data = nullptr;
			capacity = 0;
			memset( this, 0, sizeof( List ) );
		}

		class Iterator
		{
			public:

				Iterator( const List* list, u64 index )
					: list( list )
					, index( index )
				{}

				Iterator& operator++()
				{
					index++;
					return *this;
				}

				bool operator!=( const Iterator& other ) const
				{
					return index != other.index;
				}

				T operator*() const
				{
					return list->get( index );
				}

			private:

				const List* list;
				u64 index;
		};

	private:

		void* data;
		isize capacity;

		void init( isize capacity_bytes )
		{
			capacity = capacity_bytes;
			data = malloc( capacity );
		}
};

///////////////////////////////////////////////////////////////////////////////////////////////////