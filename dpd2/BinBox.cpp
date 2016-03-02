/*
 * BinBox.cpp
 *
 *  Created on: Mar 1, 2016
 *      Author: Hayden
 */

#include "BinBox.h"
#include <sstream>
#include <iostream>

namespace dpd2 {

	namespace cluster {

		/********************************* Binnable ***************************************/
		Binnable::Binnable(SimulationObject* obj, float binSize, BinBoxDimensions* dimensions) {
			this->obj = obj;

			// calculate coordinates with pbc check
			coords = new BinCoordinates();
			coords->i = (short) (obj->r->x / binSize);
			if (coords->i < 0)
				coords->i += dimensions->x;
			else if (coords->i >= dimensions->x)
				coords->i -= dimensions->x;

			coords->j = (short) (obj->r->y / binSize);
			if (coords->j < 0)
				coords->j += dimensions->y;
			else if (coords->j >= dimensions->y)
				coords->j -= dimensions->y;

			coords->k = (short) (obj->r->z / binSize);
			if (coords->k < 0)
				coords->k += dimensions->z;
			else if (coords->k >= dimensions->z)
				coords->k -= dimensions->z;


			correctGUID();
		}

		const char* Binnable::classname() {
			return "Binnable";
		}

		const std::string Binnable::toString() {
			std::stringstream ss;
			ss << GUID << " @ (" << coords->i << "," << coords->j << "," << coords->k << ") with " << obj->toString();
			return ss.str();
		}

		Binnable::~Binnable() {
			delete coords;
		}

		/********************************* Binnable ***************************************/

		/********************************* BinCube ***************************************/

		BinCube::BinCube(short i, short j, short k) {
			coords = new BinCoordinates();
			coords->i = i;
			coords->j = j;
			coords->k = k;

			grouped = false;
			correctGUID();
		}

		BinCube::~BinCube() {
			for (auto it = objects.begin(); it != objects.end(); it++) {
				delete *it;
			}
			objects.clear();
			delete coords;
		}

		const char* BinCube::classname() {
			return "BinCube";
		}

		void BinCube::addBinnable(Binnable* obj) {
			objects.push_back(obj);
		}

		bool BinCube::isEmpty() {
			return objects.empty();
		}

		bool BinCube::groupBins(BinCube* other, BinBox* solver) {
			SimulationObject* obj1, *obj2;
			for (auto it1 = objects.begin(); it1 != objects.end(); it1++) {
				obj1 = (*it1)->obj;
				for (auto it2 = other->objects.begin(); it2 != other->objects.end(); it2++) {
					obj2 = (*it2)->obj;
					if (solver->getPBCDistSq(obj1->r, obj2->r) <= solver->cutoffDistSq) {
						obj1 = NULL;
						obj2 = NULL;
						return true;
					}
				}
			}

			obj1 = NULL;
			obj2 = NULL;
			return false;
		}

		/********************************* BinCube ***************************************/

		/********************************* BinCluster ***************************************/

		std::vector<BinCube*> binList;
		BinCluster::BinCluster() {
			correctGUID();
		}

		BinCluster::~BinCluster() {
			binList.clear();
		}

		const char* BinCluster::classname() {
			return "BinCluster";
		}

		void BinCluster::addBin(BinCube* bin) {
			binList.push_back(bin);
			bin->grouped = true;
		}

		void BinCluster::populateCluster(Cluster* cluster) {
			BinCube* bin;
			Binnable* bObj;
			for (auto binIt = binList.begin(); binIt != binList.end(); binIt++) {
				bin = *binIt;
				for (auto bObjIt = bin->objects.begin(); bObjIt != bin->objects.end(); bObjIt++) {
					bObj = *bObjIt;
					cluster->addObj(bObj->obj);
				}
			}
		}

		unsigned int BinCluster::size() {
			unsigned int size = 0;
			for (auto it = binList.begin(); it != binList.end(); it++) {
				size += (*it)->objects.size();
			}
			return size;
		}

		/********************************* BinCluster ***************************************/


		/********************************** BinBox ****************************************/
		BinBox::BinBox(linalg::Vector* boxDimensions, float binSize, float cutoffDistance) :
		  ClusterSolver(boxDimensions, cutoffDistance),
		  binSize(binSize)
		{
			dimensions = new BinBoxDimensions();
			dimensions->x = (short) (boxDimensions->x / binSize);
			dimensions->y = (short) (boxDimensions->y / binSize);
			dimensions->z = (short) (boxDimensions->z / binSize);

			//printf("%d %d %d\n", (int) dimensions->x, (int) dimensions->y, (int) dimensions->z);

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
			sprintf(buffer, "Coords (%d,%d,%d) are out of bounds.",  (int) coords->i, (int) coords->j, (int) coords->k);
			return buffer;
		}

		void BinBox::addBinnable(Binnable* obj) {
			BinCoordinates* coords = obj->coords;
			if (coords->i < 0 || coords->i >= dimensions->x ||
				coords->j < 0 || coords->j >= dimensions->y ||
				coords->k < 0 || coords->k >= dimensions->z) {
				throw BinBoundsException(coords);
			}
			bins[coords->i][coords->j][coords->k]->addBinnable(obj);
		}

		void BinBox::fillBins(std::vector<SimulationObject*>& objects) {
			for (auto it = objects.begin(); it != objects.end(); it++) {
				try {
					addBinnable(new Binnable(*it, binSize, dimensions));
				} catch (BinBoundsException& e) {
					std::cout << e.what() << std::endl;
				}
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

			std::vector<BinCluster*> bcs;

			BinCube* current;
			BinCluster* bCluster;
			for (index i = 0; i < dimensions->x; i++) {
				for (index j = 0; j < dimensions->y; j++) {
					for (index k = 0; k < dimensions->z; k++) {
						current = bins[i][j][k];
						if (!current->isEmpty() && !current->grouped) {
							bCluster = new BinCluster();
							bCluster->addBin(current);
							compareBin(current, bCluster);
							//std::cout << bCluster->size() << std::endl;
							if (bCluster->size() > 1) {
								bcs.push_back(bCluster);
							} else {
								delete bCluster;
								bCluster = NULL;
							}
						}
					}
				}
			}

			Cluster* cluster;
			index idCounter = 0;
			for (int i = 0; i < bcs.size(); i++) {
				cluster = new Cluster(idCounter);
				bcs[i]->populateCluster(cluster);
				clusters.push_back(cluster);
			}
		}

		/**
		 * Recursive function for building BinCluster by checking nearest neighbors.
		 */
		void BinBox::compareBin(BinCube* bin, BinCluster* cluster) {
			//printf("%d %d %d\n", (int) bin->coords->i, (int) bin->coords->j, (int) bin->coords->k);
			BinCube* current = NULL;
			short i, j, k;
			for ( short di = -1; di < 2; di++ ) {
				for ( short dj = -1; dj < 2; dj++ ) {
				  for ( short dk = -1; dk < 2; dk++ ) {
					i = (short) (( bin->coords->i + di ) % dimensions->x);
					if ( i < 0 ) { i += dimensions->x; }
					j = (short) (( bin->coords->j + dj ) % dimensions->y);
					if ( j < 0 ) { j += dimensions->y; }
					k = (short) (( bin->coords->k + dk ) % dimensions->z);
					if ( k < 0 ) { k += dimensions->z; }


					//printf("%d %d %d\n", (int) i, (int) j, (int) k);

					current = bins[ i ][ j ][ k ];
					if ( !current->isEmpty() && !current->grouped &&
						 bin->groupBins(current, this)) {
					  cluster->addBin( current );
					  compareBin( current, cluster );
					}
				  }
				}
			}
		}

		/********************************** BinBox ****************************************/


	}
}

