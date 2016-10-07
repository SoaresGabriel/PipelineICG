#ifndef _TRANSFORMATIONS_
#define _TRANSFORMATIONS_

#include <eigen3/Eigen/Dense>
#include <cmath>

typedef Eigen::Matrix<float, 4, 4> Matrix4f;
typedef Eigen::Matrix<float, 4, 1> Vector4f;
typedef Eigen::Matrix<float, 3, 1> Vector3f;

Matrix4f MODEL, VIEW;

void loadIdentity(Matrix4f *m){
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

void defineCamera(Vector3f position, Vector3f lookAt, Vector3f up){

	Matrix4f Bt, T;
	Vector3f xCam, yCam, zCam;

	// Calcularemos os vetores que defines o sistema de coordenadas da camera

	// o Z é igual ao inverso do vetor de direcao e normalizado
	zCam = position - lookAt;
	zCam /= zCam.norm(); // normalizar

	// O X agora pode ser obtido fazendo o produto vetorial Up x zCam, para um sistema de mao direita, pois o vetor originado é perpendicular a ambos
	xCam = up.cross(zCam);
	xCam /= xCam.norm(); // normalizar

	// Agora o Y eh perpendicular a xCam e zCam, portanto, façamos o produto vetorial zCam x xCam, para um sistema de mao direita
	yCam = zCam.cross(xCam);
	yCam /= yCam.norm(); // normalizar

	//Agora podemos montar a matriz de Rotação para o Espaço da Camera
	Bt <<	xCam(1), xCam(2), xCam(3), 0,
			yCam(1), yCam(2), yCam(3), 0,
			zCam(1), zCam(2), zCam(3), 0,
			0      , 0      , 0      , 1;

	// E a matriz de translação
	T << 	1, 0, 0, -position(0),
			0, 1, 0, -position(1),
			0, 0, 1, -position(2),
			0, 0, 0, 1;

	VIEW = Bt * T;

}

#endif
