// VarType.cpp: implementation of the VarType class.
//
//////////////////////////////////////////////////////////////////////
#ifndef WINVER                  
#define WINVER 0x0501          
#endif

#ifndef _WIN32_WINNT            
#define _WIN32_WINNT 0x0501    
#endif

#include "VarType.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VarType& VarType::operator=(const VarType& obj)
{
	if(this==&obj)
		return *this;

	if(obj.m_ptr)
	{
		if(!m_ptr)
			m_ptr=new MyString;
		*m_ptr=*obj.m_ptr;
	}
	else
	{
		if(m_ptr)
		{
			delete m_ptr;
			m_ptr = NULL;
		}
	}

	m_int		= obj.m_int;
	m_nType		= obj.m_nType;

	return *this;
}

bool VarType::operator==(const VarType& obj) const
{
	if(!IsValid() || !obj.IsValid())
			return false;
	
	if(IsStringType())
	{
		if(obj.IsStringType() && *m_ptr == *obj.m_ptr)
			return true;
	}
	else if(IsIntType())
	{
		if(obj.IsIntType() && m_int == obj.m_int)
			return true;
	}

	return false;
}
