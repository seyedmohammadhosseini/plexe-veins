//
// Copyright (C) 2011 Stefan Joerer <stefan.joerer@uibk.ac.at>
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


using Veins::AirFrame;
typedef std::complex<double> dcomplex;

class AutoCorrelatedTwoRayInterferenceMapping;

/**
 * @brief
 * Extended version of Two-Ray Ground path loss model.
 * Extended version of Two-Ray Interference path loss model.
 *
 * See the Veins website <a href="http://veins.car2x.org/"> for a tutorial, documentation, and publications </a>.
 *
 * @author Christian Nelson
 *
 * @ingroup analogueModels
 */
class AutoCorrelatedTwoRayInterferenceModel: public AnalogueModel {

    protected:
        friend class AutoCorrelatedTwoRayInterferenceMapping;

        bool firstTime;

        dcomplex epsilon_r;

        double correlationDistance;
        double g_LOS;
        dcomplex g_gr_LOS;
        double delta_phi;
        double stdDev;

        double oldTxPosX;
        double oldTxPosY;
        double oldRxPosX;
        double oldRxPosY;

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

                this->g_LOS = std::pow(10, g_LOS/20.0);
                this->g_gr_LOS = std::pow(10, g_gr_LOS/20.0);

                oldTxPosX = 0.0;
                oldTxPosY = 0.0;
                oldRxPosX = 0.0;
                oldRxPosY = 0.0;
                firstTime = true;

                deterministicGain.setName("DeterministicGain");
                stochasticGain.setName("StochasticGain");
                receiverPosx.setName("x Rx");
                receiverPosy.setName("y Rx");
                transmitterPosx.setName("x Tx");
                transmitterPosy.setName("y_Tx");
                channel_d.setName("delta_d");

		}

		virtual ~AutoCorrelatedTwoRayInterferenceModel() {}

		virtual void filterSignal(AirFrame *frame, const Coord& sendersPos, const Coord& receiverPos);

};

class AutoCorrelatedTwoRayInterferenceMapping: public SimpleConstMapping {
    private:
        mutable double prevProcessValue;
        AutoCorrelatedTwoRayInterferenceModel* model;

    protected:
        dcomplex reflectionCoeff;
        double d;
        double d_dir;
        double d_ref;
        double lambda;
        double delta_d;
        bool debug;

    public:
        AutoCorrelatedTwoRayInterferenceMapping(AutoCorrelatedTwoRayInterferenceModel* model, dcomplex reflectionCoeff,
                double distance, double directDistance, double reflDistance, double delta_d, bool debug)
            : SimpleConstMapping(DimensionSet::timeFreqDomain()),
            model(model),
            reflectionCoeff(reflectionCoeff),
            d(distance),
            d_dir(directDistance),
            d_ref(reflDistance),
            delta_d(delta_d),
            debug(debug) {
                prevProcessValue = 0.0;
        }

        virtual double getValue(const Argument& pos) const;

        ConstMapping* constClone() const {
            return new AutoCorrelatedTwoRayInterferenceMapping(*this);
        }
};

#endif /* ANALOGUEMODEL_AUTOCORRELATEDTWORAYINTERFERENCEMODEL_H */
