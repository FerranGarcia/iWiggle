#include "stdafx.h"
#include "SignInstance.h"


SignInstance::SignInstance()
{
	// default sign type: NO_SIGN
	this->signType = NO_SIGN;

	// record the time of apearance (constructor call)
	this->timestamp = clock();

	reliabilityCount = 0;
}


SignInstance::~SignInstance()
{
}
