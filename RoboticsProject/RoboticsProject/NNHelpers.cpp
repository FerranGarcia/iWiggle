#include "stdafx.h"
#include "NNHelpers.h"
#include <cstdlib>

NNHelpers::NNHelpers()
{
}


NNHelpers::~NNHelpers()
{
}

// function for getting random string
string* NNHelpers::getRandomString(int len) {
	string *s = new string();
	s->resize(len);

	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (string::iterator it = s->begin(); it != s->end(); ++it) {
		*it = alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	return s;
}
