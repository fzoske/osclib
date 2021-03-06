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
 *  PatientContextDescriptor.h
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

#ifndef PATIENTCONTEXTDESCRIPTOR_H_
#define PATIENTCONTEXTDESCRIPTOR_H_

#include "OSCLib/Data/SDC/MDIB/SimpleTypesMapping.h"
#include "OSCLib/Data/SDC/SDC-fwd.h"
#include "osdm-fwd.hxx"

namespace SDCLib {
namespace Data {
namespace SDC {

class PatientContextDescriptor {
private:
	PatientContextDescriptor(const CDM::PatientContextDescriptor & object);
	operator CDM::PatientContextDescriptor() const;
	friend class ConvertFromCDM;
	friend class ConvertToCDM;
	friend class SDCProvider;
	friend class SDCConsumer;
	friend class MdState;
	friend class MdDescription;
public:
	PatientContextDescriptor(
		Handle handle
	); 
private:
	PatientContextDescriptor(){};
public:
	PatientContextDescriptor(const PatientContextDescriptor & object);
	virtual ~PatientContextDescriptor();
    
    void copyFrom(const PatientContextDescriptor & object);
    PatientContextDescriptor & operator=(const PatientContextDescriptor & object);
    
    typedef CDM::PatientContextDescriptor WrappedType;
    typedef PatientContextState StateType;

	PatientContextDescriptor & setType(const CodedValue & value);
	CodedValue getType() const;
	bool getType(CodedValue & out) const;
	bool hasType() const;

	PatientContextDescriptor & setHandle(const Handle & value);
	Handle getHandle() const;

	PatientContextDescriptor & setDescriptorVersion(const VersionCounter & value);
	VersionCounter getDescriptorVersion() const;
	bool getDescriptorVersion(VersionCounter & out) const;
	bool hasDescriptorVersion() const;

	PatientContextDescriptor & setSafetyClassification(const SafetyClassification & value);
	SafetyClassification getSafetyClassification() const;
	bool getSafetyClassification(SafetyClassification & out) const;
	bool hasSafetyClassification() const;

private:
	std::shared_ptr<CDM::PatientContextDescriptor> data;
};

} /* namespace SDC */
} /* namespace Data */
} /* namespace SDCLib */
#endif /* PATIENTCONTEXTDESCRIPTOR_H_ */
