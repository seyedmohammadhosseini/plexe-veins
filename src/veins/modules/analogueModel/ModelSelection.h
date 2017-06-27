/*
 * ModelSelection.h
 *
 *  Created on: Jun 20, 2017
 *      Author: skynet-user1
 */

#ifndef SRC_VEINS_MODULES_ANALOGUEMODEL_MODELSELECTION_H_
#define SRC_VEINS_MODULES_ANALOGUEMODEL_MODELSELECTION_H_

#include <initializer_list>
#include "veins/base/phyLayer/AnalogueModel.h"
#include "veins/base/phyLayer/MappingBase.h"
#include "veins/base/modules/BaseWorldUtility.h"

using Veins::AirFrame;

class ModelSelectionMapping;

class ModelSelection : public AnalogueModel {
protected:
    friend class ModelSelectionMapping;
    typedef std::map<std::string, cMsgPar> ParameterMap;

public:
    ModelSelection(std::initializer_list<ParameterMap> param_list);

    virtual void filterSignal(AirFrame *frame, const Coord& sendersPos, const Coord& receiverPos);
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
