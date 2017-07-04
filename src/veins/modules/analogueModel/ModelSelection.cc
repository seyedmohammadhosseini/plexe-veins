/*
 * ModelSelection.cc
 *
 *  Created on: Jun 20, 2017
 *      Author: skynet-user1
 */

#include "veins/modules/analogueModel/ModelSelection.h"
#include "veins/base/phyLayer/Signal_.h"
#include "veins/modules/phy/PhyLayer80211p.h"

using Veins::AirFrame;


ModelSelection::ModelSelection(ObstacleControl& obstacleControl, ParameterMap& scenarios) : obstacleControl(obstacleControl) {

    ParameterMap::iterator it;

    it = scenarios.find("LOS");
    if (it != scenarios.end()) {
        LOS = parseAnalagoueModels(it->second.xmlValue());
    }

    it = scenarios.find("OLOS");
    if (it != scenarios.end()) {
        OLOS = parseAnalagoueModels(it->second.xmlValue());
    }

}

AnalogueModel* ModelSelection::parseAnalagoueModels(cXMLElement* scenario) {

    PhyLayer80211p phyLayer;
    cXMLElementList analogueModelList = scenario->getElementsByTagName("AnalogueModel");

    if(analogueModelList.empty()) {
        throw cRuntimeError("No analogue models configuration found in configuration file.");
    }

    if (analogueModelList.size() > 1) {
        throw cRuntimeError("Can only handle one Analogue model in each sceanrio!");
    }

    AnalogueModel* newAnalogueModel;

    // iterate over all AnalogueModel-entries
    for(cXMLElementList::const_iterator it = analogueModelList.begin();
        it != analogueModelList.end(); it++) {

        cXMLElement* analogueModelData = *it;

        const char* name = analogueModelData->getAttribute("type");

        if(name == 0) {
            throw cRuntimeError("Could not read name of analogue model.");
        }

        ParameterMap params;
        getParametersFromXML(analogueModelData, params);

        newAnalogueModel = phyLayer.getAnalogueModelFromName(name, params);

        if(newAnalogueModel == 0) {
            throw cRuntimeError("Could not find an analogue model with the name \"%s\".", name);
        }
    }

    return newAnalogueModel;
}

void ModelSelection::getParametersFromXML(cXMLElement* xmlData, ParameterMap& outputMap) {
    cXMLElementList parameters = xmlData->getElementsByTagName("Parameter");

    for(cXMLElementList::const_iterator it = parameters.begin();
        it != parameters.end(); it++) {

        const char* name = (*it)->getAttribute("name");
        const char* type = (*it)->getAttribute("type");
        const char* value = (*it)->getAttribute("value");
        if(name == 0 || type == 0 || value == 0)
            throw cRuntimeError("Invalid parameter, could not find name, type or value");

        std::string sType = type;   //needed for easier comparision
        std::string sValue = value; //needed for easier comparision

        cMsgPar param(name);

        //parse type of parameter and set value
        if (sType == "bool") {
            param.setBoolValue(sValue == "true" || sValue == "1");

        } else if (sType == "double") {
            param.setDoubleValue(strtod(value, 0));

        } else if (sType == "string") {
            param.setStringValue(value);

        } else if (sType == "long") {
            param.setLongValue(strtol(value, 0, 0));

        } else {
            throw cRuntimeError("Unknown parameter type: '%s'", sType.c_str());
        }

        //add parameter to output map
        outputMap[name] = param;
    }
}

void ModelSelection::filterSignal(AirFrame *frame, const Coord& senderPos, const Coord& receiverPos) {
    Signal& s = frame->getSignal();
    double att = obstacleControl.calculateVehicleAttenuation(senderPos, receiverPos, s);

    if (att != 1) {
        OLOS->filterSignal(frame, senderPos, receiverPos);
    } else {
        LOS->filterSignal(frame, senderPos, receiverPos);
    }
    return;
}
