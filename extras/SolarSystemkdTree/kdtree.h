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

#ifndef __KDTREE_H
#define __KDTREE_H

class kdTreeNode
{
	//used only in rendering and traversal, is the axis the plane was split.
	uint32 m_splitplane;
	
	// bounds of the tree node
	double m_xMin,m_yMin,m_zMin;
	double m_xMax,m_yMax,m_zMax;

	// solarsystem stuff
	uint32 m_nrObjects; //number of objects
	SolarSystemObject *m_objects; // pointer to the beginning of the objects	

	//pointers to left and right tree nodes
	kdTreeNode *m_left;
	kdTreeNode *m_right;
	
public:	
	friend class SolarSystemkdTree;
	
	kdTreeNode(double xMin,double yMin,double zMin,double xMax,double yMax,double zMax);
	~kdTreeNode();
	
	void build(SolarSystemObject *solarObjects, uint32 nrObjects, uint32 maxObjectsInNode, uint32 maxDepth, uint32 cdepth = 0);
	void render();
};

class SolarSystemkdTree
{
	kdTreeNode *m_mainNode;

	//uint32 nnum = 20; //number of points to search for k-nearest

	uint32 m_maxDepth; //max depth of recursion in kd-tree
	uint32 m_maxObjectsInNode; //max number of points per cell in kd-tree
	
	uint8* mBuffer;
	size_t mSize;
	size_t mReadIndex;
	
	/*
	void traverseSaveTreeToBuffer();
	void traverseLoadTree(); 
	*/
	 
public:	

	SolarSystemkdTree(uint32 maxDepth,uint32 m_maxObjectsInNode);
	~SolarSystemkdTree();
	
	void build_tree(SolarSystemObject *solarObjects, uint32 nrObjects, double xMin,double yMin,double zMin,double xMax,double yMax,double zMax);
	//void save_tree(std::stringstream *stream);
	//void load_tree(std::stringstream *stream); 
	void render_tree();
};

#endif //__3KDTREE_H
