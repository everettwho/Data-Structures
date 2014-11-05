// **************************************************************
// *		   
// *  quadtree.h
// *		   
// *  Quadtree class
// *		   
// *  CS 225 Spring 2008
// *		   
// **************************************************************

#ifndef QUADTREE_H
#define QUADTREE_H

#include "png.h"

class Quadtree
{
	public:
	Quadtree();
	Quadtree(const PNG & source, int resolution);
	Quadtree(Quadtree const & other);
	~Quadtree();
	Quadtree const & operator=(Quadtree const & other);
	
	void buildTree(PNG const & source, int resolution);
	RGBAPixel getPixel(int x, int y) const;
	PNG decompress() const;
	void clockwiseRotate();
	void prune(int tolerance);
	int pruneSize(int tolerance) const;
	int idealPrune(int numLeaves) const;
	
	private:

	// A simple class representing a single node of a Quadtree.
	// You may want to add to this class; in particular, it could probably
	// use a constructor or two...
	class QuadtreeNode
	{
		public:
		QuadtreeNode* nwChild;  // pointer to northwest child
		QuadtreeNode* neChild;  // pointer to northeast child
		QuadtreeNode* swChild;  // pointer to southwest child
		QuadtreeNode* seChild;  // pointer to southeast child

		RGBAPixel element;  // the pixel stored as this node's "data"
		
		QuadtreeNode() {
			nwChild = NULL;
			neChild = NULL;
			swChild = NULL;
			seChild = NULL;
			element = RGBAPixel();
		}
		QuadtreeNode(RGBAPixel pixel) {
			nwChild = NULL;
			neChild = NULL;
			swChild = NULL;
			seChild = NULL;
			element = pixel;
		}
	};
	
	QuadtreeNode* root;    // pointer to root of quadtree
	int resolution;
	
	void clear(QuadtreeNode * & node);
	void copy(Quadtree const & tree);
	QuadtreeNode * copy(QuadtreeNode * const& node);
	
	void buildTreeHelper(PNG const & source, int resolution, QuadtreeNode * & newRoot, int x, int y);
	void averageChildRoots(QuadtreeNode * node);
	RGBAPixel getPixelHelper(QuadtreeNode * const & newRoot, int x, int y, int tempx, int tempy, int width) const;
	void clockwiseRotateHelper(QuadtreeNode * & node);
	void pruneHelper(int tolerance, QuadtreeNode * & node);
	bool checkPrunable(int tolerance, QuadtreeNode * const & node1, QuadtreeNode * & node2) const;
	int pruneSizeHelper(int tolerance, QuadtreeNode * const & node) const;
	int getLeafCount(QuadtreeNode * const & node) const;
	
	/**** Functions added for testing/grading                ****/
	/**** Do not remove this line or copy its contents here! ****/
	#include "quadtree_given.h"
};

#endif
