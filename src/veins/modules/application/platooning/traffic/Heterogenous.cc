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

#include "Heterogenous.h"

Define_Module(Heterogenous);

void Heterogenous::initialize(int stage){
    TraCIBaseTrafficManager::initialize(stage);

    if (stage == 0) {

        nCars = par("nCars").longValue();
        nTruck = par("nTruck").longValue();//let it be equal number to nCars parameter
        nBus = par("nBus").longValue();

        platoonSize = par("platoonSize").longValue();
        nLanes = par("nLanes").longValue();

        carLength = par("carLength").longValue();
        truckLength = par("truckLength").longValue();
        busLength = par("busLength").longValue();

        platoonInsertTime = SimTime(par("platoonInsertTime").doubleValue());
        platoonInsertSpeed = par("platoonInsertSpeed").doubleValue();
        platoonInsertDistance = par("platoonInsertDistance").doubleValue();
        platoonInsertHeadway = par("platoonInsertHeadway").doubleValue();
        platoonLeaderHeadway = par("platoonLeaderHeadway").doubleValue();

        platooningVType = par("platooningVType").stdstringValue();
        platooningVTypeTruck = par("platooningVTypeTruck").stdstringValue();
        platooningVTypeBus = par("platooningVTypeBus").stdstringValue();
        insertPlatoonMessage = new cMessage("");
        vehicleTypeInPlatoon = par("vehicleTypeInPlatoon").stdstringValue();
        scheduleAt(platoonInsertTime, insertPlatoonMessage);

    }
}

void Heterogenous::scenarioLoaded(){
    automated.id = findVehicleTypeIndex(platooningVType);
    automated.lane = -1;
    automated.position = 0;
    automated.speed = platoonInsertSpeed/3.6;

    truck.id = findVehicleTypeIndex(platooningVTypeTruck);
    truck.lane = -1;
    truck.position = 0;
    truck.speed = platoonInsertSpeed/3.6;

    bus.id = findVehicleTypeIndex(platooningVTypeBus);
    bus.lane = -1;
    bus.position = 0;
    bus.speed = platoonInsertSpeed/3.6;
}

void Heterogenous::handleSelfMsg(cMessage *msg){
    TraCIBaseTrafficManager::handleSelfMsg(msg);

    if (msg == insertPlatoonMessage) {
        //insertPlatoons();
        insertHeterogeneousVehicles();
    }
}


/*This function gives us the ability to add a new vehicle type at a desired position in the platoon*/
void Heterogenous::insertHeterogeneousVehicles(){

    /*FOR NOW THESE PARAMETERS WILL BE SAME AS OF INSERTPLATOONS()*/

    //compute inter vehicle distance
    double distance = platoonInsertDistance; //platoonInsertSpeed / 3.6 * platoonInsertHeadway + platoonInsertDistance;
    //total number of platoons per lane
    int nPlatoons = (nCars + nTruck + nBus) / platoonSize / nLanes;
    //length of 1 platoon
    double platoonLength = (nCars * carLength) + (nTruck * truckLength ) + (nBus * busLength) + (platoonSize - 1) * distance;//platoonSize * 4 + (platoonSize - 1) * distance;
    //inter-platoon distance
    double platoonDistance = platoonInsertSpeed / 3.6 * platoonLeaderHeadway;
    //total length for one lane
    double totalLength = nPlatoons * platoonLength + (nPlatoons - 1) * platoonDistance;

    convertStringToVector(vehicleTypeInPlatoon);

    //for each lane, we create an offset to have misaligned platoons
    double *laneOffset = new double[nLanes];
    for (int l = 0; l < nLanes; l++)
        laneOffset[l] = uniform(0, 20);
    double currentPos = totalLength;
    int currentCar = 0;
    int size = vehicleInOrder.size();
    for (int i = 0; i < size; i++) {
        for (int l = 0; l < nLanes; l++) {
             switch(vehicleInOrder.at(i)){
                  case 1:
                        automated.position = currentPos + laneOffset[l];
                        automated.lane = l;
                        addVehicleToQueue(0, automated);
                        currentPos -= (carLength + distance);
                        break;
                  case 2:
                        truck.position = currentPos + laneOffset[l];
                        truck.lane = l;
                        addVehicleToQueue(0, truck);
                        currentPos -= (truckLength + distance);
                        break;
                  case 3:
                        bus.position = currentPos + laneOffset[l];
                        bus.lane = l;
                        addVehicleToQueue(0, bus);
                        currentPos -= (busLength + distance);
                        break;
                    }
            }
            currentCar++;
        }
        delete[] laneOffset;

}

void Heterogenous::convertStringToVector(std::string word){
    std::stringstream ss;
    ss << word;

    int value;
    while (ss >> value)
    {
        vehicleInOrder.push_back(value);

        if (ss.peek() == ',')
            ss.ignore();
    }
}

void Heterogenous::finish(){
    TraCIBaseTrafficManager::finish();
    if (insertPlatoonMessage) {
        cancelAndDelete(insertPlatoonMessage);
        insertPlatoonMessage = 0;
    }
}



