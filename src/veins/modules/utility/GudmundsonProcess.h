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

#ifndef SRC_VEINS_MODULES_UTILITY_GUDMUNDSONPROCESS_H_
#define SRC_VEINS_MODULES_UTILITY_GUDMUNDSONPROCESS_H_


class GudmundsonProcess {
public:
    GudmundsonProcess(double correlationDistance, double sigma, int id=-1) : correlationDistance(correlationDistance), sigma(sigma), id(id) {};

    double getProcessValue(double dist) const;

    int getId() const;
private:
    mutable bool firstTime;
    mutable double processValue;

    double correlationDistance;
    double sigma;

    const int id;

};

#endif /* SRC_VEINS_MODULES_UTILITY_GUDMUNDSONPROCESS_H_ */
