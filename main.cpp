#include <cstdio>
#include <cstdlib>
#include <GL/glut.h>
#include <Cg/cgGL.h>

CGprogram cgProg;
CGcontext cgContext;
CGprofile cgProfile;
CGparameter input1;

int window;
int width=800;
int height=600;

void keyboardFunc(unsigned char key, int x, int y) {
	switch(key) {
		case 27: //ESC
		case 'q':
			exit(0);
			break;
	}
}

void displayFunc() {
	//*
	cgGLEnableProfile(cgProfile);
	cgGLBindProgram(cgProg);
	//*/

//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	gluLookAt(0,0,1,  0,0,0,  1,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
		glColor3f(1,0,0);
		glVertex3f(1,0,0);

		glColor3f(0,1,0);
		glVertex3f(0,-1,0);

		glColor3f(0,0,1);
		glVertex3f(-1,0,0);
	glEnd();

	cgGLDisableProfile(cgProfile);

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

void initCG() {
	cgContext = cgCreateContext();
	cgProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	//assert
	cgGLSetOptimalOptions(cgProfile);
	//errorCB
	cgProg = cgCreateProgramFromFile(cgContext, CG_SOURCE, "bump.cg", cgProfile, NULL, NULL);
	
	if(cgProg != NULL) {
		cgGLLoadProgram(cgProg);
		//input1 = cgGetNamedParameter(cgProg, "mChangeColorPosition");
	}
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

	glutMainLoop();
	return 0;
}
