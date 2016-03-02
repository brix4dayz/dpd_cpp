/*
 * dpd2.cpp
 *
 *  Created on: Feb 28, 2016
 *      Author: Hayden
 */

#include "dpd2.h"

#include <random>
#include <sstream>
#include <iostream>

namespace dpd2 {

	namespace utils {

		/**
		 *
		 */
		std::string randomString(int size)
		{
			static const char* charSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
			static IntegerDice<unsigned int> intDice(0, strlen(charSet));

			std::stringstream sstr;
			for (int i = 0; i < size; i++) {
				sstr << charSet[intDice.roll()];
			}
			return sstr.str();
		}

		std::random_device seed;
		std::mt19937 gen( seed() );

		template <typename T>
		IntegerDice<T>::IntegerDice(T min, T max) : dice( min, max - 1 ) {
			//correctGUID();
		//	std::cout << GUID << std::endl;
		}

		template <typename T>
		T IntegerDice<T>::roll() {
			return dice(gen);
		}

//		template <typename T>
//		const char* IntegerDice<T>::classname() {
//			return "IntegerDice";
//		}
//
//		template <typename T>
//		const char* RealDice<T>::classname() {
//			return "RealDice";
//		}

		template <typename T>
		IntegerDice<T>::~IntegerDice() {}

		template <typename T>
		RealDice<T>::RealDice(T min, T max) : dice(min, max) {
			//correctGUID();
			//std::cout << GUID << std::endl;
		}

		template <typename T>
		T RealDice<T>::roll() { return dice(gen); }

		template <typename T>
		RealDice<T>::~RealDice() {}

		template class IntegerDice<int>;
		template class IntegerDice<unsigned int>;
		template class IntegerDice<byte>;
		template class IntegerDice<index>;
		template class IntegerDice<short>;
		template class IntegerDice<char>;
		template class IntegerDice<unsigned long>;
		template class IntegerDice<long>;

		template class RealDice<double>;
		template class RealDice<float>;
	}
	/**
	 *
	 */
	const std::string& guid::localID(void)
	{
		static std::string LOCAL_ID = utils::randomString(7);
		return LOCAL_ID;
	}

	/**
	 *
	 */
	std::string guid::assignGUID(Object* obj) {
		std::stringstream ss;
		ss << obj->classname() << "::" << localID() << "::" << utils::randomString(5);
		return ss.str();
	}

}



