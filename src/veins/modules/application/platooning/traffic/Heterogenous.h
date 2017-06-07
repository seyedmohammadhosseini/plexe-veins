//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_TRAFFIC_HETEROGENOUS_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_TRAFFIC_HETEROGENOUS_H_

#include <veins/modules/mobility/traci/TraCIBaseTrafficManager.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

class Heterogenous : public TraCIBaseTrafficManager {
public:
    Heterogenous(){
        insertPlatoonMessage = 0;
        platoonInsertDistance = 0;
        platoonInsertHeadway = 0;
        platoonInsertSpeed = 0;
        platoonInsertTime = SimTime(0);
        platoonLeaderHeadway = 0;
        platoonSize = 0;
        nCars = 0;
        nLanes = 0;
        nTruck = 0;
        nBus = 0;
        carLength = 0;
        truckLength = 0;
        busLength = 0;
    }

    virtual void initialize(int stage);
    virtual void finish();

    std::vector<std::string> vehicleTypeName;

protected:
    //this is used to start traffic generation
    cMessage *insertPlatoonMessage;

    void insertHeterogeneousVehicles();
    void convertStringToVector(std::string word);

    virtual void handleSelfMsg(cMessage *msg);

    SimTime platoonInsertTime;
    double platoonInsertSpeed;

    //vehicles to be inserted
    struct Vehicle automated;
    struct Vehicle truck;
    struct Vehicle bus;

    //total number of vehicles to be injected
    int nCars; // number of vehicles of car type
    int nTruck; // number of vehicles of truck type
    int nBus;  // number of vehicles of bus type

    //Length variable of the vehicles
    int carLength;
    int truckLength;
    int busLength;

    //vehicles per platoon
    int platoonSize;
    //number of lanes
    int nLanes;
    //insert distance
    double platoonInsertDistance;
    //insert headway
    double platoonInsertHeadway;
    //headway for leader vehicles
    double platoonLeaderHeadway;

    //sumo vehicle type of platooning cars
    std::string platooningVType;
    std::string platooningVTypeTruck;
    std::string platooningVTypeBus;


    std::string vehicleTypeInPlatoon; // it is initialized in the .ini file about the order of the vehicles
    std::vector<int> vehicleInOrder;

    virtual void scenarioLoaded();
    std::vector<std::string> getVector();
};

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_TRAFFIC_HETEROGENOUS_H_ */
