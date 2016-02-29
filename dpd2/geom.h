/*
 * geom.h
 *
 *  Created on: Feb 29, 2016
 *      Author: Hayden
 */

#ifndef GEOM_H_
#define GEOM_H_

#include "linalg.h"

namespace dpd2 {

	namespace geom {
		/**
		 *
		 */
		class Point : public linalg::Vector {
		public:
			Point(float x=0.0f, float y=0.0f, float z=0.0f);
			virtual ~Point();
			virtual const char* classname();
		};

		/**
		 *
		 */
		class Line : public Object {
		public:
			Point ep1, ep2;
			linalg::Vector direction;

			Line(float x1, float y1, float x2, float y2);
			Line(Point& p1, Point& p2);
			Line(Point& p1, linalg::Vector& line);
			virtual ~Line();
			virtual const char* classname();

		};

		/**
		 * = 0 :
		 * < 0 :
		 * > 0 :
		 */
		float ptOnLine(Point& pt,
				Point& ep1, Point& ep2);

		float ptOnLine(Point& pt, Line& l);
	}

}



#endif /* GEOM_H_ */
