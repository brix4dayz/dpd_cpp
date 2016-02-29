/*
 * geom.cpp
 *
 *  Created on: Feb 29, 2016
 *      Author: Hayden
 */

#include "geom.h"

namespace dpd2 {

	namespace geom {
	float ptOnLine(Position& pt,
			Position& ep1, Position& ep2) {
				return (ep1.x - pt.x) * (ep2.y - pt.y) - (ep1.y - pt.y) * (ep2.x - pt.x);
			}

			float ptOnLine(Position& pt, Line& l) {
				return ptOnLine(pt, l.ep1, l.ep2);
			}

			/******************************* Position *******************************/
			Position::Position(float x, float y, float z) : dpd2::linalg::Vector(x,y,z) {
				correctGUID(); // all classes descending Object must call this in their
							   // constructor in order to have guid have classname
			}

			Position::~Position() {}

			const char* Position::classname() {
				return "Position";
			}
			/******************************* Position *******************************/


			/******************************* Line *******************************/
			Line::Line(float x1, float y1,
					float x2, float y2) :
					ep1(x1, y1), ep2(x2, y2) {
				direction = linalg::sub(ep2, ep1);
				direction.normalize();
				correctGUID();
			}

			Line::Line(Position& p1, Position& p2) {
				ep1 = p1;
				ep2 = p2;
				direction = linalg::sub(ep2, ep1);
				direction.normalize();
				correctGUID();
			}

			Line::Line(Position& p1, linalg::Vector& v) {
				ep1 = p1;
				linalg::Vector vv = linalg::add(p1, v);
				ep2 = *(new Position(vv.x, vv.y));

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


