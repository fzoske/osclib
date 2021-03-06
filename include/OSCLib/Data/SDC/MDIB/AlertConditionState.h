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
 *  AlertConditionState.h
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

#ifndef ALERTCONDITIONSTATE_H_
#define ALERTCONDITIONSTATE_H_

#include "OSCLib/Data/SDC/MDIB/SimpleTypesMapping.h"
#include "OSCLib/Data/SDC/SDC-fwd.h"
#include "osdm-fwd.hxx"

namespace SDCLib {
namespace Data {
namespace SDC {

class AlertConditionState {
private:
	AlertConditionState(const CDM::AlertConditionState & object);
	operator CDM::AlertConditionState() const;
	friend class ConvertFromCDM;
	friend class ConvertToCDM;
	friend class SDCProvider;
	friend class SDCConsumer;
	friend class MdState;
	friend class MdDescription;
public:
	AlertConditionState(
		HandleRef descriptorhandle
		, 
		AlertActivation activationstate
	); 
private:
	AlertConditionState(){};
public:
	AlertConditionState(const AlertConditionState & object);
	virtual ~AlertConditionState();
    
    void copyFrom(const AlertConditionState & object);
    AlertConditionState & operator=(const AlertConditionState & object);
    
    typedef CDM::AlertConditionState WrappedType;
    typedef AlertConditionDescriptor DescriptorType;

	AlertConditionState & setStateVersion(const VersionCounter & value);
	VersionCounter getStateVersion() const;
	bool getStateVersion(VersionCounter & out) const;
	bool hasStateVersion() const;

	AlertConditionState & setDescriptorHandle(const HandleRef & value);
	HandleRef getDescriptorHandle() const;

	AlertConditionState & setDescriptorVersion(const ReferencedVersion & value);
	ReferencedVersion getDescriptorVersion() const;
	bool getDescriptorVersion(ReferencedVersion & out) const;
	bool hasDescriptorVersion() const;

	AlertConditionState & setActivationState(const AlertActivation & value);
	AlertActivation getActivationState() const;

	AlertConditionState & setActualConditionGenerationDelay(const xml_schema::Duration & value);
	xml_schema::Duration getActualConditionGenerationDelay() const;
	bool getActualConditionGenerationDelay(xml_schema::Duration & out) const;
	bool hasActualConditionGenerationDelay() const;

	AlertConditionState & setActualPriority(const AlertConditionPriority & value);
	AlertConditionPriority getActualPriority() const;
	bool getActualPriority(AlertConditionPriority & out) const;
	bool hasActualPriority() const;

	AlertConditionState & setRank(const int & value);
	int getRank() const;
	bool getRank(int & out) const;
	bool hasRank() const;

	AlertConditionState & setPresence(const bool & value);
	bool getPresence() const;
	bool getPresence(bool & out) const;
	bool hasPresence() const;

	AlertConditionState & setDeterminationTime(const Timestamp & value);
	Timestamp getDeterminationTime() const;
	bool getDeterminationTime(Timestamp & out) const;
	bool hasDeterminationTime() const;

private:
	std::shared_ptr<CDM::AlertConditionState> data;
};

} /* namespace SDC */
} /* namespace Data */
} /* namespace SDCLib */
#endif /* ALERTCONDITIONSTATE_H_ */
