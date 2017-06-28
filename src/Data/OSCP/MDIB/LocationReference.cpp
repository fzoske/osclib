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
 *  LocationReference.cpp
 *
 *  @Copyright (C) 2015, SurgiTAIX AG
 *  Author: besting, roehser
 */
 
/**
 * THIS FILE IS GENERATED AUTOMATICALLY! DO NOT MODIFY!
 *
 * YOUR CHANGES WILL BE OVERWRITTEN!
 * 
 * USE THE DEFINITION FILES IN THE FOLDER "codegenerator" INSTEAD!
 */

#include "OSCLib/Data/OSCP/MDIB/LocationReference.h"
#include "OSCLib/Data/OSCP/MDIB/ConvertToCDM.h"
#include "OSCLib/Data/OSCP/MDIB/ConvertFromCDM.h"
#include "OSCLib/Data/OSCP/MDIB/custom/Defaults.h"

#include "osdm.hxx"

#include "OSCLib/Data/OSCP/MDIB/InstanceIdentifier.h"
#include "OSCLib/Data/OSCP/MDIB/LocationDetail.h"

namespace OSCLib {
namespace Data {
namespace OSCP {

LocationReference::LocationReference() : data(Defaults::LocationReference()) {
}

LocationReference::operator CDM::LocationReference() const {
	return *data;
}

LocationReference::LocationReference(const CDM::LocationReference & object) : data(new CDM::LocationReference(object)) {

}

LocationReference::LocationReference(const LocationReference & object) : data(new CDM::LocationReference(*object.data)) {

}

LocationReference::~LocationReference() {

}

void LocationReference::copyFrom(const LocationReference & object) {
	*data = *object.data;
}

LocationReference & LocationReference:: operator=(const LocationReference & object) {
	copyFrom(object);
	return *this;
}


LocationReference & LocationReference::setLocationDetail(const LocationDetail & value) {
	data->LocationDetail(ConvertToCDM::convert(value));
	return *this;
}

bool LocationReference::getLocationDetail(LocationDetail & out) const {
	if (data->LocationDetail().present()) {
		out = ConvertFromCDM::convert(data->LocationDetail().get());
		return true;
	}
	return false;
}

LocationDetail LocationReference::getLocationDetail() const {
	return ConvertFromCDM::convert(data->LocationDetail().get());
}
	
bool LocationReference::hasLocationDetail() const {
	return data->LocationDetail().present();
}
	
LocationReference & LocationReference::addIdentification(const InstanceIdentifier & value) {
	data->Identification().push_back(ConvertToCDM::convert(value));
	return *this;
}

std::vector<InstanceIdentifier> LocationReference::getIdentifications() const {
	std::vector<InstanceIdentifier> result;
	result.reserve(data->Identification().size());
	for (const auto & value: data->Identification()) {
		result.push_back(ConvertFromCDM::convert(value));
	}
	return result;
}

void LocationReference::clearIdentifications() {
	data->Identification().clear();
}


} /* namespace OSCP */
} /* namespace Data */
} /* namespace OSCLib */

