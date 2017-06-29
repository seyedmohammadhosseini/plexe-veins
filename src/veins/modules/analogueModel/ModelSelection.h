/*
 * ModelSelection.h
 *
 *  Created on: Jun 20, 2017
 *      Author: skynet-user1
 */

#ifndef SRC_VEINS_MODULES_ANALOGUEMODEL_MODELSELECTION_H_
#define SRC_VEINS_MODULES_ANALOGUEMODEL_MODELSELECTION_H_

#include "veins/base/phyLayer/AnalogueModel.h"
#include "veins/base/phyLayer/MappingBase.h"
#include "veins/base/modules/BaseWorldUtility.h"
#include "veins/base/phyLayer/BasePhyLayer.h"
#include "veins/modules/phy/PhyLayer80211p.h"

using Veins::AirFrame;

class ModelSelectionMapping;

class ModelSelection : public AnalogueModel {
protected:
    friend class ModelSelectionMapping;

    /**
     * @brief Used at initialisation to pass the parameters
     * to the AnalogueModel and Decider
     */
    typedef std::map<std::string, cMsgPar> ParameterMap;

public:

    ModelSelection(ParameterMap& scenarios);

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

class ModelSelectionMapping : public SimpleConstMapping {
private:
    ModelSelection* model;
public:
    ModelSelectionMapping(ModelSelection* model)
        : SimpleConstMapping(DimensionSet::timeFreqDomain()), model(model) {}

    virtual double getValue(const Argument& pos) const;

    ConstMapping* constClone() const {
        return new ModelSelectionMapping(*this);
    }
};

#endif /* SRC_VEINS_MODULES_ANALOGUEMODEL_MODELSELECTION_H_ */
