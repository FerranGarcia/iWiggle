#include "stdafx.h"
#include "SignInstance.h"


SignInstance::SignInstance()
{
	// default sign type: NO_SIGN
	this->signType = NO_SIGN;

	// record the time of apearance (constructor call)
	time(&this->timestamp);
}


SignInstance::~SignInstance()
{
}
