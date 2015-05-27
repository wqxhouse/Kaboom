//
//  Frustum.cpp
//  vrphysics
//
//  Created by Robin Wu on 3/3/15.
//  Copyright (c) 2015 WSH. All rights reserved.
//

#include "FrustumHelper.h"
#include <memory.h>
#include <osg/Geometry>

const osg::Vec4 FrustumHelper::_unitCubeCorners[8] =
{
	// opengl NDC space
	// near plane
	osg::Vec4(-1, -1, 1, 1),
	osg::Vec4(1, -1, 1, 1),
	osg::Vec4(1, 1, 1, 1),
	osg::Vec4(-1, 1, 1, 1),

	// far plane
	osg::Vec4(-1, -1, -1, 1),
	osg::Vec4(1, -1, -1, 1),
	osg::Vec4(1, 1, -1, 1),
	osg::Vec4(-1, 1, -1, 1),
};

FrustumHelper::FrustumHelper()
{
	// memcpy(_corners, _unitCubeCorners, sizeof(_corners));

	// _frustumGeode = new osg::Geode;
	// _frustumGeometry = new osg::Geometry;
	_frustumVertices = new osg::Vec4Array;
	_frustumVertices->resize(8);
	// _frustumIndices = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS);

	resetVertices();
}

FrustumHelper::~FrustumHelper()
{
}

void FrustumHelper::resetVertices()
{
	for (int i = 0; i < 8; i++)
	{
		(*_frustumVertices)[i] = _unitCubeCorners[i];
	}
}

void FrustumHelper::calcCorners()
{
	resetVertices();

	const osg::Matrix &projMat = *_projectionMatrixPtr;
	const osg::Matrix &viewMat = *_viewMatrixPtr;
	//    osg::Matrix viewProj = viewMat * projMat;

	//    l,b,-n
	//    r,b,-n
	//    r,t,-n
	//    l,t,-n
	//    l*s,b*s,-f
	//    r*s,b*s,-f
	//    r*s,t*s,-f
	//    l*s,t*s,-f
	//osg::Matrix viewInv = osg::Matrix::inverse(viewMat);

	//float l, b, r, t, n, f;
	//projMat.getFrustum(l, b, r, t, n, f);
	//float s = f / n;
	//osg::Vec4 p0(l, b, -n, 1);
	//osg::Vec4 p1(r, b, -n, 1);
	//osg::Vec4 p2(r, t, -n, 1);
	//osg::Vec4 p3(l, t, -n, 1);
	//osg::Vec4 p4(l*s, b*s, -f, 1);
	//osg::Vec4 p5(r*s, b*s, -f, 1);
	//osg::Vec4 p6(r*s, t*s, -f, 1);
	//osg::Vec4 p7(l*s, t*s, -f, 1);
	//p0 = p0 * viewInv;
	//p1 = p1 * viewInv;
	//p2 = p2 * viewInv;
	//p3 = p3 * viewInv;
	//p4 = p4 * viewInv;
	//p5 = p5 * viewInv;
	//p6 = p6 * viewInv;
	//p7 = p7 * viewInv;

	//(*_frustumVertices)[0] = p0;
	//(*_frustumVertices)[1] = p1;
	//(*_frustumVertices)[2] = p2;
	//(*_frustumVertices)[3] = p3;
	//(*_frustumVertices)[4] = p4;
	//(*_frustumVertices)[5] = p5;
	//(*_frustumVertices)[6] = p6;
	//(*_frustumVertices)[7] = p7;

	    osg::Matrix invViewProjMat = osg::Matrix::inverse(viewMat * projMat);
	    for(int i = 0; i < 8; i++)
	    {
	        osg::Vec4 &v = (*_frustumVertices)[i];
	        osg::Vec4 camV = v * invViewProjMat;
	        camV /= camV.w();
	        (*_frustumVertices)[i] = camV;
	    }

	    // assume perspective projection Matrix
		/*	    float l, r, b, t, n, f;

				bool tf = projMat.getFrustum(l, r, b, t, n, f);
				if(!tf)
				{
				fprintf(stderr, "FrustumHelper::calcCorners invalid, not perspective projection matrix");
				exit(0);
			}
				*/
	    
	    //// near
	    //(*_frustumVertices)[0].set(l, n, b);
	    //(*_frustumVertices)[1].set(r, n, b);
	    //(*_frustumVertices)[2].set(r, n, t);
	    //(*_frustumVertices)[3].set(l, n, t);
	    //
	    //// far
	    //(*_frustumVertices)[4].set(l, f, b);
	    //(*_frustumVertices)[5].set(r, f, b);
	    //(*_frustumVertices)[6].set(r, f, t);
	    //(*_frustumVertices)[7].set(l, f, t);
}

void FrustumHelper::calcCenter()
{
	osg::Vec3 v(0.0, 0.0, 0.0);
	for (int i = 0; i < 8; i++)
	{
		v += osg::Vec3((*_frustumVertices)[i].x(),
			(*_frustumVertices)[i].y(),
			(*_frustumVertices)[i].z());
	}
	v /= 8.0;
	_center = v;
}


float FrustumHelper::getBSphereRadius()
{
	//    for(int i = 0; i < 8; i++)
	//    {
	//        Utils::print((*_frustumVertices)[i], ' ');
	//    }
	//    printf("====\n");

	float sphereRad = 0.0f;
	for (int i = 0; i < 8; i++)
	{
		osg::Vec3 centerToCorner = osg::Vec3((*_frustumVertices)[i].x(),
			(*_frustumVertices)[i].y(),
			(*_frustumVertices)[i].z()) - _center;
		float dist = centerToCorner.length();
		sphereRad = osg::maximum(sphereRad, dist);
	}

	sphereRad = std::ceil(sphereRad * 16.0f) / 16.0f;
	return sphereRad;
}

//// for visualize the frustum
//void FrustumHelper::setupGeode()
//{
//	// near
//	_frustumIndices->push_back(0);
//	_frustumIndices->push_back(1);
//	_frustumIndices->push_back(2);
//	_frustumIndices->push_back(3);
//
//	// far
//	_frustumIndices->push_back(4);
//	_frustumIndices->push_back(5);
//	_frustumIndices->push_back(6);
//	_frustumIndices->push_back(7);
//
//	// left
//	_frustumIndices->push_back(0);
//	_frustumIndices->push_back(4);
//	_frustumIndices->push_back(7);
//	_frustumIndices->push_back(3);
//
//	// right
//	_frustumIndices->push_back(1);
//	_frustumIndices->push_back(5);
//	_frustumIndices->push_back(6);
//	_frustumIndices->push_back(2);
//
//	// top
//	_frustumIndices->push_back(4);
//	_frustumIndices->push_back(2);
//	_frustumIndices->push_back(6);
//	_frustumIndices->push_back(7);
//
//	// bottom
//	_frustumIndices->push_back(0);
//	_frustumIndices->push_back(1);
//	_frustumIndices->push_back(4);
//	_frustumIndices->push_back(5);
//
//	_frustumGeometry->addPrimitiveSet(_frustumIndices);
//	_frustumGeometry->setVertexArray(_frustumVertices);
//	_frustumGeometry->setUseVertexBufferObjects(true);
//	_frustumGeode->addDrawable(_frustumGeometry);
//}