/**
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  */

/*
 *  NumericMetricValue.h
 *
 *  @Copyright (C) 2015, SurgiTAIX AG
 *  Author: besting, buerger, roehser
 */
 
/**
 * THIS FILE IS GENERATED AUTOMATICALLY! DO NOT MODIFY!
 *
 * YOUR CHANGES WILL BE OVERWRITTEN!
 * 
 * USE THE DEFINITION FILES IN THE FOLDER "codegenerator" INSTEAD!
 */

#ifndef NUMERICMETRICVALUE_H_
#define NUMERICMETRICVALUE_H_

#include "OSCLib/Data/SDC/MDIB/SimpleTypesMapping.h"
#include "OSCLib/Data/SDC/SDC-fwd.h"
#include "osdm-fwd.hxx"

namespace SDCLib {
namespace Data {
namespace SDC {

class NumericMetricValue {
private:
	NumericMetricValue(const CDM::NumericMetricValue & object);
	operator CDM::NumericMetricValue() const;
	friend class ConvertFromCDM;
	friend class ConvertToCDM;
	friend class SDCProvider;
	friend class SDCConsumer;
	friend class MdState;
	friend class MdDescription;
public:
	NumericMetricValue(
		MetricQuality metricquality
	); 
private:
	NumericMetricValue(){};
public:
	NumericMetricValue(const NumericMetricValue & object);
	virtual ~NumericMetricValue();
    
    void copyFrom(const NumericMetricValue & object);
    NumericMetricValue & operator=(const NumericMetricValue & object);
    
    typedef CDM::NumericMetricValue WrappedType;

	NumericMetricValue & setMetricQuality(const MetricQuality & value);
	MetricQuality getMetricQuality() const;

	NumericMetricValue & setStartTime(const Timestamp & value);
	Timestamp getStartTime() const;
	bool getStartTime(Timestamp & out) const;
	bool hasStartTime() const;

	NumericMetricValue & setStopTime(const Timestamp & value);
	Timestamp getStopTime() const;
	bool getStopTime(Timestamp & out) const;
	bool hasStopTime() const;

	NumericMetricValue & setDeterminationTime(const Timestamp & value);
	Timestamp getDeterminationTime() const;
	bool getDeterminationTime(Timestamp & out) const;
	bool hasDeterminationTime() const;

	NumericMetricValue & addAnnotation(const Annotation & value);
	std::vector<Annotation> getAnnotationList() const;
	void clearAnnotationList();
	
	NumericMetricValue & setValue(const double & value);
	double getValue() const;
	bool getValue(double & out) const;
	bool hasValue() const;

private:
	std::shared_ptr<CDM::NumericMetricValue> data;
};

} /* namespace SDC */
} /* namespace Data */
} /* namespace SDCLib */
#endif /* NUMERICMETRICVALUE_H_ */
