
#include "stdafx.h"
#include <glew\glew.h>
#include <freeglut\freeglut.h>
#include <CoreStructures\CoreStructures.h>
#include "texture_loader.h"
#include "shader_setup.h"
#include <stack>
#include "Circle.h"

using namespace std;
using namespace CoreStructures;
#pragma region// Globals

stack<GUMatrix4>matrixStack;

GLuint locT,locT2; // location of "T" uniform variable in myShaderProgram

Circle *myCircle = nullptr; //circle object

// Textures
GLuint myGroundTexture = 0;
GLuint myMountainTexture = 1;
GLuint myHouseTexture = 2;
GLuint myRoofTexture = 3;
GLuint myStairTexture = 4;
GLuint myBulletTexture = 5;
GLuint myTankTexture = 6;
GLuint myTankCannonTexture = 7;
GLuint myTankTrackTexture = 8;
GLuint myBullet2Texture = 9;

// Mouse input (rotation) example
// Variables we'll use to animate (rotate) our objects
float theta0a = 0.0f;
float theta0b = 0.0f;
float theta0c = 0.0f;
float theta1 = 0.0f;
float theta2 = 0.0f;
float theta3 = 0.0f;

// Variables needed to track where the mouse pointer is so we can determine which direction it's moving in
int mouse_x, mouse_y;
bool mDown = false;

GLuint myShaderProgram, myShaderProgramNoTexture, quadPosVBO, quadColourVBO, quadTexCoordVBO, quadIndicesVBO, quadMPosVBO, quadMColourVBO, quadMTexCoordVBO, quadMIndicesVBO;


static GLfloat quadVertices[] = {-1.0, -0.5f,-1.0f, 0.05f,1.0f, -0.5f,1.0f, 0.05f};
static GLubyte quadColors[] = {255, 0, 0, 255,255, 255, 0, 255,0, 255, 0, 255,0, 255, 255, 255};
static float quadTextureCoords[] = {0.0f, 1.0f,0.0f, 0.0f,1.0f, 1.0f,1.0f, 0.0f
};
static GLubyte quadVertexIndices[] = { 0, 1, 2, 3 };

// -----------------------

static GLfloat quadMVertices[] = {-1.0, -0.5f,-1.0f, 0.95f,1.0f, -0.5f,1.0f, 0.95f};
static GLubyte quadMColors[] = {255, 0, 0, 255,255, 255, 0, 255,0, 255, 0, 255,0, 255, 255, 255};
static float quadMTextureCoords[] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f };
static GLubyte quadMVertexIndices[] = { 0, 1, 2, 3 };

// -----------------------

GLuint housePosVBO, houseColourVBO, houseTexCoordVBO, houseIndicesVBO;

static GLfloat houseVertices[] = {0.0,0.5f,0.5f,0.5f,0.5f,0.0};
static GLubyte houseColors[] = {255, 0, 0, 255,255, 255, 0, 255,0, 255, 0, 255,};
static float houseTextureCoords[] = {0.0f, 0.0f,1.0f, 0.0f,1.0f, 1.0f,0.0f,1.0f};

GLubyte houseVertexIndices[] = { 0, 1, 2, 3};

// -----------------------

GLuint roofPosVBO, roofColourVBO, roofTexCoordVBO, roofIndicesVBO;

static GLfloat roofVertices[] = { 0.25f, 0.4,0.5,0.0};
static GLubyte roofColors[] = { 255, 0, 0, 255, 255, 255, 0, 255, 0, 255, 0, 255, };
static float roofTextureCoords[] = { 0.0f, 0.0f, 1.0f, 1.0f,0.0f, 1.0f };

GLubyte roofVertexIndices[] = { 0, 1, 2};

// -----------------------

GLuint stairPosVBO, stairColourVBO, stairTexCoordVBO, stairIndicesVBO;

static GLfloat stairVertices[] = { 0.0, 0.15, 0.15, 0.15,0.15,0.0 };
static GLubyte stairColors[] = { 255, 0, 0, 255, 255, 255, 0, 255, 0, 255, 0, 255, };
static float stairTextureCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

GLubyte stairVertexIndices[] = { 0, 1, 2,3 };

// -----------------------

GLuint tankFundPosVBO, tankFundColourVBO, tankFundTexCoordVBO, tankFundIndicesVBO;

static GLfloat tankFundVertices[] = { -0.1, 0.07, 0.5, 0.07, 0.4, 0.0 };
static GLubyte tankFundColors[] = { 255, 0, 0, 255, 255, 255, 0, 255, 0, 255, 0, 255, };
static float tankFundTextureCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

GLubyte tankFundVertexIndices[] = { 0, 1, 2, 3 };

// -----------------------

GLuint tankBodyPosVBO, tankBodyColourVBO, tankBodyTexCoordVBO, tankBodyIndicesVBO;

static GLfloat tankBodyVertices[] = { 0.1, 0.1, 0.5, 0.1, 0.6, 0.0 };
static GLubyte tankBodyColors[] = { 255, 0, 0, 255, 255, 255, 0, 255, 0, 255, 0, 255, };
static float tankBodyTextureCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

GLubyte tankBodyVertexIndices[] = { 0, 1, 2, 3 };

// -----------------------

GLuint tankHeadPosVBO, tankHeadColourVBO, tankHeadTexCoordVBO, tankHeadIndicesVBO;

static GLfloat tankHeadVertices[] = { 0.1, 0.15, 0.2, 0.15, 0.3, 0.0 };
static GLubyte tankHeadColors[] = { 255, 0, 0, 255, 255, 255, 0, 255, 0, 255, 0, 255, };
static float tankHeadTextureCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

GLubyte tankHeadVertexIndices[] = { 0, 1, 2, 3 };

// -----------------------

GLuint tankCannonPosVBO, tankCannonColourVBO, tankCannonTexCoordVBO, tankCannonIndicesVBO;

static GLfloat tankCannonVertices[] = { 0.0, 0.05, -0.5, 0.05, -0.475, 0.025, -0.5, 0.0 };
static GLubyte tankCannonColors[] = { 255, 0, 0, 255, 255, 255, 0, 255, 0, 255, 0, 255, };
static float tankCannonTextureCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

GLubyte tankCannonVertexIndices[] = { 0, 1, 2, 3, 4 };

// -----------------------

GLuint bulletPosVBO, bulletColourVBO, bulletTexCoordVBO, bulletIndicesVBO;

//										1				2			3			4				5			6			7
static GLfloat bulletVertices[] = { 0.025f, 0.05f, -0.025f, 0.05f, -0.05f, 0.0f, -0.025, -0.05f, 0.025f, -0.05f, 0.05f, 0.0f };
static GLubyte bulletColors[] = { 255, 0, 0, 255, 255, 255, 0, 255, 0, 255, 0, 255, };
static float bulletTextureCoords[] = { 40.0f, 20.0f, -40.0f, 20.0f, -20.0f, 0.0f, -40.0f, -20.0f, 40.0f, -20.0f, 20.0f, 0.0f };

GLubyte bulletVertexIndices[] = { 0, 1, 2, 3, 4, 5};
#pragma endregion

#pragma region// Function Prototypes

void init(int argc, char* argv[]); //initalize function
void setupQuadTextureGroundVBO(void); //setup groundVBO function
void setupQuadTextureMountainVBO(void); //setup skyVBO function
void setupHouseVBO(void); //setup houseVBO function
void setupRoofVBO(void);
void setupStairVBO(void);
void setupStair2VBO(void);
void setupTankFundVBO(void);
void setupTankBodyVBO(void);
void setupTankHeadVBO(void);
void setupTankCannonVBO(void);
void setupBulletVBO(void);
void report_version(void); //report version function 
void display(void); //display function
void drawTexturedQuadVBOGround(void); //draw grassVBO
void drawTexturedQuadVBOMountain(void); //draw skyVBO
void drawHouse_VBO(void); //draw house VBO 
void drawRoof_VBO(void);
void drawStair_VBO(void);
void drawStair2_VBO(void);
void drawTankFund_VBO(void);
void drawTankBody_VBO(void);
void drawTankHead_VBO(void);
void drawTankCannon_VBO(void);
void drawBullet_VBO(void);
void drawBullet2_VBO(void);
void drawBullet3_VBO(void);
void drawBullet4_VBO(void);
void drawBullet5_VBO(void);
void drawMickeyMouse(GUMatrix4 &R); //hierarchial model
void drawCircle(void); //draw circle funciton
void update(void); //update function
void mouseButtonDown(int button_id, int state, int x, int y); //event handle function
void mouseMove(int x, int y);//event handle function
void keyDown(unsigned char key, int x, int y);//event handle function
#pragma endregion

int _tmain(int argc, char* argv[]) {

	init(argc, argv);

	glutMainLoop();

	// Shut down COM
	shutdownCOM();

	return 0;
}

void init(int argc, char* argv[]) {

	// Initialise COM so we can use Windows Imaging Component
	initCOM();

	// Initialise FreeGLUT
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	glutInitWindowSize(800, 800);
	glutInitWindowPosition(64, 64);
	glutCreateWindow("17065178 - Computer Graphics Coursework");

	// Register callback functions
	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutKeyboardFunc(keyDown);
	glutMouseFunc(mouseButtonDown);
	glutMotionFunc(mouseMove);


	// Initialise GLEW library
	GLenum err = glewInit();

	// Ensure GLEW was initialised successfully before proceeding
	if (err == GLEW_OK) {

		cout << "GLEW initialised okay\n";

	} else {

		cout << "GLEW could not be initialised\n";
		throw;
	}
	
	// Example query OpenGL state (get version number)
	report_version();


	// Report maximum number of vertex attributes
	GLint numAttributeSlots;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numAttributeSlots);
	cout << "GL_MAX_VERTEX_ATTRIBS = " << numAttributeSlots << endl;


	// Initialise OpenGL...

	// Setup colour to clear the window
	glClearColor(0.2f, 0.2f, 0.8f, 0.0f);

	glLineWidth(9.0f);

	// Load demo texture
	myGroundTexture = fiLoadTexture("grass.png");
	myMountainTexture = fiLoadTexture("sky.jpg");
	myHouseTexture = fiLoadTexture("house.jpg");
	myRoofTexture = fiLoadTexture("roof.jpg");
	myStairTexture = fiLoadTexture("stair.jpg");
	myBulletTexture = fiLoadTexture("bullet.jpg");
	myBullet2Texture = fiLoadTexture("bullet2.png");
	myTankTexture = fiLoadTexture("tank.jpg");
	myTankTrackTexture = fiLoadTexture("tankTrack.jpg");
	myTankCannonTexture = fiLoadTexture("cannon.jpg");

	///Load textures here

	// Shader setup 
	myShaderProgram = setupShaders(string("Shaders\\basic_vertex_shader.txt"), string("Shaders\\basic_fragment_shader.txt"));
	myShaderProgramNoTexture = setupShaders(string("Shaders\\notexture_vertex_shader.txt"), string("Shaders\\notexture_fragment_shader.txt"));

	// Get uniform location of "T" variable in shader program (we'll use this in the play function to give the uniform variable "T" a value)
	locT = glGetUniformLocation(myShaderProgram, "T");
	locT2 = glGetUniformLocation(myShaderProgramNoTexture, "T2");

	// Setup star object using Vertex Buffer Objects (VBOs)
	setupQuadTextureGroundVBO();
	setupQuadTextureMountainVBO();
	setupHouseVBO();
	setupRoofVBO();
	setupStairVBO();
	setupStair2VBO();
	setupTankFundVBO();
	setupTankBodyVBO();
	setupTankHeadVBO();
	setupTankCannonVBO();
	setupBulletVBO();
	myCircle = new Circle();
}

void report_version(void) {

	int majorVersion, minorVersion;

	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

	cout << "OpenGL version " << majorVersion << "." << minorVersion << "\n\n";
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	 drawTexturedQuadVBOMountain();//draw sky
	 drawTexturedQuadVBOGround();//draw ground
	 drawHouse_VBO();//draw house
	 drawRoof_VBO();//draw roof
	 drawStair_VBO();//draw sandbag
	 drawStair2_VBO();//draw smaller sandbag
	 drawTankFund_VBO();//draw tank track
	 drawTankBody_VBO();//draw tank body
	 drawTankHead_VBO(); // draw tank tower
	 drawTankCannon_VBO(); // draw tank gun
	 drawBullet_VBO(); //draw projectile
	 drawBullet2_VBO();
	 drawBullet3_VBO();
	 drawBullet4_VBO();
	 drawBullet5_VBO();
	 GUMatrix4 R = GUMatrix4::identity();
	 glLoadMatrixf((GLfloat*)&R);
	 //drawMickeyMouse(R); //draw character
	
	

	glutSwapBuffers();
}

#pragma region //setup functions
void setupTankFundVBO(void)
{
	glGenBuffers(1, &tankFundPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tankFundPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tankFundVertices), tankFundVertices, GL_STATIC_DRAW);
	glGenBuffers(1, &tankFundColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tankFundColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tankFundColors), tankFundColors, GL_STATIC_DRAW);
	glGenBuffers(1, &tankFundTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tankFundTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tankFundTextureCoords), tankFundTextureCoords, GL_STATIC_DRAW);
	glGenBuffers(1, &tankFundIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tankFundIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tankFundVertexIndices), tankFundVertexIndices, GL_STATIC_DRAW);
}
void setupTankBodyVBO(void){
	glGenBuffers(1, &tankBodyPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tankBodyPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tankBodyVertices), tankBodyVertices, GL_STATIC_DRAW);
	glGenBuffers(1, &tankBodyColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tankBodyColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tankBodyColors), tankBodyColors, GL_STATIC_DRAW);
	glGenBuffers(1, &tankBodyTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tankBodyTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tankBodyTextureCoords), tankBodyTextureCoords, GL_STATIC_DRAW);
	glGenBuffers(1, &tankBodyIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tankBodyIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tankBodyVertexIndices), tankBodyVertexIndices, GL_STATIC_DRAW);
}
void setupTankHeadVBO(void){
	glGenBuffers(1, &tankHeadPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tankHeadPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tankHeadVertices), tankHeadVertices, GL_STATIC_DRAW);
	glGenBuffers(1, &tankHeadColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tankHeadColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tankHeadColors), tankHeadColors, GL_STATIC_DRAW);
	glGenBuffers(1, &tankHeadTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tankHeadTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tankHeadTextureCoords), tankHeadTextureCoords, GL_STATIC_DRAW);
	glGenBuffers(1, &tankHeadIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tankHeadIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tankHeadVertexIndices), tankHeadVertexIndices, GL_STATIC_DRAW);
}
void setupTankCannonVBO(void){
	glGenBuffers(1, &tankCannonPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tankCannonPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tankCannonVertices), tankCannonVertices, GL_STATIC_DRAW);
	glGenBuffers(1, &tankCannonColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tankCannonColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tankCannonColors), tankCannonColors, GL_STATIC_DRAW);
	glGenBuffers(1, &tankCannonTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tankCannonTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tankCannonTextureCoords), tankCannonTextureCoords, GL_STATIC_DRAW);
	glGenBuffers(1, &tankCannonIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tankCannonIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tankCannonVertexIndices), tankCannonVertexIndices, GL_STATIC_DRAW);
}
void setupBulletVBO(void){
	glGenBuffers(1, &bulletPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, bulletPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bulletVertices), bulletVertices, GL_STATIC_DRAW);
	glGenBuffers(1, &bulletColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, bulletColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bulletColors), bulletColors, GL_STATIC_DRAW);
	glGenBuffers(1, &bulletTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, bulletTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bulletTextureCoords), bulletTextureCoords, GL_STATIC_DRAW);
	glGenBuffers(1, &bulletIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bulletIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bulletVertexIndices), bulletVertexIndices, GL_STATIC_DRAW);
}
void setupStairVBO(void){
	glGenBuffers(1, &stairPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, stairPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(stairVertices), stairVertices, GL_STATIC_DRAW);
	glGenBuffers(1, &stairColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, stairColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(stairColors), stairColors, GL_STATIC_DRAW);
	glGenBuffers(1, &stairTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, stairTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(stairTextureCoords), stairTextureCoords, GL_STATIC_DRAW);
	glGenBuffers(1, &stairIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, stairIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(stairVertexIndices), stairVertexIndices, GL_STATIC_DRAW);
}
void setupStair2VBO(void){
	glGenBuffers(1, &stairPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, stairPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(stairVertices), stairVertices, GL_STATIC_DRAW);
	glGenBuffers(1, &stairColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, stairColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(stairColors), stairColors, GL_STATIC_DRAW);
	glGenBuffers(1, &stairTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, stairTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(stairTextureCoords), stairTextureCoords, GL_STATIC_DRAW);
	glGenBuffers(1, &stairIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, stairIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(stairVertexIndices), stairVertexIndices, GL_STATIC_DRAW);
}
void setupRoofVBO(void){
	glGenBuffers(1, &roofPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, roofPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(roofVertices), roofVertices, GL_STATIC_DRAW);
	glGenBuffers(1, &roofColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, roofColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(roofColors), roofColors, GL_STATIC_DRAW);
	glGenBuffers(1, &roofTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, roofTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(roofTextureCoords), roofTextureCoords, GL_STATIC_DRAW);
	glGenBuffers(1, &roofIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roofIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(roofVertexIndices), roofVertexIndices, GL_STATIC_DRAW);
}
void setupHouseVBO(void) {
	glGenBuffers(1, &housePosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, housePosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(houseVertices), houseVertices, GL_STATIC_DRAW);
	glGenBuffers(1, &houseColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, houseColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(houseColors), houseColors, GL_STATIC_DRAW);
	glGenBuffers(1, &houseTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, houseTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(houseTextureCoords), houseTextureCoords, GL_STATIC_DRAW);
	glGenBuffers(1, &houseIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, houseIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(houseVertexIndices), houseVertexIndices, GL_STATIC_DRAW);
}
void setupQuadTextureGroundVBO(void) {

	// setup VBO for the quad object position data
	glGenBuffers(1, &quadPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	// setup VBO for the quad object colour data
	glGenBuffers(1, &quadColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadColors), quadColors, GL_STATIC_DRAW);

	// setup VBO for the quad object texture coord data
	glGenBuffers(1, &quadTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadTextureCoords), quadTextureCoords, GL_STATIC_DRAW);

	// setup quad vertex index array
	glGenBuffers(1, &quadIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadVertexIndices), quadVertexIndices, GL_STATIC_DRAW);
}
void setupQuadTextureMountainVBO(void) {

	// setup VBO for the quad object position data
	glGenBuffers(1, &quadMPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadMPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadMVertices), quadMVertices, GL_STATIC_DRAW);

	// setup VBO for the quad object colour data
	glGenBuffers(1, &quadMColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadMColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadMColors), quadMColors, GL_STATIC_DRAW);

	// setup VBO for the quad object texture coord data
	glGenBuffers(1, &quadMTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadMTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadMTextureCoords), quadMTextureCoords, GL_STATIC_DRAW);

	// setup quad vertex index array
	glGenBuffers(1, &quadMIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadMIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadMVertexIndices), quadMVertexIndices, GL_STATIC_DRAW);
}
#pragma endregion

#pragma region // draw functions
void drawTankFund_VBO(void)
{
	GUMatrix4 T = GUMatrix4::translationMatrix(0.4f,-0.5f, 0.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);
	glUseProgram(myShaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myTankTrackTexture);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);
	glBindBuffer(GL_ARRAY_BUFFER, tankFundPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, tankFundColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, tankFundTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tankFundIndicesVBO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisable(GL_TEXTURE_2D);
}
void drawTankBody_VBO(void){
	GUMatrix4 T = GUMatrix4::translationMatrix(0.3f, -0.43f, 0.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);
	glUseProgram(myShaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myTankTexture);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);
	glBindBuffer(GL_ARRAY_BUFFER, tankBodyPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, tankBodyColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, tankBodyTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tankBodyIndicesVBO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisable(GL_TEXTURE_2D);
}
void drawTankHead_VBO(void){
	GUMatrix4 T = GUMatrix4::translationMatrix(0.45f, -0.35f, 0.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);
	glUseProgram(myShaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myTankTexture);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);
	glBindBuffer(GL_ARRAY_BUFFER, tankHeadPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, tankHeadColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, tankHeadTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tankHeadIndicesVBO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisable(GL_TEXTURE_2D);
}
void drawTankCannon_VBO(void){
	GUMatrix4 T = GUMatrix4::translationMatrix(0.6f, -0.35f, 0.0f)*GUMatrix4::rotationMatrix(0.0f, 0.0f, theta1);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);
	glUseProgram(myShaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myTankCannonTexture);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);
	glBindBuffer(GL_ARRAY_BUFFER, tankCannonPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, tankCannonColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, tankCannonTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tankCannonIndicesVBO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisable(GL_TEXTURE_2D);
}
void drawBullet_VBO(void){
	GUMatrix4 T = GUMatrix4::translationMatrix(0.05f+theta0a, -0.33f+theta0b, 0.0f)*GUMatrix4::rotationMatrix(0.0f,0.0f,theta0c);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);
	glUseProgram(myShaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myBulletTexture);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);
	glBindBuffer(GL_ARRAY_BUFFER, bulletPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, bulletColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, bulletTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bulletIndicesVBO);
	glDrawElements(GL_TRIANGLE_FAN, 7, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisable(GL_TEXTURE_2D);
}
void drawBullet2_VBO(void){
	GUMatrix4 T = GUMatrix4::translationMatrix(0.25f, -0.5f, 0.0f)*GUMatrix4::rotationMatrix(0.0f, 0.0f, theta0c)*GUMatrix4::scaleMatrix(0.75f,0.75f,1.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);
	glUseProgram(myShaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myBulletTexture);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);
	glBindBuffer(GL_ARRAY_BUFFER, bulletPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, bulletColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, bulletTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bulletIndicesVBO);
	glDrawElements(GL_TRIANGLE_FAN, 7, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisable(GL_TEXTURE_2D);
}
void drawBullet3_VBO(void){
	GUMatrix4 T = GUMatrix4::translationMatrix(0.2f, -0.5f, 0.0f)*GUMatrix4::rotationMatrix(0.0f, 0.0f, theta0c)*GUMatrix4::scaleMatrix(0.75f, 0.75f, 1.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);
	glUseProgram(myShaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myBulletTexture);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);
	glBindBuffer(GL_ARRAY_BUFFER, bulletPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, bulletColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, bulletTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bulletIndicesVBO);
	glDrawElements(GL_TRIANGLE_FAN, 7, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisable(GL_TEXTURE_2D);
}
void drawBullet4_VBO(void){
	GUMatrix4 T = GUMatrix4::translationMatrix(0.15f, -0.5f, 0.0f)*GUMatrix4::rotationMatrix(0.0f, 0.0f, theta0c)*GUMatrix4::scaleMatrix(0.75f, 0.75f, 1.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);
	glUseProgram(myShaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myBullet2Texture);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);
	glBindBuffer(GL_ARRAY_BUFFER, bulletPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, bulletColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, bulletTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bulletIndicesVBO);
	glDrawElements(GL_TRIANGLE_FAN, 7, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisable(GL_TEXTURE_2D);
}
void drawBullet5_VBO(void){
	GUMatrix4 T = GUMatrix4::translationMatrix(0.2f, -0.45f, 0.0f)*GUMatrix4::rotationMatrix(0.0f, 0.0f, theta0c)*GUMatrix4::scaleMatrix(0.75f, 0.75f, 1.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);
	glUseProgram(myShaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myBullet2Texture);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);
	glBindBuffer(GL_ARRAY_BUFFER, bulletPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, bulletColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, bulletTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bulletIndicesVBO);
	glDrawElements(GL_TRIANGLE_FAN, 7, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisable(GL_TEXTURE_2D);
}
void drawStair2_VBO(void)
{
	GUMatrix4 T = GUMatrix4::translationMatrix(-0.35f, -0.5f, 0.0f)*GUMatrix4::scaleMatrix(1.0f,0.5f+theta2/2,0.5f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);
	glUseProgram(myShaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myStairTexture);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);
	glBindBuffer(GL_ARRAY_BUFFER, stairPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, stairColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, stairTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, stairIndicesVBO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisable(GL_TEXTURE_2D);
}
void drawStair_VBO(void)
{
	GUMatrix4 T = GUMatrix4::translationMatrix(-0.5f, -0.5f, 0.0f)*GUMatrix4::scaleMatrix(1.0f,1+theta2,1.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);
	glUseProgram(myShaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myStairTexture);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);
	glBindBuffer(GL_ARRAY_BUFFER, stairPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, stairColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, stairTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, stairIndicesVBO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisable(GL_TEXTURE_2D);
}
void drawRoof_VBO(void){
	GUMatrix4 T = GUMatrix4::translationMatrix(-1, 0.0f, 0.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);
	glUseProgram(myShaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myRoofTexture);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);
	glBindBuffer(GL_ARRAY_BUFFER, roofPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, roofColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, roofTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roofIndicesVBO);
	glDrawElements(GL_TRIANGLE_FAN, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisable(GL_TEXTURE_2D);
}
void drawHouse_VBO(void) {
	GUMatrix4 T = GUMatrix4::translationMatrix(-1, -0.5f, 0.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);
	glUseProgram(myShaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myHouseTexture);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);
	glBindBuffer(GL_ARRAY_BUFFER, housePosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, houseColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, houseTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, houseIndicesVBO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisable(GL_TEXTURE_2D);
}
void drawTexturedQuadVBOGround(void) {
	
	glUseProgram(myShaderProgram);

	// Move our ground shape to the bottom half of the screen
	GUMatrix4 T = GUMatrix4::translationMatrix(0.0f, -0.5f, 0.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);


	// Bind each vertex buffer and enable
	// The data is still stored in the GPU but we need to set it up (which also includes validation of the VBOs behind-the-scenes)
	
	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myGroundTexture);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);
	
	glBindBuffer(GL_ARRAY_BUFFER, quadPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, quadColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, quadTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);


	// Bind the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIndicesVBO);

	// Draw the object - same function call as used for vertex arrays but the last parameter is interpreted as an offset into the currently bound index buffer (set to 0 so we start drawing from the beginning of the buffer).
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glDisable(GL_TEXTURE_2D);

	// use to force disable our shaderprogram
 	 //glUseProgram(0);

}
void drawTexturedQuadVBOMountain(void) {
	
	glUseProgram(myShaderProgram);

	//Move our shape into the top position
	GUMatrix4 T = GUMatrix4::translationMatrix(0.0f, 0.05f, 0.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);


	// Bind each vertex buffer and enable
	// The data is still stored in the GPU but we need to set it up (which also includes validation of the VBOs behind-the-scenes)

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myMountainTexture);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);

	glBindBuffer(GL_ARRAY_BUFFER, quadMPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, quadMColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, quadMTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);


	// Bind the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadMIndicesVBO);


	// Draw the object - same function call as used for vertex arrays but the last parameter is interpreted as an offset into the currently bound index buffer (set to 0 so we start drawing from the beginning of the buffer).
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glDisable(GL_TEXTURE_2D);
	
	//glUseProgram(0);
}

//Hierarchial modelling

void drawCircle(void)
{
	myCircle->render();
	/*
	glBegin(GL_TRIANGLE_STRIP);

	glColor3ub(255, 255, 255);

	glTexCoord2f(0.4f, 1.0f);
	glVertex2f(-0.1f, 0.0f);

	glTexCoord2f(0.4f, 0.0f);
	glVertex2f(-0.1f, 0.9f);

	glTexCoord2f(0.6f, 1.0f);
	glVertex2f(0.1f, 0.0f);

	glTexCoord2f(0.6f, 0.0f);
	glVertex2f(0.1f, 0.9f);

	glEnd();*/
}
void drawMickeyMouse(GUMatrix4 &R)
{
	matrixStack.push(R);

	// Draw head A

	R = R * GUMatrix4::rotationMatrix(0.0f, 0.0f, 0.0f) * GUMatrix4::scaleMatrix(0.15f, 0.15f, 1.0f);

	glLoadMatrixf((GLfloat*)&R);
	drawCircle();

	matrixStack.push(R);

	// Draw right ear B

	R = R * GUMatrix4::translationMatrix(0.75f, 0.75f, 0.0f) * GUMatrix4::rotationMatrix(0.0f, 0.0f, 0.0f) * GUMatrix4::scaleMatrix(0.5f, 0.5f, 1.0f);
	glLoadMatrixf((GLfloat*)&R);

	drawCircle();

	R = matrixStack.top();
	matrixStack.pop();

	matrixStack.push(R);

	// Draw left ear C
	R = R * GUMatrix4::translationMatrix(-0.75f, 0.75f, 0.0f) * GUMatrix4::rotationMatrix(0.0f, 0.0f, 0.0f)* GUMatrix4::scaleMatrix(0.5f, 0.5f, 1.0f);
	glLoadMatrixf((GLfloat*)&R);

	drawCircle();

	R = matrixStack.top();
	matrixStack.pop();

	matrixStack.push(R);

	// Draw body
	R = R * GUMatrix4::translationMatrix(0.0f, -1.5f, 0.0f) * GUMatrix4::rotationMatrix(1.0f, 0.0f, 0.0f)* GUMatrix4::scaleMatrix(0.5f, 3.0f, 1.0f);
	glLoadMatrixf((GLfloat*)&R);

	drawCircle();
	matrixStack.push(R);


	// Draw left arm

	R = R * GUMatrix4::translationMatrix(-0.25f, 0.5f, 0.0f) * GUMatrix4::rotationMatrix(1.0f, -1.0f, 1.0f)* GUMatrix4::scaleMatrix(2, 2, 2);
	glLoadMatrixf((GLfloat*)&R);

	drawCircle();
	R = matrixStack.top();
	matrixStack.pop();

	matrixStack.push(R);

	// Draw right arm
	R = R * GUMatrix4::translationMatrix(0.25f, 0.5f, 0.0f) * GUMatrix4::rotationMatrix(1.0f, 1.0f, -1.0f)* GUMatrix4::scaleMatrix(2, 2, 2);
	glLoadMatrixf((GLfloat*)&R);

	drawCircle();
	R = matrixStack.top();
	matrixStack.pop();

	matrixStack.push(R);

	// Draw left leg
	R = R * GUMatrix4::translationMatrix(-0.4f, 0.5f, 1.0f) * GUMatrix4::rotationMatrix(0.0f, 0.0f, 1.5f)* GUMatrix4::scaleMatrix(1.5f, 0.5f, 1.0f);
	glLoadMatrixf((GLfloat*)&R);

	drawCircle();
	R = matrixStack.top();
	matrixStack.pop();

	matrixStack.push(R);

	// Draw right leg

	R = R * GUMatrix4::translationMatrix(0.5f, 0.5f, 1.0f) * GUMatrix4::rotationMatrix(0.0f, 0.0f, 1.5f)* GUMatrix4::scaleMatrix(1.5f, 0.5f, 1.0f);
	glLoadMatrixf((GLfloat*)&R);

	drawCircle();
	R = matrixStack.top();
	matrixStack.pop();

	matrixStack.push(R);
}
#pragma endregion
// update is called every frame
void update(void) {

	// Redraw the screen
	glutPostRedisplay();
}

//event handling
#pragma region Event handling functions

void mouseButtonDown(int button_id, int state, int x, int y) {

	if (button_id == GLUT_LEFT_BUTTON) {

		if (state == GLUT_DOWN) {

			mouse_x = x;
			mouse_y = y;

			mDown = true;

		}
		else if (state == GLUT_UP) {

			mDown = false;
		}
	}
}

void mouseMove(int x, int y) {

	if (mDown) {

		int dx = x - mouse_x;
		int dy = y - mouse_y;

		// Ctrl click to rotate, click on its own to move...
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {

			theta0c += float(dy);
		}
		else {

			theta0a += float(dx) * 0.0025f;
			theta0b -= float(dy) * 0.0025f;
		}


		mouse_x = x;
		mouse_y = y;

		glutPostRedisplay();
	}
}

void keyDown(unsigned char key, int x, int y) {

	if (key == 'r') {

		theta0a = 0.0;
		theta0b = 0.0;
		theta0c = 0.0;
		theta1 = 0.0;
		theta2 = 0.0;
		theta3 = 0.0;
		glutPostRedisplay();
	}
	else if (key == 'w' && theta1 >= -0.45f){
		theta1 -= 0.02f;
	}
	else if (key == 's' && theta1 <= 0.1f){
		theta1 += 0.02f;
	}
	else if (key == 'a' && theta2 >=0.0f)
		theta2 -= 0.02f;
	else if (key == 'd' && theta2 <=2.5f)
		theta2 += 0.02f;
}

#pragma endregion
