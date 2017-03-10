/*
 * AutoCorrelationProcess.cc
 *
 *  Created on: Mar 8, 2017
 *      Author: client0
 */

#include "veins/modules/utility/AutoCorrelationProcess.h"
#include "veins/base/utils/MiXiMDefs.h"

double AutoCorrelationProcess::getProcessValue(double distance) const {

    if (firstTime) {
        processValue = (RNGCONTEXT normal(0, sigma));
        firstTime = false;
    } else {
        double rho          = exp(-std::abs(distance/correlationDistance));
        double new_mean     = rho*processValue;
        double new_var      = pow(sigma,2)*(1-pow(rho,2));
        double oldVal       = processValue;
        processValue        = (RNGCONTEXT normal(new_mean, sqrt(new_var)));
        EV << " (old, new, rho, mean, var) = (" << oldVal << ", " << processValue << ", " << rho << ", " << new_mean << ", " << new_var << ")" << endl;
    }

    return processValue;
}
