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

		/************************** PBC Cluster Solving Framework **************************/
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
			linalg::Vector* boxDimensions;
			std::vector<Cluster*> clusters;
			ClusteringSolver(linalg::Vector* boxDimensions);
			void addCluster(Cluster* cluster);
			virtual const char* classname();
			virtual void deriveClusters(std::vector<SimulationObject*>& objects) = 0;
			virtual ~ClusteringSolver();
		};

		/************************** PBC Cluster Solving Framework **************************/
	}

}



#endif /* CLUSTER_H_ */
