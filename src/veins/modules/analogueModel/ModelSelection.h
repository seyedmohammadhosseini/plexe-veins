/*
 * ModelSelection.h
 *
 *  Created on: Jun 20, 2017
 *      Author: skynet-user1
 */

#ifndef SRC_VEINS_MODULES_ANALOGUEMODEL_MODELSELECTION_H_
#define SRC_VEINS_MODULES_ANALOGUEMODEL_MODELSELECTION_H_

#include "veins/base/modules/BaseWorldUtility.h"

#include "veins/base/phyLayer/AnalogueModel.h"
#include "veins/base/phyLayer/MappingBase.h"
#include "veins/base/phyLayer/BasePhyLayer.h"
#include "veins/base/phyLayer/Mapping.h"

#include "veins/base/messages/AirFrame_m.h"
#include "veins/base/utils/Move.h"

#include "veins/modules/obstacle/ObstacleControl.h"


using Veins::AirFrame;
using Veins::ObstacleControl;


class ModelSelection : public AnalogueModel {

protected:

    /**
     * @brief Used at initialisation to pass the parameters
     * to the AnalogueModel and Decider
     */
    typedef std::map<std::string, cMsgPar> ParameterMap;

    /** @brief reference to global ObstacleControl instance */
    ObstacleControl& obstacleControl;

public:

    ModelSelection(ObstacleControl& obstacleControl, ParameterMap& scenarios);

    virtual void filterSignal(AirFrame *frame, const Coord& sendersPos, const Coord& receiverPos);


private:
    AnalogueModel* LOS;
    AnalogueModel* OLOS;

    AnalogueModel* parseAnalagoueModels(cXMLElement* scenario);

    /**
     * @brief Utility function. Reads the parameters of a XML element
     * and stores them in the passed ParameterMap reference. EXACT copy of the
     * implementation in BasePhyLayer.h
     */
    void getParametersFromXML(cXMLElement* xmlData, ParameterMap& outputMap);
};


#endif /* SRC_VEINS_MODULES_ANALOGUEMODEL_MODELSELECTION_H_ */
