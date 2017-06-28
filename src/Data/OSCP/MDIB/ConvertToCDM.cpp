#include "OSCLib/Data/OSCP/MDIB/InvocationInfo.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractGet.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractGetResponse.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractReportPart.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractReport.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractSet.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractSetResponse.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractContextReport.h"
#include "OSCLib/Data/OSCP/MDIB/VersionFrame.h"
#include "OSCLib/Data/OSCP/MDIB/TimeFrame.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractMetricReport.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractComponentReport.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractAlertReport.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractOperationalStateReport.h"
#include "OSCLib/Data/OSCP/MDIB/RetrievabilityInfo.h"
#include "OSCLib/Data/OSCP/MDIB/Mdib.h"
#include "OSCLib/Data/OSCP/MDIB/MdDescription.h"
#include "OSCLib/Data/OSCP/MDIB/MdState.h"
#include "OSCLib/Data/OSCP/MDIB/LocalizedText.h"
#include "OSCLib/Data/OSCP/MDIB/CodedValue.h"
#include "OSCLib/Data/OSCP/MDIB/InstanceIdentifier.h"
#include "OSCLib/Data/OSCP/MDIB/Range.h"
#include "OSCLib/Data/OSCP/MDIB/Measurement.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractState.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractMultiState.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractDeviceComponentDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractAlertingDeviceComponentDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/CalibrationInfo.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractDeviceComponentState.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractAlertingDeviceComponentState.h"
#include "OSCLib/Data/OSCP/MDIB/MdsDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/MdsState.h"
#include "OSCLib/Data/OSCP/MDIB/VmdDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/VmdState.h"
#include "OSCLib/Data/OSCP/MDIB/ChannelDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/ChannelState.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractAlertDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractAlertState.h"
#include "OSCLib/Data/OSCP/MDIB/SystemSignalActivation.h"
#include "OSCLib/Data/OSCP/MDIB/AlertSystemDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/AlertSystemState.h"
#include "OSCLib/Data/OSCP/MDIB/CauseInfo.h"
#include "OSCLib/Data/OSCP/MDIB/RemedyInfo.h"
#include "OSCLib/Data/OSCP/MDIB/AlertConditionDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/AlertConditionState.h"
#include "OSCLib/Data/OSCP/MDIB/LimitAlertConditionDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/LimitAlertConditionState.h"
#include "OSCLib/Data/OSCP/MDIB/AlertSignalDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/AlertSignalState.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractMetricValue.h"
#include "OSCLib/Data/OSCP/MDIB/NumericMetricValue.h"
#include "OSCLib/Data/OSCP/MDIB/StringMetricValue.h"
#include "OSCLib/Data/OSCP/MDIB/SampleArrayValue.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractMetricDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractMetricState.h"
#include "OSCLib/Data/OSCP/MDIB/NumericMetricDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/NumericMetricState.h"
#include "OSCLib/Data/OSCP/MDIB/StringMetricDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/StringMetricState.h"
#include "OSCLib/Data/OSCP/MDIB/EnumStringMetricDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/EnumStringMetricState.h"
#include "OSCLib/Data/OSCP/MDIB/RealTimeSampleArrayMetricDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/RealTimeSampleArrayMetricState.h"
#include "OSCLib/Data/OSCP/MDIB/DistributionSampleArrayMetricDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/DistributionSampleArrayMetricState.h"
#include "OSCLib/Data/OSCP/MDIB/ScoDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/ScoState.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractOperationDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractSetStateOperationDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractOperationState.h"
#include "OSCLib/Data/OSCP/MDIB/SetValueOperationDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/SetValueOperationState.h"
#include "OSCLib/Data/OSCP/MDIB/SetStringOperationDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/SetStringOperationState.h"
#include "OSCLib/Data/OSCP/MDIB/ActivateOperationDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/ActivateOperationState.h"
#include "OSCLib/Data/OSCP/MDIB/SetContextStateOperationDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/SetContextStateOperationState.h"
#include "OSCLib/Data/OSCP/MDIB/SetMetricStateOperationDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/SetMetricStateOperationState.h"
#include "OSCLib/Data/OSCP/MDIB/SetComponentStateOperationDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/SetComponentStateOperationState.h"
#include "OSCLib/Data/OSCP/MDIB/SetAlertStateOperationDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/SetAlertStateOperationState.h"
#include "OSCLib/Data/OSCP/MDIB/ClockDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/ClockState.h"
#include "OSCLib/Data/OSCP/MDIB/BatteryDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/BatteryState.h"
#include "OSCLib/Data/OSCP/MDIB/SystemContextDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/SystemContextState.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractContextDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/AbstractContextState.h"
#include "OSCLib/Data/OSCP/MDIB/BaseDemographics.h"
#include "OSCLib/Data/OSCP/MDIB/PersonReference.h"
#include "OSCLib/Data/OSCP/MDIB/LocationDetail.h"
#include "OSCLib/Data/OSCP/MDIB/PatientContextDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/PatientDemographicsCoreData.h"
#include "OSCLib/Data/OSCP/MDIB/NeonatalPatientDemographicsCoreData.h"
#include "OSCLib/Data/OSCP/MDIB/PatientContextState.h"
#include "OSCLib/Data/OSCP/MDIB/LocationContextDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/LocationContextState.h"
#include "OSCLib/Data/OSCP/MDIB/WorkflowContextDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/ClinicalInfo.h"
#include "OSCLib/Data/OSCP/MDIB/ImagingProcedure.h"
#include "OSCLib/Data/OSCP/MDIB/LocationReference.h"
#include "OSCLib/Data/OSCP/MDIB/OrderDetail.h"
#include "OSCLib/Data/OSCP/MDIB/PersonParticipation.h"
#include "OSCLib/Data/OSCP/MDIB/WorkflowContextState.h"
#include "OSCLib/Data/OSCP/MDIB/OperatorContextDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/OperatorContextState.h"
#include "OSCLib/Data/OSCP/MDIB/MeansContextDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/MeansContextState.h"
#include "OSCLib/Data/OSCP/MDIB/EnsembleContextDescriptor.h"
#include "OSCLib/Data/OSCP/MDIB/EnsembleContextState.h"
#include "OSCLib/Data/OSCP/MDIB/ContainmentTree.h"
#include "OSCLib/Data/OSCP/MDIB/ContainmentTreeEntry.h"
#include "OSCLib/Data/OSCP/MDIB/ExtensionType.h"

/*
 * ConvertToCDM.cpp
 *
 *  Created on: 22.06.2017
 *      Author: buerger
 *
 *  This file is autogenerated.
 *
 *  Do not edit this file. For customization please edit the ConvertToCDM_beginning.cxx or ConvertToCDM_ending.cxx
 */

#include "osdm.hxx"

#include <stdexcept>

namespace OSCLib {
namespace Data {
namespace OSCP {

ConvertToCDM::ConvertToCDM() {

}

ConvertToCDM::~ConvertToCDM() {

}


bool ConvertToCDM::convert(const bool & source) {
	return source;
}

double ConvertToCDM::convert(const double & source) {
	return source;
}

unsigned int ConvertToCDM::convert(const unsigned int & source) {
	return source;
}

int ConvertToCDM::convert(const int & source) {
	return source;
}

long ConvertToCDM::convert(const long & source) {
	return source;
}

long long ConvertToCDM::convert(const long long & source) {
	return source;
}

std::string ConvertToCDM::convert(const std::string & source) {
	return source;
}

unsigned short int ConvertToCDM::convert(const unsigned short int & source) {
	return source;
}

CDM::InvocationState ConvertToCDM::convert(const InvocationState & source) {
	switch (source) {
		case InvocationState::Wait: return CDM::InvocationState::Wait;
		case InvocationState::Start: return CDM::InvocationState::Start;
		case InvocationState::Cnclld: return CDM::InvocationState::Cnclld;
		case InvocationState::CnclldMan: return CDM::InvocationState::CnclldMan;
		case InvocationState::Fin: return CDM::InvocationState::Fin;
		case InvocationState::FinMod: return CDM::InvocationState::FinMod;
		case InvocationState::Fail: return CDM::InvocationState::Fail;
	}
	throw std::runtime_error("Illegal value for InvocationState");
}

CDM::InvocationError ConvertToCDM::convert(const InvocationError & source) {
	switch (source) {
		case InvocationError::Unspec: return CDM::InvocationError::Unspec;
		case InvocationError::Unkn: return CDM::InvocationError::Unkn;
		case InvocationError::Inv: return CDM::InvocationError::Inv;
		case InvocationError::Oth: return CDM::InvocationError::Oth;
	}
	throw std::runtime_error("Illegal value for InvocationError");
}

CDM::DescriptionModificationType ConvertToCDM::convert(const DescriptionModificationType & source) {
	switch (source) {
		case DescriptionModificationType::Crt: return CDM::DescriptionModificationType::Crt;
		case DescriptionModificationType::Upt: return CDM::DescriptionModificationType::Upt;
		case DescriptionModificationType::Del: return CDM::DescriptionModificationType::Del;
	}
	throw std::runtime_error("Illegal value for DescriptionModificationType");
}

CDM::RetrievabilityMethod ConvertToCDM::convert(const RetrievabilityMethod & source) {
	switch (source) {
		case RetrievabilityMethod::Get: return CDM::RetrievabilityMethod::Get;
		case RetrievabilityMethod::Per: return CDM::RetrievabilityMethod::Per;
		case RetrievabilityMethod::Ep: return CDM::RetrievabilityMethod::Ep;
		case RetrievabilityMethod::Strm: return CDM::RetrievabilityMethod::Strm;
	}
	throw std::runtime_error("Illegal value for RetrievabilityMethod");
}

CDM::MeasurementValidity ConvertToCDM::convert(const MeasurementValidity & source) {
	switch (source) {
		case MeasurementValidity::Vld: return CDM::MeasurementValidity::Vld;
		case MeasurementValidity::Vldated: return CDM::MeasurementValidity::Vldated;
		case MeasurementValidity::Ong: return CDM::MeasurementValidity::Ong;
		case MeasurementValidity::Qst: return CDM::MeasurementValidity::Qst;
		case MeasurementValidity::Calib: return CDM::MeasurementValidity::Calib;
		case MeasurementValidity::Inv: return CDM::MeasurementValidity::Inv;
		case MeasurementValidity::Oflw: return CDM::MeasurementValidity::Oflw;
		case MeasurementValidity::Uflw: return CDM::MeasurementValidity::Uflw;
		case MeasurementValidity::NA: return CDM::MeasurementValidity::NA;
	}
	throw std::runtime_error("Illegal value for MeasurementValidity");
}

CDM::SafetyClassification ConvertToCDM::convert(const SafetyClassification & source) {
	switch (source) {
		case SafetyClassification::Inf: return CDM::SafetyClassification::Inf;
		case SafetyClassification::MedA: return CDM::SafetyClassification::MedA;
		case SafetyClassification::MedB: return CDM::SafetyClassification::MedB;
		case SafetyClassification::MedC: return CDM::SafetyClassification::MedC;
	}
	throw std::runtime_error("Illegal value for SafetyClassification");
}

CDM::ComponentActivation ConvertToCDM::convert(const ComponentActivation & source) {
	switch (source) {
		case ComponentActivation::On: return CDM::ComponentActivation::On;
		case ComponentActivation::NotRdy: return CDM::ComponentActivation::NotRdy;
		case ComponentActivation::StndBy: return CDM::ComponentActivation::StndBy;
		case ComponentActivation::Off: return CDM::ComponentActivation::Off;
		case ComponentActivation::Shtdn: return CDM::ComponentActivation::Shtdn;
		case ComponentActivation::Fail: return CDM::ComponentActivation::Fail;
	}
	throw std::runtime_error("Illegal value for ComponentActivation");
}

CDM::CalibrationState ConvertToCDM::convert(const CalibrationState & source) {
	switch (source) {
		case CalibrationState::No: return CDM::CalibrationState::No;
		case CalibrationState::Req: return CDM::CalibrationState::Req;
		case CalibrationState::Cal: return CDM::CalibrationState::Cal;
	}
	throw std::runtime_error("Illegal value for CalibrationState");
}

CDM::CalibrationType ConvertToCDM::convert(const CalibrationType & source) {
	switch (source) {
		case CalibrationType::Offset: return CDM::CalibrationType::Offset;
		case CalibrationType::Gain: return CDM::CalibrationType::Gain;
		case CalibrationType::TP: return CDM::CalibrationType::TP;
		case CalibrationType::Unspec: return CDM::CalibrationType::Unspec;
	}
	throw std::runtime_error("Illegal value for CalibrationType");
}

CDM::MdsOperatingMode ConvertToCDM::convert(const MdsOperatingMode & source) {
	switch (source) {
		case MdsOperatingMode::Nml: return CDM::MdsOperatingMode::Nml;
		case MdsOperatingMode::Dmo: return CDM::MdsOperatingMode::Dmo;
		case MdsOperatingMode::Srv: return CDM::MdsOperatingMode::Srv;
		case MdsOperatingMode::Mtn: return CDM::MdsOperatingMode::Mtn;
	}
	throw std::runtime_error("Illegal value for MdsOperatingMode");
}

CDM::AlertActivation ConvertToCDM::convert(const AlertActivation & source) {
	switch (source) {
		case AlertActivation::On: return CDM::AlertActivation::On;
		case AlertActivation::Off: return CDM::AlertActivation::Off;
		case AlertActivation::Psd: return CDM::AlertActivation::Psd;
	}
	throw std::runtime_error("Illegal value for AlertActivation");
}

CDM::AlertConditionKind ConvertToCDM::convert(const AlertConditionKind & source) {
	switch (source) {
		case AlertConditionKind::Phy: return CDM::AlertConditionKind::Phy;
		case AlertConditionKind::Tec: return CDM::AlertConditionKind::Tec;
		case AlertConditionKind::Oth: return CDM::AlertConditionKind::Oth;
	}
	throw std::runtime_error("Illegal value for AlertConditionKind");
}

CDM::AlertConditionPriority ConvertToCDM::convert(const AlertConditionPriority & source) {
	switch (source) {
		case AlertConditionPriority::Lo: return CDM::AlertConditionPriority::Lo;
		case AlertConditionPriority::Me: return CDM::AlertConditionPriority::Me;
		case AlertConditionPriority::Hi: return CDM::AlertConditionPriority::Hi;
		case AlertConditionPriority::None: return CDM::AlertConditionPriority::None;
	}
	throw std::runtime_error("Illegal value for AlertConditionPriority");
}

CDM::AlertConditionMonitoredLimits ConvertToCDM::convert(const AlertConditionMonitoredLimits & source) {
	switch (source) {
		case AlertConditionMonitoredLimits::All: return CDM::AlertConditionMonitoredLimits::All;
		case AlertConditionMonitoredLimits::LoOff: return CDM::AlertConditionMonitoredLimits::LoOff;
		case AlertConditionMonitoredLimits::HiOff: return CDM::AlertConditionMonitoredLimits::HiOff;
		case AlertConditionMonitoredLimits::None: return CDM::AlertConditionMonitoredLimits::None;
	}
	throw std::runtime_error("Illegal value for AlertConditionMonitoredLimits");
}

CDM::AlertSignalManifestation ConvertToCDM::convert(const AlertSignalManifestation & source) {
	switch (source) {
		case AlertSignalManifestation::Aud: return CDM::AlertSignalManifestation::Aud;
		case AlertSignalManifestation::Vis: return CDM::AlertSignalManifestation::Vis;
		case AlertSignalManifestation::Tan: return CDM::AlertSignalManifestation::Tan;
		case AlertSignalManifestation::Oth: return CDM::AlertSignalManifestation::Oth;
	}
	throw std::runtime_error("Illegal value for AlertSignalManifestation");
}

CDM::AlertSignalPresence ConvertToCDM::convert(const AlertSignalPresence & source) {
	switch (source) {
		case AlertSignalPresence::On: return CDM::AlertSignalPresence::On;
		case AlertSignalPresence::Off: return CDM::AlertSignalPresence::Off;
		case AlertSignalPresence::Latch: return CDM::AlertSignalPresence::Latch;
		case AlertSignalPresence::Ack: return CDM::AlertSignalPresence::Ack;
	}
	throw std::runtime_error("Illegal value for AlertSignalPresence");
}

CDM::AlertSignalPrimaryLocation ConvertToCDM::convert(const AlertSignalPrimaryLocation & source) {
	switch (source) {
		case AlertSignalPrimaryLocation::Loc: return CDM::AlertSignalPrimaryLocation::Loc;
		case AlertSignalPrimaryLocation::Rem: return CDM::AlertSignalPrimaryLocation::Rem;
	}
	throw std::runtime_error("Illegal value for AlertSignalPrimaryLocation");
}

CDM::GenerationMode ConvertToCDM::convert(const GenerationMode & source) {
	switch (source) {
		case GenerationMode::Real: return CDM::GenerationMode::Real;
		case GenerationMode::Test: return CDM::GenerationMode::Test;
		case GenerationMode::Demo: return CDM::GenerationMode::Demo;
	}
	throw std::runtime_error("Illegal value for GenerationMode");
}

CDM::MetricCategory ConvertToCDM::convert(const MetricCategory & source) {
	switch (source) {
		case MetricCategory::Unspec: return CDM::MetricCategory::Unspec;
		case MetricCategory::Msrmt: return CDM::MetricCategory::Msrmt;
		case MetricCategory::Clc: return CDM::MetricCategory::Clc;
		case MetricCategory::Set: return CDM::MetricCategory::Set;
		case MetricCategory::Preset: return CDM::MetricCategory::Preset;
		case MetricCategory::Rcmm: return CDM::MetricCategory::Rcmm;
	}
	throw std::runtime_error("Illegal value for MetricCategory");
}

CDM::DerivationMethod ConvertToCDM::convert(const DerivationMethod & source) {
	switch (source) {
		case DerivationMethod::Auto: return CDM::DerivationMethod::Auto;
		case DerivationMethod::Man: return CDM::DerivationMethod::Man;
	}
	throw std::runtime_error("Illegal value for DerivationMethod");
}

CDM::MetricAvailability ConvertToCDM::convert(const MetricAvailability & source) {
	switch (source) {
		case MetricAvailability::Intr: return CDM::MetricAvailability::Intr;
		case MetricAvailability::Cont: return CDM::MetricAvailability::Cont;
	}
	throw std::runtime_error("Illegal value for MetricAvailability");
}

CDM::OperatingMode ConvertToCDM::convert(const OperatingMode & source) {
	switch (source) {
		case OperatingMode::Dis: return CDM::OperatingMode::Dis;
		case OperatingMode::En: return CDM::OperatingMode::En;
		case OperatingMode::NA: return CDM::OperatingMode::NA;
	}
	throw std::runtime_error("Illegal value for OperatingMode");
}

CDM::ContextAssociation ConvertToCDM::convert(const ContextAssociation & source) {
	switch (source) {
		case ContextAssociation::No: return CDM::ContextAssociation::No;
		case ContextAssociation::Pre: return CDM::ContextAssociation::Pre;
		case ContextAssociation::Assoc: return CDM::ContextAssociation::Assoc;
		case ContextAssociation::Dis: return CDM::ContextAssociation::Dis;
	}
	throw std::runtime_error("Illegal value for ContextAssociation");
}

CDM::Sex ConvertToCDM::convert(const Sex & source) {
	switch (source) {
		case Sex::Unspec: return CDM::Sex::Unspec;
		case Sex::M: return CDM::Sex::M;
		case Sex::F: return CDM::Sex::F;
		case Sex::Unkn: return CDM::Sex::Unkn;
	}
	throw std::runtime_error("Illegal value for Sex");
}

CDM::PatientType ConvertToCDM::convert(const PatientType & source) {
	switch (source) {
		case PatientType::Unspec: return CDM::PatientType::Unspec;
		case PatientType::Ad: return CDM::PatientType::Ad;
		case PatientType::Ped: return CDM::PatientType::Ped;
		case PatientType::Neo: return CDM::PatientType::Neo;
	}
	throw std::runtime_error("Illegal value for PatientType");
}

template
std::unique_ptr<typename InvocationInfo::WrappedType> ConvertToCDM::convert(const InvocationInfo&source);

template
std::unique_ptr<typename AbstractGet::WrappedType> ConvertToCDM::convert(const AbstractGet&source);

template
std::unique_ptr<typename AbstractGetResponse::WrappedType> ConvertToCDM::convert(const AbstractGetResponse&source);

template
std::unique_ptr<typename AbstractReportPart::WrappedType> ConvertToCDM::convert(const AbstractReportPart&source);

template
std::unique_ptr<typename AbstractReport::WrappedType> ConvertToCDM::convert(const AbstractReport&source);

template
std::unique_ptr<typename AbstractSet::WrappedType> ConvertToCDM::convert(const AbstractSet&source);

template
std::unique_ptr<typename AbstractSetResponse::WrappedType> ConvertToCDM::convert(const AbstractSetResponse&source);

template
std::unique_ptr<typename AbstractContextReport::WrappedType> ConvertToCDM::convert(const AbstractContextReport&source);

template
std::unique_ptr<typename VersionFrame::WrappedType> ConvertToCDM::convert(const VersionFrame&source);

template
std::unique_ptr<typename TimeFrame::WrappedType> ConvertToCDM::convert(const TimeFrame&source);

template
std::unique_ptr<typename AbstractMetricReport::WrappedType> ConvertToCDM::convert(const AbstractMetricReport&source);

template
std::unique_ptr<typename AbstractComponentReport::WrappedType> ConvertToCDM::convert(const AbstractComponentReport&source);

template
std::unique_ptr<typename AbstractAlertReport::WrappedType> ConvertToCDM::convert(const AbstractAlertReport&source);

template
std::unique_ptr<typename AbstractOperationalStateReport::WrappedType> ConvertToCDM::convert(const AbstractOperationalStateReport&source);

template
std::unique_ptr<typename RetrievabilityInfo::WrappedType> ConvertToCDM::convert(const RetrievabilityInfo&source);

template
std::unique_ptr<typename Mdib::WrappedType> ConvertToCDM::convert(const Mdib&source);

template
std::unique_ptr<typename MdDescription::WrappedType> ConvertToCDM::convert(const MdDescription&source);

template
std::unique_ptr<typename MdState::WrappedType> ConvertToCDM::convert(const MdState&source);

template
std::unique_ptr<typename LocalizedText::WrappedType> ConvertToCDM::convert(const LocalizedText&source);

template
std::unique_ptr<typename CodedValue::WrappedType> ConvertToCDM::convert(const CodedValue&source);

template
std::unique_ptr<typename InstanceIdentifier::WrappedType> ConvertToCDM::convert(const InstanceIdentifier&source);

template
std::unique_ptr<typename Range::WrappedType> ConvertToCDM::convert(const Range&source);

template
std::unique_ptr<typename Measurement::WrappedType> ConvertToCDM::convert(const Measurement&source);

template
std::unique_ptr<typename AbstractDescriptor::WrappedType> ConvertToCDM::convert(const AbstractDescriptor&source);

template
std::unique_ptr<typename AbstractState::WrappedType> ConvertToCDM::convert(const AbstractState&source);

template
std::unique_ptr<typename AbstractMultiState::WrappedType> ConvertToCDM::convert(const AbstractMultiState&source);

template
std::unique_ptr<typename AbstractDeviceComponentDescriptor::WrappedType> ConvertToCDM::convert(const AbstractDeviceComponentDescriptor&source);

template
std::unique_ptr<typename AbstractAlertingDeviceComponentDescriptor::WrappedType> ConvertToCDM::convert(const AbstractAlertingDeviceComponentDescriptor&source);

template
std::unique_ptr<typename CalibrationInfo::WrappedType> ConvertToCDM::convert(const CalibrationInfo&source);

template
std::unique_ptr<typename AbstractDeviceComponentState::WrappedType> ConvertToCDM::convert(const AbstractDeviceComponentState&source);

template
std::unique_ptr<typename AbstractAlertingDeviceComponentState::WrappedType> ConvertToCDM::convert(const AbstractAlertingDeviceComponentState&source);

template
std::unique_ptr<typename MdsDescriptor::WrappedType> ConvertToCDM::convert(const MdsDescriptor&source);

template
std::unique_ptr<typename MdsState::WrappedType> ConvertToCDM::convert(const MdsState&source);

template
std::unique_ptr<typename VmdDescriptor::WrappedType> ConvertToCDM::convert(const VmdDescriptor&source);

template
std::unique_ptr<typename VmdState::WrappedType> ConvertToCDM::convert(const VmdState&source);

template
std::unique_ptr<typename ChannelDescriptor::WrappedType> ConvertToCDM::convert(const ChannelDescriptor&source);

template
std::unique_ptr<typename ChannelState::WrappedType> ConvertToCDM::convert(const ChannelState&source);

template
std::unique_ptr<typename AbstractAlertDescriptor::WrappedType> ConvertToCDM::convert(const AbstractAlertDescriptor&source);

template
std::unique_ptr<typename AbstractAlertState::WrappedType> ConvertToCDM::convert(const AbstractAlertState&source);

template
std::unique_ptr<typename SystemSignalActivation::WrappedType> ConvertToCDM::convert(const SystemSignalActivation&source);

template
std::unique_ptr<typename AlertSystemDescriptor::WrappedType> ConvertToCDM::convert(const AlertSystemDescriptor&source);

template
std::unique_ptr<typename AlertSystemState::WrappedType> ConvertToCDM::convert(const AlertSystemState&source);

template
std::unique_ptr<typename CauseInfo::WrappedType> ConvertToCDM::convert(const CauseInfo&source);

template
std::unique_ptr<typename RemedyInfo::WrappedType> ConvertToCDM::convert(const RemedyInfo&source);

template
std::unique_ptr<typename AlertConditionDescriptor::WrappedType> ConvertToCDM::convert(const AlertConditionDescriptor&source);

template
std::unique_ptr<typename AlertConditionState::WrappedType> ConvertToCDM::convert(const AlertConditionState&source);

template
std::unique_ptr<typename LimitAlertConditionDescriptor::WrappedType> ConvertToCDM::convert(const LimitAlertConditionDescriptor&source);

template
std::unique_ptr<typename LimitAlertConditionState::WrappedType> ConvertToCDM::convert(const LimitAlertConditionState&source);

template
std::unique_ptr<typename AlertSignalDescriptor::WrappedType> ConvertToCDM::convert(const AlertSignalDescriptor&source);

template
std::unique_ptr<typename AlertSignalState::WrappedType> ConvertToCDM::convert(const AlertSignalState&source);

template
std::unique_ptr<typename AbstractMetricValue::WrappedType> ConvertToCDM::convert(const AbstractMetricValue&source);

template
std::unique_ptr<typename NumericMetricValue::WrappedType> ConvertToCDM::convert(const NumericMetricValue&source);

template
std::unique_ptr<typename StringMetricValue::WrappedType> ConvertToCDM::convert(const StringMetricValue&source);

template
std::unique_ptr<typename SampleArrayValue::WrappedType> ConvertToCDM::convert(const SampleArrayValue&source);

template
std::unique_ptr<typename AbstractMetricDescriptor::WrappedType> ConvertToCDM::convert(const AbstractMetricDescriptor&source);

template
std::unique_ptr<typename AbstractMetricState::WrappedType> ConvertToCDM::convert(const AbstractMetricState&source);

template
std::unique_ptr<typename NumericMetricDescriptor::WrappedType> ConvertToCDM::convert(const NumericMetricDescriptor&source);

template
std::unique_ptr<typename NumericMetricState::WrappedType> ConvertToCDM::convert(const NumericMetricState&source);

template
std::unique_ptr<typename StringMetricDescriptor::WrappedType> ConvertToCDM::convert(const StringMetricDescriptor&source);

template
std::unique_ptr<typename StringMetricState::WrappedType> ConvertToCDM::convert(const StringMetricState&source);

template
std::unique_ptr<typename EnumStringMetricDescriptor::WrappedType> ConvertToCDM::convert(const EnumStringMetricDescriptor&source);

template
std::unique_ptr<typename EnumStringMetricState::WrappedType> ConvertToCDM::convert(const EnumStringMetricState&source);

template
std::unique_ptr<typename RealTimeSampleArrayMetricDescriptor::WrappedType> ConvertToCDM::convert(const RealTimeSampleArrayMetricDescriptor&source);

template
std::unique_ptr<typename RealTimeSampleArrayMetricState::WrappedType> ConvertToCDM::convert(const RealTimeSampleArrayMetricState&source);

template
std::unique_ptr<typename DistributionSampleArrayMetricDescriptor::WrappedType> ConvertToCDM::convert(const DistributionSampleArrayMetricDescriptor&source);

template
std::unique_ptr<typename DistributionSampleArrayMetricState::WrappedType> ConvertToCDM::convert(const DistributionSampleArrayMetricState&source);

template
std::unique_ptr<typename ScoDescriptor::WrappedType> ConvertToCDM::convert(const ScoDescriptor&source);

template
std::unique_ptr<typename ScoState::WrappedType> ConvertToCDM::convert(const ScoState&source);

template
std::unique_ptr<typename AbstractOperationDescriptor::WrappedType> ConvertToCDM::convert(const AbstractOperationDescriptor&source);

template
std::unique_ptr<typename AbstractSetStateOperationDescriptor::WrappedType> ConvertToCDM::convert(const AbstractSetStateOperationDescriptor&source);

template
std::unique_ptr<typename AbstractOperationState::WrappedType> ConvertToCDM::convert(const AbstractOperationState&source);

template
std::unique_ptr<typename SetValueOperationDescriptor::WrappedType> ConvertToCDM::convert(const SetValueOperationDescriptor&source);

template
std::unique_ptr<typename SetValueOperationState::WrappedType> ConvertToCDM::convert(const SetValueOperationState&source);

template
std::unique_ptr<typename SetStringOperationDescriptor::WrappedType> ConvertToCDM::convert(const SetStringOperationDescriptor&source);

template
std::unique_ptr<typename SetStringOperationState::WrappedType> ConvertToCDM::convert(const SetStringOperationState&source);

template
std::unique_ptr<typename ActivateOperationDescriptor::WrappedType> ConvertToCDM::convert(const ActivateOperationDescriptor&source);

template
std::unique_ptr<typename ActivateOperationState::WrappedType> ConvertToCDM::convert(const ActivateOperationState&source);

template
std::unique_ptr<typename SetContextStateOperationDescriptor::WrappedType> ConvertToCDM::convert(const SetContextStateOperationDescriptor&source);

template
std::unique_ptr<typename SetContextStateOperationState::WrappedType> ConvertToCDM::convert(const SetContextStateOperationState&source);

template
std::unique_ptr<typename SetMetricStateOperationDescriptor::WrappedType> ConvertToCDM::convert(const SetMetricStateOperationDescriptor&source);

template
std::unique_ptr<typename SetMetricStateOperationState::WrappedType> ConvertToCDM::convert(const SetMetricStateOperationState&source);

template
std::unique_ptr<typename SetComponentStateOperationDescriptor::WrappedType> ConvertToCDM::convert(const SetComponentStateOperationDescriptor&source);

template
std::unique_ptr<typename SetComponentStateOperationState::WrappedType> ConvertToCDM::convert(const SetComponentStateOperationState&source);

template
std::unique_ptr<typename SetAlertStateOperationDescriptor::WrappedType> ConvertToCDM::convert(const SetAlertStateOperationDescriptor&source);

template
std::unique_ptr<typename SetAlertStateOperationState::WrappedType> ConvertToCDM::convert(const SetAlertStateOperationState&source);

template
std::unique_ptr<typename ClockDescriptor::WrappedType> ConvertToCDM::convert(const ClockDescriptor&source);

template
std::unique_ptr<typename ClockState::WrappedType> ConvertToCDM::convert(const ClockState&source);

template
std::unique_ptr<typename BatteryDescriptor::WrappedType> ConvertToCDM::convert(const BatteryDescriptor&source);

template
std::unique_ptr<typename BatteryState::WrappedType> ConvertToCDM::convert(const BatteryState&source);

template
std::unique_ptr<typename SystemContextDescriptor::WrappedType> ConvertToCDM::convert(const SystemContextDescriptor&source);

template
std::unique_ptr<typename SystemContextState::WrappedType> ConvertToCDM::convert(const SystemContextState&source);

template
std::unique_ptr<typename AbstractContextDescriptor::WrappedType> ConvertToCDM::convert(const AbstractContextDescriptor&source);

template
std::unique_ptr<typename AbstractContextState::WrappedType> ConvertToCDM::convert(const AbstractContextState&source);

template
std::unique_ptr<typename BaseDemographics::WrappedType> ConvertToCDM::convert(const BaseDemographics&source);

template
std::unique_ptr<typename PersonReference::WrappedType> ConvertToCDM::convert(const PersonReference&source);

template
std::unique_ptr<typename LocationDetail::WrappedType> ConvertToCDM::convert(const LocationDetail&source);

template
std::unique_ptr<typename PatientContextDescriptor::WrappedType> ConvertToCDM::convert(const PatientContextDescriptor&source);

template
std::unique_ptr<typename PatientDemographicsCoreData::WrappedType> ConvertToCDM::convert(const PatientDemographicsCoreData&source);

template
std::unique_ptr<typename NeonatalPatientDemographicsCoreData::WrappedType> ConvertToCDM::convert(const NeonatalPatientDemographicsCoreData&source);

template
std::unique_ptr<typename PatientContextState::WrappedType> ConvertToCDM::convert(const PatientContextState&source);

template
std::unique_ptr<typename LocationContextDescriptor::WrappedType> ConvertToCDM::convert(const LocationContextDescriptor&source);

template
std::unique_ptr<typename LocationContextState::WrappedType> ConvertToCDM::convert(const LocationContextState&source);

template
std::unique_ptr<typename WorkflowContextDescriptor::WrappedType> ConvertToCDM::convert(const WorkflowContextDescriptor&source);

template
std::unique_ptr<typename ClinicalInfo::WrappedType> ConvertToCDM::convert(const ClinicalInfo&source);

template
std::unique_ptr<typename ImagingProcedure::WrappedType> ConvertToCDM::convert(const ImagingProcedure&source);

template
std::unique_ptr<typename LocationReference::WrappedType> ConvertToCDM::convert(const LocationReference&source);

template
std::unique_ptr<typename OrderDetail::WrappedType> ConvertToCDM::convert(const OrderDetail&source);

template
std::unique_ptr<typename PersonParticipation::WrappedType> ConvertToCDM::convert(const PersonParticipation&source);

template
std::unique_ptr<typename WorkflowContextState::WrappedType> ConvertToCDM::convert(const WorkflowContextState&source);

template
std::unique_ptr<typename OperatorContextDescriptor::WrappedType> ConvertToCDM::convert(const OperatorContextDescriptor&source);

template
std::unique_ptr<typename OperatorContextState::WrappedType> ConvertToCDM::convert(const OperatorContextState&source);

template
std::unique_ptr<typename MeansContextDescriptor::WrappedType> ConvertToCDM::convert(const MeansContextDescriptor&source);

template
std::unique_ptr<typename MeansContextState::WrappedType> ConvertToCDM::convert(const MeansContextState&source);

template
std::unique_ptr<typename EnsembleContextDescriptor::WrappedType> ConvertToCDM::convert(const EnsembleContextDescriptor&source);

template
std::unique_ptr<typename EnsembleContextState::WrappedType> ConvertToCDM::convert(const EnsembleContextState&source);

template
std::unique_ptr<typename ContainmentTree::WrappedType> ConvertToCDM::convert(const ContainmentTree&source);

template
std::unique_ptr<typename ContainmentTreeEntry::WrappedType> ConvertToCDM::convert(const ContainmentTreeEntry&source);

template
std::unique_ptr<typename ExtensionType::WrappedType> ConvertToCDM::convert(const ExtensionType&source);



} /* namespace OSCP */
} /* namespace Data */
} /* namespace OSCLib */
