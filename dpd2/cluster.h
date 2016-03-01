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

		/************************** Cluster Solving Framework **************************/
		class SimulationObject : public Object {
		public:
			geom::Position* r;
			SimulationObject(geom::Position* r);
			SimulationObject(float x, float y, float z);
			virtual const char* classname();
			virtual const std::string toString();
			virtual ~SimulationObject();
		};

		class Cluster : public Object {
		public:
			index id;
			std::vector<SimulationObject*> objects;
			Cluster(index& idCounter);
			virtual ~Cluster();
			virtual const char* classname();
			void addObj(SimulationObject* obj);
		};

		class ClusteringSolver : public Object {
		public:
			std::vector<Cluster*> clusters;
			ClusteringSolver();
			void addCluster(Cluster* cluster);
			virtual const char* classname();
			virtual void deriveClusters(std::vector<SimulationObject*>& objects) = 0;
			virtual ~ClusteringSolver();
		};

		/************************** Cluster Solving Framework **************************/



		/************************** BinBox Implementation **************************/

		typedef struct BinCoordinates {
			index i,j,k;
		} BinCoordinates;

		typedef struct BinBoxDimensions {
			index x,y,z;
		} BinBoxDimensions;

		class BinBoundsException : public std::exception {
		public:
			BinCoordinates* coords;
			char* buffer;
			BinBoundsException(BinCoordinates* coords);
			virtual const char* what() const noexcept;
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
			bool checked, grouped;
			BinCube(index i, index j, index k);
			virtual ~BinCube();
			virtual const char* classname();
			virtual const std::string toString();
			void addBinnable(Binnable* obj);
			bool isEmpty();
		};

		class BinCluster : public Object {
		public:
			std::vector<BinCube*> binList;
			BinCluster();
			virtual ~BinCluster();
			virtual const char* classname();
			virtual const std::string toString();
			void addBin(BinCube* bin);
			void populateCluster(Cluster* cluster);
		};

		class BinBox : public ClusteringSolver {
		private:
			BinCube**** bins;
		public:
			BinBoxDimensions* dimensions;
			float binSize;
			BinBox(linalg::Vector& boxDimensions, float binSize);
			void fillBins(std::vector<SimulationObject*>& objects);
			void addBinnable(Binnable* obj);
			virtual const char* classname();
			virtual const std::string toString();
			virtual ~BinBox();
			virtual void deriveClusters(std::vector<SimulationObject*>& objects);
			void compareBin(BinCube* bin, Cluster* cluster);
		};

		/************************** BinBox Implementation **************************/

	}

}



#endif /* CLUSTER_H_ */
