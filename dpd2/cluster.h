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

		class Cluster : public Object {
		public:
			index id;
			std::vector<SimulationObject*> objects;
			Cluster(index& idCounter);
			virtual ~Cluster();
			virtual const char* classname();
			void addObj(SimulationObject* obj);
		};

		class ClusterSolver : public Object {
		public:
			linalg::Vector* boxDimensions;
			float cutoffDistance, cutoffDistSq;
			std::vector<Cluster*> clusters;
			ClusterSolver(linalg::Vector* boxDimensions, float cutoffDistance);
			void addCluster(Cluster* cluster);
			virtual const char* classname();
			virtual void deriveClusters(std::vector<SimulationObject*>& objects) = 0;
			virtual ~ClusterSolver();
			float getPBCDistSq(geom::Position* p1, geom::Position* p2);
		};

		/************************** PBC Cluster Solving Framework **************************/
	}

}



#endif /* CLUSTER_H_ */
