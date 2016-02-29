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

		typedef struct BinBoxDimensions {
			index x,y,z;
		};

		class Binnable {
		public:
			BinCoordinates* coords;
			geom::Position r;
			Binnable(geom::Position& r);
			Binnable(float x, float y, float z);
			void calcCoords(float binSize, BinBoxDimensions* dimensions);
			virtual ~Binnable();
		};

		class Bin : public Object {
		public:
			BinCoordinates* coords;
			std::vector<Binnable&> objects;
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

		class BinBox : public Object {
		private:
			Bin**** bins;
		public:
			BinBoxDimensions* dimensions;
			std::vector<Cluster*> clusters;
			BinBox(linalg::Vector& boxDimensions, float binSize);
			void fillBins(std::vector<Binnable>& objects);
			void addBinnable(Binnable& obj);
			void addCluster(Cluster* cluster);
			virtual const char* classname();
			virtual const std::string toString();
			virtual ~BinBox();
			void deriveClusters();
			void compareBin(Bin* bin, Cluster* cluster);
		};


	}

}



#endif /* CLUSTER_H_ */
