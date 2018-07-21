#include "Collision.hpp"

bool checkCollisionWithBoundary(Vec2f wall_0, Vec2f wall_1, Vec2f p_0, Vec2f p_1, float* tMin)
{
  	//Process + equations determined by calculating simultaneous equations results for vector intersection
  	Vec2f dP = p_1 - p_0;
  	Vec2f dW = wall_1 - wall_0;

  	Vec2f a = wall_0 - p_0;
  	float t = 2.0f; //Just something to be > 1.0f
  	float r = 2.0f;

  	if(dW.x == 0.0f)
  	{
    		if(dP.x != 0.0f)
    		{
      			t = a.x / dP.x;
      			r = (t*dP.y - a.y) / dW.y;
    		}
    		else return false;
  	}
  	else if(dW.y == 0.0f)
  	{
    		if(dP.y != 0.0f)
    		{
      			t = a.y / dP.y;
      			r  = (t*dP.x - a.x) / dW.x;
    		}
    		else return false;
  	}
  	else
  	{
    		float w = dW.x / dW.y;
    		t = (a.x + a.y*w) / (dP.x - dP.y*w);
    		r = (t*dP.y - a.y) / dW.y;
  	}

  	if(t >= 0.0f && t < *tMin)
  	{
    		if(r >= 0.0f && r <= 1.0f)
    		{
      			*tMin = t;
     	 		return true;
    		}
  	}

  	return false;
}

//Colliding entity is the entity which the moving entity will collide with
bool checkRectCollision(Vec2f r_p, Vec2f r_s, Vec2f m_p_0, Vec2f m_p_1, Vec2f m_s, Vec2f* w, Vec2f* n, float* tMin)
{
  	bool collided = false;
  	Vec2f topLeftBound = r_p - 0.5f*r_s - 0.5f*m_s;
  	Vec2f bottomRightBound = r_p + 0.5f*r_s + 0.5f*m_s;
  	Vec2f wallPoints[] =
  	{
    		topLeftBound, Vec2f{bottomRightBound.x, topLeftBound.y}, //Top wall
    		Vec2f{bottomRightBound.x, topLeftBound.y}, bottomRightBound, //Right wall
    		bottomRightBound, Vec2f{topLeftBound.x, bottomRightBound.y}, //Bottom wall
    		Vec2f{topLeftBound.x, bottomRightBound.y}, topLeftBound //Left wall
  	};

  	for(uint i = 0; i < 8; i += 2)
  	{
    		Vec2f wall_0 = wallPoints[i];
    		Vec2f wall_1 = wallPoints[i+1];

    		if(checkCollisionWithBoundary(wall_0, wall_1, m_p_0, m_p_1, tMin))
    		{
      			collided = true;
      			*w = wall_1 - wall_0;
      			*n = normalise(Vec2f{w->y, -w->x});
    		}
  	}
  	return collided;
}
