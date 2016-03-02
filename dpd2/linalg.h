/*
 * linalg.h
 *
 *  Created on: Feb 29, 2016
 *      Author: Hayden
 */

#ifndef LINALG_H_
#define LINALG_H_

#include "object.h"

namespace dpd2 {

	namespace linalg {

		/**
		 *
		 */
		class Vector : public Object {
		public:
			float x,y,z;

			Vector(float x=0.0f, float y=0.0f, float z=0.0f); // 3d
			virtual ~Vector();
			virtual const char* classname();
			virtual const std::string toString();

			void add(Vector& v);
			void sub(Vector& v);
			void mult(float c);
			void div(float c);

			float dot(Vector& v);
			Vector cross(Vector& v);

			float magSq();
			float mag();

			void normalize();
		};

		Vector add(Vector& v1, Vector& v2);
		Vector sub(Vector& v1, Vector& v2);
		Vector mult(Vector& v, float c);
		Vector div(Vector& v, float c);

		float dot(Vector& v1, Vector& v2);
		Vector cross(Vector& v1, Vector& v2);

		Vector random3D();
		Vector random2D();

	}
}


#endif /* LINALG_H_ */
