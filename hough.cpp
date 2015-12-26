#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <iostream>

using namespace cv;

int main(int argc, char** argv) {

    Mat img2, img, img3, gray;
    std::vector < std::vector < cv::Point3f >  > objectPoints;
    std::vector < std::vector < cv::Point3f >  > imagePoints;

    int objectIndex = 0;

    for (int i = 1; i < argc; ++i) {

        std::vector< cv::Point3f > currObjectPoint;
        int anchorPoint = -1;
        Point anchor;
        img = imread(argv[i], 1);

        cvtColor(img, gray, CV_BGR2GRAY);
        // smooth it, otherwise a lot of false circles may be detected
        GaussianBlur( gray, gray, Size(9, 9), 2, 2 );
        vector<Vec3f> circles;

        // detect the circles
        HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 2, gray.rows/8, 150, 100, 100, 300 );

        for( size_t i = 0; i < circles.size(); i++ ) {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // draw the circle center
            circle( gray, center, 3, Scalar(0,255,0), -1, 8, 0 );
            // draw the circle outline
            circle( gray, center, radius, Scalar(0,0,255), 3, 8, 0 );

            if (radius > 200) {
                anchorPoint = i;
                anchor = center;
                Point3f pt(center.x,center.y,0);
                currObjectPoint.push_back(pt);
                circle( gray, center, radius, Scalar(255,0,255), 3, 8, 0 );
            }
        }

        int minDist = 1e9;
        int maxDist = 0;
        int minPoint;
        int maxPoint;
        for( size_t i = 0; i < circles.size(); i++ ) {
            if ( i == anchorPoint) continue;

            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            double res = cv::norm(anchor-center);
            if (res < minDist) {
                minDist = res;
                minPoint = i;
            }
            if (res > maxDist) {
                maxDist = res;
                maxPoint = i;
            }

        }

        Point centerMin(cvRound(circles[minPoint][0]), cvRound(circles[minPoint][1]));
        Point3f pt1(centerMin.x,centerMin.y,0);
        currObjectPoint.push_back(pt1);

        Point centerMax(cvRound(circles[maxPoint][0]), cvRound(circles[maxPoint][1]));
        Point3f pt2(centerMax.x,centerMax.y,0);
        currObjectPoint.push_back(pt2);

        Point halfwayLine = centerMax - centerMin;

        for( size_t i = 0; i < circles.size(); i++ ) {
            if ( i == anchorPoint || i == minPoint || i == maxPoint) continue;

            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            double x1 = center.x;
            double y1 = center.y;
            double x2 = centerMin.x;
            double y2 = centerMin.y;
            double x3 = centerMax.x;
            double y3 = centerMax.y;

            double signum = (x2*y3 - x3*y2) - (x1*y3 - x3*y1) + (x1*y2-x2*y1);
            double dist = cv::norm(center - centerMin);
            Point3f pt(x1,y1,0);
            currObjectPoint.push_back(pt);
            double sign = signum/fabs(signum);
            std::cout << "sign = " << sign << "  dist = " << dist << std::endl;
            if (sign > 0 && dist > 400 && dist < 500) {
                std::cout << "case 1 " << std::endl;
            } else if (sign > 0 && dist > 700 && dist < 900) {
                std::cout << "case 2 " << std::endl;
            } else if (sign > 0 && dist > 1000 ) {
                std::cout << "case 3 " << std::endl;
            } else if (sign < 0 && dist > 400 && dist < 500) {
                std::cout << "case 4 " << std::endl;
            } else if (sign < 0 && dist > 700 && dist < 900) {
                std::cout << "case 5 " << std::endl;
            } else if (sign < 0 && dist > 1000 ) {
                std::cout << "case 6 " << std::endl;
            }
        }  
        std::cout << "num detected circles = " << circles.size() << std::endl;
        namedWindow( "circles", 1 );

        resize(gray, img2, Size(), 0.25, 0.25, INTER_LINEAR );

        imshow( "circles", img2 );
        waitKey(0); 

    }

    return 0;
}