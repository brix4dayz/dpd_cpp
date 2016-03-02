/*
 * Object.cpp
 *
 *  Created on: Feb 28, 2016
 *      Author: Hayden
 */

#include "Object.h"
#include "dpd2.h"

namespace dpd2 {

Object::Object() {
	GUID = guid::assignGUID(this);
}

Object::~Object() {}

const char* Object::classname() {
	return "Object";
}

const std::string& Object::getGUID() {
	return GUID;
}

const std::string Object::toString() {
	return GUID;
}

void Object::correctGUID() {
	std::size_t pos = GUID.find("::");
	GUID = classname() + GUID.substr(pos);
}

} /* namespace dpd */
