/***********************************************************************
TextEntryMethod - Abstract base class for helper objects generating text
and text control events.
Copyright (c) 2019 Oliver Kreylos

This file is part of the GLMotif Widget Library (GLMotif).

The GLMotif Widget Library is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GLMotif Widget Library is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along
with the GLMotif Widget Library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
***********************************************************************/

#ifndef GLMOTIF_TEXTENTRYMETHOD_INCLUDED
#define GLMOTIF_TEXTENTRYMETHOD_INCLUDED

#include <GLMotif/WidgetManager.h>

namespace GLMotif {

class TextEntryMethod
	{
	/* Constructors and destructors: */
	public:
	virtual ~TextEntryMethod(void); // Destroys the text event source
	
	/* Methods: */
	virtual void requestNumericEntry(const WidgetManager::Transformation& transform,Widget* widget) =0; // Requests numeric entry for the given widget with the given widget transformation
	virtual void requestAlphaNumericEntry(const WidgetManager::Transformation& transform,Widget* widget) =0; // Requests alphanumeric entry for the given widget with the given widget transformation
	virtual void entryFinished(void) =0; // Notifies the text entry method that the current text entry sequence is complete
	};

}

#endif
