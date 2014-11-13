#include "aabb.h"
#include "main.h"
#include "float.h"

inline vector3 vec3Normalize(vector3 a)
{
	double distance = sqrt(vec3Dot(a,a));
	return (vector3){a.x/distance,a.y/distance,a.z/distance};
}

_aabb aabbHeap[aabbHeapSize];

aabb aabb_alloc(double x1, double y1, double z1, double x2, double y2, double z2)
{
	int i;
	for (i = 0; i<aabbHeapSize; i++)
	{
		if (!aabbHeap[i].alloced)
		{
			aabbHeap[i].min.x = x1;
			aabbHeap[i].min.y = y1;
			aabbHeap[i].min.z = z1;
			
			aabbHeap[i].max.x = x2;
			aabbHeap[i].max.y = y2;
			aabbHeap[i].max.z = z2;
			
			aabbHeap[i].alloced = true;
			return i;
		}
	}
	return -1;
}

void aabb_dealloc(aabb a)
{
	aabbHeap[a].alloced = false;
}

void aabb_set(aabb _a, double x1, double y1, double z1, double x2, double y2, double z2)
{		
	aabbHeap[_a].min.x = x1;
	aabbHeap[_a].min.y = y1;
	aabbHeap[_a].min.z = z1;
	
	aabbHeap[_a].max.x = x2;
	aabbHeap[_a].max.y = y2;
	aabbHeap[_a].max.z = z2;
}

vector3 aabb_getMin(aabb a)
{
	return aabbHeap[a].min;
}

vector3 aabb_getMax(aabb a)
{
	return aabbHeap[a].min;
}

aabb aabb_copy(aabb _a)
{
	_aabb a = aabbHeap[_a];
	return aabb_alloc(a.min.x,a.min.y,a.min.z,a.max.x,a.max.y,a.max.z);
}

void aabb_round(aabb _a)
{
	aabbHeap[_a].min.x = floor(aabbHeap[_a].min.x);
	aabbHeap[_a].min.y = floor(aabbHeap[_a].min.y);
	aabbHeap[_a].min.z = floor(aabbHeap[_a].min.z);
	
	aabbHeap[_a].max.x = ceil(aabbHeap[_a].max.x);
	aabbHeap[_a].max.y = ceil(aabbHeap[_a].max.y);
	aabbHeap[_a].max.z = ceil(aabbHeap[_a].max.z);
}

#define collisionmoveneg(a,b,n) if (equal) { \
	\
} else { \
	double move = (b.min.n - (a.max.n-a.min.n))-a.min.n;\
	netcat_logf("move: %f\n",move); \
	a.min.n += move; \
	a.max.n += move; \
}

#define collisionmovepos(a,b,n) if (equal) { \
	\
} else { \
	double move = b.max.n-a.min.n;\
	netcat_logf("move: %f\n",move); \
	netcat_logf("was: %f\n",a.min.n); \
	a.min.n += move; \
	a.max.n += move; \
	netcat_logf("now: %f\n",a.max.n); \
}

#define testcollision(a,b) a.max.x > b.min.x && \
		a.min.x < b.max.x && \
		a.max.y > b.min.y && \
		a.min.y < b.max.y && \
		a.max.z > b.min.z && \
		a.min.z < b.max.z

bool aabb_process(aabb _a, aabb _b, bool equal)
{
	#define a aabbHeap[_a]
	#define b aabbHeap[_b]
	if (testcollision(a,b))
	{
		//collided!
		double m1x = a.max.x/2.0, m1y = a.max.y/2.0, m1z = a.max.z/2.0;
		double m2x = b.max.x/2.0, m2y = b.max.y/2.0, m2z = b.max.z/2.0;
		double mabsx = fabs(m1x-m2x);
		double mabsy = fabs(m1y-m2y);
		double mabsz = fabs(m1z-m2z);
		
		double oldminx = a.min.x;
		double oldminz = a.min.z;
		double oldmaxx = a.max.x;
		double oldmaxz = a.max.z;
		
		bool canItEven = (m1y-m2y > 0.0) && (b.max.y-a.min.y < 1.0);
		
		if (mabsx > mabsz && (mabsx > mabsy || !canItEven))
		{
			//collision from x direction
			if (m1x-m2x < 0.0)
			{
				//collision from right
				netcat_log("collision from right\n");
				collisionmoveneg(a,b,x)
			}
			else
			{
				//collision from left
				netcat_log("collision from left\n");
				collisionmovepos(a,b,x)
			}
		}
		else if (mabsz > mabsx && (mabsz > mabsy || !canItEven))
		{
			//collision from z direction
			if (m1z-m2z < 0.0)
			{
				//collision from back
				netcat_log("collision from back\n");
				collisionmoveneg(a,b,z)
			}
			else
			{
				//collision from front
				netcat_log("collision from front\n");
				collisionmovepos(a,b,z)
			}
		}
		else
		{
			//collision from the y direction
			if (m1y-m2y < 0.0)
			{
				//collision from bottom
				netcat_log("collision from bottom\n");
				collisionmoveneg(a,b,y)
			}
			else if (b.max.y-a.min.y < 1.0)
			{
				//collision from top
				netcat_log("collision from top\n");
				collisionmovepos(a,b,y)
			}
			else
			{
				//antifreeze
				if (mabsx > mabsz)
				{
					//collision from x direction
					if (m1x-m2x < 0.0)
					{
						//collision from right
						netcat_log("collision from right\n");
						collisionmoveneg(a,b,x)
					}
					else
					{
						//collision from left
						netcat_log("collision from left\n");
						collisionmovepos(a,b,x)
					}
				}
				else if (mabsz > mabsx)
				{
					//collision from z direction
					if (m1z-m2z < 0.0)
					{
						//collision from back
						netcat_log("collision from back\n");
						collisionmoveneg(a,b,z)
					}
					else
					{
						//collision from front
						netcat_log("collision from front\n");
						collisionmovepos(a,b,z)
					}
				}
			}
		}
		return true;
	}
	#undef a
	#undef b
	return false;
}

bool aabb_resolvecollisionwithworld(aabb a)
{
	//alloc an aabb for block testing
	aabb blocks = aabb_alloc(0,0,0,0,0,0);
	//create copy of a
	aabb _copy = aabb_copy(a);
	aabb_round(_copy); //round it out to blocks
	_aabb copy = aabbHeap[_copy];
	//scan around the aabb for the blocks
	//netcat_logf("Player Position: %f, %f, %f to %f, %f, %f\n", _a.min.x, _a.min.y, _a.min.z, _a.max.x, _a.max.y, _a.max.y);
	int x,y,z;
	for (x = copy.min.x; x<=copy.max.x; x++)
	{
		for (y = copy.min.y; y<=copy.max.y; y++)
		{
			for (z = copy.min.z; z<=copy.max.z; z++)
			{
				if (getBlock(x,y,z) != 0)
				{
					aabb_set(blocks, x,y,z, x+1,y+1,z+1);
					if (aabb_process(a,blocks,false))
					{
						//collision, TODO: let blocks define AABB bounds
						aabb_dealloc(_copy);
						aabb_dealloc(blocks);
						return true;
					}
				}
			}
		}
	}
	aabb_dealloc(_copy);
	aabb_dealloc(blocks);
	return false;
}
