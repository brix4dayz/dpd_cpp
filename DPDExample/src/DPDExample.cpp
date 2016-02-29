//============================================================================
// Name        : DPDExample.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "geom.h"

#include <iostream>
using namespace std;

int main() {

	dpd2::linalg::Vector o = dpd2::linalg::random3D();
	cout << o.toString() << endl;
	cout << o.getGUID() << endl;


	dpd2::linalg::Vector* v = new dpd2::linalg::Vector(1, 1);
	dpd2::linalg::Vector vv(1, -1);

	std::cout << vv.cross(*v).toString() << std::endl;

	dpd2::geom::Position ep1;

	std::cout << ep1.getGUID() << std::endl;

	dpd2::geom::Position ep2(1,1);

	dpd2::geom::Position pt(0.5f, 0.5f);

	dpd2::geom::Line l(0, 0, 1, 1);

	cout << l.getGUID() << endl;

	cout << dpd2::geom::ptOnLine(pt, l) << endl;

	return 0;
}
