/*
 * ConvertToCDM.h
 *
 *  Created on: 22.06.2017
 *      Author: buerger
 *
 *  This file is autogenerated.
 *
 *  Do not edit this file. For customization please edit the ConvertToCDM_beginning.hxx or ConvertToCDM_ending.hxx
 */

#ifndef DATA_OSCP_MDIB_CONVERTTOCDM_H_
#define DATA_OSCP_MDIB_CONVERTTOCDM_H_

#include "OSCLib/Data/OSCP/MDIB/SimpleTypesMapping.h"
#include "OSCLib/Data/OSCP/MDIB/MDIB-fwd.h"
#include "osdm-fwd.hxx"

namespace OSCLib {
namespace Data {
namespace OSCP {

class ConvertToCDM {
public:
	ConvertToCDM();
	virtual ~ConvertToCDM();

	// build-in type conversions for the basetyes (see typedefs in SimpleTypesMapping.h)
	static bool convert(const bool & source);
	static double convert(const double & source);
	static unsigned int convert(const unsigned int & source);
	static int convert(const int & source);
	static long convert(const long & source);
	static long long convert(const long long & source);
	static std::string convert(const std::string & source);
	static unsigned short int convert(const unsigned short int & source);
	static unsigned long convert(const unsigned long & source);
	static unsigned long long convert(const unsigned long long & source);
	static xml_schema::Duration convert(const xml_schema::Duration & source);
	static xml_schema::Qname convert(const xml_schema::Qname & source);
	static xml_schema::Language convert(const xml_schema::Language & source);
	static xml_schema::Uri convert(const xml_schema::Uri & source);
	static xml_schema::DateTime convert(const xml_schema::DateTime & source);


	// autogenerated here
	static CDM::MeasurementValidity convert(const MeasurementValidity & source);
	static CDM::SafetyClassification convert(const SafetyClassification & source);
	static CDM::ComponentActivation convert(const ComponentActivation & source);
	static CDM::CalibrationState convert(const CalibrationState & source);
	static CDM::CalibrationType convert(const CalibrationType & source);
	static CDM::MdsOperatingMode convert(const MdsOperatingMode & source);
	static CDM::AlertActivation convert(const AlertActivation & source);
	static CDM::AlertConditionKind convert(const AlertConditionKind & source);
	static CDM::AlertConditionPriority convert(const AlertConditionPriority & source);
	static CDM::AlertConditionReference convert(const AlertConditionReference & source);
	static CDM::AlertConditionMonitoredLimits convert(const AlertConditionMonitoredLimits & source);
	static CDM::AlertSignalManifestation convert(const AlertSignalManifestation & source);
	static CDM::AlertSignalPresence convert(const AlertSignalPresence & source);
	static CDM::AlertSignalPrimaryLocation convert(const AlertSignalPrimaryLocation & source);
	static CDM::GenerationMode convert(const GenerationMode & source);
	static CDM::RealTimeValueType convert(const RealTimeValueType & source);
	static CDM::MetricCategory convert(const MetricCategory & source);
	static CDM::DerivationMethod convert(const DerivationMethod & source);
	static CDM::MetricAvailability convert(const MetricAvailability & source);
	static CDM::OperatingMode convert(const OperatingMode & source);
	static CDM::ContextAssociation convert(const ContextAssociation & source);
	static CDM::Sex convert(const Sex & source);
	static CDM::PatientType convert(const PatientType & source);
	static CDM::DicomTransferRole convert(const DicomTransferRole & source);
	static CDM::TextWidth convert(const TextWidth & source);
	static CDM::CanEscalate convert(const CanEscalate & source);
	static CDM::CanDeescalate convert(const CanDeescalate & source);
	static CDM::Kind convert(const Kind & source);
	static CDM::ChargeStatus convert(const ChargeStatus & source);
	static CDM::Criticality convert(const Criticality & source);

	template <class WrapperType>
	static std::unique_ptr<typename WrapperType::WrappedType> convert(const WrapperType & source);

}; // class

} /* namespace OSCP */
} /* namespace Data */
} /* namespace OSCLib */

#endif /* DATA_OSCP_MDIB_CONVERTTOCDM_H_ */
