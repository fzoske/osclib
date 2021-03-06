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
 *  AllowedValue.cpp
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

#include "OSCLib/Data/SDC/MDIB/AllowedValue.h"
#include "OSCLib/Data/SDC/MDIB/ConvertToCDM.h"
#include "OSCLib/Data/SDC/MDIB/ConvertFromCDM.h"
#include "OSCLib/Data/SDC/MDIB/Defaults.h"

#include "osdm.hxx"

#include "OSCLib/Data/SDC/MDIB/CodedValue.h"
#include "OSCLib/Data/SDC/MDIB/InstanceIdentifier.h"
#include "OSCLib/Data/SDC/MDIB/Measurement.h"

namespace SDCLib {
namespace Data {
namespace SDC {


AllowedValue::AllowedValue(
		std::string value
) : data(Defaults::AllowedValueInit(
		value
)) {}

AllowedValue::operator CDM::AllowedValue() const {
	return *data;
}

AllowedValue::AllowedValue(const CDM::AllowedValue & object) : data(new CDM::AllowedValue(object)) {

}

AllowedValue::AllowedValue(const AllowedValue & object) : data(new CDM::AllowedValue(*object.data)) {

}

AllowedValue::~AllowedValue() {

}

void AllowedValue::copyFrom(const AllowedValue & object) {
	data = std::shared_ptr<CDM::AllowedValue>( new CDM::AllowedValue(*object.data));
}

AllowedValue & AllowedValue:: operator=(const AllowedValue & object) {
	copyFrom(object);
	return *this;
}


AllowedValue & AllowedValue::setValue(const std::string & value) {
	data->Value(ConvertToCDM::convert(value));
	return *this;
}


std::string AllowedValue::getValue() const {
	return ConvertFromCDM::convert(data->Value());
}
	
AllowedValue & AllowedValue::setType(const CodedValue & value) {
	data->Type(ConvertToCDM::convert(value));
	return *this;
}

bool AllowedValue::getType(CodedValue & out) const {
	if (data->Type().present()) {
		out = ConvertFromCDM::convert(data->Type().get());
		return true;
	}
	return false;
}

CodedValue AllowedValue::getType() const {
	return ConvertFromCDM::convert(data->Type().get());
}
	
bool AllowedValue::hasType() const {
	return data->Type().present();
}
	
AllowedValue & AllowedValue::setIdentification(const InstanceIdentifier & value) {
	data->Identification(ConvertToCDM::convert(value));
	return *this;
}

bool AllowedValue::getIdentification(InstanceIdentifier & out) const {
	if (data->Identification().present()) {
		out = ConvertFromCDM::convert(data->Identification().get());
		return true;
	}
	return false;
}

InstanceIdentifier AllowedValue::getIdentification() const {
	return ConvertFromCDM::convert(data->Identification().get());
}
	
bool AllowedValue::hasIdentification() const {
	return data->Identification().present();
}
	
AllowedValue & AllowedValue::setCharacteristic(const Measurement & value) {
	data->Characteristic(ConvertToCDM::convert(value));
	return *this;
}

bool AllowedValue::getCharacteristic(Measurement & out) const {
	if (data->Characteristic().present()) {
		out = ConvertFromCDM::convert(data->Characteristic().get());
		return true;
	}
	return false;
}

Measurement AllowedValue::getCharacteristic() const {
	return ConvertFromCDM::convert(data->Characteristic().get());
}
	
bool AllowedValue::hasCharacteristic() const {
	return data->Characteristic().present();
}
	

} /* namespace SDC */
} /* namespace Data */
} /* namespace SDCLib */

