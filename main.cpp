#include <cstdio>
#include <cstdlib>
#include <GL/glut.h>
#include <Cg/cgGL.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "vector3f.h"

CGprogram cgFragmentShader;
CGprogram cgVertexShader;
CGcontext cgContext;
CGprofile cgFragmentProfile;
CGprofile cgVertexProfile;
CGparameter cgColorTex;
CGparameter cgBumpTex;
CGparameter cgModelViewProj;
CGparameter cgLightDiffuseColor;
CGparameter cgLightPosition;

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

void enableTexture(const int texId) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void disableTexture() {
	glDisable(GL_TEXTURE_2D);
}

void keyboardFunc(unsigned char key, int x, int y) {
	switch(key) {
		case 27: //ESC
		case 'q':
			exit(0);
			break;
	}
}

void computeTBN(Vector3f vertexCoords[3], Vector3f texCoords[3], Vector3f tbn[3]) {
	// Calculate the tangent basis for each vertex of the triangle
	// UPDATE: In the 3rd edition of the accompanying article, the for-loop located here has 
	// been removed as it was redundant (the entire TBN matrix was calculated three times 
	// instead of just one).
	//
	// Please note, that this function relies on the fact that the input geometry are triangles
	// and the tangent basis for each vertex thus is identical!
	//
	// We use the first vertex of the triangle to calculate the TBN matrix, but we could just 
	// as well have used either of the other two. Try changing 'i' below to 1 or 2. The end 
	// result is the same.
	int i = 0;

	// Calculate the index to the right and left of the current index
	int nNextIndex = (i + 1) % 3;
	int nPrevIndex = (i + 2) % 3;

	// Calculate the vectors from the current vertex to the two other vertices in the triangle
	Vector3f v2v1 = vertexCoords[nNextIndex] - vertexCoords[i];
	Vector3f v3v1 = vertexCoords[nPrevIndex] - vertexCoords[i];

	// The equation presented in the article states that:
	// c2c1_T = V2.texcoord.x – V1.texcoord.x
	// c2c1_B = V2.texcoord.y – V1.texcoord.y
	// c3c1_T = V3.texcoord.x – V1.texcoord.x
	// c3c1_B = V3.texcoord.y – V1.texcoord.y

	// Calculate c2c1_T and c2c1_B
	float c2c1_T = texCoords[nNextIndex].getX() - texCoords[i].getX();
	float c2c1_B = texCoords[nNextIndex].getY() - texCoords[i].getY();

	// Calculate c3c1_T and c3c1_B
	float c3c1_T = texCoords[nPrevIndex].getX() - texCoords[i].getX();
	float c3c1_B = texCoords[nPrevIndex].getY() - texCoords[i].getY();

	float fDenominator = c2c1_T * c3c1_B - c3c1_T * c2c1_B;
	if (fabs(fDenominator) < 0.5f)	
	{
		// We won't risk a divide by zero, so set the tangent matrix to the identity matrix
		tbn[0] = Vector3f(1.0f, 0.0f, 0.0f);
		tbn[1] = Vector3f(0.0f, 1.0f, 0.0f);
		tbn[2] = Vector3f(0.0f, 0.0f, 1.0f);
	}
	else
	{
		// Calculate the reciprocal value once and for all (to achieve speed)
		float fScale1 = 1.0f / fDenominator;

		// T and B are calculated just as the equation in the article states
		Vector3f T, B, N;
		T = Vector3f((c3c1_B * v2v1.getX() - c2c1_B * v3v1.getX()) * fScale1,
					(c3c1_B * v2v1.getY() - c2c1_B * v3v1.getY()) * fScale1,
					(c3c1_B * v2v1.getZ() - c2c1_B * v3v1.getZ()) * fScale1);

		B = Vector3f((-c3c1_T * v2v1.getX() + c2c1_T * v3v1.getX()) * fScale1,
					(-c3c1_T * v2v1.getY() + c2c1_T * v3v1.getY()) * fScale1,
					(-c3c1_T * v2v1.getZ() + c2c1_T * v3v1.getZ()) * fScale1);

		// The normal N is calculated as the cross product between T and B
		N = T.crossProduct(B);

		// Calculate the reciprocal value once and for all (to achieve speed)
		float fScale2 = 1.0f / ((T.getX() * B.getY() * N.getZ() - T.getZ() * B.getY() * N.getX()) + 
								(B.getX() * N.getY() * T.getZ() - B.getZ() * N.getY() * T.getX()) + 
								(N.getX() * T.getY() * B.getZ() - N.getZ() * T.getY() * B.getX()));
		
		// Calculate the inverse if the TBN matrix using the formula described in the article.
		// We store the basis vectors directly in the provided TBN matrix: pvTBNMatrix
		tbn[0].setX(B.crossProduct(N).getX() * fScale2);
		tbn[0].setY(-N.crossProduct(T).getX() * fScale2);
		tbn[0].setZ(T.crossProduct(B).getX() * fScale2);
		tbn[0].normalizeMe();

		tbn[1].setX(B.crossProduct(N).getY() * fScale2);
		tbn[1].setY(N.crossProduct(T).getY() * fScale2);
		tbn[1].setZ(-T.crossProduct(B).getY() * fScale2);
		tbn[1].normalizeMe();

		tbn[2].setX(B.crossProduct(N).getZ() * fScale2);
		tbn[2].setY(-N.crossProduct(T).getZ() * fScale2);
		tbn[2].setZ(T.crossProduct(B).getZ() * fScale2);
		tbn[2].normalizeMe();
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

	// Set the light position parameter in the vertex shader
	cgGLSetParameter3f(cgLightPosition, 0, 0, 2);

	// Set the diffuse of the light in the fragment shader
	cgGLSetParameter3f(cgLightDiffuseColor, 1.0f, 1.0f, 1.0f);
	//*/

	Vector3f TBN[3];
	Vector3f texCoords[3] = {Vector3f(0,0,1), Vector3f(0,1,1), Vector3f(1,0,1)};
	Vector3f vertices[3] = {Vector3f(1,0,0), Vector3f(0,-1,0), Vector3f(-1,0,0)};

	// Enable and bind the normal map
	glActiveTextureARB(GL_TEXTURE0_ARB);
	enableTexture(colorTex);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	enableTexture(bumpTex);

	computeTBN(vertices, texCoords, TBN);

	glBegin(GL_TRIANGLES);
//		glColor3f(1,0,0);
//		glTexCoord2f(0,0);
//		glVertex3f(1,0,0);
		for(int i=0; i<3; i++) {
			//glTexCoord2f(texCoords[i].getX(), texCoords[i].getY());
			// Set the texture coordinates for the rock texture and the normal map
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, texCoords[i].getX(), texCoords[i].getY());

			// Specify the tangent matrix vectors, one by one, for the first triangle and send them to the vertex shader
			glMultiTexCoord3fARB(GL_TEXTURE1_ARB, TBN[0].getX(), TBN[0].getY(), TBN[0].getZ());
			glMultiTexCoord3fARB(GL_TEXTURE2_ARB, TBN[1].getX(), TBN[1].getY(), TBN[1].getZ());
			glMultiTexCoord3fARB(GL_TEXTURE3_ARB, TBN[2].getX(), TBN[2].getY(), TBN[2].getZ());
			glVertex3f(vertices[i].getX(), vertices[i].getY(), vertices[i].getZ());
		}

//		glColor3f(0,1,0);
//		glTexCoord2f(0,1);
//		glVertex3f(0,-1,0);

//		glColor3f(0,0,1);
//		glTexCoord2f(1,0);
//		glVertex3f(-1,0,0);
	glEnd();


	glActiveTextureARB(GL_TEXTURE0_ARB);
	disableTexture();

	glActiveTextureARB(GL_TEXTURE1_ARB);
	disableTexture();

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
	cgLightPosition = cgGetNamedParameter(cgVertexShader, "lightPosition");

	cgGLLoadProgram(cgFragmentShader);
	cgColorTex = cgGetNamedParameter(cgFragmentShader, "colorTex");
	cgLightDiffuseColor = cgGetNamedParameter(cgFragmentShader, "lightDiffuseColor");

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
