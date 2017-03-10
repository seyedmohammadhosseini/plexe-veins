/*
 * AutoCorrelationProcess.h
 *
 *  Created on: Mar 8, 2017
 *      Author: client0
 */

#ifndef SRC_VEINS_MODULES_UTILITY_AUTOCORRELATIONPROCESS_H_
#define SRC_VEINS_MODULES_UTILITY_AUTOCORRELATIONPROCESS_H_


class AutoCorrelationProcess {
public:
    AutoCorrelationProcess(double correlationDistance, double sigma) : correlationDistance(correlationDistance), sigma(sigma) {};

    double getProcessValue(double dist) const;
private:
    mutable bool firstTime;
    mutable double prevProcessValue;

    double correlationDistance;
    double sigma;
};

#endif /* SRC_VEINS_MODULES_UTILITY_AUTOCORRELATIONPROCESS_H_ */
