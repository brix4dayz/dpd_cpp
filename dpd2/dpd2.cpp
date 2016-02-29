/*
 * dpd2.cpp
 *
 *  Created on: Feb 28, 2016
 *      Author: Hayden
 */

#include "dpd2.h"

#include <random>
#include <sstream>

namespace dpd2 {

	/**
	 *
	 */
	std::string utils::randomString(int size)
	{
		static const char* charSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
		static std::default_random_engine gen(time(NULL));
		static std::uniform_int_distribution<unsigned int> dist(0, strlen(charSet) - 1); // [a, b]


		std::stringstream sstr;
		for (int i = 0; i < size; i++) {
			sstr << charSet[dist(gen)];
		}
		return sstr.str();
	}

	/**
	 *
	 */
	const std::string& identifiers::localID(void)
	{
		static std::string LOCAL_ID = utils::randomString(7);
		return LOCAL_ID;
	}

	/**
	 *
	 */
	std::string identifiers::assignGUID(Object* obj) {
		std::stringstream ss;
		ss << obj->classname() << "::" << localID() << "::" << utils::randomString(5);
		return ss.str();
	}

}



