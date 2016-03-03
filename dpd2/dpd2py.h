/*
 * dpd2py.h
 *
 *  Created on: Mar 2, 2016
 *      Author: Hayden
 */

#ifndef DPD2PY_H_
#define DPD2PY_H_

#include "dpd2.h"
#include "binbox.h"
#include "linalg.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace dpd2;

extern "C" {

	void PrintFromC() {
		std::cout << "Printing from c!" << std::endl;
	}

	vector<SimulationObject*>* SimObjList() {
		return new vector<dpd2::SimulationObject*>();
	}

	SimulationObject* SimObj(float x, float y, float z) {
		return new SimulationObject(x,y,z);
	}

	void AddObjToList(vector<SimulationObject*>* list, SimulationObject* obj) {
		list->push_back(obj);
	}

	cluster::BinBox* BinBox(float boxX, float boxY, float boxZ, float binSize, float cutoffDistance) {
		return new cluster::BinBox(new linalg::Vector(boxX, boxY, boxZ), binSize, cutoffDistance);
	}

	void DeriveClusters(cluster::BinBox* box, vector<SimulationObject*> list) {
		box->deriveClusters(list);
	}

	vector<cluster::Cluster*>* GetClusters(cluster::BinBox* box) {
		return &(box->clusters);
	}

	unsigned int GetNumClusters(vector<cluster::Cluster*>* list) {
		return list->size();
	}

	cluster::Cluster* Cluster(vector<cluster::Cluster*>* list, unsigned int idx) {
		return list->at(idx);
	}

	unsigned int GetNumObjects(cluster::Cluster* cluster) {
		return cluster->objects.size();
	}

	SimulationObject* ObjectFromCluster(cluster::Cluster* cluster, unsigned int idx) {
		return cluster->objects[idx];
	}

	const char* GetGUID(SimulationObject* obj) {
		return obj->getGUID().c_str();
	}

}



#endif /* DPD2PY_H_ */
