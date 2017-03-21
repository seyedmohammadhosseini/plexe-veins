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

#ifndef ANALOGUEMODEL_AUTOCORRELATEDTWORAYINTERFERENCEMODEL_H
#define ANALOGUEMODEL_AUTOCORRELATEDTWORAYINTERFERENCEMODEL_H

#include <complex>
#include "veins/base/phyLayer/AnalogueModel.h"
#include "veins/base/modules/BaseWorldUtility.h"
#include "veins/base/phyLayer/MappingBase.h"
#include "veins/modules/utility/GudmundsonProcess.h"


using Veins::AirFrame;
typedef std::complex<double> dcomplex;

class AutoCorrelatedTwoRayInterferenceMapping;

/**
 * @brief
 * Extended version of Two-Ray Ground path loss model.
 * Extended version of Two-Ray Interference path loss model.
 *
 * Model and parameters are implemented according to:
 *
 * Nilsson, M. G., Gustafson, C., Abbas, T., & Tufvesson, F.
 * A Measurement Based Multilink Shadowing Model for V2V Network Simulations of Highway Scenarios, 1–10.
 *
 * An example config.xml for this AnalogueModel can be the following:
 * @verbatim
    <AnalogueModels>
        <AnalogueModel type="AutoCorrelatedTwoRayInterferenceModel">
            <parameter name="DielectricConstantReal" type="double" value="5.02" />
            <parameter name="DielectricConstantImag" type="double" value="-0.1" />

            <!-- Given in meters [m] -->
            <parameter name="CorrelationDistance" type="double" value="36.0" />

            <!-- Given in log scale -->
            <parameter name="stdDev" type="double" value="3.12" />

            <!-- Given in log scale -->
            <parameter name="g_LOS" type="double" value="-0.8" />

            <!-- Given in log scale -->
            <parameter name="g_gr_LOS" type="double" value="-6.42" />

            <!-- Given in radians, NOT in degrees (-34.53 deg)-->
            <parameter name="delta_phi" type="double" value="-0.60266219071364202" />
        </AnalogueModel>
    </AnalogueModels>
   @endverbatim
 *
 * @author Christian Nelson <christian.nelson@eit.lth.se>
 *
 * @ingroup analogueModels
 */
class AutoCorrelatedTwoRayInterferenceModel: public AnalogueModel {

    protected:
        friend class AutoCorrelatedTwoRayInterferenceMapping;

        Coord oldSenderPos2D;
        Coord oldReceiverPos2D;

        GudmundsonProcess* proc;
        dcomplex epsilon_r;
        double correlationDistance;
        double processValue;
        double g_LOS;
        dcomplex g_gr_LOS;
        double delta_phi;
        double stdDev;

        bool firstTime;
        bool debug;

        cOutVector deterministicGain;
        cOutVector stochasticGain;
        cOutVector receiverPosx;
        cOutVector receiverPosy;
        cOutVector transmitterPosx;
        cOutVector transmitterPosy;
        cOutVector channel_d;

    public:
        AutoCorrelatedTwoRayInterferenceModel(double dielectricConstantReal, double dielectricConstantImag, double correlationDistance,
                                              double g_LOS, double g_gr_LOS, double delta_phi, double stdDev, bool debug) :
                                                  correlationDistance(correlationDistance), delta_phi(delta_phi), stdDev(stdDev), debug(debug) {

                epsilon_r.real(dielectricConstantReal);
                epsilon_r.imag(dielectricConstantImag);
                proc = new GudmundsonProcess(correlationDistance, stdDev);
                this->g_LOS = std::pow(10, g_LOS/20.0);
                this->g_gr_LOS = std::pow(10, g_gr_LOS/20.0);
                firstTime = true;

                deterministicGain.setName("AutoCorrelatedTwoRayInterferenceModel_DeterministicGain");
                stochasticGain.setName("AutoCorrelatedTwoRayInterferenceModel_StochasticGain");
                receiverPosx.setName("AutoCorrelatedTwoRayInterferenceModel_xRx");
                receiverPosy.setName("AutoCorrelatedTwoRayInterferenceModel_yRx");
                transmitterPosx.setName("AutoCorrelatedTwoRayInterferenceModel_xTx");
                transmitterPosy.setName("AutoCorrelatedTwoRayInterferenceModel_yTx");
                channel_d.setName("AutoCorrelatedTwoRayInterferenceModel_deltaD");

        }

        virtual ~AutoCorrelatedTwoRayInterferenceModel() {}

        virtual void filterSignal(AirFrame *frame, const Coord& sendersPos, const Coord& receiverPos);

};

class AutoCorrelatedTwoRayInterferenceMapping: public SimpleConstMapping {
    private:
        AutoCorrelatedTwoRayInterferenceModel* model;

    protected:
        dcomplex reflectionCoeff;
        double d;
        double d_dir;
        double d_ref;
        double lambda;
        bool debug;

    public:
        AutoCorrelatedTwoRayInterferenceMapping(AutoCorrelatedTwoRayInterferenceModel* model, dcomplex reflectionCoeff,
                double d, double d_dir, double d_ref, bool debug)
            : SimpleConstMapping(DimensionSet::timeFreqDomain()),
            model(model),
            reflectionCoeff(reflectionCoeff),
            d(d),
            d_dir(d_dir),
            d_ref(d_ref),
            debug(debug) {}

        virtual double getValue(const Argument& pos) const;

        ConstMapping* constClone() const {
            return new AutoCorrelatedTwoRayInterferenceMapping(*this);
        }
};

#endif /* ANALOGUEMODEL_AUTOCORRELATEDTWORAYINTERFERENCEMODEL_H */
