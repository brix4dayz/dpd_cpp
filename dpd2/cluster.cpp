/*
 * cluster.cpp
 *
 *  Created on: Feb 29, 2016
 *      Author: Hayden
 */

#include "cluster.h"
#include "dpd2.h"
#include <sstream>

namespace dpd2 {

	/********************************** SimulationObject ******************************/
	SimulationObject::SimulationObject(geom::Position* r) {
		this->r = r;
		correctGUID();
	}

	SimulationObject::SimulationObject(float x, float y, float z) :
	 SimulationObject(new geom::Position(x,y,z)) {}

	SimulationObject::~SimulationObject() { delete r; }

	const char* SimulationObject::classname() {
		return "SimulationObject";
	}

	const std::string SimulationObject::toString() {
		std::stringstream ss;
		ss << GUID << " @ " << r->toString();
		return ss.str();
	}

	/********************************** SimulationObject ******************************/


	namespace cluster {

		/********************************** Cluster **************************************/
		Cluster::Cluster(index& idCounter) {
			id = idCounter++;
			correctGUID();
		}

		Cluster::~Cluster() {
			// DO NOT DELETE OBJECTS, LET WHOEVER PROVIDED THEM
			// DELETE THEM. JUST REMOVE REFERENCES
			for (int i = 0; i < objects.size(); i++) {
				objects[i] = NULL;
			}
			objects.clear();
		}

		void Cluster::addObj(SimulationObject* obj) {
			objects.push_back(obj);
		}

		const char* Cluster::classname() {
			return "Cluster";
		}

		/********************************** Cluster **************************************/


		/********************************** ClusteringSolver - Abstract *******************/
		ClusterSolver::ClusterSolver(linalg::Vector* boxDimensions, float cutoffDistance) :
		 cutoffDistance(cutoffDistance) {
			cutoffDistSq = cutoffDistance * cutoffDistance;
			this->boxDimensions = boxDimensions;
			correctGUID();
		}

		ClusterSolver::~ClusterSolver() {
			for (auto it = clusters.begin(); it != clusters.end(); it++) {
				delete *it;
			}
			clusters.clear();
		}

		void ClusterSolver::addCluster(Cluster* cluster) {
			clusters.push_back(cluster);
		}

		const char* ClusterSolver::classname() {
			return "ClusteringSolver";
		}

		float ClusterSolver::getPBCDistSq(geom::Position* p1, geom::Position* p2) {
			linalg::Vector distance = linalg::sub(*p1, *p2);

			float absD = std::abs(distance.x);
			if (absD >= (boxDimensions->x - cutoffDistance)) {
				distance.x = boxDimensions->x - absD;
			}

			absD = std::abs(distance.y);
			if (absD >= (boxDimensions->y - cutoffDistance)) {
				distance.y = boxDimensions->y - absD;
			}

			absD = std::abs(distance.z);
			if (absD >= (boxDimensions->z - cutoffDistance)) {
				distance.z = boxDimensions->z - absD;
			}

			return distance.magSq();
		}

		/********************************** ClusteringSolver - Abstract *******************/


	}

}


