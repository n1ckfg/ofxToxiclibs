#pragma once

#include "Vec3D.h"
#include "Ray3D.h"
#include "mesh/Mesh3D.h"
#include "mesh/TriangleMesh.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

namespace toxi
{
	namespace geom
	{
		class AABB : Vec3D
		{
		public:
			AABB( void );
			static AABB fromMinMax( Vec3D min, Vec3D max ) 
			{
				Vec3D a = Vec3D::min( min, max );
				Vec3D b = Vec3D::max( min , max );
				return AABB( a.interpolateTo( b, 0.5 ), b.sub( a ).scaleSelf( 0.5 ) );
			}

			static AABB getBoundingBox( std::vector<Vec3D> points  )
			{
				if ( points.size() == 0 ) {
					return NULL;
				}
				Vec3D min = Vec3D::max_value();
				Vec3D max = Vec3D::min_value();
				for (Vec3D p : points) {
					min.minSelf(p);
					max.maxSelf(p);
				}
				return fromMinMax(min, max);
			}

			AABB( AABB &box );
			AABB( double extend );
			AABB( Vec3D point, double extend );
			AABB( Vec3D point, Vec3D extend );

			~AABB(void);

			Vec3D getMin();
			Vec3D getMax();
			bool containsPoint( Vec3D point );
			//Sphere getBoundingSphere();
			Vec3D getExtend(  );
			Vec3D getNormalForPoint( Vec3D p );
			AABB growToContainPoint( Vec3D p );
			bool intersectsBox( AABB box );
			Vec3D intersectsRay( Ray3D ray, float minDist, float maxDist );
			//bool intersectsSphere(Sphere s );
			bool intersectsSphere( Vec3D c, float r );
			//bool intersectsTriangle( Triangle3D tri );
			bool planeBoxOverlap( Vec3D normal, float d, Vec3D maxBox );
			AABB set( AABB box );
			Vec3D set( float a, float b, float c );
			AABB set( Vec3D v );
			AABB setExtend( Vec3D extend );
			bool testAxis( float a, float b, float fa, float fb, float va, float vb, float wa, float wb, float ea, float eb );
			Mesh3D toMesh( Mesh3D mesh );
			Mesh3D toMesh( void );
			std::string toString( void );
			AABB updateBound( void );

		private:
			Vec3D extent, min, max;
		};
	}
}


