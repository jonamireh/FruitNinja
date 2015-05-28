#pragma once

class Component
{
public:
	virtual ~Component()
	{
	}

	virtual void update() = 0;
};