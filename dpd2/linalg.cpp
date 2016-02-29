/*
 * linalg.cpp
 *
 *  Created on: Feb 29, 2016
 *      Author: Hayden
 */

#include "linalg.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include "dpd2.h"

// Constants
#define PI 3.14159265

namespace dpd2 {

	namespace linalg {

		/******************************* Vector *******************************/
		Vector::Vector(float _x, float _y, float _z) {
			x = _x;
			y = _y;
			z = _z;
			correctGUID();
		}

		Vector::~Vector() {}

		const char* Vector::classname() {
			return "Vector";
		}

		const std::string Vector::toString() {
			std::stringstream s;
			s << "[";
			s << std::fixed << std::setprecision(3) << x << ", ";
			s << std::fixed << std::setprecision(3) << y << ", ";
			s << std::fixed << std::setprecision(3) << z << "]";
			return s.str();
		}

		void Vector::add(Vector& v) {
			x += v.x;
			y += v.y;
			z += v.z;
		}

		void Vector::sub(Vector& v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
		}

		void Vector::mult(float c) {
			x *= c;
			y *= c;
			z *= c;
		}

		void Vector::div(float c) {
			x /= c;
			y /= c;
			z /= c;
		}

		float Vector::magSq() {
			return x*x + y*y + z*z;
		}

		float Vector::mag() {
			return (float) sqrt(magSq());
		}

		void Vector::normalize() {
			div(mag());
		}

		float Vector::dot(Vector& v) {
			return x*v.x + y*v.y + z*v.z;
		}

		Vector Vector::cross(Vector& v) {
			Vector vv(
				(y * v.z) - (v.y * z),
				(z * v.x) - (v.z * x),
				(x * v.y) - (v.x * y)
			);
			return vv;
		}

		Vector add(Vector& v1, Vector& v2) {
			Vector v(v1.x + v2.x,
					v1.y + v2.y,
					v1.z + v2.z);
			return v;
		}

		float dot(Vector& v1, Vector& v2) {
			return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
		}

		Vector sub(Vector& v1, Vector& v2) {
			Vector v(v1.x - v2.x,
					v1.y - v2.y,
					v1.z - v2.z);
			return v;
		}

		Vector mult(Vector& v1, float c) {
			Vector v(v1.x * c,
					v1.y * c,
					v1.z * c);
			return v;
		}

		Vector div(Vector& v1, float c) {
			Vector v(v1.x / c,
					v1.y / c,
					v1.z / c);
			return v;
		}

		Vector cross(Vector& v1, Vector& v2) {
			Vector vv(
				(v1.y * v2.z) - (v2.y * v1.z),
				(v1.z * v2.x) - (v2.z * v1.x),
				(v1.x * v2.y) - (v2.x * v1.y)
			);
			return vv;
		}

		Vector random3D() {
			static utils::RealDice<float> dice(0.0f, 2 * PI);
			  float angle1 = dice.roll();
			  float angle2 = dice.roll();
			  float height = std::cos( angle2 );
			  Vector v (std::sin( angle1 ) * height, // x
					    std::cos( angle1 ) * height, // y
					    std::sin( angle2 )); // z
			  return v;
		}

		Vector random2D() {
			static utils::RealDice<float> dice(0.0f, 2 * PI);
			  float angle1 = dice.roll();
			  Vector v (std::sin( angle1 ), // x
					    std::cos( angle1 ), // y
					    0.0f); // z
			  return v;
		}

		/******************************* Vector *******************************/
	}
}


