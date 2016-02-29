/*
 * Object.h
 *
 *  Created on: Feb 28, 2016
 *      Author: Hayden
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <string>

namespace dpd2 {

	class Object {
	public:
		Object();
		virtual ~Object();
		virtual const char* classname();
		virtual const std::string toString();

		const std::string& getGUID();
		void correctGUID();
		// copy...?

	protected:
		std::string GUID;

	};

} /* namespace dpd */

#endif /* OBJECT_H_ */
