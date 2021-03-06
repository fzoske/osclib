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
 *  ClinicalInfo.h
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

#ifndef CLINICALINFO_H_
#define CLINICALINFO_H_

#include "OSCLib/Data/SDC/MDIB/SimpleTypesMapping.h"
#include "OSCLib/Data/SDC/SDC-fwd.h"
#include "osdm-fwd.hxx"

namespace SDCLib {
namespace Data {
namespace SDC {

class ClinicalInfo {
private:
	ClinicalInfo(const CDM::ClinicalInfo & object);
	operator CDM::ClinicalInfo() const;
	friend class ConvertFromCDM;
	friend class ConvertToCDM;
	friend class SDCProvider;
	friend class SDCConsumer;
	friend class MdState;
	friend class MdDescription;
public:
	ClinicalInfo(
	); 
	ClinicalInfo(const ClinicalInfo & object);
	virtual ~ClinicalInfo();
    
    void copyFrom(const ClinicalInfo & object);
    ClinicalInfo & operator=(const ClinicalInfo & object);
    
    typedef CDM::ClinicalInfo WrappedType;

	ClinicalInfo & setType(const CodedValue & value);
	CodedValue getType() const;
	bool getType(CodedValue & out) const;
	bool hasType() const;

	ClinicalInfo & setCode(const CodedValue & value);
	CodedValue getCode() const;
	bool getCode(CodedValue & out) const;
	bool hasCode() const;

	ClinicalInfo & setCriticality(const Criticality & value);
	Criticality getCriticality() const;
	bool getCriticality(Criticality & out) const;
	bool hasCriticality() const;

	ClinicalInfo & addDescription(const LocalizedText & value);
	std::vector<LocalizedText> getDescriptionList() const;
	void clearDescriptionList();
	
	ClinicalInfo & addRelatedMeasurement(const RelatedMeasurement & value);
	std::vector<RelatedMeasurement> getRelatedMeasurementList() const;
	void clearRelatedMeasurementList();
	
private:
	std::shared_ptr<CDM::ClinicalInfo> data;
};

} /* namespace SDC */
} /* namespace Data */
} /* namespace SDCLib */
#endif /* CLINICALINFO_H_ */
