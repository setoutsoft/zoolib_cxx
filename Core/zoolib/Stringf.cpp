/* -------------------------------------------------------------------------------------------------
Copyright (c) 2012 Andrew Green
http://www.zoolib.org

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------------------------- */

#include "zoolib/Stringf.h"

#if ZCONFIG(Compiler, MSVC)
	#define vsnprintf _vsnprintf
#else
	#include <stdio.h> // for vsnprintf
#endif

#ifndef va_copy
	#if defined __va_copy
		#define va_copy __va_copy
	#else
		#define va_copy(dest, src) dest = src
	#endif
#endif

namespace ZooLib {

// =================================================================================================

std::string sStringf(std::string iString, ...)
	{
	va_list args;
	va_start(args, iString);
	const std::string result = sStringv(iString.c_str(), args);
	va_end(args);

	return result;
	}

std::string sStringf(const char* iString, ...)
	{
	va_list args;
	va_start(args, iString);
	const std::string result = sStringv(iString, args);
	va_end(args);

	return result;
	}

std::string sStringv(const char* iString, std::va_list iArgs)
	{
	std::string result(512, ' ');
	while (true)
		{
		va_list localArgs;
		va_copy(localArgs, iArgs);

		int count = vsnprintf(const_cast<char*>(result.data()), result.size(), iString, localArgs);

		va_end(localArgs);

		if (count < 0)
			{
			// Handle -ve result from glibc prior to version 2.1 by growing the string.
			result.resize(result.size() * 2);
			}
		else if (size_t(count) > result.size())
			{
			// Handle C99 standard, where count indicates how much space would have been needed.
			result.resize(count);
			}
		else
			{
			// The string fitted
			result.resize(count);
			return result;
			}
		}
	}

} // namespace ZooLib
