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
 *  NumericMetricValue.cpp
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

#include "OSCLib/Data/SDC/MDIB/NumericMetricValue.h"
#include "OSCLib/Data/SDC/MDIB/ConvertToCDM.h"
#include "OSCLib/Data/SDC/MDIB/ConvertFromCDM.h"
#include "OSCLib/Data/SDC/MDIB/Defaults.h"

#include "osdm.hxx"

#include "OSCLib/Data/SDC/MDIB/MetricQuality.h"
#include "OSCLib/Data/SDC/MDIB/Annotation.h"

namespace SDCLib {
namespace Data {
namespace SDC {


NumericMetricValue::NumericMetricValue(
		MetricQuality metricquality
) : data(Defaults::NumericMetricValueInit(
		metricquality
)) {}

NumericMetricValue::operator CDM::NumericMetricValue() const {
	return *data;
}

NumericMetricValue::NumericMetricValue(const CDM::NumericMetricValue & object) : data(new CDM::NumericMetricValue(object)) {

}

NumericMetricValue::NumericMetricValue(const NumericMetricValue & object) : data(new CDM::NumericMetricValue(*object.data)) {

}

NumericMetricValue::~NumericMetricValue() {

}

void NumericMetricValue::copyFrom(const NumericMetricValue & object) {
	data = std::shared_ptr<CDM::NumericMetricValue>( new CDM::NumericMetricValue(*object.data));
}

NumericMetricValue & NumericMetricValue:: operator=(const NumericMetricValue & object) {
	copyFrom(object);
	return *this;
}


NumericMetricValue & NumericMetricValue::setMetricQuality(const MetricQuality & value) {
	data->MetricQuality(ConvertToCDM::convert(value));
	return *this;
}


MetricQuality NumericMetricValue::getMetricQuality() const {
	return ConvertFromCDM::convert(data->MetricQuality());
}
	
NumericMetricValue & NumericMetricValue::setStartTime(const Timestamp & value) {
	data->StartTime(ConvertToCDM::convert(value));
	return *this;
}

bool NumericMetricValue::getStartTime(Timestamp & out) const {
	if (data->StartTime().present()) {
		out = ConvertFromCDM::convert(data->StartTime().get());
		return true;
	}
	return false;
}

Timestamp NumericMetricValue::getStartTime() const {
	return ConvertFromCDM::convert(data->StartTime().get());
}
	
bool NumericMetricValue::hasStartTime() const {
	return data->StartTime().present();
}
	
NumericMetricValue & NumericMetricValue::setStopTime(const Timestamp & value) {
	data->StopTime(ConvertToCDM::convert(value));
	return *this;
}

bool NumericMetricValue::getStopTime(Timestamp & out) const {
	if (data->StopTime().present()) {
		out = ConvertFromCDM::convert(data->StopTime().get());
		return true;
	}
	return false;
}

Timestamp NumericMetricValue::getStopTime() const {
	return ConvertFromCDM::convert(data->StopTime().get());
}
	
bool NumericMetricValue::hasStopTime() const {
	return data->StopTime().present();
}
	
NumericMetricValue & NumericMetricValue::setDeterminationTime(const Timestamp & value) {
	data->DeterminationTime(ConvertToCDM::convert(value));
	return *this;
}

bool NumericMetricValue::getDeterminationTime(Timestamp & out) const {
	if (data->DeterminationTime().present()) {
		out = ConvertFromCDM::convert(data->DeterminationTime().get());
		return true;
	}
	return false;
}

Timestamp NumericMetricValue::getDeterminationTime() const {
	return ConvertFromCDM::convert(data->DeterminationTime().get());
}
	
bool NumericMetricValue::hasDeterminationTime() const {
	return data->DeterminationTime().present();
}
	
NumericMetricValue & NumericMetricValue::addAnnotation(const Annotation & value) {
	data->Annotation().push_back(ConvertToCDM::convert(value));
	return *this;
}

std::vector<Annotation> NumericMetricValue::getAnnotationList() const {
	std::vector<Annotation> result;
	result.reserve(data->Annotation().size());
	for (const auto & value: data->Annotation()) {
		result.push_back(ConvertFromCDM::convert(value));
	}
	return result;
}

void NumericMetricValue::clearAnnotationList() {
	data->Annotation().clear();
}

NumericMetricValue & NumericMetricValue::setValue(const double & value) {
	data->Value(ConvertToCDM::convert(value));
	return *this;
}

bool NumericMetricValue::getValue(double & out) const {
	if (data->Value().present()) {
		out = ConvertFromCDM::convert(data->Value().get());
		return true;
	}
	return false;
}

double NumericMetricValue::getValue() const {
	return ConvertFromCDM::convert(data->Value().get());
}
	
bool NumericMetricValue::hasValue() const {
	return data->Value().present();
}
	

} /* namespace SDC */
} /* namespace Data */
} /* namespace SDCLib */

