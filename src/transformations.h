#ifndef _TRANSFORMATIONS_
#define _TRANSFORMATIONS_

#include <eigen3/Eigen/Dense>
#include <cmath>

typedef Eigen::Matrix<float, 4, 4> Matrix4f;
typedef Eigen::Matrix<float, 4, 1> Vector4f;

Matrix4f MODEL;

void carregaIdentidade(Matrix4f *m){
	*m <<	1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1;
}

void mScale(float sX, float sY, float sZ){
	Matrix4f s;
	s <<	sX,  0,  0, 0,
			 0, sY,  0, 0,
			 0,  0, sZ, 0,
			 0,  0,  0, 1;

	MODEL = s * MODEL; // aplica primeiro o que tinha, depois a escala
}

void mTranslate(float tX, float tY, float tZ){
	Matrix4f t;
		t << 1, 0, 0, tX,
			 0, 1, 0, tY,
			 0, 0, 1, tZ,
			 0, 0, 0, 1;

		MODEL = t * MODEL;
}

void mRotateX(float teta){
	Matrix4f rX;
	rX <<	1, 0,         0,          0,
			0, cos(teta), -sin(teta), 0,
			0, sin(teta), cos(teta),  0,
			0, 0,         0,          1;
	MODEL = rX * MODEL;
}

void mRotateY(float teta){
	Matrix4f rY;
	rY <<	cos(teta),  0, sin(teta), 0,
		    0,          1, 0,         0,
		    -sin(teta), 0, cos(teta), 0,
		    0,          0, 0,         1;
	MODEL = rY * MODEL;
}

void mRotateZ(float teta){
	Matrix4f rZ;
	rZ <<	cos(teta), -sin(teta), 0, 0,
			sin(teta), cos(teta),  0, 0,
			0,         0,          1, 0,
			0,         0,          0, 1;
	MODEL = rZ * MODEL;
}

#endif
