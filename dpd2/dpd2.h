/*
 * dpd2.h
 *
 *  Created on: Feb 28, 2016
 *      Author: Hayden
 */

#ifndef DPD2_H_
#define DPD2_H_

#include "Object.h"

#include <string>

namespace dpd2 {

	typedef unsigned char byte;

	typedef unsigned short index;

	namespace utils {
		std::string randomString(int size);
	}

	namespace identifiers {
		const std::string& localID(void);
		std::string assignGUID(Object* obj);
	}

}

#endif /* DPD2_H_ */
