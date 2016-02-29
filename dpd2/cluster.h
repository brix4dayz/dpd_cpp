/*
 * cluster.h
 *
 *  Created on: Feb 29, 2016
 *      Author: Hayden
 */

#ifndef CLUSTER_H_
#define CLUSTER_H_

#include "geom.h"
#include <vector>

namespace dpd2 {

	namespace cluster {

		typedef struct BinCoordinates {
			index i,j,k;
		} BinCoordinates;

		class Binnable {
		public:
			BinCoordinates* coords;
			geom::Position r;
			Binnable(geom::Position& r);
			Binnable(float x, float y, float z);
			void calcCoords(float binSize, index numBins);
			virtual ~Binnable();
		};

		class Bin : public Object {
		public:
			BinCoordinates* coords;
			std::vector<Binnable> objects;
			bool checked, grouped;
			Bin(index i, index j, index k);
			virtual ~Bin();
			virtual const char* classname();
			virtual const std::string toString();
			void addBinnable(Binnable& obj);
			bool isEmpty();
		};

		class Cluster : public Object {
		public:
			index id;
			std::vector<Bin*> binList;
			Cluster(index& idCounter);
			virtual ~Cluster();
			virtual const char* classname();
			virtual const std::string toString();
			void addBin(Bin* bin);
		};

		class BinCube : public Object {
		private:
			Bin**** bins;
		public:
			BinCube(float boxLength, float binSize);
			void fillBins(std::vector<Binnable>& objects);
			void addBinnable(Binnable& obj);
			virtual const char* classname();
			virtual const std::string toString();
			virtual ~BinCube();
			std::vector<Cluster*> determineClusters();
			void compareBin(Bin* bin, Cluster* cluster);
		};


	}

}



#endif /* CLUSTER_H_ */
