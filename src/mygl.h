#ifndef _MYGL_H_
#define _MYGL_H_

#include "math.h"
#include "definitions.h"
#include <eigen3/Eigen/Dense>

#define OBJ "monkey_head2.obj"

typedef Eigen::Matrix<float, 4, 4> Matrix4f;
typedef Eigen::Matrix<float, 4, 1> Vector4f;

typedef struct Color {
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;
} Color;

typedef struct Pixel {
	int x;
	int y;
	Color color;
} Pixel;

typedef struct Line {
	Pixel pA;
	Pixel pB;
} Line;

typedef struct Triangle {
	Pixel vA;
	Pixel vB;
	Pixel vC;
} Triangle;

typedef struct Vertice {
	double x;
	double y;
	double z;
	double w;
} Vertice;

unsigned int getMemoryAdress(Pixel p) {
	return (p.x) * 4 + (p.y) * IMAGE_WIDTH * 4;
}

void putPixel(Pixel p) {
	unsigned int adress = getMemoryAdress(p);

	FBptr[adress] = p.color.R;
	FBptr[adress + 1] = p.color.G;
	FBptr[adress + 2] = p.color.B;
	FBptr[adress + 3] = p.color.A;
}

void drawLine(Line l) {

	// Se |DeltaY| < |DeltaX| -> permuta as coordenadas  de cada ponto
	int permuta = 0;
	if (abs(l.pB.y - l.pA.y) > abs(l.pB.x - l.pA.x)) {
		int aux;

		// permuta coordenadas do ponto de inicio da reta
		aux = l.pA.x;
		l.pA.x = l.pA.y;
		l.pA.y = aux;

		// permuta coordenadas do ponto de fim da reta
		aux = l.pB.x;
		l.pB.x = l.pB.y;
		l.pB.y = aux;

		permuta = 1; // variavel para saber se na hora de desenhar, precisa permutar

	}

	// se DeltaX < 0 inverte os pontos de inicio e fim da linha
	if (l.pB.x < l.pA.x) {
		Pixel a = l.pA;
		l.pA = l.pB;
		l.pB = a;
	}

	int dx = l.pB.x - l.pA.x;
	int dy = l.pB.y - l.pA.y;
	int addY = 1;

	// se DeltaY < 0 os valores de y e da variavel de decisão d serão decrementados
	if (dy < 0) {
		addY = -1;
		dy *= -1;
	}

	int d = 2 * dy - dx;
	int addE = 2 * dy;
	int addNE = 2 * (dy - dx);
	int x = l.pA.x;
	int y = l.pA.y;

	Pixel pixel = { x, y, l.pA.color };
	Pixel *p = &pixel;

	//desenha o primeiro pixel
	if (permuta) {
		Pixel permutado = { pixel.y, pixel.x, pixel.color };
		putPixel(permutado);
	} else {
		putPixel(pixel);
	}

	//Calculos para Interpolação de Cor
	float lineSize = dx;
	float addR = (l.pB.color.R - l.pA.color.R) / lineSize;
	float addG = (l.pB.color.G - l.pA.color.G) / lineSize;
	float addB = (l.pB.color.B - l.pA.color.B) / lineSize;
	float addA = (l.pB.color.A - l.pA.color.A) / lineSize;
	float R = l.pA.color.R;
	float G = l.pA.color.G;
	float B = l.pA.color.B;
	float A = l.pA.color.A;

	while (pixel.x < l.pB.x) {

		// Definição das cores para interpolação
		R += addR;
		G += addG;
		B += addB;
		A += addA;
		p->color.R = R;
		p->color.G = G;
		p->color.B = B;
		p->color.A = A;

		if (d <= 0) {
			d += addE;
			p->x++;
		} else {
			d += addNE;
			p->x++;
			p->y += addY;
		}

		//se as coordenadas foram permutadas, aqui elas permutadas novamente
		if (permuta) {
			Pixel permutado = { pixel.y, pixel.x, pixel.color };
			putPixel(permutado);
		} else {
			putPixel(pixel);
		}
	}
}

void drawTriangle(Triangle t) {

	Line line1 = { t.vA, t.vB };
	Line line2 = { t.vB, t.vC };
	Line line3 = { t.vC, t.vA };

	drawLine(line1);
	drawLine(line2);
	drawLine(line3);

}

#endif // _MYGL_H_

