#include <GLTools.h>
#include <gl/glew.h>
#include <gl/glut.h>
#include <gl/glext.h>
#include <GL/GLAUX.H>
#include <cmath>
#include<stdio.h>

#pragma comment (lib,"advapi32.lib")
GLint sun_self_rot = 0;
GLint earth_circle_rot = 0;
GLint earth_self_rot = 0;
GLuint uSunTexture;
GLuint uEarthTexture;
GLUquadricObj* sunTexture;
GLUquadricObj* earthTexture;
AUX_RGBImageRec* sunImage = nullptr;
AUX_RGBImageRec* earthImage = nullptr;


void displayInit()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	sunTexture = gluNewQuadric();
	gluQuadricNormals(sunTexture, GL_SMOOTH);
	gluQuadricTexture(sunTexture, GL_TRUE);

	earthTexture = gluNewQuadric();
	gluQuadricNormals(earthTexture, GL_SMOOTH);
	gluQuadricTexture(earthTexture, GL_TRUE);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	sunImage = auxDIBImageLoad(L"sunmap.tga");
	glGenTextures(1, &uSunTexture);
	glBindTexture(GL_TEXTURE_2D, uSunTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, sunImage->sizeX, sunImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, sunImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	earthImage = auxDIBImageLoad(L"earthmap.tga");
	glGenTextures(1, &uEarthTexture);
	glBindTexture(GL_TEXTURE_2D, uEarthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, earthImage->sizeX, earthImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, earthImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

}

void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	{
		GLfloat sun_light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
		GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);
	}
	{
		GLfloat sun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_mat_diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_mat_emission[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		GLfloat sun_mat_shininess = 30.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);
		glRotatef((GLfloat)sun_self_rot, 0, 1, 0);
		glBindTexture(GL_TEXTURE_2D, uSunTexture);
		gluSphere(sunTexture, 1, 40, 32);
	}
	glPopMatrix();


	glPushMatrix();
	{
		GLfloat earth_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat earth_mat_diffuse[] = { 0.0f, 0.0f, 0.5f, 1.0f };
		GLfloat earth_mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat earth_mat_emission[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		GLfloat earth_mat_shininess = 30.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, earth_mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, earth_mat_shininess);
	}
	glBegin(GL_LINE_LOOP);
	GLfloat earth_track_angle = 0;
	GLint earth_track_num = 100;
	GLfloat earth_angle_step = 2 * 3.14f / earth_track_num;
	for (earth_track_angle = 0; earth_track_angle < 2 * 3.14; earth_track_angle += earth_angle_step)
		glVertex3f(3 * sin(earth_track_angle), 0.0, 3 * cos(earth_track_angle));
	glEnd();
	glRotatef((GLfloat)earth_circle_rot, 0, 1, 0);
	glTranslatef(3, 0, 0);
	glRotatef((GLfloat)earth_self_rot, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, uEarthTexture);
	gluSphere(earthTexture, 0.3, 40, 32);
	glPopMatrix();

	glutSwapBuffers();
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glFrustum(-1,1,-1,1,1.5,20.0);  
	gluLookAt(0, 5, 5, 0, 0, 0, 0, 1, 0);
}
void changeAngle(int id)
{
	earth_circle_rot = (earth_circle_rot + 2) % 360;
	earth_self_rot = (earth_self_rot + 2) % 360;
	sun_self_rot = (sun_self_rot + 1) % 360;
	glutPostRedisplay();
	glutTimerFunc(33, changeAngle, 1);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("¹âÕÕºÍÎÆÀí");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}
	displayInit();
	glutTimerFunc(1, changeAngle, 1);
	glutMainLoop();
	return 0;
}
