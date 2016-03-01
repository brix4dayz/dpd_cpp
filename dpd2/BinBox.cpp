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
			coords->i = (index) (obj->r->x / binSize);
			if (coords->i < 0)
				coords->i += dimensions->x;
			else if (coords->i >= dimensions->x)
				coords->i -= dimensions->x;

			coords->j = (index) (obj->r->y / binSize);
			if (coords->j < 0)
				coords->j += dimensions->y;
			else if (coords->j >= dimensions->y)
				coords->j -= dimensions->y;

			coords->k = (index) (obj->r->z / binSize);
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

		BinCube::BinCube(index i, index j, index k) {
			coords = new BinCoordinates();
			coords->i = i;
			coords->j = j;
			coords->k = k;

			checked = false;
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
		}

		void BinCluster::populateCluster(Cluster* cluster) {
			// TODO
		}

		/********************************* BinCluster ***************************************/


		/********************************** BinBox ****************************************/
		BinBox::BinBox(linalg::Vector* boxDimensions, float binSize) :
		  ClusteringSolver(boxDimensions),
		  binSize(binSize)
		{
			dimensions = new BinBoxDimensions();
			dimensions->x = (index) (boxDimensions->x / binSize);
			dimensions->y = (index) (boxDimensions->y / binSize);
			dimensions->z = (index) (boxDimensions->z / binSize);

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
			// TODO
		}

		void BinBox::compareBin(BinCube* bin, Cluster* cluster) {
			// TODO might want to change parameters
		}

		/********************************** BinBox ****************************************/


	}
}

