/*
 * ModelSelection.cc
 *
 *  Created on: Jun 20, 2017
 *      Author: skynet-user1
 */

#include "veins/modules/analogueModel/ModelSelection.h"
#include "veins/base/messages/AirFrame_m.h"
#include "veins/modules/obstacle/ObstacleControl.h"
#include "veins/modules/obstacle/VehicleObstacle.h"

ModelSelection::ModelSelection(std::initializer_list<ParameterMap> param_list) {}

void ModelSelection::filterSignal(AirFrame *frame, const Coord& senderPos, const Coord& receiverPos) {
    return;
}

double ModelSelectionMapping::getValue(const Argument& pos) const {
    // TODO: Implement selection of LOS or OLOS model depending on situation.
    return 1;
}

