/*
 * dpd.cpp
 *
 *  Created on: Feb 28, 2016
 *      Author: Hayden
 */

#include <iostream>
#include "dpd.h"

namespace dpd {

	namespace clustering {

		Example::Example() : stuff("Hello!") {}

		Example::~Example() {}

		void Example::sayHi() { std::cout << stuff << std::endl; }

	}

}



