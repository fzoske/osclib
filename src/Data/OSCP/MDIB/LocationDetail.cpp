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
 *  LocationDetail.cpp
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

#include "OSCLib/Data/OSCP/MDIB/LocationDetail.h"
#include "OSCLib/Data/OSCP/MDIB/ConvertToCDM.h"
#include "OSCLib/Data/OSCP/MDIB/ConvertFromCDM.h"
#include "OSCLib/Data/OSCP/MDIB/custom/Defaults.h"

#include "osdm.hxx"


namespace OSCLib {
namespace Data {
namespace OSCP {

LocationDetail::LocationDetail() : data(Defaults::LocationDetail()) {
}

LocationDetail::operator CDM::LocationDetail() const {
	return *data;
}

LocationDetail::LocationDetail(const CDM::LocationDetail & object) : data(new CDM::LocationDetail(object)) {

}

LocationDetail::LocationDetail(const LocationDetail & object) : data(new CDM::LocationDetail(*object.data)) {

}

LocationDetail::~LocationDetail() {

}

void LocationDetail::copyFrom(const LocationDetail & object) {
	*data = *object.data;
}

LocationDetail & LocationDetail:: operator=(const LocationDetail & object) {
	copyFrom(object);
	return *this;
}


LocationDetail & LocationDetail::setPoC(const string & value) {
	data->PoC(ConvertToCDM::convert(value));
	return *this;
}

bool LocationDetail::getPoC(string & out) const {
	if (data->PoC().present()) {
		out = ConvertFromCDM::convert(data->PoC().get());
		return true;
	}
	return false;
}

string LocationDetail::getPoC() const {
	return ConvertFromCDM::convert(data->PoC().get());
}
	
bool LocationDetail::hasPoC() const {
	return data->PoC().present();
}
	
LocationDetail & LocationDetail::setRoom(const string & value) {
	data->Room(ConvertToCDM::convert(value));
	return *this;
}

bool LocationDetail::getRoom(string & out) const {
	if (data->Room().present()) {
		out = ConvertFromCDM::convert(data->Room().get());
		return true;
	}
	return false;
}

string LocationDetail::getRoom() const {
	return ConvertFromCDM::convert(data->Room().get());
}
	
bool LocationDetail::hasRoom() const {
	return data->Room().present();
}
	
LocationDetail & LocationDetail::setBed(const string & value) {
	data->Bed(ConvertToCDM::convert(value));
	return *this;
}

bool LocationDetail::getBed(string & out) const {
	if (data->Bed().present()) {
		out = ConvertFromCDM::convert(data->Bed().get());
		return true;
	}
	return false;
}

string LocationDetail::getBed() const {
	return ConvertFromCDM::convert(data->Bed().get());
}
	
bool LocationDetail::hasBed() const {
	return data->Bed().present();
}
	
LocationDetail & LocationDetail::setFacility(const string & value) {
	data->Facility(ConvertToCDM::convert(value));
	return *this;
}

bool LocationDetail::getFacility(string & out) const {
	if (data->Facility().present()) {
		out = ConvertFromCDM::convert(data->Facility().get());
		return true;
	}
	return false;
}

string LocationDetail::getFacility() const {
	return ConvertFromCDM::convert(data->Facility().get());
}
	
bool LocationDetail::hasFacility() const {
	return data->Facility().present();
}
	
LocationDetail & LocationDetail::setBuilding(const string & value) {
	data->Building(ConvertToCDM::convert(value));
	return *this;
}

bool LocationDetail::getBuilding(string & out) const {
	if (data->Building().present()) {
		out = ConvertFromCDM::convert(data->Building().get());
		return true;
	}
	return false;
}

string LocationDetail::getBuilding() const {
	return ConvertFromCDM::convert(data->Building().get());
}
	
bool LocationDetail::hasBuilding() const {
	return data->Building().present();
}
	
LocationDetail & LocationDetail::setFloor(const string & value) {
	data->Floor(ConvertToCDM::convert(value));
	return *this;
}

bool LocationDetail::getFloor(string & out) const {
	if (data->Floor().present()) {
		out = ConvertFromCDM::convert(data->Floor().get());
		return true;
	}
	return false;
}

string LocationDetail::getFloor() const {
	return ConvertFromCDM::convert(data->Floor().get());
}
	
bool LocationDetail::hasFloor() const {
	return data->Floor().present();
}
	

} /* namespace OSCP */
} /* namespace Data */
} /* namespace OSCLib */

