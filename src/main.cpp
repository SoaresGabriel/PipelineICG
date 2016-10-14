#include "main.h"
#define PI 3.1415

//-----------------------------------------------------------------------------
void MyGlDraw(void) {

	Vector3f 	camPosition(0,0,4),
				camLookAt(0,0,0),
				camUp(0,1,0);

	defineCamera(camPosition, camLookAt, camUp);

	defineViewPlane(1.92);

	defineViewPort(IMAGE_WIDTH, IMAGE_HEIGHT);

	clearScreen();
	drawObj();

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
	strcpy(obj, OBJ);
	objData->load(obj);
	loadIdentityModel();

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
