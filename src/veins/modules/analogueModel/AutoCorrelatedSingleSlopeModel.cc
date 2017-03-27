//
// Copyright (C) 2017 Christian Nelson <christian.nelson@eit.lth.se>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "veins/modules/analogueModel/AutoCorrelatedSingleSlopeModel.h"
#include "veins/base/messages/AirFrame_m.h"
#include "veins/modules/phy/PhyLayer80211p.h"

using Veins::AirFrame;


#define debugACSSEV EV << "PhyLayer(AutoCorrelatedSingleSlopeModel): "

void AutoCorrelatedSingleSlopeModel::filterSignal(AirFrame *frame, const Coord& senderPos, const Coord& receiverPos) {
    Signal& s = frame->getSignal();

    int idTx = s.getSendingModule()->getId();
    int idRx = s.getReceptionModule()->getId();

    std::stringstream sstm;
    if (idTx < idRx) {
        sstm << idTx << idRx;
    } else {
        sstm << idRx << idTx;
    }
    int id = std::stoi(sstm.str());

    PhyLayer80211p* phyTx = dynamic_cast<PhyLayer80211p *>(s.getSendingModule());
    PhyLayer80211p* phyRx = dynamic_cast<PhyLayer80211p *>(s.getReceptionModule());
    assert(phyTx); assert(phyRx);

    const Coord senderPos2D(senderPos.x, senderPos.y);
    const Coord receiverPos2D(receiverPos.x, receiverPos.y);

    double delta_d;

    if (firstTime) {
        delta_d = 0;
        firstTime = false;
    } else {
        double dTx = senderPos2D.distance(oldSenderPos2D);
        double dRx = receiverPos2D.distance(oldReceiverPos2D);
        delta_d = (dTx + dRx)/2;
    }

    debugACSSEV << "TxID -> RxID = " << idTx << " - > " << idRx << endl;
    if (s.getSendingModule()->getId() < s.getReceptionModule()->getId()) {
        processValue = phyTx->getAutoCorrelationProcess(id)->getProcessValue(delta_d);    }
    else {
        processValue = phyRx->getAutoCorrelationProcess(id)->getProcessValue(delta_d);
    }

    oldSenderPos2D = Coord(senderPos2D);
    oldReceiverPos2D  = Coord(receiverPos2D);

    receiverPosx.record(oldReceiverPos2D.x);
    receiverPosy.record(oldReceiverPos2D.y);
    transmitterPosx.record(oldSenderPos2D.x);
    transmitterPosy.record(oldSenderPos2D.y);
    channel_d.record(delta_d);

    double d = senderPos2D.distance(receiverPos2D);

    s.addAttenuation(new AutoCorrelatedSingleSlopeMapping(this, d, debug));

    if (firstTime) {firstTime = false;}
}

double AutoCorrelatedSingleSlopeMapping::getValue(const Argument& pos) const {

    double att = model->PL_d0 + model->alpha*10*log10(d/model->d0);
    double att_process = att + model->processValue;

    double gain_dB = -att;
    double gain_dB_process = -att_process;
    double gain_linear_process = pow(10, gain_dB_process/10);

    if (debug) {
        model->deterministicGain.record(gain_dB);
        model->stochasticGain.record(gain_dB_process);
        debugACSSEV << "Add gain (gain, gain_dB) = (" << gain_linear_process << ", " << FWMath::mW2dBm(gain_linear_process) << ")" << endl;
    }

    return gain_linear_process;
}

