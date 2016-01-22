#include "CardDetect.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <concrt.h>
#include <stdio.h>
#include <cmath>
#include <vector>

CardDetect::CardDetect(int camNum)
{
	cap = new VideoCapture(camNum);
	screenWidth = 720;
	screenHeight = 576;
	cap->set(CV_CAP_PROP_FRAME_WIDTH,720); 
    cap->set(CV_CAP_PROP_FRAME_HEIGHT,576);
	cap->set(CV_CAP_PROP_FORMAT,1);
	cap->set(CV_CAP_PROP_FPS,60);

	preloadRefs();
}


CardDetect::~CardDetect(void)
{
	delete cap;
}

Point2f CardDetect::computeIntersect(Vec4i a, Vec4i b)
{
	int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3], x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];

	if (float d = ((float)(x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)))
	{
		cv::Point2f pt;
		pt.x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / d;
		pt.y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / d;
		return pt;
	}
	else
		return cv::Point2f(-1, -1);
}

double CardDetect::computeDistance(Point2f a, Point2f b)
{
	int x1 = a.x, y1 = a.y, x2 = b.x, y2 = b.y;
	double inside = pow(x2-x1,2)+pow(y2-y1,2);
	return sqrt(inside);
}

double CardDetect::computeDistance(Vec4i a, Vec4i b)
{
	int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3], x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];
	double slopeA = computeSlope(a);
	double reciprocalSlope = -1/slopeA;
	Point2f midA = findMidpoint(a);
	Point2f midB = findMidpoint(b);
	int x5 = 10;
	int y5 = (x5-midA.x)*reciprocalSlope+midA.y;
	Vec4i parallel(midA.x,midA.y,x5,y5);
	Point2f intersectionB = computeIntersect(parallel,b);
	double distance = computeDistance(midA,intersectionB);
	return distance;
}

double CardDetect::computeSlope(Vec4i a)
{
	int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
	double num = y2-y1;
	double den = x2-x1;
	return num/den;
}

Point2f CardDetect::findMidpoint(Vec4i a)
{
	int x1 = a[0], y1 = a[1], x2 = a[2],y2 = a[3];
	Point2f pt((x2+x1)/2,(y1+y2)/2);
	return pt;
}

double CardDetect::computeAngle(Vec4i a, Vec4i b)
{
	int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3], x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];
	double angle1 = atan2(x1-x2,y1-y2);
	double angle2 = atan2(x3-x4,y3-y4);
	double degree =  abs(angle1-angle2)*180/CV_PI;
	return fmod(degree,180);
}

double CardDetect::computeLength(Vec4i a)
{
	Point2f pt1(a[0],a[1]);
	Point2f pt2(a[2],a[3]);
	return computeDistance(pt1,pt2);
}

bool CardDetect::areLengthsSimular(Vec4i a, Vec4i b)
{
	bool areSimular =false;
	double lenA = computeLength(a);
	double lenB = computeLength(b);

	if(lenA>lenB*.95&&lenA<lenB*1.05) areSimular = true;

	return areSimular;
}

bool CardDetect::areSlopesSimular(Vec4i a, Vec4i b)
{
	bool areSimular = false;

	vector<float> x1,x2,y1,y2;
	vector<float> mag1, mag2, angle1, angle2;

	x1.push_back(a[0] - a[2]);
	y1.push_back(a[1] - a[3]);
	x2.push_back(b[0] - b[2]);
	y2.push_back(b[1] - b[3]);

	cartToPolar(x1,y1,mag1,angle1,true);
	cartToPolar(x2,y2,mag2,angle2,true);
	//cout << "angle1: " << angle1[0] << "   angle2: " << angle2[0] << endl;
	//cout << "mag1: " << mag1[0] << "   mag2: " << mag2[0] << endl;
	//if(angle1[0]>angle2[0]*.9&&angle1[0]<angle2[0]*1.1) areSimular = true;
	if(angle1[0]>angle2[0]-3&&angle1[0]<angle2[0]+3) areSimular = true;

	return areSimular;
}

void CardDetect::sortCorners(std::vector<cv::Point2f>& corners, cv::Point2f center)
{
    std::vector<cv::Point2f> top, bot;

    for (int i = 0; i < corners.size(); i++)
    {
        if (corners[i].y < center.y)
            top.push_back(corners[i]);
        else
            bot.push_back(corners[i]);
    }

	if(top.size()>1&&bot.size()>1)
	{
    cv::Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
    cv::Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
    cv::Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
    cv::Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];

    corners.clear();
    corners.push_back(tl);
    corners.push_back(tr);
    corners.push_back(br);
    corners.push_back(bl);
	}
}

void CardDetect::preloadRefs()
{
	heart = imread("refs/heart.jpg",1);
	spade = imread("refs/spade.jpg",1);
	club = imread("refs/club.jpg",1);
	diamond = imread("refs/diamond.jpg",1);

	k = imread("refs/k.jpg",1);
	q = imread("refs/q.jpg",1);
	a = imread("refs/a.jpg",1);
	j = imread("refs/j.jpg",1);
	ten = imread("refs/ten.jpg",1);
	nine = imread("refs/nine.jpg",1);
	eight = imread("refs/eight.jpg",1);
	seven = imread("refs/seven.jpg",1);
	six = imread("refs/six.jpg",1);
	five = imread("refs/five.jpg",1);
	four = imread("refs/four.jpg",1);
	three = imread("refs/three.jpg",1);
	two = imread("refs/two.jpg",1);
}

bool CardDetect::match(vector<Point2f> inRect,Card &outCard)
{
	Mat warped = Mat::zeros(350,250, CV_8UC3);
	vector<Point2f> quad_pts;
	quad_pts.push_back(cv::Point2f(0, 0));
	quad_pts.push_back(cv::Point2f((float)warped.cols, 0));
	quad_pts.push_back(cv::Point2f((float)warped.cols, (float)warped.rows));
	quad_pts.push_back(cv::Point2f(0, (float)warped.rows));

	//Warp Perspective
	Mat transmtx = getPerspectiveTransform(inRect,quad_pts);
	warpPerspective(src,warped,transmtx, warped.size(), INTER_NEAREST, BORDER_CONSTANT);
	imshow("Perspective Transform", warped);

	//Crop Corner
	Rect roi(0,10,40,87);
	Mat cropped = warped(roi);
	cvtColor(cropped,cropped, CV_BGR2GRAY);
	cvtColor(cropped,cropped, CV_GRAY2BGR);
	imshow("Corner", cropped);

	//Set up matching
	int rank = -1;
	int suit = -1;
	double suitMatchPercent = .90;
	double rankMatchPercent = .80;

	Mat result;
	int result_cols = cropped.cols;
	int result_rows = cropped.rows;
	result.create(result_rows,result_cols, CV_32FC1);

	//Capture Corner (for references)
	imwrite("refs/cornerCapture.jpg",cropped);

	//Match Suit
	matchTemplate(cropped, heart, result, CV_TM_CCOEFF_NORMED);
	double minVal; double maxVal; double maxSuitVal[4]; Point minLoc; Point maxLoc;
	Point matchLoc;
	minMaxLoc( result, &minVal, &maxSuitVal[0], &minLoc, &maxLoc, Mat() );

	matchTemplate(cropped, spade, result, CV_TM_CCOEFF_NORMED);
	minMaxLoc( result, &minVal, &maxSuitVal[1], &minLoc, &maxLoc, Mat() );

	matchTemplate(cropped, club, result, CV_TM_CCOEFF_NORMED);
	minMaxLoc( result, &minVal, &maxSuitVal[2], &minLoc, &maxLoc, Mat() );

	matchTemplate(cropped, diamond, result, CV_TM_CCOEFF_NORMED);
	minMaxLoc( result, &minVal, &maxSuitVal[3], &minLoc, &maxLoc, Mat() );

	int suitMax= 0;
	for(int i=0;i<3;i++)
	{
		if(maxSuitVal[suitMax]<maxSuitVal[i+1]) suitMax =i+1;
	}
	if(maxSuitVal[suitMax]>suitMatchPercent) suit = suitMax;


	//Match Rank (if suit is found)
	if(suit!=-1)
	{
		matchTemplate(cropped, a, result, CV_TM_CCOEFF_NORMED);
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		if(maxVal>suitMatchPercent) rank = 14;

		matchTemplate(cropped, k, result, CV_TM_CCOEFF_NORMED);
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		if(maxVal>suitMatchPercent) rank = 13;

		matchTemplate(cropped, q, result, CV_TM_CCOEFF_NORMED);
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		if(maxVal>suitMatchPercent) rank = 12;

		matchTemplate(cropped, j, result, CV_TM_CCOEFF_NORMED);
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		if(maxVal>suitMatchPercent) rank = 11;

		matchTemplate(cropped, ten, result, CV_TM_CCOEFF_NORMED);
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		if(maxVal>suitMatchPercent) rank = 10;

		matchTemplate(cropped, nine, result, CV_TM_CCOEFF_NORMED);
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		if(maxVal>suitMatchPercent) rank = 9;

		matchTemplate(cropped, eight, result, CV_TM_CCOEFF_NORMED);
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		if(maxVal>suitMatchPercent) rank = 8;

		matchTemplate(cropped, seven, result, CV_TM_CCOEFF_NORMED);
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		if(maxVal>suitMatchPercent) rank = 7;

		matchTemplate(cropped, six, result, CV_TM_CCOEFF_NORMED);
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		if(maxVal>suitMatchPercent) rank = 6;

		matchTemplate(cropped, five, result, CV_TM_CCOEFF_NORMED);
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		if(maxVal>suitMatchPercent) rank = 5;

		matchTemplate(cropped, four, result, CV_TM_CCOEFF_NORMED);
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		if(maxVal>suitMatchPercent) rank = 4;

		matchTemplate(cropped, three, result, CV_TM_CCOEFF_NORMED);
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		if(maxVal>suitMatchPercent) rank = 3;

		matchTemplate(cropped, two, result, CV_TM_CCOEFF_NORMED);
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		if(maxVal>suitMatchPercent) rank = 2;
	}
	if(rank!=-1&&suit!=-1) 
	{
		outCard.set(rank,suit);
		return true;
	}
	else return false;
}

vector<Card> CardDetect::getCards(int numCards, Deck &deck)
{
	int cardsDetected = 0;
	vector<Card> cards;
	while(cardsDetected!=numCards)
	{
		//Read Image
		cap->read(src);

		//Convert to Grayscale
		cvtColor(src,gray,CV_BGR2GRAY);

		//Blur
		//blur(gray,gray,Size(3,3));
		GaussianBlur(gray,gray,Size(3,3),1000,1000,BORDER_DEFAULT);

		//Canny Edge Detection
		Canny(gray,canny,30,90,3);

		//Hough Line Detection
		vector<Vec4i> lines;
		cvtColor(canny,hough,COLOR_GRAY2BGR);
		HoughLinesP(canny,lines,1,CV_PI/180,100,80,5);

		//Filter Out Simular Lines
		vector<Vec4i> unique_lines;
		if(lines.size()>0) unique_lines.push_back(lines[0]);
		for(int i=1;i<lines.size();i++)
		{
			bool unique = true;
			for(int j =0;j<unique_lines.size();j++)
			{
				if(areSlopesSimular(lines[i],unique_lines[j])&&computeDistance(lines[i],unique_lines[j])<10) unique = false;
			}
			if(unique) unique_lines.push_back(lines[i]);
		}

		//Pair Lines with simular angle and set distance
		vector<vector<Vec4i>> line_pairs;
		int pairNumber =0;
		for(int i=0;i<unique_lines.size();i++)
		{
			for(int j=i+1;j<unique_lines.size();j++)
			{
				//if(areSlopesSimular(unique_lines[i],unique_lines[j])) cout << "Distance: " << computeDistance(unique_lines[i],unique_lines[j]) << endl;
				vector<Vec4i> temp;
				if(areSlopesSimular(unique_lines[i],unique_lines[j])
					&&computeDistance(unique_lines[i],unique_lines[j])>300
					&&computeDistance(unique_lines[i],unique_lines[j])<600)
				{
					temp.push_back(unique_lines[i]);
					temp.push_back(unique_lines[j]);
					line_pairs.push_back(temp);
				}
			}
		}

		//Make Rects from pairs
		vector<vector<Point2f>> corners;
		for(int i=0;i<line_pairs.size();i++)
		{
			for(int j=i+1;j<line_pairs.size();j++)
			{
				Point2f pt1,pt2,pt3,pt4;
				vector<Point2f> temp;

				//Get intersections (corners)
				pt1 = computeIntersect(line_pairs[i][0],line_pairs[j][0]);
				pt2 = computeIntersect(line_pairs[i][0],line_pairs[j][1]);
				pt3 = computeIntersect(line_pairs[i][1],line_pairs[j][0]);
				pt4 = computeIntersect(line_pairs[i][1],line_pairs[j][1]);
				temp.push_back(pt1);
				temp.push_back(pt2);
				temp.push_back(pt3);
				temp.push_back(pt4);

				// Get mass center
				if(temp.size()>=4)
				{
				Point2f center(0,0);
				for (int i = 0; i < temp.size(); i++) center += temp[i];
				center *= (1. / temp.size());
				sortCorners(temp, center);
				}

				//Make lines for areLengthsSimular()
				Vec4i l1(pt1.x,pt1.y,pt2.x,pt2.y);
				Vec4i l2(pt3.x,pt3.y,pt4.x,pt4.y);
				Vec4i l3(pt1.x,pt1.y,pt3.x,pt3.y);
				Vec4i l4(pt2.x,pt2.y,pt4.x,pt4.y);

				if(isContourConvex(temp)&&
					contourArea(temp)>155000&&
					contourArea(temp)<170000&&
					areLengthsSimular(l1,l2)&&
					areLengthsSimular(l3,l4))
				{
					//cout << "Temp Area: " << contourArea(temp) << endl;
					corners.push_back(temp);
				}
			}
		}

		//Matching
		for(int i = 0;i<corners.size();i++)
		{
			Card detected;
			bool unique = true;
			
			if(match(corners[i],detected))
			{
				if(deck.isNotActive(detected))
				{
					deck.addActive(detected);
					cards.push_back(detected);
					cardsDetected++;
				}
			}
		}


		//cout << "Cards Detected: " << cardsDetected << endl;

		//Display Info
		//cout << "Lines: " << lines.size() << endl;
		//cout << "Unique lines: " << unique_lines.size() << endl;
		//cout << "Line Pairs: " << line_pairs.size() << endl;
		//cout << "Sets of Corners: " << corners.size() << endl;

		//Draw Lines
		/*for( size_t i = 0; i < lines.size(); i++ )
		{
			Vec4i l = lines[i];
			line(hough, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,0), 3, CV_AA);
		}*/

		//Draw Unique Lines
		for( size_t i = 0; i < unique_lines.size(); i++ )
		{
			Vec4i l = unique_lines[i];
			line(hough, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA);
		}

		//Draw Line Pairs
		for( size_t i = 0; i < line_pairs.size(); i++ )
		{
			Vec4i l1 = line_pairs[i][0];
			Vec4i l2 = line_pairs[i][1];
			Point2f midA = findMidpoint(l1);
			Point2f midB = findMidpoint(l2);
			line(hough, Point(l1[0], l1[1]), Point(l1[2], l1[3]), Scalar(255,0,255), 3, CV_AA);
			line(hough, Point(l2[0], l2[1]), Point(l2[2], l2[3]), Scalar(255,0,255), 3, CV_AA);
			line(hough, midA, midB, Scalar(0,255,0),3,CV_AA);
		}

		//Draw Rects
		for(int i=0;i<corners.size();i++)
		{
			line(hough, corners[i][0], corners[i][1], Scalar(0,0,255), 3, CV_AA);
			line(hough, corners[i][1], corners[i][2], Scalar(0,0,255), 3, CV_AA);
			line(hough, corners[i][2], corners[i][3], Scalar(0,0,255), 3, CV_AA);
			line(hough, corners[i][3], corners[i][0], Scalar(0,0,255), 3, CV_AA);
		}

		//Display Windows
		imshow("Source",src);
		imshow("Canny",canny);
		imshow("Hough",hough);

		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
		}
	}
	return cards;
}