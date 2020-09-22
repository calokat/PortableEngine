#pragma once
class IPlatform
{
public:
	virtual int InitWindow() = 0;
	virtual long Run() = 0;
};

