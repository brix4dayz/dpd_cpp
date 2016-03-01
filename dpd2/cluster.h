/*
 * cluster.h
 *
 *  Created on: Feb 29, 2016
 *      Author: Hayden
 */

#ifndef CLUSTER_H_
#define CLUSTER_H_

#include "geom.h"
#include "dpd2.h"
#include <vector>
#include <exception>

namespace dpd2 {

	namespace cluster {

		typedef struct BinCoordinates {
			index i,j,k;
		} BinCoordinates;

		typedef struct BinBoxDimensions {
			index x,y,z;
		} BinBoxDimensions;

		class Binnable {
		public:
			BinCoordinates* coords;
			geom::Position r;
			Binnable(geom::Position& r);
			Binnable(float x, float y, float z);
			void calcCoords(float binSize, BinBoxDimensions* dimensions);
			virtual ~Binnable();
		};

		class BinCube : public Object {
		public:
			BinCoordinates* coords;
			std::vector<Binnable*> objects;
			bool checked, grouped;
			BinCube(index i, index j, index k);
			virtual ~BinCube();
			virtual const char* classname();
			virtual const std::string toString();
			void addBinnable(Binnable* obj);
			bool isEmpty();
		};

		class Cluster : public Object {
		public:
			index id;
			std::vector<BinCube*> binList;
			Cluster(index& idCounter);
			virtual ~Cluster();
			virtual const char* classname();
			virtual const std::string toString();
			void addBin(BinCube* bin);
		};

		class ClusteringSolver : public Object {
		public:
			std::vector<Cluster*> clusters;
			ClusteringSolver();
			void addCluster(Cluster* cluster);
			virtual const char* classname();
			virtual void deriveClusters() = 0;
			virtual ~ClusteringSolver();
		};

		class BinBox : public ClusteringSolver {
		private:
			BinCube**** bins;
		public:
			BinBoxDimensions* dimensions;
			float binSize;
			BinBox(linalg::Vector& boxDimensions, float binSize);
			void fillBins(std::vector<Binnable*>& objects);
			void addBinnable(Binnable* obj);
			virtual const char* classname();
			virtual const std::string toString();
			virtual ~BinBox();
			virtual void deriveClusters();
			void compareBin(BinCube* bin, Cluster* cluster);
		};

		class BinBoundsException : public std::exception {
		public:
			BinCoordinates* coords;
			BinBoundsException(BinCoordinates* coords);
			virtual const char* what() const noexcept;
		};


	}

}



#endif /* CLUSTER_H_ */
