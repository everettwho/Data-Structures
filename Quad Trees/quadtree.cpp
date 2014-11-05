// **************************************************************
// *		   
// *  quadtree.cpp
// *		   
// *  Quadtree class
// *		   
// *  CS 225 Spring 2008
// *		   
// **************************************************************

#include "quadtree.h"
#include "iostream"

using namespace std;

Quadtree::Quadtree()
{
	root = NULL;
	resolution = 0;
}

Quadtree::Quadtree(const PNG & source, int res)
{
	root = new QuadtreeNode();
	resolution = res;
	
	buildTree(source, resolution);
}

Quadtree::Quadtree(Quadtree const & other)
{
	copy(other);
}

Quadtree::~Quadtree()
{
	clear(root);
	root == NULL;
}

Quadtree const & Quadtree::operator=(Quadtree const & other)
{
	if (this != &other) {
		clear(root);
		copy(other);
	}
	return *this;
}

void Quadtree::clear(QuadtreeNode * & node)
{
	if (node == NULL) {return;}
	
	clear(node->nwChild);
	clear(node->neChild);
	clear(node->swChild);
	clear(node->seChild);
	
	delete node;
	node = NULL;
}

void Quadtree::copy(Quadtree const & tree)
{
	this->root = copy(tree.root);
	this->resolution = tree.resolution;
}

Quadtree::QuadtreeNode * Quadtree::copy(QuadtreeNode * const& node)
{
	QuadtreeNode * newNode;
	
	if (node == NULL) {newNode = NULL;}
	else {
		newNode = new QuadtreeNode(node->element);
		
		newNode->nwChild = copy(node->nwChild);
		newNode->neChild = copy(node->neChild);
		newNode->swChild = copy(node->swChild);
		newNode->seChild = copy(node->seChild);
	}
	
	return newNode;
}

void Quadtree::buildTree(PNG const & source, int res)
{
	clear(root);
	this->resolution = res;	
	buildTreeHelper(source, res, root, 0, 0);
}

void Quadtree::buildTreeHelper(PNG const & source, int res, QuadtreeNode * & newRoot, int x, int y)
{
	if (res == 1) {
		newRoot = new QuadtreeNode(*source(x, y));
		return;
	}
	
	newRoot = new QuadtreeNode();

	buildTreeHelper(source, res / 2, newRoot->nwChild, x, y);
	buildTreeHelper(source, res / 2, newRoot->neChild, x + res / 2, y);
	buildTreeHelper(source, res / 2, newRoot->swChild, x, y + res / 2);
	buildTreeHelper(source, res / 2, newRoot->seChild, x + res / 2, y + res / 2);
	
	averageChildRoots(newRoot);
}

void Quadtree::averageChildRoots(QuadtreeNode * node)
{
	node->element.red = ((node->nwChild->element.red) + (node->neChild->element.red) + (node->swChild->element.red) + (node->seChild->element.red)) / 4;
	
	node->element.green = ((node->nwChild->element.green) + (node->neChild->element.green) + (node->swChild->element.green) + (node->seChild->element.green)) / 4;
	
	node->element.blue = ((node->nwChild->element.blue) + (node->neChild->element.blue) + (node->swChild->element.blue) + (node->seChild->element.blue)) / 4;
}


RGBAPixel Quadtree::getPixel(int x, int y) const
{
	if (x >= resolution || y >= resolution || x < 0 || y < 0) {
		return RGBAPixel();
	}
	else {
		return getPixelHelper(root, x, y, resolution / 2, resolution / 2, resolution / 4);
	}
}

RGBAPixel Quadtree::getPixelHelper(QuadtreeNode * const& newRoot, int x, int y, int xlim, int ylim, int dim) const
{
	if (newRoot->nwChild == NULL) {
		return newRoot->element;
	}
	
	if (x < xlim) {
		if (y < ylim) {
			return getPixelHelper(newRoot->nwChild, x, y, xlim - dim, ylim - dim, dim / 2);
		}
		else {
			return getPixelHelper(newRoot->swChild, x, y, xlim - dim, ylim + dim, dim / 2);
		}
	}
	else {
		if (y < ylim) {
			return getPixelHelper(newRoot->neChild, x, y, xlim + dim, ylim - dim, dim / 2);
		}
		else {
			return getPixelHelper(newRoot->seChild, x, y, xlim + dim, ylim + dim, dim / 2);
		} 
	}
}

PNG Quadtree::decompress() const
{
	PNG output;
	if (root == NULL) {return output;}
	
	output.resize(resolution, resolution);
	
	for (int i = 0; i < resolution; i++) {
		for (int j = 0; j < resolution; j++) {
			*output(i, j) = getPixel(i, j);
		}
	}
	
	return output;
}

void Quadtree::clockwiseRotate()
{
	clockwiseRotateHelper(root);
}

void Quadtree::clockwiseRotateHelper(QuadtreeNode * & node)
{
	if (node == NULL) {return;}
	if (node->nwChild == NULL) {return;}
	
	QuadtreeNode * nw = node->nwChild;
	QuadtreeNode * ne = node->neChild;
	QuadtreeNode * sw = node->swChild;
	QuadtreeNode * se = node->seChild;
	
	node->nwChild = sw;
	node->neChild = nw;
	node->swChild = se;
	node->seChild = ne;
	
	clockwiseRotateHelper(node->nwChild);
	clockwiseRotateHelper(node->neChild);
	clockwiseRotateHelper(node->swChild);
	clockwiseRotateHelper(node->seChild);
}

void Quadtree::prune(int tolerance)
{
	pruneHelper(tolerance, root);
}

void Quadtree::pruneHelper(int tolerance, QuadtreeNode * & node)
{
	if (node == NULL) {return;}
	if (node->nwChild == NULL) {return;}
	
	bool nw = checkPrunable(tolerance, node, node->nwChild);
	bool ne = checkPrunable(tolerance, node, node->neChild);
	bool sw = checkPrunable(tolerance, node, node->swChild);
	bool se = checkPrunable(tolerance, node, node->seChild);
	
	if (nw && ne && sw && se) {
		clear(node->nwChild);
		clear(node->neChild);
		clear(node->swChild);
		clear(node->seChild);
		
		node->nwChild = NULL;
		node->neChild = NULL;
		node->swChild = NULL;
		node->seChild = NULL;
		
		return;
	}
	
	pruneHelper(tolerance, node->nwChild);
	pruneHelper(tolerance, node->neChild);
	pruneHelper(tolerance, node->swChild);
	pruneHelper(tolerance, node->seChild);
		
}

bool Quadtree::checkPrunable(int tolerance, QuadtreeNode * const & node1, QuadtreeNode * & node2) const
{
	if (node1 == NULL || node2 == NULL) {return false;}
	
	if (node2->nwChild == NULL) {
		int red = (node1->element.red - node2->element.red) * (node1->element.red - node2->element.red);
		int green = (node1->element.green - node2->element.green) * (node1->element.green - node2->element.green);
		int blue = (node1->element.blue - node2->element.blue) * (node1->element.blue - node2->element.blue);
		
		return ((red + green + blue) <= tolerance);
	}
	else {
		return (checkPrunable(tolerance, node1, node2->nwChild) && checkPrunable(tolerance, node1, node2->neChild) && checkPrunable(tolerance, node1, node2->swChild) && checkPrunable(tolerance, node1, node2->seChild));
	}
}

int Quadtree::pruneSize(int tolerance) const
{
	if (root == NULL) {return 0;}
	return ((resolution * resolution) - pruneSizeHelper(tolerance, root));
}

int Quadtree::pruneSizeHelper(int tolerance, QuadtreeNode * const & node) const
{
	if (node == NULL) {return 0;}
	if (node->nwChild == NULL) {return 0;}
	
	bool nw = checkPrunable(tolerance, node, node->nwChild);
	bool ne = checkPrunable(tolerance, node, node->neChild);
	bool sw = checkPrunable(tolerance, node, node->swChild);
	bool se = checkPrunable(tolerance, node, node->seChild);
	
	if (nw && ne && sw && se) {return (getLeafCount(node) - 1);}
	else {return (pruneSizeHelper(tolerance, node->nwChild) + pruneSizeHelper(tolerance, node->neChild) + pruneSizeHelper(tolerance, node->swChild) + pruneSizeHelper(tolerance, node->seChild));}
}

int Quadtree::getLeafCount(QuadtreeNode * const & node) const
{
	if (node == NULL) {return 0;}
	if (node->nwChild == NULL) {return 1;}
	
	return (getLeafCount(node->nwChild) + getLeafCount(node->neChild) + getLeafCount(node->swChild) + getLeafCount(node->seChild));
}

int Quadtree::idealPrune(int numLeaves) const
{
	int min = 0;
	int max = 123456789;
	int midPoint;
	int size;
	
	while (min < max) {
		midPoint = (min + max) / 2;
		size = pruneSize(midPoint);
		
		if (size > numLeaves) {min = ++midPoint;}
		else if (size < numLeaves) {max = --midPoint;}
		else {
			while(pruneSize(midPoint - 1) == numLeaves) {
				midPoint--;
			}
			
			return midPoint;
		}
	}
	
	return midPoint;
}



