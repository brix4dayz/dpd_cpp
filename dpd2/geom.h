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
		class Position : public linalg::Vector {
		public:
			Position(float x=0.0f, float y=0.0f, float z=0.0f);
			virtual ~Position();
			virtual const char* classname();
		};

		/**
		 *
		 */
		class Line : public Object {
		public:
			Position ep1, ep2;
			linalg::Vector direction;

			Line(float x1, float y1, float x2, float y2);
			Line(Position& p1, Position& p2);
			Line(Position& p1, linalg::Vector& line);
			virtual ~Line();
			virtual const char* classname();

		};

		/**
		 * = 0 :
		 * < 0 :
		 * > 0 :
		 */
		float ptOnLine(Position& pt,
				Position& ep1, Position& ep2);

		float ptOnLine(Position& pt, Line& l);
	}

}



#endif /* GEOM_H_ */
