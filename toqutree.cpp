
/**
 *
 * toqutree (pa3)
 * significant modification of a quadtree .
 * toqutree.cpp
 * This file will be used for grading.
 *
 */

#include "toqutree.h"

toqutree::Node::Node(pair<int,int> ctr, int dim, HSLAPixel a)
	:center(ctr),dimension(dim),avg(a),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
	{}

toqutree::~toqutree(){
	clear(root);
}

toqutree::toqutree(const toqutree & other) {
	root = copy(other.root);
}


toqutree & toqutree::operator=(const toqutree & rhs){
	if (this != &rhs) {
		clear(root);
		root = copy(rhs.root);
	}
	return *this;
}

toqutree::toqutree(PNG & imIn, int k){ 

/* This constructor grabs the 2^k x 2^k sub-image centered */
/* in imIn and uses it to build a quadtree. It may assume  */
/* that imIn is large enough to contain an image of that size. */

PNG * im = new PNG(imIn);

root = buildTree(im,k);

}

int toqutree::size() {

/* your code here */
	return size(root);
}

int toqutree::size(Node* subRoot){
	if(subRoot->dimension ==0 && subRoot->NW ==NULL){
		return 1;
	}
	else{
		return 1+size(subRoot->NW)+size(subRoot->NE)+size(subRoot->SW)+size(subRoot->SE);
	}

}

toqutree::Node * toqutree::buildTree(PNG * im, int k) {
    stats a = stats(*im);
    if(k > 1) {
        pair<int,int> ul (0,0);
        pair<int,int> lr (pow(2, k) - 1, pow(2,k) -1);
        pair<unsigned int,unsigned int> center;
        HSLAPixel avg = a.getAvg(ul,lr);
        double minSum = -1;
        for(int x = pow(2,k-2); x < (pow(2,k-2)+pow(2,k-1)); x++){
            for(int y = pow(2,k-2); y < ( pow(2,k-2)+ pow(2,k-1)); y++){
                double sum = 0;
                pair<int, int> ul;
                pair<int, int> lr;
                ul.first = x;
                ul.second = y;
                lr.first = x + pow(2, k - 1)-1;
                lr.second = y + pow(2, k - 1)-1;
                sum += a.entropy(ul,lr);
                ul.first = lr.first + 1;
                ul.second = lr.second + 1;
                lr.first = x - 1;
                lr.second = y - 1;
                sum += a.entropy(ul,lr);
                ul.first = x + pow(2,k-1);
                ul.second = y;
                lr.first = x - 1;
                lr.second = y + pow(2,k-1)-1;
                sum += a.entropy(ul,lr);
                ul.first = x;
                ul.second = y + pow(2,k-1);
                lr.first = x + pow(2,k-1)-1;
                lr.second = y - 1;
                sum += a.entropy(ul,lr);

                if(minSum == -1){
                    minSum = sum;
                    center.first = x;
                    center.second = y;
                }
                else if(minSum > sum){
                    minSum = sum;
                    center.first = x;
                    center.second = y;
                }
            }
        }

        Node * mainNode = new Node(center,k,avg);

        ul.first = center.first;
        ul.second = center.second;
        lr.first = center.first + pow(2,k - 1)-1;
        lr.second = center.second + pow(2,k - 1)-1;
        mainNode->SE = buildTree(buildPNG(im, ul, lr),k-1);
        ul.first = lr.first + 1;
        ul.second = lr.second + 1;
        lr.first = center.first - 1;
        lr.second = center.second - 1;
        mainNode->NW = buildTree(buildPNG(im, ul, lr),k-1);
        ul.first = center.first + pow(2,k-1);
        ul.second = center.second;
        lr.first = center.first - 1;
        lr.second = center.second + pow(2,k-1)-1;
        mainNode->SW = buildTree(buildPNG(im, ul, lr),k-1);
        ul.first = center.first;
        ul.second = center.second + pow(2,k-1);
        lr.first = center.first + pow(2,k-1)-1;
        lr.second = center.second - 1;
        mainNode->NE = buildTree(buildPNG(im, ul, lr),k-1);
        return mainNode;
    }

    else if (k == 1){
        pair<int,int> center (1,1);
        pair<int,int> ul (0,0);
        HSLAPixel avg = a.getAvg(ul,center);
        Node * mainNode = new Node(center,k,avg);
        return mainNode;
    }

    else{
        pair<int,int> center (0,0);
        HSLAPixel avg = a.getAvg(center,center);
        Node * mainNode = new Node(center,k,avg);
        return mainNode;
    }
// Note that you will want to practice careful memory use
// In this function. We pass the dynamically allocated image
// via pointer so that it may be released after it is used .
// similarly, at each level of the tree you will want to 
// declare a dynamically allocated stats object, and free it
// once you've used it to choose a split point, and calculate
// an average.

}

cs221util::PNG* toqutree::buildPNG(PNG *im, pair<unsigned int,unsigned int> ul, pair<unsigned int,unsigned int> lr){
	unsigned int I = 0;
	unsigned int J = 0;
	PNG *subIm;
	if(ul.first > lr.first && ul.second > lr.second ){
		subIm = new PNG(im->width() - ul.first + lr.first, im->height()- ul.second + lr.second);

		for (unsigned int i = ul.first; i<im->width(); i++){
			for(unsigned int j = ul.second; j<im->height();j++){
				*subIm->getPixel(I, J) = *im->getPixel(i, j);
				J++;
			}
			J = 0;
			I++;
		}

		J = 0;
		int temp = 0;
		for (unsigned int i = 0; i<=lr.first; i++){
			for(unsigned int j = ul.second; j<im->height();j++){
				*subIm->getPixel(I, J) = *im->getPixel(i, j);
				J++;
			}
			temp = J;
			J = 0;
			I++;
		}

		I = 0;
		J = temp;
		for (unsigned int i = ul.first; i<im->width(); i++){
			for(unsigned int j = 0; j<=lr.second; j++){
				*subIm->getPixel(I, J) = *im->getPixel(i, j);
				J++;
			}
			J = temp;
			I++;
		}

		J = temp;
		for(unsigned int i = 0; i<= lr.first; i++){
			for(unsigned int j = 0; j<=lr.second; j++){
				*subIm->getPixel(I, J) = *im->getPixel(i, j);
				J++;
			}
			J = temp;
			I++;
		}


	}
	else if(ul.first > lr.first){
		subIm = new PNG(im->width() - ul.first + lr.first, ul.second - lr.second);
		for(unsigned int i = ul.first; i<im->width(); i++){
			for(unsigned int j = ul.second; j<=lr.second; j++){
				*subIm->getPixel(I, J) = *im->getPixel(i, j);
				J++;
			}
			J = 0;
			I++;
		}

		for(unsigned int i = 0; i<= lr.first; i++){
			for(unsigned int j = ul.second; j<=lr.second; j++){
				*subIm->getPixel(I, J) = *im->getPixel(i, j);
				J++;
			}
			J = 0;
			I++;
		}

	}
	else if(ul.second > lr.second){
		subIm = new PNG(ul.first - lr.first, im->height()- ul.second + lr.second);
		for(unsigned int j = ul.second; j<im->height(); j++){
			for (unsigned int i = ul.first; i<=lr.first; i++){
				*subIm->getPixel(I, J) = *im->getPixel(i, j);
				I++;
			}
			I = 0;
			J++;
		}
		for(unsigned int j = 0; j<=lr.second; j++){
			for(unsigned int i = ul.first; i <= lr.first; i++){
				*subIm->getPixel(I, J) = *im->getPixel(i, j);
				I++;
			}
			I = 0;
			J++;
		}

	}
	else {
		subIm = new PNG(lr.first - ul.first + 1, lr.second - ul.second + 1);
		for (unsigned int i = ul.first; i < lr.first + 1; i++) {
			for (unsigned int j = ul.second; j < lr.second + 1; j++) {
				*subIm->getPixel(I, J) = *im->getPixel(i, j);
				J++;
			}
			I++;
		}
	}

	return subIm;
}

PNG toqutree::render(){

// My algorithm for this problem included a helper function
// that was analogous to Find in a BST, but it navigated the 
// quadtree, instead.

/* your code here */
	PNG *image = new PNG(pow(2,root->dimension), pow(2,root->dimension));
	return render(root, *image);
}

PNG toqutree::render(Node* subRoot, PNG & image){
	if(subRoot->NW == NULL){
		unsigned int x = subRoot->center.first;
		unsigned int y = subRoot->center.second;
		HSLAPixel* pixel = image.getPixel(x,y);
		*pixel = subRoot->avg;
		return image;
	}
	else{
		render(subRoot->NW, image);
		render(subRoot->NE, image);
		render(subRoot->SE, image);
		render(subRoot->SW, image);

		return image;
	}
}

/* oops, i left the implementation of this one in the file! */
void toqutree::prune(double tol){

	prune(root,tol);

}

void toqutree::prune(Node* subRoot, double tol){
	if(subRoot->dimension == 0){
		return;
	}

	else{
		HSLAPixel average = subRoot->avg;
		if(average.dist(subRoot->NW->avg)<=tol && average.dist(subRoot->NE->avg)<=tol && average.dist(subRoot->SE->avg)<=tol && average.dist(subRoot->SW->avg)<=tol){
			clear(subRoot->NW);
			clear(subRoot->NE);
			clear(subRoot->SW);
			clear(subRoot->SE);
			render();
		}
		else{
			prune(subRoot->NW, tol);
			prune(subRoot->NE, tol);
			prune(subRoot->SW, tol);
			prune(subRoot->SE, tol);
		}
	}
}

/* called by destructor and assignment operator*/
void toqutree::clear(Node * & curr){
/* your code here */
	if(curr->NW ==NULL){
		delete(curr);
		curr = NULL;
	}
	else{
		clear(curr->NW);
		clear(curr->NE);
		clear(curr->SW);
		clear(curr->SE);
		delete(curr);
		curr = NULL;
	}
}

/* done */
/* called by assignment operator and copy constructor */
toqutree::Node * toqutree::copy(const Node * other) {

/* your code here */
	Node *newRoot = new Node(other->center, other->dimension, other->avg);
	if(other!=NULL){
		copy(newRoot, other);
	}
	return newRoot;
}

void toqutree::copy(Node *& subRoot, const Node* other){
	if(other->NW==NULL){
		Node * newNode = new Node(other->center, other->dimension, other->avg);
		subRoot = newNode;
		newNode = NULL;
		subRoot->NW = NULL;
		subRoot->NE = NULL;
		subRoot->SW = NULL;
		subRoot->SE = NULL;
	}
	else{
		Node * newNode = new Node(other->center, other->dimension, other->avg);
		subRoot = newNode;
		newNode = NULL;
		copy(subRoot->NW, other->NW);
		copy(subRoot->NE, other->NE);
		copy(subRoot->SW, other->SW);
		copy(subRoot->SE, other->SE);

	}

}


