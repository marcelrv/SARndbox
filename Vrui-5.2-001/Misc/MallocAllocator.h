/***********************************************************************
MallocAllocator - Class to allocate memory using the C standard malloc/
free functions.
Copyright (c) 2018 Oliver Kreylos

This file is part of the Miscellaneous Support Library (Misc).

The Miscellaneous Support Library is free software; you can
redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

The Miscellaneous Support Library is distributed in the hope that it
will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with the Miscellaneous Support Library; if not, write to the Free
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA
***********************************************************************/

#ifndef MISC_MALLOCALLOCATOR_INCLUDED
#define MISC_MALLOCALLOCATOR_INCLUDED

#include <stdlib.h>

namespace Misc {

template <class ContentParam>
class MallocAllocator
	{
	/* Methods: */
	public:
	static void* allocate(void)
		{
		return malloc(sizeof(ContentParam));
		}
	void free(void* item)
		{
		::free(item);
		}
	void destroy(ContentParam* item)
		{
		if(item!=0)
			{
			item->~ContentParam();
			::free(item);
			}
		}
	};

}

#endif
