#include "Render.h"

#include <windows.h>

#include <GL\gl.h>
#include <GL\glu.h>
#include "GL\glext.h"

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "MyShaders.h"

#include "ObjLoader.h"
#include "GUItextRectangle.h"

#include "Texture.h"

GuiTextRectangle rec, flowerText;

bool textureMode = true;
bool lightMode = true;
int x = 0;
int y = 0;
int z = 0;

double Gx = 0;
double Gy = 0;
double Gz = 0;


double Fx = 16;
double Fy = 16;
double Fz = 0;

int randX;
int randY;

double i = 0;
double flagH = 0;
double flagB = 0;
double flagN = 0;
double flagM = 0;
double flagF = 0;

int Count = 0;
//��������� ������ ��� ��������� ����
#define POP glPopMatrix()
#define PUSH glPushMatrix()


ObjFile *model;

Texture texture1;
Texture sTex;
Texture rTex;
Texture tBox;

Shader s[10];  //��������� ��� ������ ��������
Shader frac;
Shader cassini;




//����� ��� ��������� ������
class CustomCamera : public Camera
{
public:
	//��������� ������
	double camDist;
	//���� �������� ������
	double fi1, fi2;

	
	//������� ������ �� ���������
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	
	//������� ������� ������, ������ �� ����� ��������, ���������� �������
	virtual void SetUpCamera()
	{

		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist*cos(fi2)*cos(fi1),
			camDist*cos(fi2)*sin(fi1),
			camDist*sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //������� ������ ������


//����� ���������!
class WASDcamera :public CustomCamera
{
public:
		
	float camSpeed;

	WASDcamera()
	{
		camSpeed = 0.4;
		pos.setCoords(5, 5, 5);
		lookPoint.setCoords(0, 0, 0);
		normal.setCoords(0, 0, 1);
	}

	virtual void SetUpCamera()
	{

		if (OpenGL::isKeyPressed('W'))
		{
			Vector3 forward = (lookPoint - pos).normolize()*camSpeed;
			pos = pos + forward;
			lookPoint = lookPoint + forward;
			
		}
		if (OpenGL::isKeyPressed('S'))
		{
			Vector3 forward = (lookPoint - pos).normolize()*(-camSpeed);
			pos = pos + forward;
			lookPoint = lookPoint + forward;
			
		}

		LookAt();
	}

} WASDcam;


//����� ��� ��������� �����
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//��������� ������� �����
		pos = Vector3(1, 1, 3);
	}

	
	//������ ����� � ����� ��� ���������� �����, ���������� �������
	void  DrawLightGhismo()
	{
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		Shader::DontUseShaders();
		bool f1 = glIsEnabled(GL_LIGHTING);
		glDisable(GL_LIGHTING);
		bool f2 = glIsEnabled(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_2D);
		bool f3 = glIsEnabled(GL_DEPTH_TEST);
		
		glDisable(GL_DEPTH_TEST);
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.08;
		s.Show();

		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//����� �� ��������� ����� �� ����������
				glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//������ ���������
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale*1.5;
			c.Show();
		}
		/*
		if (f1)
			glEnable(GL_LIGHTING);
		if (f2)
			glEnable(GL_TEXTURE_2D);
		if (f3)
			glEnable(GL_DEPTH_TEST);
			*/
	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// ��������� ��������� �����
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// �������������� ����������� �����
		// ������� ��������� (���������� ����)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// ��������� ������������ �����
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// ��������� ���������� ������������ �����
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //������� �������� �����



//������ ���������� ����
int mouseX = 0, mouseY = 0;
int stop = 0;



float offsetX = 0, offsetY = 0;
float zoom=1;
float Time = 0;
int tick_o = 0;
int tick_n = 0;

//���������� �������� ����
void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//������ ���� ������ ��� ������� ����� ������ ����
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}


	if (OpenGL::isKeyPressed(VK_LBUTTON))
	{
		offsetX -= 1.0*dx/ogl->getWidth()/zoom;
		offsetY += 1.0*dy/ogl->getHeight()/zoom;
	}


	
	//������� ���� �� ���������, � ����� ��� ����
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y,60,ogl->aspect);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}

	
}

//���������� �������� ������  ����
void mouseWheelEvent(OpenGL *ogl, int delta)
{


	float _tmpZ = delta*0.003;
	if (ogl->isKeyPressed('Z'))
		_tmpZ *= 10;
	zoom += 0.2*zoom*_tmpZ;


	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01*delta;
}

ObjFile objModel, land, sheep, flower;

Texture landTex, sheepTex, flowerTex;


//���������� ������� ������ ����������
void keyDownEvent(OpenGL* ogl, int key)
{
	if (OpenGL::isKeyPressed('L'))
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}

	if (key == 'S')
	{
		frac.LoadShaderFromFile();
		frac.Compile();

		s[0].LoadShaderFromFile();
		s[0].Compile();

		cassini.LoadShaderFromFile();
		cassini.Compile();
	}

	if (key == 'Q') {
		Time = 0;
	}
	if (OpenGL::isKeyPressed('H')) {
		
		flagH = 1;
	}
	if (OpenGL::isKeyPressed('B')) {
		
		flagB = 1;
	}
	if (OpenGL::isKeyPressed('N')) {

		flagN = 1;
	}
	if (OpenGL::isKeyPressed('M')) {

		flagM = 1;
	}
}

void keyUpEvent(OpenGL *ogl, int key)
{

}


void DrawQuad()
{
	double A[] = { 0,0 };
	double B[] = { 1,0 };
	double C[] = { 1,1 };
	double D[] = { 0,1 };
	glBegin(GL_QUADS);
	glColor3d(.5, 0, 0);
	glNormal3d(0, 0, 1);
	glTexCoord2d(0, 0);
	glVertex2dv(A);
	glTexCoord2d(1, 0);
	glVertex2dv(B);
	glTexCoord2d(1, 1);
	glVertex2dv(C);
	glTexCoord2d(0, 1);
	glVertex2dv(D);
	glEnd();
}




//����������� ����� ������ ��������
void initRender(OpenGL *ogl)
{

	//��������� �������

	//4 ����� �� �������� �������
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//��������� ������ ��������� �������
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//�������� ��������
	glEnable(GL_TEXTURE_2D);
	
	


	//������ � ���� ����������� � "������"
	ogl->mainCamera = &camera;
	//ogl->mainCamera = &WASDcam;
	ogl->mainLight = &light;

	// ������������ �������� : �� ����� ����� ����� 1
	glEnable(GL_NORMALIZE);

	// ���������� ������������� ��� �����
	glEnable(GL_LINE_SMOOTH); 


	//   ������ ��������� ���������
	//  �������� GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  ������� � ���������� �������� ���������(�� ���������), 
	//                1 - ������� � ���������� �������������� ������� ��������       
	//                �������������� ������� � ���������� ��������� ����������.    
	//  �������� GL_LIGHT_MODEL_AMBIENT - ������ ������� ���������, 
	//                �� ��������� �� ���������
	// �� ��������� (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	/*
	//texture1.loadTextureFromFile("textures\\texture.bmp");   �������� �������� �� �����
	*/


	frac.VshaderFileName = "shaders\\v.vert"; //��� ����� ���������� ������
	frac.FshaderFileName = "shaders\\frac.frag"; //��� ����� ������������ �������
	frac.LoadShaderFromFile(); //��������� ������� �� �����
	frac.Compile(); //�����������

	cassini.VshaderFileName = "shaders\\v.vert"; //��� ����� ���������� ������
	cassini.FshaderFileName = "shaders\\cassini.frag"; //��� ����� ������������ �������
	cassini.LoadShaderFromFile(); //��������� ������� �� �����
	cassini.Compile(); //�����������
	

	s[0].VshaderFileName = "shaders\\v.vert"; //��� ����� ���������� ������
	s[0].FshaderFileName = "shaders\\light.frag"; //��� ����� ������������ �������
	s[0].LoadShaderFromFile(); //��������� ������� �� �����
	s[0].Compile(); //�����������

	s[1].VshaderFileName = "shaders\\v.vert"; //��� ����� ���������� ������
	s[1].FshaderFileName = "shaders\\textureShader.frag"; //��� ����� ������������ �������
	s[1].LoadShaderFromFile(); //��������� ������� �� �����
	s[1].Compile(); //�����������

	

	 //��� ��� ��� ������� ������ *.obj �����, ��� ��� ��� ��������� �� ���������� � ���������� �������, 
	 // ������������ �� ����� ����������, � ������������ ������ � *.obj_m
	//
	glActiveTexture(GL_TEXTURE0);
	loadModel("models\\sheep2.obj_m", &sheep);
	sheepTex.loadTextureFromFile("textures//sheep.bmp");
	sheepTex.bindTexture();

	
	loadModel("models\\sveta.obj_m", &land);
	landTex.loadTextureFromFile("textures//land.bmp");
	landTex.bindTexture();

	loadModel("models\\flower.obj_m", &flower);
	flowerTex.loadTextureFromFile("textures//flower.bmp");
	flowerTex.bindTexture();


	tick_n = GetTickCount();
	tick_o = tick_n;

	rec.setSize(300, 100);
	rec.setPosition(10, ogl->getHeight() - 100-10);
	rec.setText("G - ������� ���� �� �����������\nG+��� ������� ���� �� ���������\nH - ������\nN - �����\nB - ������\nM - �����");
	
}




void Render(OpenGL* ogl)
{

	tick_o = tick_n;
	tick_n = GetTickCount();
	Time += (tick_n - tick_o) / 1000.0;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);

	//��������� ���������
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;

	//�������
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//��������
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//����������
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	//������ �����
	glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//===================================
	//������� ���  
	//������

	if (flagF == 1)
		Count++;
	if (flagF == 1) {
		while ((Fx == Gx) && (Fy == Gy)) {
			randX = rand() % (3 - 1 + 1) + 1;
			randY = rand() % (3 - 1 + 1) + 1;

			switch (randX)
			{
			case 1: {
				Fx = 0;
				flagF = 0;
				break;
			}
			case 2: {
				flagF = 0;
				Fx = 8;
				break;
			}

			default: {
				flagF = 0;
				Fx = 16;
				break; }
			}

			switch (randY)
			{
			case 1: {
				flagF = 0;
				Fy = 0;
				break;
			}
			case 2: {
				flagF = 0;
				Fy = 8;
				break;
			}

			default: {
				flagF = 0;
				Fy = 16;
				break; }
			}
		}
	}
	s[1].UseShader();
	glPushMatrix();
	glTranslated(Fy, Fx, 1.8);
	flowerTex.bindTexture();
	flower.DrawObj();
	glPopMatrix();



	s[1].UseShader();
	if ((flagH == 0 && flagB == 0 && flagM == 0 && flagN == 0)) {	
		glPushMatrix();
		glTranslated(Gy, Gx, 2.8);
		sheepTex.bindTexture();
		sheep.DrawObj();
		glPopMatrix();
	}

	if (((flagH == 1)&&(Gx == 16)) || ((flagB == 1)&&(Gy == 16)))
	{
		if (Gx == 16)
			flagH = 0;
		if (Gy == 16)
			flagB = 0;
		glPushMatrix();
		glTranslated(Gy, Gx, 2.8);
		sheepTex.bindTexture();
		sheep.DrawObj();
		glPopMatrix();
	}
	if (((flagM == 1) && (Gy == 0))|| ((flagN == 1)&&(Gx == 0))) {
		if (Gx == 0)
			flagN = 0;
		if (Gy == 0)
			flagM = 0;
		glPushMatrix();
		glTranslated(Gy, Gx, 2.8);
		sheepTex.bindTexture();
		sheep.DrawObj();
		glPopMatrix();
	}


	if (flagH == 1) {
		if (x != 16)
		while (Gx < x+8.5){
			glPushMatrix();
			glTranslated(Gy, Gx, (2 * sin(i)+2.8));
			Sleep(50);
			sheepTex.bindTexture();
			sheep.DrawObj();
			glPopMatrix();
			i += 0.2;
			Gx += 0.5;
			break;
		}	
		if (Gx == x + 8) {
			flagH = 0;
	
			x = Gx;
			i = 0;
		}
		if ((Gx == Fx) && (Gy == Fy)) {
			flagF = 1;
		}
	}

	if (flagB == 1) {
		
		if (y != 16)
			glPushMatrix();
		
			while (Gy < y + 8.5) {
				
				
				glTranslated(Gy, Gx, (2 * sin(i) + 2.8));
				Sleep(50);
				sheepTex.bindTexture();
				glRotated(-90, 0, 0, 1);
				sheep.DrawObj();
				
				i += 0.2;
				Gy += 0.5;
				break;
			}
			glPopMatrix();
		if (Gy == y + 8) {
			flagB = 0;
			y = Gy;
			i = 0;
		}
		if ((Gx == Fx) && (Gy == Fy)) {
			flagF = 1;
		}
	}
		if (flagN == 1) {
			if (x != 0) {
				while (Gx > x - 8.5) {
					glPushMatrix();

					glTranslated(Gy, Gx, (2 * sin(i) + 2.8));
					Sleep(50);
					sheepTex.bindTexture();
					glRotated(-180, 0, 0, 1);
					sheep.DrawObj();
					glPopMatrix();
					i += 0.2;
					Gx -= 0.5;
					break;
				}
				if (Gx == x - 8) {
					flagN = 0;
					x = Gx;
					i = 0;
				}
			}
			if ((Gx == Fx) && (Gy == Fy)) {
				flagF = 1;
			}
		}

		if (flagM == 1) {
			/*glRotated(45, 0, 1, 0);*/
			if (y != 0)
				while (Gy > y - 8.5) {
					glPushMatrix();
					
					glTranslated(Gy, Gx, (2 * sin(i) + 2.8));
					Sleep(50);
					sheepTex.bindTexture();
					glRotated(90, 0, 0, 1);
					sheep.DrawObj();
					glPopMatrix();
					i += 0.2;
					Gy -= 0.5;
					break;
				}
			if (Gy == y - 8) {
				flagM = 0;
				y = Gy;
				i = 0;
			}
			if ((Gx == Fx) && (Gy == Fy)) {
				flagF = 1;
			}
	}


	s[0].UseShader();

	//�������� ���������� � ������.  ��� ���� - ���� ����� uniform ���������� �� �� �����. 
	int location = glGetUniformLocationARB(s[0].program, "light_pos");
	//��� 2 - �������� �� ��������
	glUniform3fARB(location, light.pos.X(), light.pos.Y(),light.pos.Z());

	location = glGetUniformLocationARB(s[0].program, "Ia");
	glUniform3fARB(location, 0.2, 0.2, 0.2);

	location = glGetUniformLocationARB(s[0].program, "Id");
	glUniform3fARB(location, 1.0, 1.0, 1.0);

	location = glGetUniformLocationARB(s[0].program, "Is");
	glUniform3fARB(location, .7, .7, .7);


	location = glGetUniformLocationARB(s[0].program, "ma");
	glUniform3fARB(location, 0.2, 0.2, 0.1);

	location = glGetUniformLocationARB(s[0].program, "md");
	glUniform3fARB(location, 0.4, 0.65, 0.5);

	location = glGetUniformLocationARB(s[0].program, "ms");
	glUniform4fARB(location, 0.9, 0.8, 0.3, 25.6);

	location = glGetUniformLocationARB(s[0].program, "camera");
	glUniform3fARB(location, camera.pos.X(), camera.pos.Y(), camera.pos.Z());


	Shader::DontUseShaders();
	

	s[1].UseShader();
	glPushMatrix();
	glRotated(-90, 0, 0, 1);
	landTex.bindTexture();
	land.DrawObj();
	glPopMatrix();

	glTranslated(8, 0, 0);
	landTex.bindTexture();
	land.DrawObj();
	glPopMatrix();
	
	glTranslated(8, 0, 0);
	landTex.bindTexture();
	land.DrawObj();
	glPopMatrix();

	glTranslated(0, 8, 0);
	landTex.bindTexture();
	land.DrawObj();
	glPopMatrix();

	glTranslated(0, 8, 0);
	landTex.bindTexture();
	land.DrawObj();
	glPopMatrix();

	glTranslated(-8, 0, 0);
	landTex.bindTexture();
	land.DrawObj();
	glPopMatrix();

	glTranslated(-8, 0, 0);
	landTex.bindTexture();
	land.DrawObj();
	glPopMatrix();

	glTranslated(0, -8, 0);
	landTex.bindTexture();
	land.DrawObj();
	glPopMatrix();

	glTranslated(8, 0, 0);
	landTex.bindTexture();
	land.DrawObj();
	glPopMatrix();

	
	Shader::DontUseShaders();

	
	
}   //����� ���� �������


bool gui_init = false;

//������ ���������, ��������� ����� �������� �������
void RenderGUI(OpenGL *ogl)
{
	
	Shader::DontUseShaders();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	

	glActiveTexture(GL_TEXTURE0);
	rec.Draw();


		
	Shader::DontUseShaders(); 



	
}

void resizeEvent(OpenGL *ogl, int newW, int newH)
{
	rec.setPosition(10, newH - 100 - 10);
}

