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

		const char* classname() {
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
		ClusteringSolver::ClusteringSolver() {
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

		/********************************** BinBox ****************************************/
		BinBox::BinBox(linalg::Vector& boxDimensions, float binSize) :
		  ClusteringSolver(),
		  binSize(binSize)
		{
			dimensions = new BinBoxDimensions();
			dimensions->x = (index) (boxDimensions.x / binSize);
			dimensions->y = (index) (boxDimensions.y / binSize);
			dimensions->z = (index) (boxDimensions.z / binSize);

			bins = new BinCube***[dimensions->x];
			for (index i = 0; i < dimensions->x; i++) {
				bins[i] = new BinCube**[dimensions->y];
				for (index j = 0; j < dimensions->y; j++) {
					bins[i][j] = new BinCube*[dimensions->z];
					for (index k = 0; k < dimensions->z; k++) {
						bins[i][j][k] = new BinCube(i,j,k);
					}
				}
			}

			correctGUID();
		}

		BinBoundsException::BinBoundsException(BinCoordinates* coords) {
			this->coords = coords;
			buffer = new char[50];
		}

		const char* BinBoundsException::what() const noexcept {
			sprintf(buffer, "Coords (%hu,%hu,%hu) are out of bounds.",  coords->i, coords->j, coords->k);
			return buffer;
		}

		void BinBox::addBinnable(Binnable* obj) {
			BinCoordinates* coords = obj->coords;
			if (coords->i < 0 || coords->i > dimensions->x ||
				coords->j < 0 || coords->j > dimensions->y ||
				coords->k < 0 || coords->k > dimensions->z) {
				throw BinBoundsException(coords);
			}
			bins[coords->i][coords->j][coords->k]->addBinnable(obj);
		}

		void BinBox::fillBins(std::vector<SimulationObject*>& objects) {
			for (auto it = objects.begin(); it != objects.end(); it++) {
				addBinnable(new Binnable(*it, binSize, dimensions));
			}
		}

		const char* BinBox::classname() {
			return "BinBox";
		}

		BinBox::~BinBox() {
			for (index i = 0; i < dimensions->x; i++) {
				for (index j = 0; j < dimensions->y; j++) {
					for (index k = 0; k < dimensions->z; k++) {
						delete bins[i][j][k];
					}
					delete bins[i][j];
				}
				delete bins[i];
			}
			delete bins;
			delete dimensions;
		}

		void BinBox::deriveClusters(std::vector<SimulationObject*>& objects) {
			fillBins(objects);
			// TODO
		}

		void BinBox::compareBin(BinCube* bin, Cluster* cluster) {
			// TODO might want to change parameters
		}

		/********************************** BinBox ****************************************/
	}

}


