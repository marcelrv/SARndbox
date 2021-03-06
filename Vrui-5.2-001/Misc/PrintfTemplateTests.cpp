/***********************************************************************
PrintfTemplateTests - Helper functions to test the well-formedness of
strings to be used as templates for printf functions.
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

#include <Misc/PrintfTemplateTests.h>

namespace Misc {

namespace {

/**************
Helper classes:
**************/

class TemplateChecker // Helper class to run a template checking state machine
	{
	/* Embedded classes: */
	private:
	enum State // Enumerated type for template checking machine states
		{
		Start,Percent,Width,Precision,Error
		};
	
	/* Elements: */
	char conversionType; // Type of conversion to look for
	unsigned int maxLength; // Maximum allowed length of resulting strings, NUL included
	bool haveConversion; // Flag if a valid conversion of the given type was found
	unsigned int conversionStart; // Index of first character of the found conversion
	unsigned int conversionLength; // Length of conversion in template string
	unsigned int conversionWidth; // Width for the found conversion
	unsigned int conversionPrecision; // Precision for the found conversion
	unsigned int length; // Number of non-conversion characters generated by the template
	State state; // Machine's current state
	
	/* Constructors and destructors: */
	public:
	TemplateChecker(char sConversionType,unsigned int sMaxLength) // Creates a template checker for the given conversion type and maximum string length
		:conversionType(sConversionType),maxLength(sMaxLength),
		 haveConversion(false),conversionStart(0),conversionLength(0),
		 conversionWidth(0),conversionPrecision(0),length(0),
		 state(Start)
		{
		}
	
	/* Methods: */
	void operator()(char c) // Advances the state machine by processing the given character
		{
		switch(state)
			{
			case Start:
				if(c=='%')
					{
					conversionLength=1;
					state=Percent;
					}
				else
					++length;
				break;
			
			case Percent:
				++conversionLength;
				if(c=='%')
					{
					++length;
					state=Start;
					}
				else if(c>='0'&&c<='9')
					{
					conversionWidth=(unsigned int)(c-'0');
					state=Width;
					}
				else if(c=='.')
					state=Precision;
				else if(c==conversionType)
					{
					if(haveConversion)
						state=Error;
					else
						{
						haveConversion=true;
						conversionStart=length;
						state=Start;
						}
					}
				else
					state=Error;
				break;
			
			case Width:
				++conversionLength;
				if(c>='0'&&c<='9')
					{
					conversionWidth=conversionWidth*10+(unsigned int)(c-'0');
					if(conversionWidth>=maxLength)
						state=Error;
					}
				else if(c=='.')
					state=Precision;
				else if(c==conversionType)
					{
					if(haveConversion)
						state=Error;
					else
						{
						haveConversion=true;
						conversionStart=length;
						state=Start;
						}
					}
				else
					state=Error;
				break;
			
			case Precision:
				++conversionLength;
				if(c>='0'&&c<='9')
					{
					conversionPrecision=conversionPrecision*10+(unsigned int)(c-'0');
					if(conversionPrecision>=maxLength)
						state=Error;
					}
				else if(c==conversionType)
					{
					if(haveConversion)
						state=Error;
					else
						{
						haveConversion=true;
						conversionStart=length;
						state=Start;
						}
					}
				else
					state=Error;
				break;
			
			case Error:
				break;
			}
		}
	unsigned int getConversionStart(void) const // Returns the starting character index of the found conversion
		{
		return conversionStart;
		}
	unsigned int getConversionLength(void) const // Returns the length of the found conversion
		{
		return conversionLength;
		}
	unsigned int getMaxReplacementLength(void) const // Returns the maximum length of string created by the found conversion
		{
		/* Initialize the replacement length based on the maximum unpadded length for the given conversion type: */
		unsigned int maxReplacementLength=0;
		switch(conversionType)
			{
			case 'o':
				maxReplacementLength=11; // Widest unsigned int in octal is 37777777777
			
			case 'u':
				maxReplacementLength=10; // Widest unsigned int in decimal is 4294967295
				break;
			
			case 'x':
			case 'X':
				maxReplacementLength=8; // Widest unsigned int in hexadecimal is ffffffff
				break;
			
			case 'd':
			case 'i':
				maxReplacementLength=11; // Widest int is -2147483648
				break;
			}
		
		/* Adjust for conversion width and precision, which are basically the same for integer types: */
		if(maxReplacementLength<conversionWidth)
			maxReplacementLength=conversionWidth;
		if(maxReplacementLength<conversionPrecision)
			maxReplacementLength=conversionPrecision;
		
		return maxReplacementLength;
		}
	bool isValid(void) const // Returns true if the parsed template was valid and does not exceed the given maximum length
		{
		/* Template was invalid if it has an unfinished conversion, and error, or not exactly one conversion of the requested type: */
		if(state!=Start||!haveConversion)
			return false;
		
		/* Check that the maximum total string length (verbatim length + maximum conversion length) is smaller than the maximum: */
		return length+getMaxReplacementLength()<maxLength;
		}
	};

}

bool isValidTemplate(const char* templateString,char conversion,unsigned int maxLength,unsigned int* conversionStart,unsigned int* conversionLength,unsigned int* maxReplacementLength)
	{
	/* Create a template checker: */
	TemplateChecker tc(conversion,maxLength);
	
	/* Process the template string: */
	for(const char* tPtr=templateString;*tPtr!='\0';++tPtr)
		tc(*tPtr);
	
	/* Return the parsing result: */
	if(conversionStart!=0)
		*conversionStart=tc.getConversionStart();
	if(conversionLength!=0)
		*conversionLength=tc.getConversionLength();
	if(maxReplacementLength!=0)
		*maxReplacementLength=tc.getMaxReplacementLength();
	return tc.isValid();
	}

bool isValidTemplate(const std::string& templateString,char conversion,unsigned int maxLength,unsigned int* conversionStart,unsigned int* conversionLength,unsigned int* maxReplacementLength)
	{
	/* Create a template checker: */
	TemplateChecker tc(conversion,maxLength);
	
	/* Process the template string: */
	for(std::string::const_iterator tIt=templateString.begin();tIt!=templateString.end();++tIt)
		tc(*tIt);
	
	/* Return the parsing result: */
	if(conversionStart!=0)
		*conversionStart=tc.getConversionStart();
	if(conversionLength!=0)
		*conversionLength=tc.getConversionLength();
	if(maxReplacementLength!=0)
		*maxReplacementLength=tc.getMaxReplacementLength();
	return tc.isValid();
	}

}
