/*
 * dpd2.h
 *
 *  Created on: Feb 28, 2016
 *      Author: Hayden
 */

#ifndef DPD2_H_
#define DPD2_H_

#include <string>
#include <random>
#include "object.h"

namespace dpd2 {

	typedef unsigned char byte;

	typedef unsigned short index;

	typedef unsigned long identifier;

	namespace utils {

		/**
		 * Random
		 */
		template <typename T>
		class IntegerDice {
		  private:
			std::uniform_int_distribution< T > dice;
		  public:
			T roll();
			~IntegerDice();
			IntegerDice( T min, T max );
			//virtual const char* classname();
		};

		template <typename T>
		class RealDice{
		  private:
			std::uniform_real_distribution< T > dice;
		  public:
			T roll();
			~RealDice();
			RealDice( T min, T max );
			//virtual const char* classname();
		};


		std::string randomString(int size);
	}

	namespace guid {
		const std::string& localID(void);
		std::string assignGUID(Object* obj);
	}

}

#endif /* DPD2_H_ */
