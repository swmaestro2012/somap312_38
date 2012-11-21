#pragma once
#include <Box2D/Box2D.h>
class WContactListener : public b2ContactListener
{
public:
	WContactListener(void);
	~WContactListener(void);

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};
