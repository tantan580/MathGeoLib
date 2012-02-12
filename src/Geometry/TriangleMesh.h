/* Copyright 2011 Jukka Jyl�nki

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

/** @file TriangleMesh.h
	@author Jukka Jyl�nki
	@brief The TriangleMesh geometry object. */
#pragma once

#include "Math/MathFwd.h"

MATH_BEGIN_NAMESPACE

/// Represents an unindiced triangle mesh.
/** This class stores a triangle mesh as flat array, optimized for ray intersections. */
class TriangleMesh
{
public:
	/// Specifies the vertex data of this triangle mesh. Replaces any old
	/// specified geometry.
	void Set(const float *triangleMesh, int numTriangles);
	void Set(const float3 *triangleMesh, int numTriangles) { Set(reinterpret_cast<const float *>(triangleMesh), numTriangles); }
	void Set(const Triangle *triangleMesh, int numTriangles) { Set(reinterpret_cast<const float *>(triangleMesh), numTriangles); }

	float IntersectRay(const Ray &ray) const;
	float IntersectRay_TriangleIndex(const Ray &ray, int &outTriangleIndex) const;
	float IntersectRay_TriangleIndex_UV(const Ray &ray, int &outTriangleIndex, float &outU, float &outV) const;

	void SetAoS(const float *vertexData, int numTriangles);
	void SetSoA4(const float *vertexData, int numTriangles);
	void SetSoA8(const float *vertexData, int numTriangles);

	float IntersectRay_SSE2(const Ray &ray) const;
	float IntersectRay_SSE41(const Ray &ray) const;
	float IntersectRay_AVX(const Ray &ray) const;
	float IntersectRay_TriangleIndex_SSE2(const Ray &ray, int &outTriangleIndex) const;
	float IntersectRay_TriangleIndex_SSE41(const Ray &ray, int &outTriangleIndex) const;
	float IntersectRay_TriangleIndex_AVX(const Ray &ray, int &outTriangleIndex) const;
	float IntersectRay_TriangleIndex_UV_CPP(const Ray &ray, int &outTriangleIndex, float &outU, float &outV) const;
	float IntersectRay_TriangleIndex_UV_SSE2(const Ray &ray, int &outTriangleIndex, float &outU, float &outV) const;
	float IntersectRay_TriangleIndex_UV_SSE41(const Ray &ray, int &outTriangleIndex, float &outU, float &outV) const;
	float IntersectRay_TriangleIndex_UV_AVX(const Ray &ray, int &outTriangleIndex, float &outU, float &outV) const;

private:
	float *data;
#ifdef _DEBUG
	int vertexDataLayout; // 0 - AoS, 1 - SoA4, 2 - SoA8
#endif
	int numTriangles;
	void ReallocVertexBuffer(int numTriangles);
};

MATH_END_NAMESPACE
