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
