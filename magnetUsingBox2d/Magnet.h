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

#include <vector>
#ifndef __MAGNET_H__
#define __MAGNET_H__

	// class representing magnetic material with ability to 
	// attract magnetic material
class MagnetBody{
	friend class Magnet;
public:
	//activate the magentic field of body
	void activate(){active = true;}
	
	//deactivate the magentic field of body
	void deActivate(){active = false;}
	
	//check whether magnetic field is active or not
	bool isActive() {return active;}
private:
	//pointer to body acting as a magnet
	b2Body* body;

	//force exerted at unit distace towards the centre of magnet
	float force;

	float radius;
	
	bool active;
};
class Magnet
{
private:
	b2Draw *draw;
	float MIN_FORCE;
	Magnet()
	{
		draw = 0;
		world = 0;
		MIN_FORCE = 0.5f;
	}

	//pointer to world
	b2World *world;

	//list of magnets in world
	std::vector<MagnetBody*> magnetBodyList;

	//list of magnetic materials in world
	//only this body are under magnetic influence
	std::vector<b2Body*> magneticBodyList;
public:
	static Magnet* getInstance()
	{
		static Magnet *instance = 0;
		if(instance == 0)
		{
			instance = new Magnet();
		}
		return instance;
	}
	void init()
	{
		magnetBodyList.clear();
		magneticBodyList.clear();
	}
	void setWorld(b2World* world)
	{
		this->world = world;
	}
	MagnetBody* createMagnetBody(b2Body *body , float force=500,float radius = 10)
	{
		b2Assert(world != 0);
		MagnetBody *magnetBody = new MagnetBody();
		magnetBody->body = body;
		magnetBody->active = true;
		magnetBody->force = force;
		magnetBody->radius = radius;
		magnetBodyList.push_back(magnetBody);
		return magnetBody;
	}
	void addMagnetism(b2Body *body)
	{
		b2Assert(world != 0);
		magneticBodyList.push_back(body);
	}
	void removeMagnetism(b2Body *body)
	{
		
		for(int i=0;i<magneticBodyList.size();i++)
		{
			b2Body *mbody = magneticBodyList.at(i);
			if(mbody == body)
			{
				magneticBodyList.erase(magneticBodyList.begin() + i);
			}
		}
	}
	void enableMagnet(b2Body* body)
	{
		MagnetBody *magnetBody;
		for(int i=0;i<magnetBodyList.size();i++)
		{
			magnetBody = magnetBodyList.at(i);
			if(magnetBody->body == body)
			{
				magnetBody->active = true;
			}
		}
	}
	void disableMagnet(b2Body* body)
	{
		MagnetBody *magnetBody;
		for(int i=0;i<magnetBodyList.size();i++)
		{
			magnetBody = magnetBodyList.at(i);
			if(magnetBody->body == body)
			{
				magnetBody->active = false;
			}
		}
	}
	void setDeugDraw(b2Draw *_draw)
	{
		draw = _draw;
	}
	void update(float dt)
	{
		MagnetBody *magnetBody;
		for(int i=0;i<magnetBodyList.size();i++)
		{
			magnetBody = magnetBodyList.at(i);
			if(!magnetBody->active)
				continue;
			else
			{
#ifdef _DEBUG
				if(draw)
				{
					draw->DrawCircle(magnetBody->body->GetPosition(),magnetBody->radius,b2Color(0.5f,0,1));
				}
#endif
				b2Body *body;
				for(int i=0;i<magneticBodyList.size();i++)
				{
					body = magneticBodyList.at(i);
					b2Assert(body!=magnetBody->body);//body should not be magnet body
					b2Vec2 distance = magnetBody->body->GetPosition() - body->GetPosition();
					float r = distance.Length();
					if(r <= magnetBody->radius)
					{
						distance.Normalize();
                        body->ApplyForce(b2Vec2(magnetBody->force /(r*r) * distance), magnetBody->body->GetPosition());
                        //if (r < 2) {
                            //body->GetFixtureList()->SetRestitution(0.0f);
                        //    body->SetAwake(false);
                        //}
                    }
				}
			}
		}
	}
};
#endif//__MAGNET_H__