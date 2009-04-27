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

#include <cstdlib>
#include <cstdio>

#include <GL/glut.h>

#include <cmath>


#include "SpaceForm.h"
#include "kdtree.h"


#define SPLIT_PLANE_X 0
#define SPLIT_PLANE_Y 1
#define SPLIT_PLANE_Z 2

// rand methods
int rand_int(const int & l, const int & r)
{
	return rand()%(r-l)+l;
}

uint32 partition(SolarSystemObject *solarObjects, uint32 left, uint32 right, uint32 splitplane, uint32 pivot)
{
	double pval = splitplane == SPLIT_PLANE_X ? solarObjects[pivot].x : splitplane == SPLIT_PLANE_Y ? solarObjects[pivot].y : solarObjects[pivot].z;
	
	SolarSystemObject temp; 
	temp = solarObjects[right];
	solarObjects[right] = solarObjects[pivot];
	solarObjects[pivot] = temp;
	
	uint32 sindex = left;
	for(uint32 i = left; i < right; i++)
	{
		if(( splitplane == SPLIT_PLANE_X ? solarObjects[i].x : splitplane == SPLIT_PLANE_Y ? solarObjects[i].y : solarObjects[i].z ) < pval)
		{
			temp = solarObjects[i];
			solarObjects[i] = solarObjects[sindex];
			solarObjects[sindex] = temp;
			sindex++;
		}
	}
	
	temp = solarObjects[sindex];
	solarObjects[sindex] = solarObjects[right];
	solarObjects[right] = temp;

	return sindex;
}

void quicksort(SolarSystemObject *solarObjects, uint32 left, uint32 right, uint32 splitplane)
{	
	if(right > left)
	{
		uint32 pivot = rand_int(left,right);
		uint32 npivot = partition(solarObjects,left,right,splitplane,pivot);
		if(npivot > 0)
		{
			quicksort(solarObjects,left,npivot-1,splitplane);
			quicksort(solarObjects,npivot+1,right,splitplane);
		}
	}
}

kdTreeNode::kdTreeNode(double xMin,double yMin,double zMin,double xMax,double yMax,double zMax) :
m_splitplane(SPLIT_PLANE_X),m_xMin(xMin),m_yMin(yMin),m_zMin(zMin),m_xMax(xMax),m_yMax(yMax),m_zMax(zMax),
m_nrObjects(0),m_objects(NULL),m_left(NULL),m_right(NULL)
{
}

kdTreeNode::~kdTreeNode()
{
	if(m_left) 
		delete m_left;
	if(m_right) 
		delete m_right;
}

// kd-tree generation
void kdTreeNode::build(SolarSystemObject *solarObjects, uint32 nrObjects, uint32 maxObjectsInNode, uint32 maxDepth, uint32 cdepth)
{
	if(cdepth==maxDepth || nrObjects <= maxObjectsInNode)
	{
		m_nrObjects = nrObjects;
		m_objects = solarObjects;
		return;
	}
	
	//printf("nrObjects: %i\n",nrObjects);
	
	//splits along the axis of who the treeNode bound is largest
	m_splitplane = (m_xMax-m_xMin > m_yMax-m_yMin) ? SPLIT_PLANE_X : (m_yMax-m_yMin > m_zMax-m_zMin) ? SPLIT_PLANE_Y : SPLIT_PLANE_Z; 
	
	// sorts the points in order along the axis splited
	quicksort(solarObjects,0,nrObjects-1,m_splitplane); 
		
	// calculate the nr objects to the left and right of the objects size.	
	uint32 numl = nrObjects >> 1;
	uint32 numr = nrObjects-numl;
	
	//printf("numl: %i\n",numl);
	//printf("numr: %i\n",numr);
	
	//calculate bounds for each split
	switch(m_splitplane)
	{
		case SPLIT_PLANE_X:
		{
			double px;
			if(numl!=numr)
				px = solarObjects[numl].x;
			else
				px = (solarObjects[numl-1].x+solarObjects[numl].x)*0.5;
				
			m_left = new kdTreeNode(m_xMin,m_yMin,m_zMin,px,m_yMax,m_zMax);
			m_right = new kdTreeNode(px,m_yMin,m_zMin,m_xMax,m_yMax,m_zMax);	
		}break;
		
		case SPLIT_PLANE_Y:
		{
			double py;
			if(numl!=numr)
				py = solarObjects[numl].y;
			else
				py = (solarObjects[numl-1].y+solarObjects[numl].y)*0.5;
		
			m_left = new kdTreeNode(m_xMin,m_yMin,m_zMin,m_xMax,py,m_zMax);
			m_right = new kdTreeNode(m_xMin,py,m_zMin,m_xMax,m_yMax,m_zMax);

		}break;
		
		case SPLIT_PLANE_Z:
		{
			double pz;
			if(numl!=numr)
				pz = solarObjects[numl].z;
			else
				pz = (solarObjects[numl-1].z+solarObjects[numl].z)*0.5;
				
			m_left = new kdTreeNode(m_xMin,m_yMin,m_zMin,m_xMax,m_yMax,pz);
			m_right = new kdTreeNode(m_xMin,m_yMin,pz,m_xMax,m_yMax,m_zMax);
		}break;
	}

	//build sub-trees
	m_left->build(solarObjects,numl,maxObjectsInNode,maxDepth,cdepth+1);
	m_right->build(solarObjects+numl,numr,maxObjectsInNode,maxDepth,cdepth+1);
}

/*void kdTreeNode::save(std::stringstream *stream)
{	
	*stream << m_splitplane;
	
	*stream << m_xMin << m_yMin << m_zMin;
	*stream << m_xMax << m_yMax << m_zMax;
	
	if(!m_left && !m_right)
	{
		// solar data
		*stream << m_nrObjects;
		
		for(uint32 i=0; i < m_nrObjects; i++)
		{
			*stream << m_objects[i].objectID << m_objects[i].typeID << m_objects[i].groupID;
			*stream << m_objects[i].x << m_objects[i].y << m_objects[i].z << m_objects[i].radius;
			*stream << m_objects[i].objectName;
		}
		
		*stream << false;
		*stream << false;
	}

	if(m_left)
	{
		*stream << true; 
		m_left->save(stream);
	}
		
	if(m_right) 
	{
		*stream << true;
		m_right->save(stream);
	}	
}

void kdTreeNode::traverseSaveTreeToBuffer() {
   std::stack<kdTreeNode*> nodes = new std::stack();
   kdTreeNode *currentNode,*right,*left;
   
   nodes.push(m_mainNode);  
   
   while (!nodes.empty()){
      currentNode = nodes.top();
      nodes.pop();
      right = currentNode->m_right;
      if (right != NULL)
         nodes.push(right);
      left = currentNode->m_left;
      if (left != null)
         nodes.push(left);      

      //save to the buffer.
      currentNode->saveToBuffer();
   }
}*/

/*
char finalhash[20];

char hexval[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
for(int j = 0; j < 10; j++){
finalhash[j*2] = hexval[((hval[j] >> 4) & 0xF)];
finalhash[(j*2) + 1] = hexval[(hval[j]) & 0x0F];
}
*/
/*
void kdTreeNode::load(std::stringstream *stream) 
{
	printf("TEST\n");
	*stream >> m_splitplane;
	
	*stream >> m_xMin >> m_yMin >> m_zMin;
	*stream >> m_xMax >> m_yMax >> m_zMax;
	
	bool left,right;
	
	*stream >> left;
	
	if(left)
		m_left->load(stream);
	
	*stream >> right;
	
	if(right)
		m_right->load(stream);
		
	if(!left && !right)
	{
		// solar data
		*stream >> m_nrObjects;
		
		m_objects = new SolarSystemObject[m_nrObjects]; 
		
		for(uint32 i=0; i < m_nrObjects; i++)
		{
			*stream >> m_objects[i].objectID >> m_objects[i].typeID >> m_objects[i].groupID;
			*stream >> m_objects[i].x >> m_objects[i].y >> m_objects[i].z >> m_objects[i].radius;
			*stream >> m_objects[i].objectName;
		}
	}
}
*/

//kd-tree rendering method
void kdTreeNode::render()
{
	if(m_objects != NULL)
	{
		glVertex3f(m_xMin,m_yMin,m_zMin);
		 glVertex3f(m_xMax,m_yMin,m_zMin);
		glVertex3f(m_xMax,m_yMin,m_zMin);
		 glVertex3f(m_xMax,m_yMax,m_zMin);
		glVertex3f(m_xMax,m_yMax,m_zMin);
		 glVertex3f(m_xMin,m_yMax,m_zMin);
		glVertex3f(m_xMin,m_yMax,m_zMin);
		 glVertex3f(m_xMin,m_yMin,m_zMin);
		
		glVertex3f(m_xMin,m_yMin,m_zMax);
		 glVertex3f(m_xMax,m_yMin,m_zMax);
		glVertex3f(m_xMax,m_yMin,m_zMax);
		 glVertex3f(m_xMax,m_yMax,m_zMax);
		glVertex3f(m_xMax,m_yMax,m_zMax);
		 glVertex3f(m_xMin,m_yMax,m_zMax);
		glVertex3f(m_xMin,m_yMax,m_zMax);
		 glVertex3f(m_xMin,m_yMin,m_zMax);
		 
		glVertex3f(m_xMin,m_yMin,m_zMin);
		 glVertex3f(m_xMin,m_yMin,m_zMax);
		glVertex3f(m_xMax,m_yMin,m_zMin);
		 glVertex3f(m_xMax,m_yMin,m_zMax);
		glVertex3f(m_xMax,m_yMax,m_zMin);
		 glVertex3f(m_xMax,m_yMax,m_zMax);
		glVertex3f(m_xMin,m_yMax,m_zMin);
		 glVertex3f(m_xMin,m_yMax,m_zMax);
	}
	// according the plane change the color.
	switch(m_splitplane)
	{
		case SPLIT_PLANE_X:
			glColor3f(0.1,0,0.1);
			break;
		case SPLIT_PLANE_Y:
			glColor3f(0,0.1,0.1);
			break;
		case SPLIT_PLANE_Z:
			glColor3f(0.1,0.1,0);
			break;
	}
		
	//render left and right node
	if(m_left)
		m_left->render();
	if(m_right)
		m_right->render();
}

SolarSystemkdTree::SolarSystemkdTree(uint32 maxDepth,uint32 m_maxObjectsInNode) :
m_mainNode(NULL),m_maxDepth(maxDepth),m_maxObjectsInNode(m_maxObjectsInNode)
{

}
	
SolarSystemkdTree::~SolarSystemkdTree()
{
	if(m_mainNode)
		delete m_mainNode;
}

void SolarSystemkdTree::build_tree(SolarSystemObject *solarObjects, uint32 nrObjects, double xMin,double yMin,double zMin,double xMax,double yMax,double zMax)
{
	m_mainNode = new kdTreeNode(xMin,yMin,zMin,xMax,yMax,zMax);
	m_mainNode->build(solarObjects,nrObjects,m_maxObjectsInNode,m_maxDepth,0);
}

/*
void SolarSystemkdTree::save_tree(std::stringstream *stream)
{
	*stream << m_maxDepth << m_maxObjectsInNode;
	
	if(m_mainNode) 
		m_mainNode->save(stream);
}

void SolarSystemkdTree::load_tree(std::stringstream *stream) 
{
	*stream >> m_maxDepth >> m_maxObjectsInNode;
	
	if(m_mainNode) 
		m_mainNode->load(stream);
}
*/

void SolarSystemkdTree::render_tree()
{
	if(m_mainNode)
		m_mainNode->render();
}

/*
//kd-tree traversal
node * traverse(node * const ctr, const double3 & pos)
{
	if(ctr==NULL) return NULL;
	if(ctr->points!=NULL) return ctr;
	if(ctr->spx==0)
	{
		if(pos.x<ctr->left->x1)
			return traverse(ctr->left,pos);
		else
			return traverse(ctr->right,pos);
	}else if(ctr->spx==1)
	{
		if(pos.y<ctr->left->y1)
			return traverse(ctr->left,pos);
		else
			return traverse(ctr->right,pos);
	}else
	{
		if(pos.z<ctr->left->z1)
			return traverse(ctr->left,pos);
		else
			return traverse(ctr->right,pos);
	}
}
*/

//helper method - returns squared distance to the node's bounds (0 if inside)
/*double disq(const node * const ctr, const double3 & pos)
{
	const double & x = pos.x;
	const double & y = pos.y;
	const double & z = pos.z;
	const double & x0 = m_xMin;
	const double & y0 = m_yMin;
	const double & x1 = m_xMax;
	const double & y1 = m_yMax;
	const double & z0 = m_zMin;
	const double & z1 = m_zMax;
	
	if(x>x0&&y>y0&&x<x1&&y<y1&&z>z0&&z<z1) return 0.0;
	double dx = 0.0;
	double dy = 0.0;
	double dz = 0.0;
	if(x>x1)
	{
		dx = x-x1;
		if(y<y0)
			dy = y-y0;
		else if(y>y1)
			dy = y-y1;
		if(z<z0)
			dz = z-z0;
		else if(z>z1)
			dz = z-z1;
	}else if(x<x0)
	{
		dx = x-x0;
		if(y<y0)
			dy = y-y0;
		else if(y>y1)
			dy = y-y1;
		if(z<z0)
			dz = z-z0;
		else if(z>z1)
			dz = z-z1;
	}else if(z<z0)
	{
		dz = z-z0;
		if(y<y0)
			dy = y-y0;
		else if(y>y1)
			dy = y-y1;
	}else if(z>z1)
	{
		dz = z-z1;
		if(y<y0)
			dy = y-y0;
		else if(y>y1)
			dy = y-y1;
	}else
	{
		if(y<y0)
			dy = y-y0;
		else if(y>y1)
			dy = y-y1;
	}
	return dx*dx+dy*dy+dz*dz;
}

//nearest neighbour
double3 * search1_s(node * const ctr, const double3 & pos, double & dis, double min)
{
	double3 * minp = NULL;
	if(ctr->points!=NULL)
	{//node is leaf node
		const double & x = pos.x;
		const double & y = pos.y;
		const double & z = pos.z;
		for(int i = 0; i<ctr->m_nrObjects; i++)
		{//check all points inside the leaf for new minimum
			double3 * cp = ctr->points+i;
			double dx = cp->x-x;
			double dy = cp->y-y;
			double dz = cp->z-z;
			double dl = dx*dx+dy*dy+dz*dz;
			if(dl<min)
			{
				min = dl;
				minp = cp;
			}
		}
	}else
	{
		double ad = disq(ctr->left,pos);
		double bd = disq(ctr->right,pos);
		if(ad<bd)
		{//search left first
			if(ad<=min)
			{
				double3 * a = search1_s(ctr->left,pos,dis,min);
				if(dis<min)
				{
					min = dis;
					minp = a;
				}
			}
			if(bd<=min)
			{
				double3 * b = search1_s(ctr->right,pos,dis,min);
				if(dis<min)
				{
					min = dis;
					minp = b;
				}
			}
		}else
		{//search right first
			if(bd<=min)
			{
				double3 * b = search1_s(ctr->right,pos,dis,min);
				if(dis<min)
				{
					min = dis;
					minp = b;
				}
			}
			if(ad<=min)
			{
				double3 * a = search1_s(ctr->left,pos,dis,min);
				if(dis<min)
				{
					min = dis;
					minp = a;
				}
			}
		}
	}
	dis = min;
	return minp;
}

double3 * search1(node * const ctr, const double3 & pos)
{//method to be used.
	double dis;
	return search1_s(ctr,pos,dis,1e+250);
}

//nearest k neighbors
double attempt(double3 ** const list, double * const list2, const int & num, double3 * const cp, const double & dl, const double & max, int & cnum)
{//attempts to add data point 'cp' into the current list of k-nearest.
	if(cnum<num)
	{//max number has not been reached, can simply push it on
		list[cnum] = cp;
		list2[cnum++] = dl;
		if(dl>max || cnum==1) return dl;
		return max;
	}
	if(dl>max) return max; //is more than max, can break here
	
	double3 ** cur = NULL;
	double * cur2 = NULL;
	double curv = 0;
	double curv2 = 0;
	for(int i = 0; i<num; i++)
	{//find point furthest from target, and distance to second furthest (this part can probably be optimized by sorting, maybe)
		double3 ** tp = list+i;
		double * dl2 = list2+i;
		double vdl2 = *dl2;
		if(vdl2>curv)
		{
			curv = vdl2;
			cur2 = dl2;
			cur = tp;
		}
		if(vdl2>curv2 && vdl2<curv)
			curv2 = vdl2;
	}
	//replace data point, and adjust current max distance
	*cur = cp;
	*cur2 = dl;
	if(dl>curv2) return dl;
	return curv2;
}

double searchk_s(node * const ctr, const double3 & pos, double3 ** const list, const int & num, int & cnum, double * const list2, double max)
{
	if(ctr->points!=NULL)
	{//leaf node
		const double & x = pos.x;
		const double & y = pos.y;
		const double & z = pos.z;
		for(int i = 0; i<ctr->m_nrObjects; i++)
		{//attempt to add all the points contained in the leaf node
			double3 * cp = ctr->points+i;
			double dx = cp->x-x;
			double dy = cp->y-y;
			double dz = cp->z-z;
			double dl = dx*dx+dy*dy+dz*dz;
			max = attempt(list,list2,num,cp,dl,max,cnum);
		}
	}else
	{
		double ad = disq(ctr->left,pos);
		double bd = disq(ctr->right,pos);
		if(ad<bd)
		{//search left first
			if(ad<=max || cnum<num)
				max = searchk_s(ctr->left,pos,list,num,cnum,list2,max);
			if(bd<=max || cnum<num)
				max = searchk_s(ctr->right,pos,list,num,cnum,list2,max);
		}else
		{//search right first
			if(bd<=max || cnum<num)
				max = searchk_s(ctr->right,pos,list,num,cnum,list2,max);
			if(ad<=max || cnum<num)
				max = searchk_s(ctr->left,pos,list,num,cnum,list2,max);
		}
	}
	return max;
}

void searchk(node * const tree, const double3 & pos, double3 ** const list, const int & num)
{//method to be used.
	double * list2 = new double[num];
	int cnum = 0;
	searchk_s(tree,pos,list,num,cnum,list2,1e+250);
	delete [] list2;
}*/
