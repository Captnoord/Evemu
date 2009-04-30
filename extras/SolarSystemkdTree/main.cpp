/*
	------------------------------------------------------------------------------------
	LICENSE:
	------------------------------------------------------------------------------------
	This file is part of EVEmu: EVE Online Solar System Viewer
	Copyright 2006 - 2009 The EVEmu Team
	For the latest information visit http://evemu.mmoforge.org
	------------------------------------------------------------------------------------
	This program is free software; you can redistribute it and/or modify it under
	the terms of the GNU Lesser General Public License as published by the Free Software
	Foundation; either version 2 of the License, or (at your option) any later
	version.

	This program is distributed in the hope that it will be useful, but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
	FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along with
	this program; if not, write to the Free Software Foundation, Inc., 59 Temple
	Place - Suite 330, Boston, MA 02111-1307, USA, or go to
	http://www.gnu.org/copyleft/lesser.txt.
	------------------------------------------------------------------------------------
	Author:		mmcs
*/

#include "ascent.h"
#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "SpaceForm.h"
#include "kdtree.h"
#include "camera.h"

/* database handle for static/read only data. */
#define StaticDatabase (*Database_static)

/* database handle for dynamic saving/loading server related data. */
#define DynamicDatabase (*Database_dynamic)


#define CAMSPEEDLIGHT 4
#define CAMSPEED 0.01f

#define SCALE_RESOLUTION 1e-10
//uint32 nnum = 20; //number of points to search for k-nearest
#define MAX_TREE_DEPTH 10 // max depth of recursion in kd-tree
#define MAX_NROBJECTS_NODE 1 //max number of objects per cell in kd-tree

// 142 objects
#define SOLARSYSTEMID 5249 // current fetch solar id

//#define SOLARSYSTEMID 3020 // current fetch solar id

Database* Database_static;
Database* Database_dynamic;

double speed = CAMSPEEDLIGHT;
float ratio;
// GLUT_BITMAP_HELVETICA_10
int font=(int)GLUT_BITMAP_HELVETICA_10;

CCamera Camera;

// solar stuff
uint32 solarSize = 0;
double xMin,yMin,zMin,xMax,yMax,zMax;
SolarSystemObject *mysolarData = NULL;
SolarSystemkdTree mysolarTree(MAX_TREE_DEPTH, MAX_NROBJECTS_NODE);


//***************************************************  glut shit
bool keys[0xff];

void display();

void KeyDown(unsigned char key, int /*x*/, int /*y*/)
{
	switch (key) 
	{
		case 27:		//ESC
			exit(0);
			break;
		case 'a':		
			Camera.RotateY(5.0);
			display();
			break;
		case 'd':		
			Camera.RotateY(-5.0);
			display();
			break;
		case 'w':		
			Camera.MoveForward( -speed ) ;
			display();
			break;
		case 's':		
			Camera.MoveForward( speed ) ;
			display();
			break;
		case 'x':		
			Camera.RotateX(5.0);
			display();
			break;
		case 'y':		
			Camera.RotateX(-5.0);
			display();
			break;
		case 'c':		
			Camera.StrafeRight(-speed);
			display();
			break;
		case 'v':		
			Camera.StrafeRight(speed);
			display();
			break;
		case 'f':
			Camera.MoveUpward(-speed);
			display();
			break;
		case 'r':
			Camera.MoveUpward(speed);
			display();
			break;

		case 'm':
			Camera.RotateZ(-5.0);
			display();
			break;
		case 'n':
			Camera.RotateZ(5.0);
			display();
			break;
		case 'l':
			if(speed == CAMSPEEDLIGHT) 
				speed = CAMSPEED;
			else 
				speed = CAMSPEEDLIGHT;
			break;
	}
	keys[key] = true;
}


void keyUp(unsigned char key,int /*x*/,int /*y*/)
{
	keys[key] = false;
}

void renderBitmapCharacter(float x, float y, float z, void *font,char *string)
{
  char *c;
  glRasterPos3f(x, y, z);
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}

void DrawNet(GLfloat size, GLint LinesX, GLint LinesZ)
{
	glBegin(GL_LINES);
	for (int xc = 0; xc < LinesX; xc++)
	{
		glVertex3f(	-size / 2.0 + xc / (GLfloat)(LinesX-1)*size,
					0.0,
					size / 2.0);
		glVertex3f(	-size / 2.0 + xc / (GLfloat)(LinesX-1)*size,
					0.0,
					size / -2.0);
	}
	for (int zc = 0; zc < LinesX; zc++)
	{
		glVertex3f(	size / 2.0,
					0.0,
					-size / 2.0 + zc / (GLfloat)(LinesZ-1)*size);
		glVertex3f(	size / -2.0,
					0.0,
					-size / 2.0 + zc / (GLfloat)(LinesZ-1)*size);
	}
	glEnd();
}	

/* internal db start up wrapper... */
bool _DBStartup(std::string _hostname, uint32 _port, std::string _username, std::string _password, std::string _databaseStatic, std::string _databaseDynamic)
{
	std::string hostname = _hostname;
	std::string username = _username;
	std::string password = _password;
	std::string staticdb = _databaseStatic;
	std::string dynamicdb = _databaseDynamic;

	uint32 port = _port;
	int type = 1; // 1 is MYSQL
	int connectionCount = 3; // make this configurable

	// we use 2 database handles
	Database_static = Database::CreateDatabaseInterface(type);
	Database_dynamic = Database::CreateDatabaseInterface(type);

	// Initialize it
	if( !StaticDatabase.Initialize(hostname.c_str(), port, username.c_str(),
		password.c_str(), staticdb.c_str(), connectionCount, 16384 ) )
	{
		Log.Error( "Database","Static database initialization failed. Exiting..." );
		return false;
	}

	if( !DynamicDatabase.Initialize(hostname.c_str(), port, username.c_str(),
		password.c_str(), dynamicdb.c_str(), connectionCount, 16384 ) )
	{
		Log.Error( "Database","Dynamic database initialization failed. Exiting..." );
		return false;
	}

	return true;
}

/* internal db shutdown wrapper... */
bool _DBShutdown()
{
	SafeDelete(Database_static);
	SafeDelete(Database_dynamic);
	return true;
}

//*************************************************** kd-store

//double3 ** near_k; //store for k-nearest
//double3 * near_1; //store for nearest
//node * trav; //store for traversal

GLuint kd_rend; //display list for kd-tree rendering

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	Camera.Render();

	/*if(traverse!=NULL)
	{
		glColor4f(0.65,0.65,0.65,0.5);
		glBegin(GL_QUADS);
		glVertex3f(trav->x0,trav->y0,trav->z0);
		glVertex3f(trav->x1,trav->y0,trav->z0);
		glVertex3f(trav->x1,trav->y1,trav->z0);
		glVertex3f(trav->x0,trav->y1,trav->z0);
		
		glVertex3f(trav->x0,trav->y0,trav->z1);
		glVertex3f(trav->x1,trav->y0,trav->z1);
		glVertex3f(trav->x1,trav->y1,trav->z1);
		glVertex3f(trav->x0,trav->y1,trav->z1);
		glEnd();
	}*/

	glCallList(kd_rend);

	/*if(near_k!=NULL)
	{
		glColor3f(1,1,0);
		glBegin(GL_QUADS);
		for(int i = 0; i<nnum; i++)
		{
			double3 * near1 = near_k[i];
			glVertex3f(near1->x-1,near1->y-1,near1->z);
			glVertex3f(near1->x+1,near1->y-1,near1->z);
			glVertex3f(near1->x+1,near1->y+1,near1->z);
			glVertex3f(near1->x-1,near1->y+1,near1->z);
		}
		glEnd();
	}
	if(near_1!=NULL)
	{
		glColor3f(1,0,1);
		glBegin(GL_QUADS);
		 glVertex3f(near_1->x-2,near_1->y-2,near_1->z);
		 glVertex3f(near_1->x+2,near_1->y-2,near_1->z);
		 glVertex3f(near_1->x+2,near_1->y+2,near_1->z);
		 glVertex3f(near_1->x-2,near_1->y+2,near_1->z);
		glEnd();
	}*/
	glFlush();
	glutSwapBuffers();
}

void reshape(int x, int y)
{	
	if (y == 0 || x == 0) return;  //Nothing is visible then, so return

	ratio = 1.0f * x / y;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the clipping volume
	gluPerspective(45,ratio,0.01f,600);

	glMatrixMode(GL_MODELVIEW);
	// Set the view port to be the entire window
	glViewport(0, 0, x, y);
	glLoadIdentity();
}

//*************************************************** old main

/*double3 * points;
node * tree;
double3 mpos;

void mainLoop()
{
	double s = 3.0;
	if(keys[119]) mpos.z+=s;
	if(keys[115]) mpos.z-=s;
	if(keys[97]) mpos.x-=s;
	if(keys[100]) mpos.x+=s;
	if(keys[113]) mpos.y-=s;
	if(keys[101]) mpos.y+=s;

	trav = traverse(tree,mpos);
	searchk(tree,mpos,near_k,nnum);
	near_1 = search1(tree,mpos);
	display();
}*/

SolarSystemObject* Load_SolarSystem_fromDB(uint64 solarsystemID,uint32 *solarSize, double *xMin, double *yMin, double *zMin, double *xMax, double *yMax, double *zMax)
{
	QueryResult *result = NULL;
	Field *field = NULL;
	double xCenter,yCenter,zCenter;
	
	printf("getting solarsystemID info.\n");
	
	result = StaticDatabase.Query("SELECT x,y,z,xMin,yMin,zMin,xMax,yMax,zMax FROM mapSolarSystems WHERE SolarSystemID = %u", solarsystemID);
	if(!result)
	{
		sLog.String("Query failed: SELECT x,y,z,xMin,yMin,zMin,xMax,yMax,zMax FROM mapSolarSystems WHERE SolarSystemID = %u", solarsystemID);
		return NULL;
	}
	
	field = result->Fetch();
	
	xCenter = field[0].GetDouble();
	yCenter = field[1].GetDouble();
	zCenter = field[2].GetDouble();
	
	*xMin = field[3].GetDouble();
	*yMin = field[4].GetDouble();
	*zMin = field[5].GetDouble();
	*xMax = field[6].GetDouble();
	*yMax = field[7].GetDouble();
	*zMax = field[8].GetDouble();

	// move the bounds of the box to the (0,0,0) coordinations, since the bound is relative to the solar center vector.
	// this way we get the bounds in the same position of the objects, since the sun is always in the (0,0,0)
	*xMin -= xCenter;
	*yMin -= yCenter;
	*zMin -= zCenter;
	*xMax -= xCenter;
	*yMax -= yCenter;
	*zMax -= zCenter;
	
	printf("xMin: %e, yMin: %e, zMin: %e, xMax: %e, yMax: %e, zMax: %e\n",*xMin,*yMin,*zMin,*xMax,*yMax,*zMax);
		
	printf("getting objects by solarsystemID.\n");
	
	delete result;
	result = StaticDatabase.Query("SELECT itemID,itemName,typeID,groupID,x,y,z,radius FROM mapDenormalize WHERE SolarSystemID = %u", solarsystemID);
	if(!result)
	{
		sLog.String("Query failed: select itemID,itemName,typeID,groupID,x,y,z,radius from mapDenormalize WHERE SolarSystemID = %u", solarsystemID);
		return NULL;
	}
	
	*solarSize = result->GetRowCount();
	
	SolarSystemObject *newSolarData = new SolarSystemObject[*solarSize];
	
	// NOTE: since we got the above, if we need absolute coordinates for this objects, then we sum the center vector :)
		
	uint32 index = 0;
	do
	{
		field = result->Fetch();
		newSolarData[index].objectID = field[0].GetUInt32();
		strncpy(newSolarData[index].objectName,field[1].GetString(),OBJECT_NAME_MAX);
		newSolarData[index].typeID = field[2].GetUInt16();
		newSolarData[index].groupID = field[3].GetUInt16();
		newSolarData[index].x = field[4].GetDouble();
		newSolarData[index].y = field[5].GetDouble();
		newSolarData[index].z = field[6].GetDouble();
		newSolarData[index].radius = field[7].GetDouble();		

		printf("typeID: %i,objectName: %s, groupID: %i\n",newSolarData[index].typeID,newSolarData[index].objectName,newSolarData[index].groupID);		
		printf("objectID: %i, x: %e, y: %e, z: %e,rad: %e\n",newSolarData[index].objectID,newSolarData[index].x,newSolarData[index].y,newSolarData[index].z,
																	newSolarData[index].radius);	
		index++;
	} while(result->NextRow());
	
	delete result;
	return newSolarData;
}

int main(int argc, char ** argv)
{

	bool dbret =  _DBStartup("localhost",3306, "powanda","theforce", "evemu_trunk", "evemu_rewrite_dynamic");
	if(dbret == false)
	{
		Log.Error("Database", "Unable to connect to the db.");
		return 1;
	}
	uint32 now;

    now = GetTickCount();

	mysolarData = Load_SolarSystem_fromDB(SOLARSYSTEMSUMEDVAL+SOLARSYSTEMID,&solarSize,&xMin,&yMin,&zMin,&xMax,&yMax,&zMax);
	mysolarTree.build_tree(mysolarData,solarSize,xMin,yMin,zMin,xMax,yMax,zMax);
	
	printf("time to build: %ums\n", GetTickCount() - now);
	printf("solar_objects_size: %d\n",solarSize);
	
	glutInit(&argc,argv); 
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1280, 1024);
    glutCreateWindow("evemu solar system viewer"); 
	
	glEnable(GL_DEPTH_TEST);
	glLineWidth(1.5f);
	
	Camera.Move( F3dVector(0.0f, 5.0f, 5.0f ));
	Camera.MoveForward( 5.0f );
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(KeyDown);
	
	glClearColor(0.0f,0.0f,0.0f,0.0f);

	//render tree to display list
	kd_rend = glGenLists(1);
	glNewList(kd_rend,GL_COMPILE);
		// scale the coordinates to fit the prespective.
		glScaled(SCALE_RESOLUTION,SCALE_RESOLUTION,SCALE_RESOLUTION);
		glBegin(GL_LINES);
		 mysolarTree.render_tree();
		glEnd();

		for(uint32 i = 0; i< solarSize; i++)
		{			
			renderBitmapCharacter(mysolarData[i].x,mysolarData[i].y+mysolarData[i].radius*1.5f,mysolarData[i].z,(void *)font,mysolarData[i].objectName);
			glPushMatrix();
				glColor3f(0,1.0f-(i%8)*0.1f,0.2+(i%8)*0.1f);
				glTranslatef(mysolarData[i].x,mysolarData[i].y,mysolarData[i].z);
				glutWireSphere(mysolarData[i].radius, 10, 10);
			glPopMatrix();
		}
	glEndList();

	//enter main execution
	glutMainLoop();

	_DBShutdown();
	
	//clear memory, delete list
	glDeleteLists(kd_rend,1);
	if(mysolarData)	
		delete[] mysolarData;
	
	return 0;
}
