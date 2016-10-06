#include "main.h"
#include "objLoader.h"

objLoader *objData;

//-----------------------------------------------------------------------------
void MyGlDraw(void) {

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

	}



}

void FreeMemFunc(void) {
	std::clog << "Exiting...\n";

	if (!objData)
		delete objData;
}

//-----------------------------------------------------------------------------
int main(int argc, char **argv) {

	// Carrega Objeto
	objData = new objLoader();
	char *obj = (char *) malloc(sizeof(char) * (strlen(OBJ) + 1));
	objData->load(obj);

	// Inicializações.
	InitOpenGL(&argc, argv);
	InitCallBacks();
	InitDataStructures();

	// Ajusta a função que chama as funções do mygl.h
	DrawFunc = MyGlDraw;

	// Limpar memoria ao encerrar o programa
	atexit(FreeMemFunc);

	// Framebuffer scan loop.
	glutMainLoop();

	return 0;
}
