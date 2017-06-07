//
// Copyright (c) 2012-2016 Michele Segata <segata@ccs-labs.org>
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

#include "veins/modules/application/platooning/utilities/PositionHelper.h"

Define_Module(PositionHelper);

void PositionHelper::initialize(int stage) {

	BasePositionHelper::initialize(stage);

	if (stage == 0) {
		nCars = par("nCars").longValue();
		nTruck = par("nTruck").longValue();
		nBus = par("nBus").longValue();

		vehicleType = par("vehicleType").stdstringValue();
		vehicleTypeNames = par("vehicleTypeNames").stdstringValue();
		stringToVector(vehicleType);
		getVehicleID(vehicleTypeNames);
		Id_sumo = vehIDInString.at(0);
		myId = getIdOfDifferentVehicles(getExternalId());
		leaderId = getPlatoonLeader(myId, nLanes, platoonSize);
		leader = myId == leaderId;
		frontId = getFrontVehicle(myId, nLanes, platoonSize);
		position = getPositionInPlatoon(myId, nLanes, platoonSize);
		platoonId = getPlatoonNumber(myId, nLanes, platoonSize);
		platoonLane = getPlatoonLane(myId, nLanes);


	}

}

void PositionHelper::finish() {
	BasePositionHelper::finish();
}

int PositionHelper::getPosition() {
	return position;
}

int PositionHelper::getMemberId(int position) {
	return leaderId + position * nLanes;
}

int PositionHelper::getMemberPosition(int vehicleId) {
	return (vehicleId - leaderId) / nLanes;
}

int PositionHelper::getLeaderId() {
	return leaderId;
}

bool PositionHelper::isLeader() {
	return leader;
}

int PositionHelper::getFrontId() {
	return frontId;
}

int PositionHelper::getPlatoonId() {
	return platoonId;
}

int PositionHelper::getPlatoonLane() {
	return platoonLane;
}

bool PositionHelper::isInSamePlatoon(int vehicleId) {
	return platoonId == getPlatoonNumber(vehicleId, nLanes, platoonSize);
}

int PositionHelper::getIdFromExternalId(std::string externalId) {
	int dotIndex = externalId.find_last_of('.');
	std::string strId = externalId.substr(dotIndex + 1);
	return strtol(strId.c_str(), 0, 10);
}

bool PositionHelper::isLeader(int vehicleId, int nLanes, int platoonSize) {
	return (vehicleId / nLanes) % platoonSize == 0;
}
int PositionHelper::getPlatoonNumber(int vehicleId, int nLanes, int platoonSize) {
	return getPlatoonColumn(vehicleId, nLanes, platoonSize) * nLanes + getPlatoonLane(vehicleId, nLanes);
}
int PositionHelper::getPlatoonLane(int vehicleId, int nLanes) {
	return vehicleId % nLanes;
}
int PositionHelper::getPlatoonColumn(int vehicleId, int nLanes, int platoonSize) {
	return vehicleId / (nLanes * platoonSize);
}
int PositionHelper::getPlatoonLeader(int vehicleId, int nLanes, int platoonSize) {
	return getPlatoonColumn(vehicleId, nLanes, platoonSize) * nLanes * platoonSize + getPlatoonLane(getPlatoonNumber(vehicleId, nLanes, platoonSize), nLanes);
}
int PositionHelper::getFrontVehicle(int vehicleId, int nLanes, int platoonSize) {
	if (getPlatoonLeader(vehicleId, nLanes, platoonSize) == vehicleId)
		return -1;
	else
		return vehicleId - nLanes;
}
bool PositionHelper::isInSamePlatoon(int vehicleId, int myId, int nLanes, int platoonSize) {
	return getPlatoonNumber(vehicleId, nLanes, platoonSize) == getPlatoonNumber(myId, nLanes, platoonSize);
}
bool PositionHelper::isFrontVehicle(int vehicleId, int myId, int nLanes, int platoonSize) {
	return getFrontVehicle(myId, nLanes, platoonSize) == vehicleId;
}
int PositionHelper::getPositionInPlatoon(int vehicleId, int nLanes, int platoonSize) {
	return (vehicleId - getPlatoonLeader(vehicleId, nLanes, platoonSize)) / nLanes;
}

//Implemented by karthikeyan
int PositionHelper::getIdOfDifferentVehicles(std::string externalId) {


    if(externalId.substr(5,5).compare("auto1") == 0){
        int dotIndex = externalId.find_last_of('.');
        std::string strId = externalId.substr(dotIndex + 1);
        int num = strtol(strId.c_str(), 0, 10);
        int count=0;
        int index=0;
        std::vector<int>::iterator it = vehInOrder.begin();
        while(it != vehInOrder.end()){
            if(*it == 1){
                if(count == num){
                    return index;
                }
                count++;
            }
            index++;
            it++;
        }
    }
    else if(externalId.substr(5,5).compare("auto2") == 0){
        int dotIndex = externalId.find_last_of('.');
        std::string strId = externalId.substr(dotIndex + 1);
        int num = strtol(strId.c_str(), 0, 10);
        int count=0;
        int index=0;
        std::vector<int>::iterator it = vehInOrder.begin();
        while(it != vehInOrder.end()){
            if(*it == 2){
                if(count == num){
                    return index;
                }
                count++;
            }
            index++;
            it++;
        }
    }
    else if(externalId.substr(5,5).compare("auto3") == 0){
        int dotIndex = externalId.find_last_of('.');
        std::string strId = externalId.substr(dotIndex + 1);
        int num = strtol(strId.c_str(), 0, 10);
        int count=0;
        int index=0;
        std::vector<int>::iterator it = vehInOrder.begin();
        while(it != vehInOrder.end()){
            if(*it == 3){
                if(count == num){
                    return index;
                }
                count++;
            }
            index++;
            it++;
        }
    }
    return -1;
}

//Implemented by karthikeyan
void PositionHelper::stringToVector(std::string word){

    std::stringstream ss;
    ss << word;

    int value;
    while (ss >> value)
    {
        vehInOrder.push_back(value);

        if (ss.peek() == ',')
            ss.ignore();
    }
}

void PositionHelper::getVehicleID(std::string word){
    std::stringstream ss;
    ss << word;
    std::string val;
    while(getline(ss,val,',')){
        vehIDInString.push_back(val);
    }
}



//Implemented by karthikeyan
std::string PositionHelper::getStringIDOfVehicle(int vehicleID){

    return vehIDInString.at(vehicleID);
/*    return "blaa";*/
  /*  std::list<std::string> veh = traci->getVehicleTypeIds();
    int type_num = vehInOrder.at(vehicleID);
    std::vector<std::string> v;
    v.reserve(veh.size());
    std::copy(std::begin(veh), std::end(veh), std::back_inserter(v));
    //std::vector<std::string> v{std::make_move_iterator(std::begin(veh)), std::make_move_iterator(std::end(veh)) };
    std::string typeID = v.at(type_num);*/
//    return trafficManager->vehicleTypeName.at(vehicleID);
/*    std::string val = "blas";
    return val;*/
}
