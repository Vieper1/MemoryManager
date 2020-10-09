#include <assert.h>
#include <string.h>

namespace Engine
{
inline HashedString::HashedString( ) :
	m_Hash( Hash( "" ) )
{
}

inline HashedString::HashedString( const char * i_string ) :
	m_Hash( Hash( i_string ) )
#ifdef DEBUG_KEEP_STRING
	, m_pString( strdup( i_string ) )
#endif
{
}

inline HashedString::HashedString( const HashedString & i_other ) :
m_Hash( i_other.m_Hash )
#ifdef DEBUG_KEEP_STRING
	, m_pString( strdup( i_other.m_pString ) )
#endif
{
}

inline HashedString::~HashedString()
{
#ifdef DEBUG_KEEP_STRING
	if( m_pString )
		free( m_pString )
#endif
	
}

inline HashedString & HashedString::operator=( const HashedString & i_other )
{
	m_Hash = i_other.m_Hash;

#ifdef DEBUG_KEEP_STRING
	if( m_pString )
		free( m_pString )
	
	m_pString = i_other.m_pString;
#endif

	return *this;
}

inline unsigned int HashedString::Get( void ) const
{
	return m_Hash;
}

inline bool HashedString::operator==( const HashedString & i_other ) const
{
	return m_Hash == i_other.m_Hash;
}

inline bool HashedString::operator<( const HashedString & i_other ) const
{
	return m_Hash < i_other.m_Hash;
}
} // namespace Engine
