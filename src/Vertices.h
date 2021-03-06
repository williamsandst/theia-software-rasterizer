#pragma once
#include "Eigen/Dense"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace Eigen;
using namespace std;

//Due to performance optimizations, there are no isolated Vertex objects, but instead
//only Vertices, which is a group of vertex using matrices

struct Vertex //Single vertex, used only for the rasterizer, not for Vertex shading!
{
public:
	Vector4f point;
	Vector4f normal;
	Vector2f UVCoord;
	Vector4f color;
	Vertex(Vector4f point, Vector4f normal, Vector2f UVCoord, Vector4f color, float w)
	{
		this->point = point;
		point[3] = w;
		this->normal = normal;
		this->UVCoord = UVCoord;
		this->color = color;
	}
	Vertex() {};
};

struct Vertices
{
public:
	Matrix4Xf points;
	Matrix4Xf normals;
	Matrix2Xf UVCoords;
	Matrix4Xf colors;

	Matrix4Xf surfaceNormals;
	vector<float> wVector;

	Vertices(Matrix4Xf points, Matrix4Xf normals, Matrix2Xf UVCoords, Matrix4Xf colors)
	{
		this->points = points;
		this->normals = normals;
		this->UVCoords = UVCoords;
		this->colors = colors;
		wVector = vector<float>(points.cols());
	}
	Vertices(Matrix4Xf points)
	{
		this->points = points;
		wVector = vector<float>(points.cols());
	}
	Vertices(Matrix4Xf points, Matrix4Xf colors)
	{
		this->points = points;
		this->colors = colors;
		wVector = vector<float>(points.cols());
	}
	void combine(Vertices vertices2)
	{
		Matrix4Xf new_points(4, points.cols() + vertices2.points.cols());
		new_points << points, vertices2.points;
		points = new_points;
		
		Matrix4Xf new_normals(4, normals.cols() + vertices2.normals.cols());
		new_normals << normals, vertices2.normals;
		normals = new_normals;

		Matrix2Xf new_UVCoords(2, UVCoords.cols() + vertices2.UVCoords.cols());
		new_UVCoords << UVCoords, vertices2.UVCoords;
		UVCoords = new_UVCoords;

		Matrix4Xf new_colors(4, colors.cols() + vertices2.colors.cols());
		new_colors << colors, vertices2.colors;
		colors = new_colors;

		Matrix4Xf new_surfaceNormals(4, colors.cols() + vertices2.colors.cols());
		new_colors << surfaceNormals, vertices2.surfaceNormals;
		colors = new_surfaceNormals;
	}
	void resize(int newSize)
	{
		points.conservativeResize(Eigen::NoChange, newSize);
		normals.conservativeResize(Eigen::NoChange, newSize);
		UVCoords.conservativeResize(Eigen::NoChange, newSize);
		colors.conservativeResize(Eigen::NoChange, newSize);
	}
	void normalizeNormals()
	{
		for (size_t i = 0; i < normals.cols(); i++)
		{
			normals(3, i) = 0;
			normals.col(i).normalize();
		}
	}
	void normalizeSurfaceNormals()
	{
		for (size_t i = 0; i < surfaceNormals.cols(); i++)
		{
			surfaceNormals(3, i) = 0;
			surfaceNormals.col(i).normalize();
		}
	}
	int getVertexCount()
	{
		return points.cols();
	}
	Vertex getVertex(int pointIndex, int normalIndex, int UVCoordIndex, int colorIndex)
	{
		return Vertex(points.col(pointIndex), normals.col(normalIndex), UVCoords.col(UVCoordIndex), colors.col(colorIndex), wVector[pointIndex]);
	}
	void createFromVector(vector<Vector4f> points, vector<Vector4f> normals, vector<Vector2f> UVCoords, vector<Vector4f> colors)
	{
		this->points.resize(Eigen::NoChange, points.size());
		if (normals.size() != 0) { this->normals.resize(Eigen::NoChange, normals.size()); }
		if (UVCoords.size() != 0) { this->UVCoords.resize(Eigen::NoChange, UVCoords.size()); }
		if (colors.size() != 0) { this->colors.resize(Eigen::NoChange, colors.size()); }
		for (size_t i = 0; i < points.size(); i++) { this->points.col(i) = points[i]; }
		for (size_t i = 0; i < normals.size(); i++) { this->normals.col(i) = normals[i]; }
		for (size_t i = 0; i < UVCoords.size(); i++) { this->UVCoords.col(i) = UVCoords[i]; }
		for (size_t i = 0; i < colors.size(); i++) { this->colors.col(i) = colors[i]; }
		wVector = vector<float>(this->points.cols());
	}
	void setSurfaceNormalSize(int size)
	{
		surfaceNormals.resize(Eigen::NoChange, size);
	}
	void createSurfaceNormal(int normalA, int normalB, int normalC, int pos)
	{
		Vector4f surfaceNormal = normals.col(normalA) + normals.col(normalB) + normals.col(normalC);
		surfaceNormal.normalize();
		surfaceNormals.col(pos) = surfaceNormal;
	}
	Vertices(int size)
	{
		points = Matrix4Xf();
		normals = Matrix4Xf();
		UVCoords = Matrix2Xf();
		colors = Matrix4Xf();
		points.resize(Eigen::NoChange, size);
		normals.resize(Eigen::NoChange, size);
		UVCoords.resize(Eigen::NoChange, size);
		colors.resize(Eigen::NoChange, size);
		wVector = vector<float>(size);
	};
	Vertices() 
	{
		points = Matrix4Xf();
		normals = Matrix4Xf();
		UVCoords = Matrix2Xf();
		colors = Matrix4Xf();
		surfaceNormals = Matrix4Xf();
		points.resize(Eigen::NoChange, 1);
		normals.resize(Eigen::NoChange, 1);
		UVCoords.resize(Eigen::NoChange, 1);
		colors.resize(Eigen::NoChange, 1);
		surfaceNormals.resize(Eigen::NoChange, 1);
		points.col(0) = Vector4f(0, 0, 0, 1);
		normals.col(0) = Vector4f(0, 0, 0, 1);
		UVCoords.col(0) = Vector2f(0, 0);
		colors.col(0) = Vector4f(1, 1, 1, 1);
		surfaceNormals.col(0) = Vector4f(0, 0, 0, 1);
		wVector = vector<float>(1);
	};
};

