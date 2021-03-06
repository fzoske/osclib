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
 *  SetComponentStateOperationState.cpp
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

#include "OSCLib/Data/SDC/MDIB/SetComponentStateOperationState.h"
#include "OSCLib/Data/SDC/MDIB/ConvertToCDM.h"
#include "OSCLib/Data/SDC/MDIB/ConvertFromCDM.h"
#include "OSCLib/Data/SDC/MDIB/Defaults.h"

#include "osdm.hxx"


namespace SDCLib {
namespace Data {
namespace SDC {


SetComponentStateOperationState::SetComponentStateOperationState(
		HandleRef descriptorhandle
		, 
		OperatingMode operatingmode
) : data(Defaults::SetComponentStateOperationStateInit(
		descriptorhandle
		,
		operatingmode
)) {}

SetComponentStateOperationState::operator CDM::SetComponentStateOperationState() const {
	return *data;
}

SetComponentStateOperationState::SetComponentStateOperationState(const CDM::SetComponentStateOperationState & object) : data(new CDM::SetComponentStateOperationState(object)) {

}

SetComponentStateOperationState::SetComponentStateOperationState(const SetComponentStateOperationState & object) : data(new CDM::SetComponentStateOperationState(*object.data)) {

}

SetComponentStateOperationState::~SetComponentStateOperationState() {

}

void SetComponentStateOperationState::copyFrom(const SetComponentStateOperationState & object) {
	data = std::shared_ptr<CDM::SetComponentStateOperationState>( new CDM::SetComponentStateOperationState(*object.data));
}

SetComponentStateOperationState & SetComponentStateOperationState:: operator=(const SetComponentStateOperationState & object) {
	copyFrom(object);
	return *this;
}


SetComponentStateOperationState & SetComponentStateOperationState::setStateVersion(const VersionCounter & value) {
	data->StateVersion(ConvertToCDM::convert(value));
	return *this;
}

bool SetComponentStateOperationState::getStateVersion(VersionCounter & out) const {
	if (data->StateVersion().present()) {
		out = ConvertFromCDM::convert(data->StateVersion().get());
		return true;
	}
	return false;
}

VersionCounter SetComponentStateOperationState::getStateVersion() const {
	return ConvertFromCDM::convert(data->StateVersion().get());
}
	
bool SetComponentStateOperationState::hasStateVersion() const {
	return data->StateVersion().present();
}
	
SetComponentStateOperationState & SetComponentStateOperationState::setDescriptorHandle(const HandleRef & value) {
	data->DescriptorHandle(ConvertToCDM::convert(value));
	return *this;
}


HandleRef SetComponentStateOperationState::getDescriptorHandle() const {
	return ConvertFromCDM::convert(data->DescriptorHandle());
}
	
SetComponentStateOperationState & SetComponentStateOperationState::setDescriptorVersion(const ReferencedVersion & value) {
	data->DescriptorVersion(ConvertToCDM::convert(value));
	return *this;
}

bool SetComponentStateOperationState::getDescriptorVersion(ReferencedVersion & out) const {
	if (data->DescriptorVersion().present()) {
		out = ConvertFromCDM::convert(data->DescriptorVersion().get());
		return true;
	}
	return false;
}

ReferencedVersion SetComponentStateOperationState::getDescriptorVersion() const {
	return ConvertFromCDM::convert(data->DescriptorVersion().get());
}
	
bool SetComponentStateOperationState::hasDescriptorVersion() const {
	return data->DescriptorVersion().present();
}
	
SetComponentStateOperationState & SetComponentStateOperationState::setOperatingMode(const OperatingMode & value) {
	data->OperatingMode(ConvertToCDM::convert(value));
	return *this;
}


OperatingMode SetComponentStateOperationState::getOperatingMode() const {
	return ConvertFromCDM::convert(data->OperatingMode());
}
	

} /* namespace SDC */
} /* namespace Data */
} /* namespace SDCLib */

