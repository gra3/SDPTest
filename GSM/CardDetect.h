#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Card.h"
#include "Deck.h"

using namespace cv;
using namespace std;

class CardDetect
{
public:
	VideoCapture *cap;
	int screenWidth, screenHeight;

	Mat src,gray,canny,hough;
	Mat heart, spade, club, diamond;
	Mat a,k,q,j,ten,nine,eight,seven,six,five,four,three,two;

	CardDetect(int camNum);
	~CardDetect(void);

	Point2f computeIntersect(Vec4i a, Vec4i b);

	double computeDistance(Point2f a, Point2f b);
	
	double computeDistance(Vec4i a, Vec4i b);

	double computeSlope(Vec4i a);

	Point2f findMidpoint(Vec4i a);

	double computeAngle(Vec4i a, Vec4i b);

	double computeLength(Vec4i a);

	bool areLengthsSimular(Vec4i a, Vec4i b);

	bool areSlopesSimular(Vec4i a, Vec4i b);

	void sortCorners(vector<Point2f> &corners, Point2f center);

	void preloadRefs();

	bool match(vector<Point2f> inRect,Card &outCard);

	vector<Card> getCards(int numCards, Deck &deck);
	
};

