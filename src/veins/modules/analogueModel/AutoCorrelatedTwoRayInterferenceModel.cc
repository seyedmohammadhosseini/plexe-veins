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

#include "veins/modules/analogueModel/AutoCorrelatedTwoRayInterferenceModel.h"
#include "veins/base/messages/AirFrame_m.h"

using Veins::AirFrame;


#define debugEV EV << "PhyLagammayer(AutoCorrelatedTwoRayInterferenceModel): "

void AutoCorrelatedTwoRayInterferenceModel::filterSignal(AirFrame *frame, const Coord& senderPos, const Coord& receiverPos) {
	Signal& s = frame->getSignal();

	const Coord senderPos2D(senderPos.x, senderPos.y);
	const Coord receiverPos2D(receiverPos.x, receiverPos.y);

	double delta_d;

	if (firstTime) {
	    delta_d = 0;
	    firstTime = false;
	} else {
	    double dTx = std::sqrt(pow(oldTxPosX - senderPos.x, 2) + pow(oldTxPosY - senderPos.y, 2));
	    double dRx = std::sqrt(pow(oldRxPosX - receiverPos.x, 2) + pow(oldRxPosY - receiverPos.y, 2));
	    delta_d = (dTx + dRx)/2;
	}

    oldTxPosX = senderPos.x;
    oldTxPosY = senderPos.y;
    oldRxPosX = receiverPos.x;
    oldRxPosY = receiverPos.y;

	ASSERT(senderPos.z > 0); // make sure send a    ntenna is above ground
	ASSERT(receiverPos.z > 0); // make sure receive antenna is above ground

	double d = senderPos2D.distance(receiverPos2D);
	double ht = senderPos.z, hr = receiverPos.z;

	debugEV << "(ht, hr) = (" << ht << ", " << hr << ")" << endl;

	double d_dir = sqrt( pow (d,2) + pow((ht - hr),2) ); // direct distance
	double d_ref = sqrt( pow (d,2) + pow((ht + hr),2) ); // distance via ground reflection
	double sin_theta = (ht + hr)/d_ref;
	double cos_theta = d/d_ref;

	dcomplex reflectionCoeff = (epsilon_r*sin_theta - std::sqrt(epsilon_r - std::pow(cos_theta,2)))/
	                           (epsilon_r*sin_theta + std::sqrt(epsilon_r - std::pow(cos_theta,2)));

	//is the signal defined to attenuate over frequency?
	bool hasFrequency = s.getTransmissionPower()->getDimensionSet().hasDimension(Dimension::frequency());
	debugEV << "Signal contains frequency dimension: " << (hasFrequency ? "yes" : "no") << endl;

	assert(hasFrequency);

	debugEV << "Add TwoRayInterferenceModel attenuation (gamma, d, d_dir, d_ref) = (" << reflectionCoeff << ", " << d << ", " << d_dir << ", " << d_ref << ")" << endl;
    s.addAttenuation(new AutoCorrelatedTwoRayInterferenceModel::Mapping(reflectionCoeff, d, d_dir, d_ref, g_LOS, g_gr_LOS, delta_phi, correlationDistance, delta_d, stdDev, debug));
}

double AutoCorrelatedTwoRayInterferenceModel::Mapping::getValue(const Argument& pos) const {

	assert(pos.hasArgVal(Dimension::frequency()));
	double freq = pos.getArgValue(Dimension::frequency());
	double lambda = BaseWorldUtility::speedOfLight() / freq;
    double k = 2*M_PI/lambda;
	double phi =  k * (d_dir - d_ref);

	g_LOS = std::pow(10, g_LOS/20.0);
	g_gr_LOS = std::pow(10, g_gr_LOS/20.0);

	dcomplex i = dcomplex(0,1);
	dcomplex A = std::exp(-i*k*d_dir)/d_dir + std::sqrt(g_gr_LOS) * std::exp(i*delta_phi) * reflectionCoeff * std::exp(-i*k*d_ref)/d_ref;
	double A_abs = std::abs(A);

    double att = 20*log10(4*M_PI/lambda) - 10*log10(g_LOS) - 20*log10(A_abs);

    debugEV << "(k, g_LOS, g_gr_LOS, delta_phi, delta_d, stdDev) = (" << k << ", " << g_LOS << ", " << g_gr_LOS << ", " << delta_phi << ", " << delta_d << ", " << sigma << ")" << endl;

    if (firstTime) {
        EV << "First time in process; generating process value." << endl;
        prevProcessValue = (RNGCONTEXT normal(0, sigma));
        firstTime = false;
    } else {
        double rho          = exp(-std::abs(delta_d/correlationDistance));
        double new_mean     = rho*prevProcessValue;
        double new_var      = pow(sigma,2)*(1-pow(rho,2));
        prevProcessValue    = (RNGCONTEXT normal(new_mean, sqrt(new_var)));
        EV << "firstTime is now false. (correlation, mean, var) = (" << rho << ", " << new_mean << ", " << new_var <<")" << endl;
    }

    double att_process = att + prevProcessValue;

    double gain_dB = -att;
    double gain_dB_process = -att_process;

    double gain_linear = pow(10, gain_dB/10);
    double gain_linear_process = pow(10, gain_dB_process/10);

    debugEV << "gain = " << gain_dB << " dB" << endl;
    debugEV << "gain [process] = " << gain_dB_process << " dB" << endl;
    debugEV << "gain = " << gain_linear << " []" << endl;
    debugEV << "gain [process] = " << gain_linear_process << " []" << endl;

    debugEV << "Add gain for (freq, lambda, phi, gamma, att, att_dBm) = (" << freq << ", " << lambda << ", " << phi << ", " << reflectionCoeff << ", " << gain_linear << ", " << FWMath::mW2dBm(gain_linear) << ")" << endl;

    EV << "New process value = " << gain_linear_process << endl;

	return gain_linear_process;
}

