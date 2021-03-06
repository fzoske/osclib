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
 *  ActivateOperationState.cpp
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

#include "OSCLib/Data/SDC/MDIB/ActivateOperationState.h"
#include "OSCLib/Data/SDC/MDIB/ConvertToCDM.h"
#include "OSCLib/Data/SDC/MDIB/ConvertFromCDM.h"
#include "OSCLib/Data/SDC/MDIB/Defaults.h"

#include "osdm.hxx"


namespace SDCLib {
namespace Data {
namespace SDC {


ActivateOperationState::ActivateOperationState(
		HandleRef descriptorhandle
		, 
		OperatingMode operatingmode
) : data(Defaults::ActivateOperationStateInit(
		descriptorhandle
		,
		operatingmode
)) {}

ActivateOperationState::operator CDM::ActivateOperationState() const {
	return *data;
}

ActivateOperationState::ActivateOperationState(const CDM::ActivateOperationState & object) : data(new CDM::ActivateOperationState(object)) {

}

ActivateOperationState::ActivateOperationState(const ActivateOperationState & object) : data(new CDM::ActivateOperationState(*object.data)) {

}

ActivateOperationState::~ActivateOperationState() {

}

void ActivateOperationState::copyFrom(const ActivateOperationState & object) {
	data = std::shared_ptr<CDM::ActivateOperationState>( new CDM::ActivateOperationState(*object.data));
}

ActivateOperationState & ActivateOperationState:: operator=(const ActivateOperationState & object) {
	copyFrom(object);
	return *this;
}


ActivateOperationState & ActivateOperationState::setStateVersion(const VersionCounter & value) {
	data->StateVersion(ConvertToCDM::convert(value));
	return *this;
}

bool ActivateOperationState::getStateVersion(VersionCounter & out) const {
	if (data->StateVersion().present()) {
		out = ConvertFromCDM::convert(data->StateVersion().get());
		return true;
	}
	return false;
}

VersionCounter ActivateOperationState::getStateVersion() const {
	return ConvertFromCDM::convert(data->StateVersion().get());
}
	
bool ActivateOperationState::hasStateVersion() const {
	return data->StateVersion().present();
}
	
ActivateOperationState & ActivateOperationState::setDescriptorHandle(const HandleRef & value) {
	data->DescriptorHandle(ConvertToCDM::convert(value));
	return *this;
}


HandleRef ActivateOperationState::getDescriptorHandle() const {
	return ConvertFromCDM::convert(data->DescriptorHandle());
}
	
ActivateOperationState & ActivateOperationState::setDescriptorVersion(const ReferencedVersion & value) {
	data->DescriptorVersion(ConvertToCDM::convert(value));
	return *this;
}

bool ActivateOperationState::getDescriptorVersion(ReferencedVersion & out) const {
	if (data->DescriptorVersion().present()) {
		out = ConvertFromCDM::convert(data->DescriptorVersion().get());
		return true;
	}
	return false;
}

ReferencedVersion ActivateOperationState::getDescriptorVersion() const {
	return ConvertFromCDM::convert(data->DescriptorVersion().get());
}
	
bool ActivateOperationState::hasDescriptorVersion() const {
	return data->DescriptorVersion().present();
}
	
ActivateOperationState & ActivateOperationState::setOperatingMode(const OperatingMode & value) {
	data->OperatingMode(ConvertToCDM::convert(value));
	return *this;
}


OperatingMode ActivateOperationState::getOperatingMode() const {
	return ConvertFromCDM::convert(data->OperatingMode());
}
	

} /* namespace SDC */
} /* namespace Data */
} /* namespace SDCLib */

