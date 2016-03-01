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

	namespace cluster {

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
		ClusteringSolver::ClusteringSolver(linalg::Vector* boxDimensions) {
			this->boxDimensions = boxDimensions;
			correctGUID();
		}

		ClusteringSolver::~ClusteringSolver() {
			for (auto it = clusters.begin(); it != clusters.end(); it++) {
				delete *it;
			}
			clusters.clear();
		}

		void ClusteringSolver::addCluster(Cluster* cluster) {
			clusters.push_back(cluster);
		}

		const char* ClusteringSolver::classname() {
			return "ClusteringSolver";
		}

		/********************************** ClusteringSolver - Abstract *******************/


	}

}


