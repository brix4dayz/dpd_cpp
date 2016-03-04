/*
 * BinBox.h
 *
 *  Created on: Mar 1, 2016
 *      Author: Hayden
 */

#ifndef BINBOX_H_
#define BINBOX_H_

#include "cluster.h"

namespace dpd2 {

	namespace cluster {

		/************************** BinBox Implementation **************************/
		class BinBox;

		typedef struct BinCoordinates {
			short i,j,k;
		} BinCoordinates;

		typedef struct BinBoxDimensions {
			short x,y,z;
		} BinBoxDimensions;

		class BinBoundsException : public std::exception {
		public:
			BinCoordinates* coords;
			char* buffer;
			BinBoundsException(BinCoordinates* coords);
			virtual const char* what() const noexcept;
			virtual ~BinBoundsException();
		};

		class Binnable : public Object {
		public:
			BinCoordinates* coords;
			SimulationObject* obj;
			Binnable(SimulationObject* obj, float binSize, BinBoxDimensions* dimensions);
			virtual const char* classname();
			virtual const std::string toString();
			virtual ~Binnable();
		};

		class BinCube : public Object {
		public:
			BinCoordinates* coords;
			std::vector<Binnable*> objects;
			bool grouped;
			BinCube(short i, short j, short k);
			virtual ~BinCube();
			virtual const char* classname();
			void addBinnable(Binnable* obj);
			bool isEmpty();
			bool groupBins(BinCube* other, BinBox* solver);
		};

		class BinCluster : public Object {
		public:
			std::vector<BinCube*> binList;
			BinCluster();
			virtual ~BinCluster();
			virtual const char* classname();
			void addBin(BinCube* bin);
			void populateCluster(Cluster* cluster);
			unsigned int size();
		};

		class BinBox : public ClusterSolver {
		private:
			BinCube**** bins;
		public:
			BinBoxDimensions* dimensions;
			float binSize;
			BinBox(linalg::Vector* boxDimensions, float binSize, float cutoffDistance);
			void fillBins(std::vector<SimulationObject*>& objects);
			void addBinnable(Binnable* obj);
			virtual const char* classname();
			virtual ~BinBox();
			virtual void deriveClusters(std::vector<SimulationObject*>& objects);
			void compareBin(BinCube* bin, BinCluster* cluster);
			// TODO void clearBins
			// TODO void empty() { clearBins(); emptyCluster(); }
		};

		/************************** BinBox Implementation **************************/


	}
}

#endif /* BINBOX_H_ */
