#include <assert.h>

#include "HashedString.h"

namespace Engine
{

unsigned int HashedString::Hash( const char * i_string )
{
	assert( i_string );

	return Hash( reinterpret_cast<void *>( const_cast<char *>( i_string ) ), strlen( i_string ) );
}

unsigned int HashedString::Hash( const void * i_bytes, size_t i_bytecount )
{
	// FNV hash, http://isthe.com/chongo/tech/comp/fnv/

	register const unsigned char * p = static_cast<const unsigned char *>( i_bytes );
	unsigned int hash = 2166136261;

	for( size_t i = 0; i < i_bytecount; ++i )
		hash = 16777619 * (hash ^ p[i]);

	return hash ^ (hash >> 16);
} ;

} // namespace Engine