/*
 * testcluster.cpp
 *
 *  Created on: Mar 2, 2016
 *      Author: Hayden
 */

#include <binbox.h>
#include <iostream>

using namespace dpd2;
using namespace dpd2::cluster;

/******************************** testcluster ********************************************/

/**
 * 1 cluster. 1 separate point is outside of pbc range.
 */
void clustercase1() {
    /******************* Test 1 ***********************************/
    BinBox* box = new BinBox(new linalg::Vector(36.0f, 36.0f, 40.0f), 4.0f, 4.25f);

    std::vector<SimulationObject*> objs;

    objs.push_back(new SimulationObject(new geom::Position(0.0f, 1.0f, 0.0f)));
    objs.push_back(new SimulationObject(new geom::Position(0.0f, 35.0f, 0.0f)));
    objs.push_back(new SimulationObject(new geom::Position(35.5f, 0.5f, 39.8f)));
    objs.push_back(new SimulationObject(new geom::Position(0.0f, 35.25f, 39.38f)));

    objs.push_back(new SimulationObject(new geom::Position(100.0f, 100.0f, 100.0f)));

    box->deriveClusters(objs);

    std::cout << box->clusters.size() << std::endl;

    delete box;

	for (auto it = objs.begin(); it != objs.end(); it++) {
		delete *it;
		*it = NULL;
	}
	objs.clear();
    /******************* Test 1 ***********************************/
}

/**
 * 0 clusters. 4 separate points.
 */
void clustercase2() {
    /******************* Test 2 ***********************************/
    BinBox* box = new BinBox(new linalg::Vector(36.0f, 36.0f, 40.0f), 4.0f, 4.25f);

    std::vector<SimulationObject*> objs;

    objs.push_back(new SimulationObject(new geom::Position(5.0f, 12.0f, 21.0f)));

    objs.push_back(new SimulationObject(new geom::Position(0.0f, 35.0f, 32.0f)));

    objs.push_back(new SimulationObject(new geom::Position(1.0f, 0.5f, 0.8f)));

    objs.push_back(new SimulationObject(new geom::Position(29.7f, 8.25f, 15.79f)));

    box->deriveClusters(objs);

    std::cout << box->clusters.size() << std::endl;

    delete box;

	for (auto it = objs.begin(); it != objs.end(); it++) {
		delete *it;
		*it = NULL;
	}
	objs.clear();
    /******************* Test 2 ***********************************/
}

/**
 * 1 cluster. 2 separate points.
 */
void clustercase3() {
    /******************* Test 3 ***********************************/
    BinBox* box = new BinBox(new linalg::Vector(15.0f, 21.0f, 13.0f), 3.0f, 3.125f);

    std::vector<SimulationObject*> objs;

    objs.push_back(new SimulationObject(new geom::Position(7.5f, 0.5f, 6.5f)));
    objs.push_back(new SimulationObject(new geom::Position(7.5f, 20.5f, 6.5f)));
    objs.push_back(new SimulationObject(new geom::Position(7.0f, 21.333f, 7.0f)));
    objs.push_back(new SimulationObject(new geom::Position(8.0f, 0.0f, 6.2f)));
    objs.push_back(new SimulationObject(new geom::Position(7.2f, 20.0f, 6.9f)));

    objs.push_back(new SimulationObject(new geom::Position(11.0f, 13.0f, 0.0f)));

    objs.push_back(new SimulationObject(new geom::Position(3.0f, 20.0f, 2.0f)));

    box->deriveClusters(objs);

    std::cout << box->clusters.size() << std::endl;

    delete box;

	for (auto it = objs.begin(); it != objs.end(); it++) {
		delete *it;
		*it = NULL;
	}
	objs.clear();
    /******************* Test 3 ***********************************/
}

/**
 * 3 clusters.
 */
void clustercase4() {
    /******************* Test 4 ***********************************/
    BinBox* box = new BinBox(new linalg::Vector(36.0f, 36.0f, 40.0f), 4.0f, 4.25f);

    std::vector<SimulationObject*> objs;

    objs.push_back(new SimulationObject(new geom::Position(0.0f, 0.5f, 0.0f)));
    objs.push_back(new SimulationObject(new geom::Position(0.0f, 0.7f, 4.0f)));

    objs.push_back(new SimulationObject(new geom::Position(35.5f, 18.0f, 39.0f)));
    objs.push_back(new SimulationObject(new geom::Position(34.3f, 15.1f, 37.0f)));

    objs.push_back(new SimulationObject(new geom::Position(15.0f, 15.0f, 20.0f)));
    objs.push_back(new SimulationObject(new geom::Position(17.0f, 13.0f, 24.0f)));
    objs.push_back(new SimulationObject(new geom::Position(16.0f, 14.0f, 22.0f)));

    box->deriveClusters(objs);

    std::cout << box->clusters.size() << std::endl;

    delete box;

	for (auto it = objs.begin(); it != objs.end(); it++) {
		delete *it;
		*it = NULL;
	}
	objs.clear();
    /******************* Test 4 ***********************************/
}

void testcluster() {
	clustercase1();
	clustercase2();
	clustercase3();
	clustercase4();
}
/******************************** testcluster ********************************************/



