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
 *  SetContextStateOperationState.cpp
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

#include "OSCLib/Data/OSCP/MDIB/SetContextStateOperationState.h"
#include "OSCLib/Data/OSCP/MDIB/ConvertToCDM.h"
#include "OSCLib/Data/OSCP/MDIB/ConvertFromCDM.h"
#include "OSCLib/Data/OSCP/MDIB/custom/Defaults.h"

#include "osdm.hxx"


namespace OSCLib {
namespace Data {
namespace OSCP {

SetContextStateOperationState::SetContextStateOperationState() : data(Defaults::SetContextStateOperationState()) {
}

SetContextStateOperationState::operator CDM::SetContextStateOperationState() const {
	return *data;
}

SetContextStateOperationState::SetContextStateOperationState(const CDM::SetContextStateOperationState & object) : data(new CDM::SetContextStateOperationState(object)) {

}

SetContextStateOperationState::SetContextStateOperationState(const SetContextStateOperationState & object) : data(new CDM::SetContextStateOperationState(*object.data)) {

}

SetContextStateOperationState::~SetContextStateOperationState() {

}

void SetContextStateOperationState::copyFrom(const SetContextStateOperationState & object) {
	*data = *object.data;
}

SetContextStateOperationState & SetContextStateOperationState:: operator=(const SetContextStateOperationState & object) {
	copyFrom(object);
	return *this;
}


SetContextStateOperationState & SetContextStateOperationState::setStateVersion(const VersionCounter & value) {
	data->StateVersion(ConvertToCDM::convert(value));
	return *this;
}


VersionCounter SetContextStateOperationState::getStateVersion() const {
	return ConvertFromCDM::convert(data->StateVersion());
}
	
SetContextStateOperationState & SetContextStateOperationState::setDescriptorHandle(const HandleRef & value) {
	data->DescriptorHandle(ConvertToCDM::convert(value));
	return *this;
}


HandleRef SetContextStateOperationState::getDescriptorHandle() const {
	return ConvertFromCDM::convert(data->DescriptorHandle());
}
	
SetContextStateOperationState & SetContextStateOperationState::setDescriptorVersion(const ReferencedVersion & value) {
	data->DescriptorVersion(ConvertToCDM::convert(value));
	return *this;
}


ReferencedVersion SetContextStateOperationState::getDescriptorVersion() const {
	return ConvertFromCDM::convert(data->DescriptorVersion());
}
	
SetContextStateOperationState & SetContextStateOperationState::setOperatingMode(const OperatingMode & value) {
	data->OperatingMode(ConvertToCDM::convert(value));
	return *this;
}


OperatingMode SetContextStateOperationState::getOperatingMode() const {
	return ConvertFromCDM::convert(data->OperatingMode());
}
	

} /* namespace OSCP */
} /* namespace Data */
} /* namespace OSCLib */

