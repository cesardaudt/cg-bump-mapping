#include <cstdio>
#include <cstdlib>
#include <GL/glut.h>
#include <Cg/cgGL.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

CGprogram cgFragmentShader;
CGprogram cgVertexShader;
CGcontext cgContext;
CGprofile cgFragmentProfile;
CGprofile cgVertexProfile;
CGparameter cgColorTex;
CGparameter cgBumpTex;
CGparameter cgModelViewProj;

int window;
int width=800;
int height=600;
GLuint colorTex, bumpTex;

GLuint loadTexture(const char* file)
{
	GLuint tex;
	SDL_Surface* surface = IMG_Load(file);
	//surface = IMG_Load(file);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	SDL_PixelFormat *format = surface->format;

	if (format->Amask) {
		glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, 3, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	}

	return tex;
}

void keyboardFunc(unsigned char key, int x, int y) {
	switch(key) {
		case 27: //ESC
		case 'q':
			exit(0);
			break;
	}
}

void displayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//*
	cgGLEnableProfile(cgFragmentProfile);
	cgGLBindProgram(cgFragmentShader);

	cgGLEnableProfile(cgVertexProfile);
	cgGLBindProgram(cgVertexShader);

	cgGLSetStateMatrixParameter(cgModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetTextureParameter(cgBumpTex, bumpTex);
	cgGLEnableTextureParameter(cgBumpTex);

	cgGLSetTextureParameter(cgColorTex, colorTex);
	cgGLEnableTextureParameter(cgColorTex);
	//*/

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, colorTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBegin(GL_TRIANGLES);
		glColor3f(1,0,0);
		glTexCoord2f(0,0);
		glVertex3f(1,0,0);

		glColor3f(0,1,0);
		glTexCoord2f(0,1);
		glVertex3f(0,-1,0);

		glColor3f(0,0,1);
		glTexCoord2f(1,0);
		glVertex3f(-1,0,0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	cgGLDisableTextureParameter(cgColorTex);
	cgGLDisableTextureParameter(cgBumpTex);
	cgGLDisableProfile(cgVertexProfile);
	cgGLDisableProfile(cgFragmentProfile);

	glutSwapBuffers();
	glutPostRedisplay();
}

void idleFunc() {
	displayFunc();
}

void reshapeFunc(int x, int y) {
	width = x; height = y;
	glutPositionWindow(25, 25);
	glutReshapeWindow(width, height);

	glMatrixMode(GL_VIEWPORT);
	glLoadIdentity();
	glViewport(0,0, width,height);

	glutPostRedisplay();
}

bool initCG() {
	cgContext = cgCreateContext();

	cgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	cgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	//assert
	cgGLSetOptimalOptions(cgFragmentProfile);
	cgGLSetOptimalOptions(cgVertexProfile);
	//errorCB
	cgFragmentShader = cgCreateProgramFromFile(cgContext, CG_SOURCE, "bump_fragment.cg", cgFragmentProfile, NULL, NULL);
	cgVertexShader = cgCreateProgramFromFile(cgContext, CG_SOURCE, "bump_vertex.cg", cgVertexProfile, NULL, NULL);
	
	if(!cgFragmentShader) return false;
	if(!cgVertexShader) return false;

	cgGLLoadProgram(cgVertexShader);
	cgModelViewProj = cgGetNamedParameter(cgVertexShader, "modelViewProj");
	cgBumpTex = cgGetNamedParameter(cgVertexShader, "bumpTex");

	cgGLLoadProgram(cgFragmentShader);
	cgColorTex = cgGetNamedParameter(cgFragmentShader, "colorTex");

	return true;
}

void initGL() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH);

	glutInitWindowPosition(25, 25);
	glutInitWindowSize(width, height);
	window = glutCreateWindow("Bump Mapping");

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Culling
	//glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	// Shading
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

	// Alpha Blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Visibility
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Callbacks
	glutReshapeFunc(reshapeFunc);
	glutIdleFunc(idleFunc);
	glutDisplayFunc(displayFunc);
	glutKeyboardFunc(keyboardFunc);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	initGL();
	initCG();

	colorTex = loadTexture("brick-c.jpg");
	bumpTex = loadTexture("brick-n.jpg");

	glutMainLoop();
	return 0;
}
