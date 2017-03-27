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

#ifndef ANALOGUEMODEL_AUTOCORRELATEDSINGLESLOPEMODEL_H
#define ANALOGUEMODEL_AUTOCORRELATEDSINGLESLOPEMODEL_H

#include "veins/base/phyLayer/AnalogueModel.h"
#include "veins/base/phyLayer/MappingBase.h"


using Veins::AirFrame;

class AutoCorrelatedSingleSlopeMapping;

/**
 * @brief
 *
 * Model and parameters are implemented according to:
 *
 * Nilsson, M. G., Gustafson, C., Abbas, T., & Tufvesson, F.
 * A Measurement Based Multilink Shadowing Model for V2V Network Simulations of Highway Scenarios, 1–10.
 *
 * An example config.xml for this AnalogueModel can be the following:
 * @verbatim
    <AnalogueModels>
        <AnalogueModel type="AutoCorrelatedSingleSlopeModel">

            <!-- Given in meters -->
            <parameter name="d0" type="double" value="10.0" />

            <!-- Given in log scale -->
            <parameter name="PL_d0" type="double" value="59.53" />

            <!-- Given in log scale -->
            <parameter name="alpha" type="double" value="2.73" />

        </AnalogueModel>
    </AnalogueModels>
   @endverbatim
 *
 * @author Christian Nelson <christian.nelson@eit.lth.se>
 *
 * @ingroup analogueModels
 */
class AutoCorrelatedSingleSlopeModel: public AnalogueModel {

    protected:
        friend class AutoCorrelatedSingleSlopeMapping;

        double d0;
        double PL_d0;
        double alpha;

        bool firstTime;
        bool debug;

        double processValue;

        Coord oldSenderPos2D;
        Coord oldReceiverPos2D;

        cOutVector deterministicGain, stochasticGain;
        cOutVector receiverPosx, receiverPosy;
        cOutVector transmitterPosx, transmitterPosy;
        cOutVector channel_d;

    public:
        AutoCorrelatedSingleSlopeModel(double d0, double PL_d0, double alpha, bool debug) :
            d0(d0), PL_d0(PL_d0), debug(debug) {

                firstTime = true;

                this->alpha = std::pow(10, alpha/10);

                deterministicGain.setName("AutoCorrelatedSingleSlopeModel_DeterministicGain");
                stochasticGain.setName("AutoCorrelatedSingleSlopeModel_StochasticGain");
                receiverPosx.setName("AutoCorrelatedSingleSlopeModel_xRx");
                receiverPosy.setName("AutoCorrelatedSingleSlopeModel_yRx");
                transmitterPosx.setName("AutoCorrelatedSingleSlopeModel_xTx");
                transmitterPosy.setName("AutoCorrelatedSingleSlopeModel_yTx");
                channel_d.setName("AutoCorrelatedSingleSlopeModel_deltaD");

        }

        virtual ~AutoCorrelatedSingleSlopeModel() {}

        virtual void filterSignal(AirFrame *frame, const Coord& sendersPos, const Coord& receiverPos);

};

class AutoCorrelatedSingleSlopeMapping: public SimpleConstMapping {
    private:
        AutoCorrelatedSingleSlopeModel* model;

    protected:
        double d;
        bool debug;

    public:
        AutoCorrelatedSingleSlopeMapping(AutoCorrelatedSingleSlopeModel* model, double d, bool debug)
            : SimpleConstMapping(DimensionSet::timeFreqDomain()), model(model), d(d), debug(debug) {}

        virtual double getValue(const Argument& pos) const;

        ConstMapping* constClone() const {
            return new AutoCorrelatedSingleSlopeMapping(*this);
        }
};

#endif /* ANALOGUEMODEL_AUTOCORRELATEDSINGLESLOPEMODEL_H */
