#pragma once

#include <core/types.hpp>

// TODO: Windows error conflict strlen and strdup
//#include <vendor/libc/stdio.hpp>
//#include <vendor/libc/string.hpp>
#include <stdio.h>
#include <string.h>

template< typename T>
class CoreString
{
private:

    T m_data;
	usize m_size;

public:

    CoreString( const char* cstr ) { init( cstr ); }
    CoreString( CoreString& string ) { init( string ); }

    T data() const
    {
        return m_data;
    }

    usize size() const
    {
        return m_size;
    }


private:

    void init( T string  )
    {
        m_data = string;
        m_size = strlen( string );
    }



};
