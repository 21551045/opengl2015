#include <GLTools.h>
#include <GLMatrixStack.h>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLGeometryTransform.h>
#include <StopWatch.h>
#include <math.h>
#ifdef __APPLE__
#include <glut/glut.h>
#else
#include <GL/glut.h>
#endif

#pragma comment(lib,"glew32.lib")

GLFrame             frame;
GLMatrixStack       viewMatrix;
GLMatrixStack       projectionMatrix;
GLFrustum           frustum;
GLGeometryTransform transform;
GLShaderManager     shaderManager;
GLTriangleBatch     triangleBatch;

GLuint	shader;// ADS光线着色器
GLint	ambient;// 环境光颜色
GLint   diffuse;// 漫反射颜色
GLint   specular;// 镜面反射颜色
GLint	light;// 视点坐标系下光源坐标
GLint	mvp;// 模型视图投影矩阵
GLint	mv;// 模型视图矩阵
GLint	nm;// 模型视图矩阵旋转分量

void RenderScene(void) {
	static CStopWatch rotTimer;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除颜色缓存区和深度缓冲区
	viewMatrix.PushMatrix(frame);
	{
		viewMatrix.Rotate(rotTimer.GetElapsedSeconds() * 10.0f, 0.0f, 1.0f, 0.0f);
		GLfloat vEyeLight[] = { -100.0f, 100.0f, 100.0f };
		GLfloat vambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
		GLfloat vdiffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		GLfloat vspecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

		glUseProgram(shader);
		glUniform4fv(ambient, 1, vambient);
		glUniform4fv(diffuse, 1, vdiffuse);
		glUniform4fv(specular, 1, vspecular);
		glUniform3fv(light, 1, vEyeLight);
		glUniformMatrix4fv(mvp, 1, GL_FALSE, transform.GetModelViewProjectionMatrix());
		glUniformMatrix4fv(mv, 1, GL_FALSE, transform.GetModelViewMatrix());
		glUniformMatrix3fv(nm, 1, GL_FALSE, transform.GetNormalMatrix());
		triangleBatch.Draw();
	}
	viewMatrix.PopMatrix();
	glutSwapBuffers();
	glutPostRedisplay();
}

void SetupRC(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	shaderManager.InitializeStockShaders();
	frame.MoveForward(4.0f);

	gltMakeSphere(triangleBatch, 1.0f, 26, 13);

	shader = shaderManager.LoadShaderPairWithAttributes("shader.vp", "shader.fp", 2, GLT_ATTRIBUTE_VERTEX, "vVertex",
		GLT_ATTRIBUTE_NORMAL, "vNormal"); // 加载着色器程序

	ambient = glGetUniformLocation(shader, "ambient");
	diffuse = glGetUniformLocation(shader, "diffuse");
	specular = glGetUniformLocation(shader, "specular");
	light = glGetUniformLocation(shader, "vLightPosition");
	mvp = glGetUniformLocation(shader, "mvpMatrix"); 
	mv = glGetUniformLocation(shader, "mvMatrix");
	nm = glGetUniformLocation(shader, "normalMatrix");
}

void ShutdownRC(void) {

}


void ChangeSize(int w, int h) {
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	frustum.SetPerspective(35.0f, float(w) / float(h), 1.0f, 100.0f);
	projectionMatrix.LoadMatrix(frustum.GetProjectionMatrix());
	transform.SetMatrixStacks(viewMatrix, projectionMatrix);
}


int main(int argc, char* argv[]) {
	gltSetWorkingDirectory(argv[0]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(800, 600);
	glutCreateWindow("texture and light");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}

	SetupRC();
	glutMainLoop();
	ShutdownRC();
	return 0;
}