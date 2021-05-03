#ifndef UTILITIES_H
#define UTILITIES_H

#include <Arduino.h>

inline String gen_random(const int len) {

	String tmp_s;
	static const char alphanum[] =
        "(/\\)";
		/* "0123456789" */
		/* "ABCDEFGHIJKLMNOPQRSTUVWXYZ" */
		/* "abcdefghijklmnopqrstuvwxyz"; */

	for (auto i = 0; i < len; ++i) 
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];


	return tmp_s;
}

#endif
