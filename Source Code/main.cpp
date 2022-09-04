// Template untuk mainan objek 3D 
// Kamera bisa maju mundur kiri kanan 
// Sudah dilengkapi pencahayaan 
// Sekarang pake texture 

#include <math.h> 
#include <gl/glut.h> 
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <assert.h>
#include <fstream>
#include "imageloader.h"
#define EN_SIZE 15

GLuint _textureId, _textureIdDepan, _textureIdKanan, _textureIdBelakang, _textureIdKiri, _textureIdAtas; //ID OpenGL untuk tekstur
float angle=0.0, deltaAngle = 0.0, ratio; 
float x=0.0f,y=20.0f,z=95.0f; // posisi awal kamera 
float lx=0.0f,ly=0.0f,lz=-1.0f; // arah kamera
int deltaMove = 0,h,w; // maju mundur kamera
static int rotAngleX =0, rotAngleY =0, rotAngleZ =0; // rotasi kamera
int bitmapHeight=12;
float panjang, lebar, tinggi;
const double PI = 3.141592653589793;
int i,j,radius,jumlah_titik,x_tengah,y_tengah;
bool start = false;
float r[] = {0.1,0.4,0.0,0.9,0.2,0.5,0.0,0.7,0.5,0.0};
float g[] = {0.2,0.0,0.4,0.5,0.2,0.0,0.3,0.9,0.0,0.2};
float b[] = {0.4,0.5,0.0,0.7,0.9,0.0,0.1,0.2,0.5,0.0};
int tola[5000][5000];
float tX=0,tY=0,tZ=-8,rX=0,rY=0,rZ=14;
float tZ1=-20,tZ2=-40,tZ3=-60,tZ4=-80,tZ5=-100,tZ6=-120,tZ7=-140,tZ8=-160,tZ9=-180,tZ10=-200;
float yappari = 15;
int belok = 0;
float angTrukDepan = 0.0, angTrukBelakang = 0;
float posXTruk = 0.0, posZTruk = 0.0;
float speed = 1;
float posZJalan = -100,posZPohon = -100,posZRumah = -250,posZMobil = 200; //Posisi awal tiap object
int ujung = 0, ujungP = 0, ujungR = 0, ujungM = 0;
bool touch = false;


// Fungsi menginisialisasi ID OpenGL tekstur
GLuint loadTexture(Image* image)
{
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return textureId;
}

// Fungsi reshape
void Reshape(int w1, int h1) 
{
	if(h1 == 0) h1 = 1; 
	w = w1; 
	h = h1; 
	ratio = 1.0f * w / h; 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	glViewport(0, 0, w, h); 
	gluPerspective(45,ratio,0.1,1000); 
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 
	gluLookAt(x, y, z, x + lx,y + ly,z + lz, 0.0f,1.0f,0.0f); 
} 

// Fungsi ini untuk memutar arah kamera (tengok kiri/kanan) 
void orientMe(float ang) 
{
	lx = sin(ang); 
	lz = -cos(ang); 
	glLoadIdentity(); 
	gluLookAt(x, y, z, x + lx,y + ly,z + lz, 0.0f,1.0f,0.0f); 
} 

// Fungsi ini untuk maju mundur kamera 
void moveMeFlat(int i) 
{ 
	x = x + i*(lx)*0.5; 
	z = z + i*(lz)*0.5; 
	glLoadIdentity(); 
	gluLookAt(x, y, z, x + lx,y + ly,z + lz, 0.0f,1.0f,0.0f); 
} 


// Fungsi ini akan dijalankan saat tombol keyboard ditekan dan belum dilepas
void pressKey(int key, int x, int y) 
{  
 	// Selama tombol ditekan, variabel angle dan move diubah => kamera bergerak 
 	switch (key) { 
 		case GLUT_KEY_LEFT : 
		 	if (touch) deltaAngle = -0.02f;
			else deltaAngle = -0.1f;
			break; 
 		case GLUT_KEY_RIGHT :
		 	if (touch) deltaAngle = 0.02f;
			else deltaAngle = 0.1f;
			break;
 		case GLUT_KEY_UP : 
			if (touch) deltaMove = 1;
			else deltaMove = 2; 
			break; 
 		case GLUT_KEY_DOWN : 
			if (touch) deltaMove = -1;
			else deltaMove = -2; 
			break; 
 	} 
} 

void releaseKey(int key, int x, int y) 
{ 
 	// Fungsi ini akan dijalankan saat tekanan tombol keyboard dilepas 
 	// Saat tombol dilepas, variabel angle dan move diset nol => kamera berhenti 
 	switch (key) { 
 		case GLUT_KEY_LEFT : if (deltaAngle < 0.0f) 
			deltaAngle = 0.0f; 
 			break; 
 		case GLUT_KEY_RIGHT : if (deltaAngle > 0.0f) 
 			deltaAngle = 0.0f; 
			break; 
 		case GLUT_KEY_UP : if (deltaMove > 0) 
			deltaMove = 0; 
 			break; 
		case GLUT_KEY_DOWN : if (deltaMove < 0) 
 			deltaMove = 0; 
			break; 
 	} 
}


// Fungsi untuk membuat grid di "lantai" 
void Grid() 
{ 
	double i; 
	const float Z_MIN = -50, Z_MAX = 50; 
	const float X_MIN = -50, X_MAX = 50; 
	const float gap = 1.5; 
	glColor3f(0.5, 0.5, 0.5); 
	glBegin(GL_LINES); 
	for(i=Z_MIN;i<Z_MAX;i+=gap) 
	{ 
		glVertex3f(i, 0, Z_MIN); 
		glVertex3f(i, 0, Z_MAX); 
	} 
	for(i=X_MIN;i<X_MAX;i+=gap) 
	{ 
		glVertex3f(X_MIN, 0, i); 
		glVertex3f(X_MAX, 0, i); 
	} 
	glEnd(); 
} 

// Fungsi untuk membuat pohon jenis 1
void Pohon1() 
{
	GLUquadricObj *pObj;
	pObj =gluNewQuadric();
	gluQuadricNormals(pObj, GLU_SMOOTH);
	
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glColor3f(0.7,0.3,0);
	glRotatef(270,1,0,0);
	glTranslatef(-30,7,0);
	gluCylinder(pObj, 1, 0.7, 10, 20, 15);
	glPopMatrix();
	glPushMatrix();
	glColor3ub(104,70,14);
	glTranslatef(-30,7,-7);
	glRotatef(330,1,0,0);
	gluCylinder(pObj, 0.6, 0.1, 7, 25, 25);
	glPopMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glColor3f(0,0.3,0.06);
	glScaled(4, 3, 5);
	glTranslatef(-7.4,4.7,-1);
	glutSolidDodecahedron();
	glPopMatrix();
}

// Fungsi untuk membuat Rumah
void Rumah() 
{
	glRotated(-90,0,1,0);
	glTranslatef(0,0,-20);
	
	//atap
	glPushMatrix();
	glScaled(6,4,3);
	glTranslatef(0.0, 2.5, -5);
	glRotated(45, 0, 1, 0);
	glRotated(-90, 1, 0, 0);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3d(0.803921568627451, 0.5215686274509804, 0.2470588235294118);
	glutSolidCone(4.2, 1.5, 4, 1);
	glPopMatrix();
	//lantai 1
	glPushMatrix();
	glScaled(6, 2, 3);
	glTranslatef(0, 2.5, -5);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(0.3402, 0.3412, 0.3117);
	glutSolidCube(5.0);
	glPopMatrix();
	//pintu 
	glPushMatrix();
	glScaled(1, 1.5, 0.035);
	glTranslatef(0, 2.5, -215);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(0.0980, 0.0608, 0.0077);
	glutSolidCube(5.0);
	glPopMatrix();
}

void TrukDepan()
{
	//Bagian kepala
	glPushMatrix(); 
 	 panjang = 4;
 	 lebar = 6;
 	 tinggi = 5;
	 glTranslatef(0,1.5,0+yappari);
	 glColor3f(0.5f,0,0.0f);

	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,tinggi,0.0f); 
	 glVertex3f(-lebar/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // atas 
	 glPushMatrix(); 
	 glRotated(-90, 1, 0, 0); 
	 glTranslatef(0,0,tinggi); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,-panjang/2,0.0f); 
	 glVertex3f(lebar/2,-panjang/2,0.0f); 
	 glVertex3f(lebar/2,panjang/2,0.0f); 
	 glVertex3f(-lebar/2,panjang/2,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // belakang 
	 glPushMatrix(); 
	 glRotated(-180, 1, 0, 0);
	 glTranslatef(0,0,panjang/2); 
	 glBegin(GL_QUADS);  
	 glVertex3f(-lebar/2,-tinggi,0.0f); 
	 glVertex3f(lebar/2,-tinggi,0.0f); 
	  
	 glVertex3f(lebar/2,0.0f,0.0f); 
	  
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // bawah 
	 glPushMatrix(); 
	 glRotated(90, 1, 0, 0); 
	 glTranslatef(0,0,0); 
	 glBegin(GL_QUADS); 
	  
	 glVertex3f(-lebar/2,-panjang/2,0.0f); 
	  
	 glVertex3f(lebar/2,-panjang/2,0.0f); 
	  
	 glVertex3f(lebar/2,panjang/2,0.0f); 
	  
	 glVertex3f(-lebar/2,panjang/2,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // kiri 
	 glPushMatrix(); 
	 glRotated(-90, 0, 1, 0); 
	 glTranslatef(0,0,lebar/2); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,tinggi,0.0f); 
	 glVertex3f(-panjang/2,tinggi,0.0f); 
	 glEnd();
	 glPopMatrix(); 

	 // kanan 
	 glPushMatrix(); 
	 glRotated(90, 0, 1, 0); 
	 glTranslatef(0,0,lebar/2); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,tinggi,0.0f); 
	 glVertex3f(-panjang/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix();
 glPopMatrix();
 
 glPushMatrix(); //Bagian Lampu depan kiri
 	 panjang = 0.1;
 	 lebar = 0.9;
 	 tinggi = 0.4;
	 glTranslatef(2.3,1,2.01+yappari);
	 glColor3f(1,1,1);

	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,tinggi,0.0f); 
	 glVertex3f(-lebar/2+0.09,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
 glPopMatrix();
 
 glPushMatrix(); //Bagian Lampu depan kanan
 	 panjang = 0.1;
 	 lebar = 0.9;
 	 tinggi = 0.4;
	 glTranslatef(-2.3,1,2.01+yappari);
	 glColor3f(1,1,1);

	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2-0.09,tinggi,0.0f); 
	 glVertex3f(-lebar/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
 glPopMatrix();
 
 glPushMatrix(); //Bagian Lampu sen depan kiri
 	 panjang = 0.1;
 	 lebar = 0.25;
 	 tinggi = 0.4;
	 glTranslatef(2.85,1,2.01+yappari);
	 glColor3f(1,0.5,0);

	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,tinggi,0.0f); 
	 glVertex3f(-lebar/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
 glPopMatrix();
 
 glPushMatrix(); //Bagian Lampu sen depan kanan
 	 panjang = 0.1;
 	 lebar = 0.25;
 	 tinggi = 0.4;
	 glTranslatef(-2.85,1,2.01+yappari);
	 glColor3f(1,0.5,0);

	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,tinggi,0.0f); 
	 glVertex3f(-lebar/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
 glPopMatrix();
 
 glPushMatrix(); //Bagian grill depan
 	 panjang = 0.1;
 	 lebar = 5.7;
 	 tinggi = 2.35;
	 glTranslatef(0,1,2.01+yappari);
	 glColor3f(0.1,0.1,0.1);

	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2+1,0.0f,0.0f); 
	 glVertex3f(lebar/2-1,0.0f,0.0f); 
	 glVertex3f(lebar/2-0.5,tinggi,0.0f); 
	 glVertex3f(-lebar/2+0.5,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
 glPopMatrix();
 
 glPushMatrix(); //Bagian grill depan 1
 	 panjang = 0.1;
 	 lebar = 3;
 	 tinggi = 0.2;
	 glTranslatef(0,1.2,2.02+yappari);
	 glColor3f(0,0,0);

	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,tinggi,0.0f); 
	 glVertex3f(-lebar/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
 glPopMatrix();
 
 glPushMatrix(); //Bagian grill depan 2
 	 panjang = 0.1;
 	 lebar = 3;
 	 tinggi = 0.2;
	 glTranslatef(0,1.6,2.02+yappari);
	 glColor3f(0,0,0);

	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,tinggi,0.0f); 
	 glVertex3f(-lebar/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
 glPopMatrix();
 
 glPushMatrix(); //Bagian grill depan 3
 	 panjang = 0.1;
 	 lebar = 3;
 	 tinggi = 0.2;
	 glTranslatef(0,2.0,2.02+yappari);
	 glColor3f(0,0,0);

	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,tinggi,0.0f); 
	 glVertex3f(-lebar/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
 glPopMatrix();
 
 glPushMatrix(); //Bagian grill depan 4
 	 panjang = 0.1;
 	 lebar = 3.5;
 	 tinggi = 0.2;
	 glTranslatef(0,2.4,2.02+yappari);
	 glColor3f(0,0,0);

	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,tinggi,0.0f); 
	 glVertex3f(-lebar/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
 glPopMatrix();

 glPushMatrix(); //Bagian grill depan 5
 	 panjang = 0.1;
 	 lebar = 3.5;
 	 tinggi = 0.2;
	 glTranslatef(0,2.8,2.02+yappari);
	 glColor3f(0,0,0);

	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,tinggi,0.0f); 
	 glVertex3f(-lebar/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
 glPopMatrix();
 
 glPushMatrix(); //Bagian Kaca depan
 	 panjang = 0.1;
 	 lebar = 5.7;
 	 tinggi = 2.3;
	 glTranslatef(0,3.5,2+yappari);
	 glColor3f(0.3,0.3,0.3);

	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,tinggi,0.0f); 
	 glVertex3f(-lebar/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // atas 
	 glPushMatrix(); 
	 glRotated(-90, 1, 0, 0); 
	 glTranslatef(0,0,tinggi); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,-panjang/2,0.0f); 
	 glVertex3f(lebar/2,-panjang/2,0.0f); 
	 glVertex3f(lebar/2,panjang/2,0.0f); 
	 glVertex3f(-lebar/2,panjang/2,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // belakang 
	 glPushMatrix(); 
	 glRotated(-180, 1, 0, 0);
	  glTranslatef(0,0,panjang/2); 
	 glBegin(GL_QUADS); 
	  
	 glVertex3f(-lebar/2,-tinggi,0.0f); 
	   
	 glVertex3f(lebar/2,-tinggi,0.0f); 
	  
	 glVertex3f(lebar/2,0.0f,0.0f); 
	  
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // bawah 
	 glPushMatrix(); 
	 glRotated(90, 1, 0, 0); 
	 glTranslatef(0,0,0); 
	 glBegin(GL_QUADS); 
	  
	 glVertex3f(-lebar/2,-panjang/2,0.0f); 
	   
	 glVertex3f(lebar/2,-panjang/2,0.0f); 
	  
	 glVertex3f(lebar/2,panjang/2,0.0f); 
	  
	 glVertex3f(-lebar/2,panjang/2,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // kiri 
	 glPushMatrix(); 
	 glRotated(-90, 0, 1, 0); 
	 glTranslatef(0,0,lebar/2); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,tinggi,0.0f); 
	 glVertex3f(-panjang/2,tinggi,0.0f); 
	 glEnd();
	 glPopMatrix(); 

	 // kanan 
	 glPushMatrix(); 
	 glRotated(90, 0, 1, 0); 
	 glTranslatef(0,0,lebar/2); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,tinggi,0.0f); 
	 glVertex3f(-panjang/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix();
 glPopMatrix(); 
 
 glPushMatrix(); //Bagian Kaca kiri
 	 panjang = 0.1;
 	 lebar = 3;
 	 tinggi = 2.3;
	 glTranslatef(3,3.5,0+yappari);
	 glRotated(90,0,1,0);
	 glColor3f(0.3,0.3,0.3);

	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,0.8f,0.0f); 
	 glVertex3f(lebar/2,tinggi,0.0f); 
	 glVertex3f(-lebar/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
 glPopMatrix(); 
 
 glPushMatrix(); //Bagian Kaca kanan
 	 panjang = 0.1;
 	 lebar = 3;
 	 tinggi = 2.3;
	 glTranslatef(-3,3.5,0+yappari);
	 glRotated(-90,0,1,0);
	 glColor3f(0.3,0.3,0.3);
	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.8f,0.0f); 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,tinggi,0.0f); 
	 glVertex3f(-lebar/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
 glPopMatrix(); 
 
 glPushMatrix(); //Bagian Bemper depan
 	 panjang = 1.7;
 	 lebar = 6;
 	 tinggi = -0.95;
	 glTranslatef(0,1.5,1.2+yappari);
	 glColor3f(0.5f,0,0.0f);

	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,tinggi,0.0f); 
	 glVertex3f(-lebar/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // atas 
	 glPushMatrix(); 
	 glRotated(-90, 1, 0, 0); 
	 glTranslatef(0,0,tinggi); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,-panjang/2,0.0f); 
	 glVertex3f(lebar/2,-panjang/2,0.0f); 
	 glVertex3f(lebar/2,panjang/2,0.0f); 
	 glVertex3f(-lebar/2,panjang/2,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // belakang 
	 glPushMatrix(); 
	 glRotated(-180, 1, 0, 0);
	  glTranslatef(0,0,panjang/2); 
	 glBegin(GL_QUADS); 
	  
	 glVertex3f(-lebar/2,-tinggi,0.0f); 
	   
	 glVertex3f(lebar/2,-tinggi,0.0f); 
	  
	 glVertex3f(lebar/2,0.0f,0.0f); 
	  
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // bawah 
	 glPushMatrix(); 
	 glRotated(90, 1, 0, 0); 
	 glTranslatef(0,0,0); 
	 glBegin(GL_QUADS); 
	  
	 glVertex3f(-lebar/2,-panjang/2,0.0f); 
	   
	 glVertex3f(lebar/2,-panjang/2,0.0f); 
	  
	 glVertex3f(lebar/2,panjang/2,0.0f); 
	  
	 glVertex3f(-lebar/2,panjang/2,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // kiri 
	 glPushMatrix(); 
	 glRotated(-90, 0, 1, 0); 
	 glTranslatef(0,0,lebar/2); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,tinggi,0.0f); 
	 glVertex3f(-panjang/2,tinggi,0.0f); 
	 glEnd();
	 glPopMatrix(); 

	 // kanan 
	 glPushMatrix(); 
	 glRotated(90, 0, 1, 0); 
	 glTranslatef(0,0,lebar/2); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,tinggi,0.0f); 
	 glVertex3f(-panjang/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix();
 glPopMatrix();
   

 glPushMatrix(); //Bagian bawah angkutan
 	 panjang = 17;
 	 lebar = 6;
 	 tinggi = 0.5;
	 glTranslatef(0,1,-10.8+yappari);
	 glColor3f(1.0f,0.5f,0.0f);

	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,tinggi,0.0f); 
	 glVertex3f(-lebar/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // atas 
	 glPushMatrix(); 
	 glRotated(-90, 1, 0, 0); 
	 glTranslatef(0,0,tinggi); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,-panjang/2,0.0f); 
	 glVertex3f(lebar/2,-panjang/2,0.0f); 
	 glVertex3f(lebar/2,panjang/2,0.0f); 
	 glVertex3f(-lebar/2,panjang/2,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // belakang 
	 glPushMatrix(); 
	 glRotated(-180, 1, 0, 0);
	  glTranslatef(0,0,panjang/2); 
	 glBegin(GL_QUADS); 
	  
	 glVertex3f(-lebar/2,-tinggi,0.0f); 
	   
	 glVertex3f(lebar/2,-tinggi,0.0f); 
	  
	 glVertex3f(lebar/2,0.0f,0.0f); 
	  
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // bawah 
	 glPushMatrix(); 
	 glRotated(90, 1, 0, 0); 
	 glTranslatef(0,0,0); 
	 glBegin(GL_QUADS); 
	  
	 glVertex3f(-lebar/2,-panjang/2,0.0f); 
	   
	 glVertex3f(lebar/2,-panjang/2,0.0f); 
	  
	 glVertex3f(lebar/2,panjang/2,0.0f); 
	  
	 glVertex3f(-lebar/2,panjang/2,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // kiri 
	 glPushMatrix(); 
	 glRotated(-90, 0, 1, 0); 
	 glTranslatef(0,0,lebar/2); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,tinggi,0.0f); 
	 glVertex3f(-panjang/2,tinggi,0.0f); 
	 glEnd();
	 glPopMatrix(); 

	 // kanan 
	 glPushMatrix(); 
	 glRotated(90, 0, 1, 0); 
	 glTranslatef(0,0,lebar/2); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,tinggi,0.0f); 
	 glVertex3f(-panjang/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix();
 glPopMatrix(); 

 glPushMatrix(); //Bagian barang angkutan
 	 panjang = 17;
 	 lebar = 6;
 	 tinggi = 6;
	 glTranslatef(0,1.5,-10.8+yappari);
	 glColor3f(0.5f,0.5f,0.0f);

	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,tinggi,0.0f); 
	 glVertex3f(-lebar/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // atas 
	 glPushMatrix(); 
	 glRotated(-90, 1, 0, 0); 
	 glTranslatef(0,0,tinggi); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,-panjang/2,0.0f); 
	 glVertex3f(lebar/2,-panjang/2,0.0f); 
	 glVertex3f(lebar/2,panjang/2,0.0f); 
	 glVertex3f(-lebar/2,panjang/2,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // belakang 
	 glPushMatrix(); 
	 glRotated(-180, 1, 0, 0);
	  glTranslatef(0,0,panjang/2); 
	 glBegin(GL_QUADS); 
	  
	 glVertex3f(-lebar/2,-tinggi,0.0f); 
	   
	 glVertex3f(lebar/2,-tinggi,0.0f); 
	  
	 glVertex3f(lebar/2,0.0f,0.0f); 
	  
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // bawah 
	 glPushMatrix(); 
	 glRotated(90, 1, 0, 0); 
	 glTranslatef(0,0,0); 
	 glBegin(GL_QUADS); 
	  
	 glVertex3f(-lebar/2,-panjang/2,0.0f); 
	   
	 glVertex3f(lebar/2,-panjang/2,0.0f); 
	  
	 glVertex3f(lebar/2,panjang/2,0.0f); 
	  
	 glVertex3f(-lebar/2,panjang/2,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 

	 // kiri 
	 glPushMatrix(); 
	 glRotated(-90, 0, 1, 0); 
	 glTranslatef(0,0,lebar/2); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,tinggi,0.0f); 
	 glVertex3f(-panjang/2,tinggi,0.0f); 
	 glEnd();
	 glPopMatrix(); 
	 
	 // kanan 
	 glPushMatrix(); 
	 glRotated(90, 0, 1, 0); 
	 glTranslatef(0,0,lebar/2); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,tinggi,0.0f); 
	 glVertex3f(-panjang/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix();
 glPopMatrix();
 
 //Roda 1
 glPushMatrix();
 glTranslatef(3.1,1.2,-0.9+yappari);
 //lingkaran ban 
	glPushMatrix();
	for(j=0;j<=360;j++){
		glPushMatrix();
		glRotated(90,0,0,1);
		glRotated(j,0,1,0);
		glTranslatef(0,0,1.2);
		glColor3f(0.2,0.2,0.2);
		glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(0.1,0,0);
			glVertex3f(0.1,0.6,0);
			glVertex3f(0,0.6,0);
		glEnd();
		glPopMatrix();
	}
	glPopMatrix();
	
 //muka luar ban
	glPushMatrix();
	glTranslatef(0,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(0.2,0.2,0.2);
		glBegin(GL_TRIANGLE_FAN);
		radius=120;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
			glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.01,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(1,1,1);
		glBegin(GL_TRIANGLE_FAN);
		radius=80;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
				glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix(); 
 	
 	//muka luar ban
	glPushMatrix();
	glTranslatef(-0.6,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(0.2,0.2,0.2);
		glBegin(GL_TRIANGLE_FAN);
		radius=120;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
			glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix();
	
 glPopMatrix();
 //End Roda 1
 
 //Roda 2
 glPushMatrix();
 glTranslatef(-3.1,1.2,-0.9+yappari);
 glRotated(180,0,1,0);
 //lingkaran ban 
	glPushMatrix();
	for(j=0;j<=360;j++){
		glPushMatrix();
		glRotated(90,0,0,1);
		glRotated(j,0,1,0);
		glTranslatef(0,0,1.2);
		glColor3f(0.2,0.2,0.2);
		glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(0.1,0,0);
			glVertex3f(0.1,0.6,0);
			glVertex3f(0,0.6,0);
		glEnd();
		glPopMatrix();
	}
	glPopMatrix();
	
 //muka luar ban
	glPushMatrix();
	glTranslatef(0,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(0.2,0.2,0.2);
		glBegin(GL_TRIANGLE_FAN);
		radius=120;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
			glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.01,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(1,1,1);
		glBegin(GL_TRIANGLE_FAN);
		radius=80;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
				glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix(); 
 	
 	//muka luar ban
	glPushMatrix();
	glTranslatef(-0.6,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(0.2,0.2,0.2);
		glBegin(GL_TRIANGLE_FAN);
		radius=120;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
			glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix();
	
 glPopMatrix();
 //End Roda 2
 
 //Roda 3
 glPushMatrix();
 glTranslatef(3.1,1.2,-13.9+yappari);
 //lingkaran ban 
	glPushMatrix();
	for(j=0;j<=360;j++){
		glPushMatrix();
		glRotated(90,0,0,1);
		glRotated(j,0,1,0);
		glTranslatef(0,0,1.2);
		glColor3f(0.2,0.2,0.2);
		glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(0.1,0,0);
			glVertex3f(0.1,0.6,0);
			glVertex3f(0,0.6,0);
		glEnd();
		glPopMatrix();
	}
	glPopMatrix();
	
 //muka luar ban
	glPushMatrix();
	glTranslatef(0,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(0.2,0.2,0.2);
		glBegin(GL_TRIANGLE_FAN);
		radius=120;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
			glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.01,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(1,1,1);
		glBegin(GL_TRIANGLE_FAN);
		radius=80;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
				glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix(); 
 	
 	//muka luar ban
	glPushMatrix();
	glTranslatef(-0.6,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(0.2,0.2,0.2);
		glBegin(GL_TRIANGLE_FAN);
		radius=120;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
			glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix();
	
 glPopMatrix();
 //End Roda 3
 
 //Roda 4
 glPushMatrix();
 glTranslatef(-3.1,1.2,-13.9+yappari);
 glRotated(180,0,1,0);
 //lingkaran ban 
	glPushMatrix();
	for(j=0;j<=360;j++){
		glPushMatrix();
		glRotated(90,0,0,1);
		glRotated(j,0,1,0);
		glTranslatef(0,0,1.2);
		glColor3f(0.2,0.2,0.2);
		glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(0.1,0,0);
			glVertex3f(0.1,0.6,0);
			glVertex3f(0,0.6,0);
		glEnd();
		glPopMatrix();
	}
	glPopMatrix();
	
 //muka luar ban
	glPushMatrix();
	glTranslatef(0,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(0.2,0.2,0.2);
		glBegin(GL_TRIANGLE_FAN);
		radius=120;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
			glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.01,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(1,1,1);
		glBegin(GL_TRIANGLE_FAN);
		radius=80;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
				glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix(); 
 	
 	//muka luar ban
	glPushMatrix();
	glTranslatef(-0.6,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(0.2,0.2,0.2);
		glBegin(GL_TRIANGLE_FAN);
		radius=120;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
			glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix();
	
 glPopMatrix();
 //End Roda 4
}

void TrukBelakang() 
{
 glPushMatrix(); //Bagian bawah angkutan 2
 	 panjang = 17;
 	 lebar = 6;
 	 tinggi = 0.5;
	 glTranslatef(0,1,-29.8+yappari);
	 glColor3f(1.0f,0.5f,0.0f);
	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,tinggi,0.0f); 
	 glVertex3f(-lebar/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
	 // atas 
	 glPushMatrix(); 
	 glRotated(-90, 1, 0, 0); 
	 glTranslatef(0,0,tinggi); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,-panjang/2,0.0f); 
	 glVertex3f(lebar/2,-panjang/2,0.0f); 
	 glVertex3f(lebar/2,panjang/2,0.0f); 
	 glVertex3f(-lebar/2,panjang/2,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
	 // belakang 
	 glPushMatrix(); 
	 glRotated(-180, 1, 0, 0);
	 glTranslatef(0,0,panjang/2); 
	 glBegin(GL_QUADS); 
	  
	 glVertex3f(-lebar/2,-tinggi,0.0f); 
	   
	 glVertex3f(lebar/2,-tinggi,0.0f); 
	  
	 glVertex3f(lebar/2,0.0f,0.0f); 
	  
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
	 // bawah 
	 glPushMatrix(); 
	 glRotated(90, 1, 0, 0); 
	 glTranslatef(0,0,0); 
	 glBegin(GL_QUADS); 
	  
	 glVertex3f(-lebar/2,-panjang/2,0.0f); 
	   
	 glVertex3f(lebar/2,-panjang/2,0.0f); 
	  
	 glVertex3f(lebar/2,panjang/2,0.0f); 
	  
	 glVertex3f(-lebar/2,panjang/2,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
	 // kiri 
	 glPushMatrix(); 
	 glRotated(-90, 0, 1, 0); 
	 glTranslatef(0,0,lebar/2); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,tinggi,0.0f); 
	 glVertex3f(-panjang/2,tinggi,0.0f); 
	 glEnd();
	 glPopMatrix(); 
	 // kanan 
	 glPushMatrix(); 
	 glRotated(90, 0, 1, 0); 
	 glTranslatef(0,0,lebar/2); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,tinggi,0.0f); 
	 glVertex3f(-panjang/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix();
 glPopMatrix(); 

 glPushMatrix(); //Bagian barang angkutan 2
 	 panjang = 17;
 	 lebar = 6;
 	 tinggi = 6;
	 glTranslatef(0,1.5,-29.8+yappari);
	 glColor3f(0.5f,0.5f,0.0f);
	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,tinggi,0.0f); 
	 glVertex3f(-lebar/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
	 // atas 
	 glPushMatrix(); 
	 glRotated(-90, 1, 0, 0); 
	 glTranslatef(0,0,tinggi); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,-panjang/2,0.0f); 
	 glVertex3f(lebar/2,-panjang/2,0.0f); 
	 glVertex3f(lebar/2,panjang/2,0.0f); 
	 glVertex3f(-lebar/2,panjang/2,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
	 // belakang 
	 glPushMatrix(); 
	 glRotated(-180, 1, 0, 0);
	  glTranslatef(0,0,panjang/2); 
	 glBegin(GL_QUADS); 
	  
	 glVertex3f(-lebar/2,-tinggi,0.0f); 
	   
	 glVertex3f(lebar/2,-tinggi,0.0f); 
	  
	 glVertex3f(lebar/2,0.0f,0.0f); 
	  
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
	 // bawah 
	 glPushMatrix(); 
	 glRotated(90, 1, 0, 0); 
	 glTranslatef(0,0,0); 
	 glBegin(GL_QUADS); 
	  
	 glVertex3f(-lebar/2,-panjang/2,0.0f); 
	   
	 glVertex3f(lebar/2,-panjang/2,0.0f); 
	  
	 glVertex3f(lebar/2,panjang/2,0.0f); 
	  
	 glVertex3f(-lebar/2,panjang/2,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
	 // kiri 
	 glPushMatrix(); 
	 glRotated(-90, 0, 1, 0); 
	 glTranslatef(0,0,lebar/2); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,tinggi,0.0f); 
	 glVertex3f(-panjang/2,tinggi,0.0f); 
	 glEnd();
	 glPopMatrix(); 
	 // kanan 
	 glPushMatrix(); 
	 glRotated(90, 0, 1, 0); 
	 glTranslatef(0,0,lebar/2); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,tinggi,0.0f); 
	 glVertex3f(-panjang/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix();
 glPopMatrix();
 
  glPushMatrix(); //Bagian penyambung antar angkutan
 	 panjang = 3;
 	 lebar = 1.5;
 	 tinggi = 0.5;
	 glTranslatef(0,1,-20+yappari);
	 glColor3f(0.01,0.01,0.01);
	 //depan 
	 glPushMatrix(); 
	 glTranslatef(0,0,panjang/2);
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glVertex3f(lebar/2,tinggi,0.0f); 
	 glVertex3f(-lebar/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
	 // atas 
	 glPushMatrix(); 
	 glRotated(-90, 1, 0, 0); 
	 glTranslatef(0,0,tinggi); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-lebar/2,-panjang/2,0.0f); 
	 glVertex3f(lebar/2,-panjang/2,0.0f); 
	 glVertex3f(lebar/2,panjang/2,0.0f); 
	 glVertex3f(-lebar/2,panjang/2,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
	 // belakang 
	 glPushMatrix(); 
	 glRotated(-180, 1, 0, 0);
	  glTranslatef(0,0,panjang/2); 
	 glBegin(GL_QUADS); 
	 glTexCoord2f(0.0f,0.0f); // kiri bawah 
	 glVertex3f(-lebar/2,-tinggi,0.0f); 
	 glTexCoord2f(1.0f,0.0f); // kanan bawah 
	 glVertex3f(lebar/2,-tinggi,0.0f); 
	 glTexCoord2f(1.0f,1.0f); // kanan atas 
	 glVertex3f(lebar/2,0.0f,0.0f); 
	 glTexCoord2f(0.0f,1.0f); // kanan bawah 
	 glVertex3f(-lebar/2,0.0f,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
	 // bawah 
	 glPushMatrix(); 
	 glRotated(90, 1, 0, 0); 
	 glTranslatef(0,0,0); 
	 glBegin(GL_QUADS); 
	 glTexCoord2f(0.0f,0.0f); // kiri bawah 
	 glVertex3f(-lebar/2,-panjang/2,0.0f); 
	 glTexCoord2f(1.0f,0.0f); // kanan bawah 
	 glVertex3f(lebar/2,-panjang/2,0.0f); 
	 glTexCoord2f(1.0f,1.0f); // kanan atas 
	 glVertex3f(lebar/2,panjang/2,0.0f); 
	 glTexCoord2f(0.0f,1.0f); // kanan bawah 
	 glVertex3f(-lebar/2,panjang/2,0.0f); 
	 glEnd(); 
	 glPopMatrix(); 
	 // kiri 
	 glPushMatrix(); 
	 glRotated(-90, 0, 1, 0); 
	 glTranslatef(0,0,lebar/2); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,tinggi,0.0f); 
	 glVertex3f(-panjang/2,tinggi,0.0f); 
	 glEnd();
	 glPopMatrix(); 
	 // kanan 
	 glPushMatrix(); 
	 glRotated(90, 0, 1, 0); 
	 glTranslatef(0,0,lebar/2); 
	 glBegin(GL_QUADS); 
	 glVertex3f(-panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,0.0f,0.0f); 
	 glVertex3f(panjang/2,tinggi,0.0f); 
	 glVertex3f(-panjang/2,tinggi,0.0f); 
	 glEnd(); 
	 glPopMatrix();
 glPopMatrix(); 

 
 // Pembuatan Roda
 
 //Roda 5
 glPushMatrix();
 glTranslatef(3.1,1.2,-24+yappari);
 //lingkaran ban 
	glPushMatrix();
	for(j=0;j<=360;j++){
		glPushMatrix();
		glRotated(90,0,0,1);
		glRotated(j,0,1,0);
		glTranslatef(0,0,1.2);
		glColor3f(0.2,0.2,0.2);
		glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(0.1,0,0);
			glVertex3f(0.1,0.6,0);
			glVertex3f(0,0.6,0);
		glEnd();
		glPopMatrix();
	}
	glPopMatrix();
	
 //muka luar ban
	glPushMatrix();
	glTranslatef(0,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(0.2,0.2,0.2);
		glBegin(GL_TRIANGLE_FAN);
		radius=120;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
			glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.01,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(1,1,1);
		glBegin(GL_TRIANGLE_FAN);
		radius=80;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
				glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix(); 
 	
 	//muka luar ban
	glPushMatrix();
	glTranslatef(-0.6,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(0.2,0.2,0.2);
		glBegin(GL_TRIANGLE_FAN);
		radius=120;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
			glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix();
	
 glPopMatrix();
 //End Roda 5
 
 //Roda 6
 glPushMatrix();
 glTranslatef(-3.1,1.2,-24+yappari);
 glRotated(180,0,1,0);
 //lingkaran ban 
	glPushMatrix();
	for(j=0;j<=360;j++){
		glPushMatrix();
		glRotated(90,0,0,1);
		glRotated(j,0,1,0);
		glTranslatef(0,0,1.2);
		glColor3f(0.2,0.2,0.2);
		glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(0.1,0,0);
			glVertex3f(0.1,0.6,0);
			glVertex3f(0,0.6,0);
		glEnd();
		glPopMatrix();
	}
	glPopMatrix();
	
 //muka luar ban
	glPushMatrix();
	glTranslatef(0,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(0.2,0.2,0.2);
		glBegin(GL_TRIANGLE_FAN);
		radius=120;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
			glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.01,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(1,1,1);
		glBegin(GL_TRIANGLE_FAN);
		radius=80;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
				glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix(); 
 	
 	//muka luar ban
	glPushMatrix();
	glTranslatef(-0.6,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(0.2,0.2,0.2);
		glBegin(GL_TRIANGLE_FAN);
		radius=120;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
			glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix();
	
 glPopMatrix();
 //End Roda 6
 
 //Roda 7
 glPushMatrix();
 glTranslatef(3.1,1.2,-36+yappari);
 //lingkaran ban 
	glPushMatrix();
	for(j=0;j<=360;j++){
		glPushMatrix();
		glRotated(90,0,0,1);
		glRotated(j,0,1,0);
		glTranslatef(0,0,1.2);
		glColor3f(0.2,0.2,0.2);
		glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(0.1,0,0);
			glVertex3f(0.1,0.6,0);
			glVertex3f(0,0.6,0);
		glEnd();
		glPopMatrix();
	}
	glPopMatrix();
	
 //muka luar ban
	glPushMatrix();
	glTranslatef(0,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(0.2,0.2,0.2);
		glBegin(GL_TRIANGLE_FAN);
		radius=120;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
			glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.01,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(1,1,1);
		glBegin(GL_TRIANGLE_FAN);
		radius=80;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
				glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix(); 
 	
 	//muka luar ban
	glPushMatrix();
	glTranslatef(-0.6,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(0.2,0.2,0.2);
		glBegin(GL_TRIANGLE_FAN);
		radius=120;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
			glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix();
	
 glPopMatrix();
 //End Roda 7
 
 //Roda 8
 glPushMatrix();
 glTranslatef(-3.1,1.2,-36+yappari);
 glRotated(180,0,1,0);
 //lingkaran ban 
	glPushMatrix();
	for(j=0;j<=360;j++){
		glPushMatrix();
		glRotated(90,0,0,1);
		glRotated(j,0,1,0);
		glTranslatef(0,0,1.2);
		glColor3f(0.2,0.2,0.2);
		glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(0.1,0,0);
			glVertex3f(0.1,0.6,0);
			glVertex3f(0,0.6,0);
		glEnd();
		glPopMatrix();
	}
	glPopMatrix();
	
 //muka luar ban
	glPushMatrix();
	glTranslatef(0,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(0.2,0.2,0.2);
		glBegin(GL_TRIANGLE_FAN);
		radius=120;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
			glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.01,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(1,1,1);
		glBegin(GL_TRIANGLE_FAN);
		radius=80;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
				glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix(); 
 	
 	//muka luar ban
	glPushMatrix();
	glTranslatef(-0.6,0,1.005);
	glRotated(90,0,1,0);
	glColor3f(0.2,0.2,0.2);
		glBegin(GL_TRIANGLE_FAN);
		radius=120;
		jumlah_titik=20;
		x_tengah=100;
		y_tengah=0;
		for (i=0;i<=360;i++){
			float sudut=i*(2*PI/jumlah_titik);
			float x=x_tengah+radius*cos(sudut);
			float y=y_tengah+radius*sin(sudut);
			glVertex2f(x/100,y/100);
		}
		glEnd();
	glPopMatrix();
	
 glPopMatrix();
 //End Roda 8
 
}

void Environment()
{
	//box bawah
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-100, 0.05, 100);
	glTexCoord2f(1.0f, 0.0f);
 	glVertex3f(-100, 0.05, -100);
 	glTexCoord2f(1.0f, 1.0f);
 	glVertex3f(100, 0.05, -100);
 	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(100, 0.05, 100);
	glEnd();	
	
	//box depan
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureIdDepan);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(100.0f,  100.05f, 100.0f);	
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f( -100.0f,  100.05f, 100.0f);	
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-100.0f, 0.05f, 100.0f);	
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(100.0f, 0.05f, 100.0f);	
	glEnd();
	
	//box kiri
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureIdKiri);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(100.0f, 0.05f, -100.0f);	
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(100.0f,  100.05f, -100.0f);	
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f( 100.0f,  100.05f, 100.0f);	
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(100.0f, 0.05f, 100.0f);	
	glEnd();
	
	//box belakang
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureIdBelakang);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(100.0f, 0.05f, -100.0f);	
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(100.0f,  100.05f, -100.0f);	
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-100.0f,  100.05f, -100.0f);	
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-100.0f, 0.05f, -100.0f);
	glEnd();
	
	//box kanan
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureIdKanan);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-100.0f, 0.05f, 100.0f);	
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(-100.0f,  100.05f, 100.0f);	
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f( -100.0f,  100.05f, -100.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-100.0f, 0.05f, -100.0f);	
	glEnd();
	
	//box atas
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureIdAtas);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-100.0f, 100.05f, -100.0f);	
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(-100.0f,  100.05f, 100.0f);	
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(100.0f,  100.05f, 100.0f);	
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(100.0f, 100.05f, -100.0f);	
	glEnd();
}
// Fungsi untuk membuat Pohon
void Pohon(int R,int G,int B){
   	GLUquadricObj *pObj;
	pObj =gluNewQuadric();
	gluQuadricNormals(pObj, GLU_SMOOTH);
	
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glColor3f(0.7,0.3,0);
	glRotatef(270,1,0,0);
	glTranslatef(-30,7,0);
	gluCylinder(pObj, 1, 0.7, 10, 20, 15);
	glPopMatrix();
	glPushMatrix();
	glColor3ub(104,70,14);
	glTranslatef(-30,7,-7);
	glRotatef(330,1,0,0);
	gluCylinder(pObj, 0.6, 0.1, 7, 25, 25);
	glPopMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glColor3f(0,0.3,0.06);
	glScaled(4, 3, 5);
	glTranslatef(-7.4,4.7,-1);
	glutSolidDodecahedron();
	glPopMatrix();

}
 
void tree(int n,int R,int G){
    for(int i=0;i<n;i++){
        glPushMatrix();
            glTranslated(-10,0.8+i,100);
            Pohon(G,R,i);
        glPopMatrix();
    }
}
 void awan(void){
	//awan 	
 	glPushMatrix();
 	glTranslatef(-10,30,-8);
 	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
 	glColor3ub(153, 223, 255);
 	glutSolidSphere(6, 50, 50);
 	glPopMatrix();
 	
 	glPushMatrix();
 	glTranslatef(-15,43,-8);
 	glutSolidSphere(4, 50, 50);
 	glPopMatrix();
 	
 	glPushMatrix();
 	glTranslatef(-25,43,-8);
 	glutSolidSphere(4, 50, 50);
 	glPopMatrix();
 }
 
void lingkungan(int n){
        for(int i =- (EN_SIZE/2)+1;i<(EN_SIZE/2);i+=2){
            for(int j=-(EN_SIZE/2)+1;j<(EN_SIZE/2);j+=2){
                if(tola[i+(EN_SIZE/2)+1][j+(EN_SIZE/2)+1]!=0){
                    glPushMatrix();
                    	glTranslated(i-5,0,j*25);
                    	awan();
                        glTranslated(i+5,0,j*10);
                        tree(tola[i+(EN_SIZE/2)+1][j+(EN_SIZE/2)+1],i,j+1);
                        glTranslated(i+105,0,j*25);
                        tree(tola[i+(EN_SIZE/2)+1][j+(EN_SIZE/2)+1],i,j+1);
                        
                    glPopMatrix();
                }else if(i>=-5&&i<=5){}
                else{
                    tola[i+(EN_SIZE/2)+1][j+(EN_SIZE/2)+1]=(rand()%5)+1;
                    glPushMatrix();
                        glTranslated(i,0,j*25);
                        tree(tola[i+(EN_SIZE/2)+1][j+(EN_SIZE/2)+1],i,j);
                    glPopMatrix();
                }
            }
        }
}
 
void draw(){
    glPushMatrix();
        glTranslated(tX,tY,tZ);
        lingkungan(2);
    glPopMatrix();
}

void Jalan() {
	glPushMatrix(); //Bagian kepala
		panjang = 5;
		lebar = 50;
		tinggi = 0.2;
		glTranslatef(0,0,5);
		glColor3f(0.1f,0.1f,0.1f);
		//depan 
		glPushMatrix(); 
		glTranslatef(0,0,panjang/2);
		glBegin(GL_QUADS); 
		glVertex3f(-lebar/2,0.0f,0.0f); 
		glVertex3f(lebar/2,0.0f,0.0f); 
		glVertex3f(lebar/2,tinggi,0.0f); 
		glVertex3f(-lebar/2,tinggi,0.0f); 
		glEnd(); 
		glPopMatrix(); 
		// atas 
		glPushMatrix(); 
		glRotated(-90, 1, 0, 0); 
		glTranslatef(0,0,tinggi); 
		glBegin(GL_QUADS); 
		glVertex3f(-lebar/2,-panjang/2,0.0f); 
		glVertex3f(lebar/2,-panjang/2,0.0f); 
		glVertex3f(lebar/2,panjang/2,0.0f); 
		glVertex3f(-lebar/2,panjang/2,0.0f); 
		glEnd(); 
		glPopMatrix(); 
		// belakang 
		glPushMatrix(); 
		glRotated(-180, 1, 0, 0);
		glTranslatef(0,0,panjang/2); 
		glBegin(GL_QUADS); 
		glTexCoord2f(0.0f,0.0f); // kiri bawah 
		glVertex3f(-lebar/2,-tinggi,0.0f); 
		glTexCoord2f(1.0f,0.0f); // kanan bawah 
		glVertex3f(lebar/2,-tinggi,0.0f); 
		glTexCoord2f(1.0f,1.0f); // kanan atas 
		glVertex3f(lebar/2,0.0f,0.0f); 
		glTexCoord2f(0.0f,1.0f); // kanan bawah 
		glVertex3f(-lebar/2,0.0f,0.0f); 
		glEnd(); 
		glPopMatrix(); 
		// bawah 
		glPushMatrix(); 
		glRotated(90, 1, 0, 0); 
		glTranslatef(0,0,0); 
		glBegin(GL_QUADS); 
		glTexCoord2f(0.0f,0.0f); // kiri bawah 
		glVertex3f(-lebar/2,-panjang/2,0.0f); 
		glTexCoord2f(1.0f,0.0f); // kanan bawah 
		glVertex3f(lebar/2,-panjang/2,0.0f); 
		glTexCoord2f(1.0f,1.0f); // kanan atas 
		glVertex3f(lebar/2,panjang/2,0.0f); 
		glTexCoord2f(0.0f,1.0f); // kanan bawah 
		glVertex3f(-lebar/2,panjang/2,0.0f); 
		glEnd(); 
		glPopMatrix(); 
		// kiri 
		glPushMatrix(); 
		glRotated(-90, 0, 1, 0); 
		glTranslatef(0,0,lebar/2); 
		glBegin(GL_QUADS); 
		glVertex3f(-panjang/2,0.0f,0.0f); 
		glVertex3f(panjang/2,0.0f,0.0f); 
		glVertex3f(panjang/2,tinggi,0.0f); 
		glVertex3f(-panjang/2,tinggi,0.0f); 
		glEnd();
		glPopMatrix(); 
		// kanan 
		glPushMatrix(); 
		glRotated(90, 0, 1, 0); 
		glTranslatef(0,0,lebar/2); 
		glBegin(GL_QUADS); 
		glVertex3f(-panjang/2,0.0f,0.0f); 
		glVertex3f(panjang/2,0.0f,0.0f); 
		glVertex3f(panjang/2,tinggi,0.0f); 
		glVertex3f(-panjang/2,tinggi,0.0f); 
		glEnd(); 
		glPopMatrix();
 	glPopMatrix();
 	
 	glPushMatrix(); 
		panjang = 5;
		lebar = 1;
		tinggi = 4;
		glRotated(-90,1,0,0);
		glTranslatef(0,-7.5,-2.29);
		glColor3f(1,1,1);
		//depan 
		glPushMatrix(); 
		glTranslatef(0,0,panjang/2);
		glBegin(GL_QUADS); 
		glVertex3f(-lebar/2,0.0f,0.0f); 
		glVertex3f(lebar/2,0.0f,0.0f); 
		glVertex3f(lebar/2,tinggi,0.0f); 
		glVertex3f(-lebar/2,tinggi,0.0f); 
		glEnd(); 
		glPopMatrix();
 	glPopMatrix();
}

// Fungsi untuk membuat Mobil
void Mobil() {
 glMatrixMode(GL_MODELVIEW);
 glPushMatrix();
 glPushMatrix();
    glPushMatrix();                   // body
    glScalef(25,10,20);
    glTranslatef(0,0,3);
    glutSolidCube(.5);
    glTranslatef(-.05,.3,0);
    glScalef(0.6,3,2);
    glutSolidCube(.25);
    glTranslatef(-.12,.001,-.001);  
    glScalef(1,1.8,2.48);
    glRotatef(230, 0, 0, 250);
    glutSolidCube(.1);
    glPopMatrix();
    glTranslatef(0,0,.5);
    glPushMatrix();
    glTranslatef(-.4,-.2,0);
    glutSolidTorus(.1,.2,8,8);       // wheel
    glTranslatef(1,0,0);
    glutSolidTorus(.1,.2,8,8);       // wheel
    glPopMatrix();
    glTranslatef(0,0,-1);
    glPushMatrix();
    glTranslatef(-.4,-.2,0);
    glutSolidTorus(.1,.2,8,8);       // wheel
    glTranslatef(1,0,0);
    glutSolidTorus(.1,.2,8,8);       // wheel
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}




//Start////////////////////////////




//Pergerakan pada Truk //////////////////////////////////////////////////////




void TrukMove()
{
	//Belok kanan
	if (belok == -1){
		if (angTrukDepan >= -10){
			angTrukDepan -= 0.5;
		}
		if (angTrukDepan < -9.9){
			belok = -2;
		}
	}
	if (belok == -2){
		if (angTrukDepan <= -0.5){
			angTrukDepan += 0.5;
			posXTruk -= 0.2;
		}
		if (angTrukDepan > -0.1){
			belok = 0;
		}
	}
	
	//Belok kiri
	if (belok == 1){
		if (angTrukDepan <= 10){
			angTrukDepan += 0.5;
		}
		if (angTrukDepan > 9.9){
			belok = 2;
		}
	}
	if (belok == 2){
		if (angTrukDepan >= 0.5){
			angTrukDepan -= 0.5;
			posXTruk += 0.2;
		}
		if (angTrukDepan < 0.1){
			belok = 0;
		}
	}
}


void TrukMove2()
{
	if (belok == -1 and angTrukDepan <= -4.9){
		if (angTrukBelakang >= -10){
			angTrukBelakang -= 0.5;
		}
	}
	if (belok == -2 and angTrukDepan >= -5.1){
		if (angTrukBelakang <= -0.5){
			angTrukBelakang += 0.5;
		}
	}
	if (belok == 1 and angTrukDepan >= 4.9){
		if (angTrukBelakang <= 10){
			angTrukBelakang += 0.5;
		}
	}
	if (belok == 2 and angTrukDepan <= 5.1){
		if (angTrukBelakang >= 0.5){
			angTrukBelakang -= 0.5;
		}
	}
}


void PergerakanJalan()
{
	//jika jalan belom mencapai ujung
	if(ujung == 0)
	{
		posZJalan -= speed; //Kecepatan jalan
	} else 
	{
		posZJalan = -100;
		ujung = 0;
	}
	
	if(posZJalan <= -400) ujung = 1;
}


void PohonMove()
{
	if(ujungP == 0)
	{
		posZPohon -= speed; //Kecepatan jalan
	} else 
	{
		posZPohon = -100;
		ujungP = 0;
	}
	
	if(posZPohon <= -400) ujungP = 1;
}

void RumahMove(){
	if(ujungR == 0)
	{
		posZRumah -= speed; //Kecepatan jalan
	} else 
	{
		posZRumah = -250;
		ujungR = 0;
	}
	
	if(posZRumah <= -400) ujungR = 1;
}


void MobilMove(){
	if(ujungM == 0)
	{
		posZMobil -= speed; //Kecepatan jalan
	} else 
	{
		posZMobil = 100;
		ujungM = 0;
	}
	
	if(posZMobil <= -400) ujungM = 1;
}


void isTouch(){
	if ((posZTruk < posZMobil+80 and posZTruk > posZMobil +49) and (posXTruk >= -2 and posXTruk <= 2)){
		touch = true;
		
	}
	if (posXTruk < -30 or posXTruk > 18){
		touch = true;
	}
}

void Object()
{		
	glPushMatrix();
		glPushMatrix();
			TrukMove();
			glRotatef(angTrukDepan,0,1,0);
			glTranslatef(posXTruk,0, posZTruk);
			TrukDepan();
		glPopMatrix();
		glPushMatrix();
			TrukMove2();
			glRotatef(angTrukBelakang,0,1,0);
			glTranslatef(posXTruk,0, posZTruk);
			TrukBelakang();
		glPopMatrix();
	glPopMatrix();
	
	glPushMatrix();
	int a;	
	PergerakanJalan();
	for(a = 100; a > 0; a -= 1){
		glPushMatrix();
			glTranslatef(-4,0,posZJalan+(a*5));
			Jalan();
		glPopMatrix();
	}
	glPopMatrix();
	
	glPushMatrix();
	int b;	
	PohonMove();
	for(b = 10; b > 0; b -= 1){
		glPushMatrix();
			glTranslatef(-4,0,posZPohon+(b*50));
			Pohon1();
			Rumah();
		glPopMatrix();
	}
	glPopMatrix();
	
	glPushMatrix();
	int c;	
	RumahMove();
	for(c = 5; c > 0; c -= 1){
		glPushMatrix();
			glTranslatef(-4,0,posZRumah+(c*100));
			draw();
		glPopMatrix();
	}
	glPopMatrix();
	
	
	
	glPushMatrix();
		MobilMove();
		glTranslatef(0,0,posZMobil);
		Mobil();
	glPopMatrix();
}

void drawStrokeText(char* str,int x,int y,int z)
{
	  char *c;
	  glPushMatrix();
	  glTranslatef(x, y+8,z);
	  glScalef(0.02f,0.02f,z);

	  for (c=str; *c != '\0'; c++)
	  {
    		glutStrokeCharacter(GLUT_STROKE_ROMAN , *c);
	  }
	  glPopMatrix();
}

void drawStrokeText2(char* str,int x,int y,int z)
{
	  char *c;
	  glPushMatrix();
	  glTranslatef(x, y+8,z);
	  glScalef(0.03f,0.03f,0.02f);

	  for (c=str; *c != '\0'; c++)
	  {
    		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN , *c);
	  }
	  glPopMatrix();
}

void display() { 
	// Kalau move dan angle tidak nol, gerakkan kamera... 
 	if (deltaMove) 
 		moveMeFlat(deltaMove); 
 		if (deltaAngle) { 
 			angle += deltaAngle; 
 			orientMe(angle); 
 		} 
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	
	glPushMatrix();
	glRotated(rotAngleX+10, 1, 0, 0);
 	glRotated(rotAngleY, 0, 1, 0);
 	glRotated(rotAngleZ, 0, 0, 1);
 	glRotated(180,0,1,0);
	
	isTouch(); //Mengecek apakah truk menabrak 
	if (touch){
		TrukDepan();
		glRotated(180,0,1,0);
		drawStrokeText2("Cie Nabrak :v",-10,16,0);
		drawStrokeText("Pencet 'r' untuk memulai ulang",-10,10,0);
	} else {
	 	Environment();
	 	Object();
	}
	
 	glPopMatrix();
 	glFlush(); 
 	glutSwapBuffers(); 
}
 
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'a':
			belok = 1; break; //Belok kiri
		case 'd':
			belok = -1; break; //Belok kanan
		case 'w':
			posZTruk += 0.5; break; //Truk Maju
		case 's':
			posZTruk -= 0.5; break; //Truk Mundur
		//Rotasi Kamera/////////////////////////////////////
		case 'i':
			rotAngleX -= 1; break; 
		case 'k':
			rotAngleX += 1; break;
		case 'j':
			rotAngleY -= 1; break;
		case 'l':
			rotAngleY += 1; break;
		///////////////////////////////////////////////////
		
		case 'r': //Restart Game
			if (touch){
				posXTruk = 0; posZTruk = 0; posZMobil = 200; touch = false; break;	
			}; break;
		case 27: //Quit Game
			exit(0);
		default:
			break;
	} glutPostRedisplay();
}

GLUquadricObj *IDquadric;
// Variable untuk pencahayaan  
const GLfloat light_ambient[] = { 0.5f, 0.5f, 0.5f, 0.0f }; 
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
const GLfloat light_position[] = { 0.0f, 20.0f, 10.0f, 1.0f }; 
const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f }; 
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f }; 
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
const GLfloat high_shininess[] = { 100.0f }; 

void lighting(){ 
 // Fungsi mengaktifkan pencahayaan 
 glEnable(GL_DEPTH_TEST); 
 glDepthFunc(GL_LESS); 
 glEnable(GL_LIGHT0); 
 glEnable(GL_NORMALIZE); 
 glEnable(GL_COLOR_MATERIAL); 
 glEnable(GL_LIGHTING); 
 glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); 
 glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); 
 glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); 
 glLightfv(GL_LIGHT0, GL_POSITION, light_position); 
 glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient); 
 glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse); 
 glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); 
 glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess); 
}
 
void init(void) 
{ 
 	glEnable (GL_DEPTH_TEST); 
 	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
 	IDquadric=gluNewQuadric();      // Create A Pointer To The Quadric Object ( NEW )
  	gluQuadricNormals(IDquadric, GLU_SMOOTH);  // Create Smooth Normals ( NEW )
  	gluQuadricTexture(IDquadric, GL_TRUE);    // Create Texture Coords ( NEW )
  	Image* imagebawah = loadBMP("bawah.bmp");
  	Image* imagekanan = loadBMP("kanan.bmp");
  	Image* imagekiri = loadBMP("kiri.bmp");
  	Image* imagebelakang = loadBMP("belakang.bmp");
  	Image* imagedepan = loadBMP("depan.bmp");
  	Image* imageatas = loadBMP("atas.bmp");
	_textureId = loadTexture(imagebawah);
	_textureIdDepan = loadTexture(imagedepan);
	_textureIdKanan = loadTexture(imagekanan);
	_textureIdBelakang = loadTexture(imagebelakang);
	_textureIdKiri = loadTexture(imagekiri);
	_textureIdAtas = loadTexture(imageatas);
	delete imagebawah;
	delete imagedepan;
	delete imagekanan;
	delete imagebelakang;
	delete imagekiri;
	delete imageatas;
} 

int main(int argc, char **argv) 
{ 
 glutInit(&argc, argv); 
 glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); 
 glutInitWindowPosition(100,100); 
 glutInitWindowSize(640,480); 
 glutCreateWindow("Truk Scania"); 
 glutSpecialFunc(pressKey);
 glutSpecialUpFunc(releaseKey);
 glutDisplayFunc(display); 
 glClearColor(1,1,1,1);
 glutKeyboardFunc(keyboard);
 glutIdleFunc(display); // Fungsi display-nya dipanggil terusmenerus 
 glutReshapeFunc(Reshape); 
 lighting(); 
 init(); 
 glutMainLoop(); 
 return(0); 
}

