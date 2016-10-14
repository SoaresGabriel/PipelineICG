#ifndef _MYGL_H_
#define _MYGL_H_

#include "math.h"
#include "definitions.h"
#include "objLoader.h"
#include "transformations.h"

#define OBJ "monkey_head2.obj"

objLoader *objData;

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
	if(p.x < 0 || p.x >= IMAGE_WIDTH || p.y < 0 || p.y >= IMAGE_HEIGHT)
		return;

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

void drawObj() {

	Matrix4f MVPV;
	MVPV = VIEWPORT*PROJECTION*VIEW*MODEL;

	Color color = {255,255,255,255};

	// CARREGA TRIANGULOS VERTICE A VERTICE
	for (int i = 0; i < objData->faceCount; i++) {
		obj_face *o = objData->faceList[i];

		Vector4f v1, v2, v3;

		v1(0) = objData->vertexList[o->vertex_index[0]]->e[0];
		v1(1) = objData->vertexList[o->vertex_index[0]]->e[1];
		v1(2) = objData->vertexList[o->vertex_index[0]]->e[2];
		v1(3) = 1; //coordenada homogenea;

		v2(0) = objData->vertexList[o->vertex_index[1]]->e[0];
		v2(1) = objData->vertexList[o->vertex_index[1]]->e[1];
		v2(2) = objData->vertexList[o->vertex_index[1]]->e[2];
		v2(3) = 1;

		v3(0) = objData->vertexList[o->vertex_index[2]]->e[0];
		v3(1) = objData->vertexList[o->vertex_index[2]]->e[1];
		v3(2) = objData->vertexList[o->vertex_index[2]]->e[2];
		v3(3) = 1;


		v1 = MVPV * v1;
		v2 = MVPV * v2;
		v3 = MVPV * v3;

		v1 /= v1(3);
		v2 /= v2(3);
		v3 /= v3(3);

		Pixel p1 = {(int) v1(0), (int) v1(1), color};
		Pixel p2 = {(int) v2(0), (int) v2(1), color};
		Pixel p3 = {(int) v3(0), (int) v3(1), color};

		Triangle t = {p1, p2, p3};

		drawTriangle(t);
	}
}

void clearScreen(){
	int totalBytes = IMAGE_WIDTH * IMAGE_HEIGHT * 4;

	for(int i = 0; i < totalBytes; i++){
		FBptr[i] = 0;
	}
}

#endif // _MYGL_H_

