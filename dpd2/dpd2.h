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
#include "geom.h"
#include <ostream>

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

	class SimulationObject : public Object {
	public:
		geom::Position* r;
		SimulationObject(geom::Position* r);
		SimulationObject(float x, float y, float z);
		virtual const char* classname();
		virtual const std::string toString();
		virtual ~SimulationObject();
	};

	// TODO class SimulationFrame

	class Bead : public Object {
	public:
		unsigned int id, molId;
		byte type;

		Bead(std::string& line);
		Bead(geom::Position* r, byte type, unsigned int& beadCounter, unsigned int molId);

		void printXYZ(std::ostream& out);
		void printDat(std::ostream& out);

		virtual const char* classname();
		virtual const std::string toString();
		virtual ~Bead();
	};

}

#endif /* DPD2_H_ */
