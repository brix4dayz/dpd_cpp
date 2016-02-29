/*
 * geom.cpp
 *
 *  Created on: Feb 29, 2016
 *      Author: Hayden
 */

#include "geom.h"

namespace dpd2 {

	namespace geom {
	float ptOnLine(Point& pt,
					Point& ep1, Point& ep2) {
				return (ep1.x - pt.x) * (ep2.y - pt.y) - (ep1.y - pt.y) * (ep2.x - pt.x);
			}

			float ptOnLine(Point& pt, Line& l) {
				return ptOnLine(pt, l.ep1, l.ep2);
			}

			/******************************* Point *******************************/
			Point::Point(float x, float y, float z) : dpd2::linalg::Vector(x,y,z) {
				correctGUID(); // all classes descending Object must call this in their
							   // constructor in order to have guid have classname
			}

			Point::~Point() {}

			const char* Point::classname() {
				return "Point";
			}
			/******************************* Point *******************************/


			/******************************* Line *******************************/
			Line::Line(float x1, float y1,
					float x2, float y2) :
					ep1(x1, y1), ep2(x2, y2) {
				direction = linalg::sub(ep2, ep1);
				direction.normalize();
				correctGUID();
			}

			Line::Line(Point& p1, Point& p2) {
				ep1 = p1;
				ep2 = p2;
				direction = linalg::sub(ep2, ep1);
				direction.normalize();
				correctGUID();
			}

			Line::Line(Point& p1, linalg::Vector& v) {
				ep1 = p1;
				linalg::Vector vv = linalg::add(p1, v);
				ep2 = *(new Point(vv.x, vv.y));

				direction = v;
				direction.normalize();

				correctGUID();
			}

			Line::~Line() {}

			const char* Line::classname() {
				return "Line";
			}
			/******************************* Line *******************************/
	}

}


