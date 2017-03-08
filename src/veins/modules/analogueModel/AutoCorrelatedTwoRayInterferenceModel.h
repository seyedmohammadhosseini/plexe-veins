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

	public:
		AutoCorrelatedTwoRayInterferenceModel(double dielectricConstantReal, double dielectricConstantImag, double correlationDistance,
                                              double g_LOS, double g_gr_LOS, double delta_phi,
                                              double stdDev, bool debug) :
			correlationDistance(correlationDistance),
			g_LOS(g_LOS),
			g_gr_LOS(g_gr_LOS),
			delta_phi(delta_phi),
			stdDev(stdDev),
			debug(debug) {
		    epsilon_r.real(dielectricConstantReal);
		    epsilon_r.imag(dielectricConstantImag);
		}

		virtual ~AutoCorrelatedTwoRayInterferenceModel() {}

		virtual void filterSignal(AirFrame *frame, const Coord& sendersPos, const Coord& receiverPos);


	protected:

		class Mapping: public SimpleConstMapping {
		    private:
		        mutable bool firstTime;
		        mutable double prevProcessValue;

			protected:
				dcomplex reflectionCoeff;
				double d;
				double d_dir;
				double d_ref;
				double lambda;
				mutable double g_LOS;
                mutable dcomplex g_gr_LOS;
                double delta_phi;
                double correlationDistance;

				/** @brief Standard deviation */
				double sigma;

				/** @brief Delta_d */
				double delta_d;

				bool debug;
			public:
				Mapping(dcomplex reflectionCoeff, double distance, double directDistance, double reflDistance,
				        double g_LOS, double g_gr_LOS, double delta_phi, double correlationDistance,
				        double delta_d, double sigma, bool debug)
					: SimpleConstMapping(DimensionSet::timeFreqDomain()),
                    reflectionCoeff(reflectionCoeff),
					d(distance),
					d_dir(directDistance),
					d_ref(reflDistance),
					g_LOS(g_LOS),
					g_gr_LOS(g_gr_LOS),
					delta_phi(delta_phi),
					correlationDistance(correlationDistance),
					sigma(sigma),
					delta_d(delta_d),
					debug(debug) {
				    firstTime = true;
				    prevProcessValue = 0.0;
				}

				virtual double getValue(const Argument& pos) const;

				ConstMapping* constClone() const {
					return new Mapping(*this);
				}
		};

		/** @brief stores the dielectric constant used for calculation */
		dcomplex epsilon_r;

        double correlationDistance;
		double g_LOS;
		double g_gr_LOS;
		double delta_phi;

		/** @brief The standard deviation for the stochastic process */
		double stdDev;

		/** @brief Whether debug messages should be displayed. */
		bool debug;
};

#endif /* ANALOGUEMODEL_AUTOCORRELATEDTWORAYINTERFERENCEMODEL_H */
