
/*
* Copyright (c) 2006-2010 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/
//Author: Ramesh Bhagat
//Mail: mbsoft01@gmail.com


#include "Magnet.h"
class MagnetTest : public Test
{
public:
	MagnetBody *magnetBody;
	MagnetTest()
	{
		Magnet::getInstance()->init();
		Magnet::getInstance()->setWorld(m_world);
		Magnet::getInstance()->setDeugDraw(&m_debugDraw);
		
		//magnet 0
		{
			b2BodyDef def;
			def.type = b2_staticBody;
			def.position.Set(0,10);
			b2Body *body;
			body = m_world->CreateBody(&def);
			body->SetUserData("magnet");
			{
				b2PolygonShape shape;
				shape.SetAsBox(2,2);
				body->CreateFixture(&shape,2);
			}
			magnetBody = Magnet::getInstance()->createMagnetBody(body,5000);
		}
		
		//ground body
		{
			b2BodyDef def;
			def.type = b2_staticBody;
			def.position.Set(0,0);
			b2Body *body = m_world->CreateBody(&def);
			body->SetUserData("ground");
			{
				b2FixtureDef def;
				def.density = 1;
				b2PolygonShape shape;
				shape.SetAsBox(20,1);
				def.shape = &shape;
				body->CreateFixture(&def);
			}
		}
	}
	bool mouseDown;
	b2Vec2 point;
	void MouseDown(const b2Vec2 &p)
	{
		point = p;
		b2BodyDef def;
		def.type = b2_dynamicBody;
		def.position = p;
		b2Body *body = m_world->CreateBody(&def);
		body->SetUserData("free body");
		{
			b2CircleShape shape;
			shape.m_radius = 0.5f;
			body->CreateFixture(&shape,1);
		}
		mouseDown = true;
	}
	void MouseUp(const b2Vec2 &p)
	{
		mouseDown = false;
	}
	void KeyboardUp(unsigned char key)
	{
		if(key == VK_SPACE)
		{
			magnetBody->isActive()?magnetBody->deActivate():magnetBody->activate();
		}
	}
	void Step(Settings *settings)
	{
		Test::Step(settings);
		Magnet::getInstance()->update(1.0f / settings->hz);
		if(mouseDown)
		{
			b2BodyDef def;
			def.type = b2_dynamicBody;
			def.position = point;
			b2Body *body = m_world->CreateBody(&def);
			Magnet::getInstance()->addMagnetism(body);
			body->SetUserData("free body");
			{
				b2CircleShape shape;
				shape.m_radius = 0.5f;
				body->CreateFixture(&shape,1);
			}
		}
	}
	static Test* Create()
	{
		return new MagnetTest;
	}
};