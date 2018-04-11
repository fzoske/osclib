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
 * SDCProvider.cpp
 *
 *  @Copyright (C) 2018, SurgiTAIX AG
 *  Author: roehser, besting, buerger
 *
 */

#include <memory>

#include "Poco/AutoPtr.h"
#include "Poco/Net/NetException.h"
#include "Poco/Notification.h"
#include "Poco/UUIDGenerator.h"

#include "osdm.hxx"

#include "OSCLib/SDCLibrary.h"
#include "OSELib/SDC/SDCConstants.h"

#include "OSCLib/Data/SDC/SDCProvider.h"
#include "OSCLib/Data/SDC/SDCProviderStateHandler.h"
#include "OSCLib/Data/SDC/SDCProviderActivateOperationHandler.h"
#include "OSCLib/Data/SDC/SDCProviderAlertConditionStateHandler.h"
#include "OSCLib/Data/SDC/SDCProviderComponentStateHandler.h"
#include "OSCLib/Data/SDC/SDCProviderMDStateHandler.h"

#include "OSCLib/Data/SDC/MDIB/ActivateOperationDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/AlertConditionDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/AlertConditionState.h"
#include "OSCLib/Data/SDC/MDIB/AlertSignalDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/AlertSignalState.h"
#include "OSCLib/Data/SDC/MDIB/AlertSystemDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/AlertSystemState.h"
#include "OSCLib/Data/SDC/MDIB/AllowedValue.h"
#include "OSCLib/Data/SDC/MDIB/ClockState.h"
#include "OSCLib/Data/SDC/MDIB/ConvertFromCDM.h"
#include "OSCLib/Data/SDC/MDIB/ConvertToCDM.h"
#include "OSCLib/Data/SDC/MDIB/Defaults.h"
#include "OSCLib/Data/SDC/MDIB/DistributionSampleArrayMetricState.h"
#include "OSCLib/Data/SDC/MDIB/SimpleTypesMapping.h"
#include "OSCLib/Data/SDC/MDIB/EnumStringMetricDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/EnumStringMetricState.h"
#include "OSCLib/Data/SDC/MDIB/MdsDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/MdsState.h"
#include "OSCLib/Data/SDC/MDIB/VmdState.h"
#include "OSCLib/Data/SDC/MDIB/ChannelState.h"
#include "OSCLib/Data/SDC/MDIB/LimitAlertConditionDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/LimitAlertConditionState.h"
#include "OSCLib/Data/SDC/MDIB/LocationContextState.h"
#include "OSCLib/Data/SDC/MDIB/custom/MdibContainer.h"
#include "OSCLib/Data/SDC/MDIB/MdDescription.h"
#include "OSCLib/Data/SDC/MDIB/NumericMetricDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/NumericMetricState.h"
#include "OSCLib/Data/SDC/MDIB/NumericMetricValue.h"
#include "OSCLib/Data/SDC/MDIB/custom/OperationInvocationContext.h"

#include "OSCLib/Data/SDC/MDIB/PatientContextState.h"
#include "OSCLib/Data/SDC/MDIB/PatientContextDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/LocationContextDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/EnsembleContextDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/EnsembleContextState.h"
#include "OSCLib/Data/SDC/MDIB/OperatorContextDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/OperatorContextState.h"
#include "OSCLib/Data/SDC/MDIB/WorkflowContextDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/WorkflowContextState.h"
#include "OSCLib/Data/SDC/MDIB/MeansContextDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/MeansContextState.h"
#include "OSCLib/Data/SDC/MDIB/ScoDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/StringMetricDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/StringMetricState.h"
#include "OSCLib/Data/SDC/MDIB/StringMetricValue.h"
#include "OSCLib/Data/SDC/MDIB/RealTimeSampleArrayMetricState.h"
#include "OSCLib/Dev/DeviceCharacteristics.h"
#include "OSCLib/Util/Task.h"
#include "OSELib/DPWS/DPWS11Constants.h"
#include "OSELib/Helper/Message.h"
#include "OSELib/Helper/XercesDocumentWrapper.h"
#include "OSELib/Helper/XercesParserWrapper.h"
#include "OSELib/SDC/DefaultOSCPSchemaGrammarProvider.h"
#include "OSCLib/Data/SDC/SDCProviderAdapter.h"

namespace SDCLib {
namespace Data {
namespace SDC {

template<class T>
class SetNotification : public Poco::Notification {
public:
	SetNotification(const T & request, const OperationInvocationContext & oic) : request(request), oic(oic) {
	}

    const T request;
    const OperationInvocationContext oic;
};

class AsyncProviderInvoker : public Util::Task, OSELib::WithLogger {
public:
	AsyncProviderInvoker(SDCProvider & provider,
			Poco::NotificationQueue & queue) :
		WithLogger(OSELib::Log::sdcProvider),
		provider(provider),
		queue(queue){
    }

    void runImpl() {
		Poco::AutoPtr<Poco::Notification> pNf(queue.waitDequeueNotification(100));
		if (pNf) {
			try {
				if (auto n = dynamic_cast<const SetNotification<MDM::SetValue> *>(pNf.get())) {
					provider.SetValue(n->request, n->oic);
				}
				else if (auto n = dynamic_cast<const SetNotification<MDM::SetString> *>(pNf.get())) {
					provider.SetString(n->request, n->oic);
				}
				else if (auto n = dynamic_cast<const SetNotification<MDM::Activate> *>(pNf.get())) {
					provider.OnActivate(n->oic);
				}
				else if (auto n = dynamic_cast<const SetNotification<MDM::SetAlertState> *>(pNf.get())) {
					provider.SetAlertState(n->request, n->oic);
				}
				else if (auto n = dynamic_cast<const SetNotification<MDM::SetContextState> *>(pNf.get())) {
					provider.SetContextState(n->request, n->oic);
				}
				else {
					log_error([&] { return "Unknown invoke data type!"; });
				}
			}
			catch (...) {
				log_error([&] { return "Exception caught during async provider invoke."; });
			}
		}

		if (this->isInterrupted()) {
			return;
		}

		Poco::Mutex::ScopedLock lock(provider.getMutex());
		if (provider.periodicEventInterval < provider.lastPeriodicEvent.elapsed()) {
			provider.firePeriodicReportImpl(provider.getHandlesForPeriodicUpdate());
			provider.lastPeriodicEvent.update();
		}
    }

private:
    SDCProvider & provider;
    Poco::NotificationQueue & queue;
};

//template SDCProviderStateHandler<NumericMetricState>;


template<class StateType>
bool SDCProvider::isMetricChangeAllowed(const StateType & state, SDCProvider & provider) {
	typename StateType::DescriptorType descriptor;
	if (!provider.getMdDescription().findDescriptor(state.getDescriptorHandle(), descriptor)) {
		return false;
	}
	if (descriptor.getMetricCategory() == MetricCategory::Msrmt) {
		return false;
	}

	if (state.hasActivationState()) {
		return state.getActivationState() == ComponentActivation::On;
	} else {
		return true;
	}
}

SDCProvider::SDCProvider() :
	WithLogger(OSELib::Log::sdcProvider),
	providerInvoker(),
	periodicEventInterval(10, 0),
	configuration(MDPWSTransportLayerConfiguration()),
	runningState(false)
{
	atomicTransactionId.store(0);
	mdibVersion.store(0);
    setEndpointReference(Poco::UUIDGenerator::defaultGenerator().create().toString());
    m_mdDescription = std::unique_ptr<MdDescription>(new MdDescription());
	_adapter = std::unique_ptr<SDCProviderAdapter>(new SDCProviderAdapter(*this));
}

SDCProvider::~SDCProvider() {

}

unsigned int SDCProvider::incrementAndGetTransactionId() {
	const unsigned int result = ++atomicTransactionId;
	return result;
}

template<class T>
void SDCProvider::enqueueInvokeNotification(const T & request, const OperationInvocationContext & oic) {
	invokeQueue.enqueueNotification(new SetNotification<T>(request, oic));
}

MDM::SetValueResponse SDCProvider::SetValueAsync(const MDM::SetValue & request) {
	const OperationInvocationContext oic(request.OperationHandleRef(), incrementAndGetTransactionId());
	const std::string metricHandle(getMdDescription().getOperationTargetForOperationHandle(oic.operationHandle));

	auto genResponse = [this, &oic](InvocationState v) {
		notifyOperationInvoked(oic, v);
		// TODO: 0 = replace with real sequence ID
		MDM::SetValueResponse svr(MDM::InvocationInfo(oic.transactionId,MDM::InvocationState(ConvertToCDM::convert(v))),xml_schema::Uri("0"));
		svr.MdibVersion(getMdibVersion());

		return svr;
	};

	NumericMetricState nms;
	if (!getMdState().findState(metricHandle, nms)) {
		return genResponse(InvocationState::Fail);
	}
	if (!isMetricChangeAllowed(nms, *this)) {
		return genResponse(InvocationState::Fail);
	}

	enqueueInvokeNotification(request, oic);
	return genResponse(InvocationState::Wait);
}

void SDCProvider::SetValue(const MDM::SetValue & request, const OperationInvocationContext & oic) {
	const std::string metricHandle(getMdDescription().getOperationTargetForOperationHandle(oic.operationHandle));

	NumericMetricState nms;
	if (!getMdState().findState(metricHandle, nms)) {
		notifyOperationInvoked(oic, InvocationState::Fail);
		return;
	}
	if (!isMetricChangeAllowed(nms, *this)) {
		notifyOperationInvoked(oic, InvocationState::Fail);
		return;
	}
	nms.setMetricValue(nms.getMetricValue().setValue(request.RequestedNumericValue()));

	const InvocationState outState(onStateChangeRequest(nms, oic));
	notifyOperationInvoked(oic, outState);

	if (outState == InvocationState::Fin) {
		// Success
		updateState(nms);
	}
}

MDM::ActivateResponse SDCProvider::OnActivateAsync(const MDM::Activate & request) {
	const OperationInvocationContext oic(request.OperationHandleRef(), incrementAndGetTransactionId());
	notifyOperationInvoked(oic, InvocationState::Wait);
	enqueueInvokeNotification(request, oic);

	MDM::ActivateResponse ar(MDM::InvocationInfo(oic.transactionId,MDM::InvocationState(ConvertToCDM::convert(InvocationState::Wait))),xml_schema::Uri("0"));
	ar.MdibVersion(getMdibVersion());

	return ar;
}


void SDCProvider::OnActivate(const OperationInvocationContext & oic) {
	std::map<std::string, SDCProviderStateHandler *>::iterator it = stateHandlers.find(oic.operationHandle);
    if (it != stateHandlers.end()) {
    	if (SDCProviderActivateOperationHandler * handler = dynamic_cast<SDCProviderActivateOperationHandler *>(it->second)) {
    		const InvocationState isVal(handler->onActivateRequest(oic));
    		notifyOperationInvoked(oic, isVal);
    		return;
    	}
    }
    notifyOperationInvoked(oic, InvocationState::Fail);
}

MDM::SetStringResponse SDCProvider::SetStringAsync(const MDM::SetString & request) {
	const OperationInvocationContext oic(request.OperationHandleRef(), incrementAndGetTransactionId());
	const MdDescription mdd(getMdDescription());

	const std::string metricHandle(mdd.getOperationTargetForOperationHandle(oic.operationHandle));
	const MdState mdsc(getMdState());

	auto genResponse = [this, &oic](InvocationState v) {
		notifyOperationInvoked(oic, v);
		MDM::SetStringResponse ssr(MDM::InvocationInfo(oic.transactionId,MDM::InvocationState(ConvertToCDM::convert(v))),xml_schema::Uri("0"));
		ssr.MdibVersion(getMdibVersion());
		return ssr;
	};

	{
		StringMetricState sms;
		if (mdsc.findState(metricHandle, sms)) {
            if (!isMetricChangeAllowed(sms, *this)) {
                return genResponse(InvocationState::Fail);
            }
			enqueueInvokeNotification(request, oic);
			return genResponse(InvocationState::Wait);
		}
	}
	{
		EnumStringMetricState state;
		if (mdsc.findState(metricHandle, state)) {
			EnumStringMetricDescriptor descriptor;
			if (!mdd.findDescriptor(state.getDescriptorHandle(), descriptor)) {
				return genResponse(InvocationState::Fail);
			}
            if (!isMetricChangeAllowed(state, *this)) {
                return genResponse(InvocationState::Fail);
            }
			const std::vector<AllowedValue> allowedValues(descriptor.getAllowedValueList());

			const std::string & requestedStringVal(request.RequestedStringValue());
			std::vector<std::string> allowedValuesString;
			for (auto allowedValue : allowedValues) {
				allowedValuesString.push_back(allowedValue.getValue());
			}

			std::vector<std::string>::const_iterator it = std::find(allowedValuesString.begin(), allowedValuesString.end(), requestedStringVal);
			if (it == allowedValuesString.end()) {
				return genResponse(InvocationState::Fail);
			}
			enqueueInvokeNotification(request, oic);
			return genResponse(InvocationState::Wait);
		}
	}

	return genResponse(InvocationState::Fail);
}

template<class T>
void SDCProvider::SetStringImpl(const T & state, const OperationInvocationContext & oic) {

	const InvocationState outState(onStateChangeRequest(state, oic));
	notifyOperationInvoked(oic, outState);

	if (outState == InvocationState::Fin) {
		// Success
		updateState(state);
	}

}

void SDCProvider::SetString(const MDM::SetString & request, const OperationInvocationContext & oic) {

	const std::string & requestedStringVal(request.RequestedStringValue());

	const MdState mdsc(getMdState());
	const MdDescription mdd(getMdDescription());

	const std::string metricHandle(mdd.getOperationTargetForOperationHandle(oic.operationHandle));

	{
		StringMetricState sms;
		if (mdsc.findState(metricHandle, sms)) {
			sms.setMetricValue(sms.getMetricValue().setValue(requestedStringVal));
			SetStringImpl(sms, oic);
			return;
		}
	}
	{
		EnumStringMetricState state;
		if (mdsc.findState(metricHandle, state)) {
			EnumStringMetricDescriptor descriptor;
			if (!mdd.findDescriptor(state.getDescriptorHandle(), descriptor)) {
				notifyOperationInvoked(oic, InvocationState::Fail);
				return;
			}

			const std::vector<AllowedValue> allowedValues(descriptor.getAllowedValueList());
			std::vector<std::string> allowedValuesString;
			for (auto allowedValue : allowedValues) {
				allowedValuesString.push_back(allowedValue.getValue());
			}

			std::vector<std::string>::const_iterator it = std::find(allowedValuesString.begin(), allowedValuesString.end(), requestedStringVal);
			if (it == allowedValuesString.end()) {
				notifyOperationInvoked(oic, InvocationState::Fail);
			} else {
				StringMetricValue smv = state.getMetricValue();
				smv.setValue(it->data());
				state.setMetricValue(smv);
				SetStringImpl(state, oic);
			}

			return;
		}
	}

	notifyOperationInvoked(oic, InvocationState::Fail);
}


template<typename StateType>
void SDCProvider::SetAlertStateImpl(const StateType & state, const OperationInvocationContext & oic) {

	const MdDescription mdd(getMdDescription());

	typename StateType::DescriptorType descriptor;
	if (!mdd.findDescriptor(state.getDescriptorHandle(), descriptor)) {
		notifyOperationInvoked(oic, InvocationState::Fail);
		return;
	}

	const InvocationState outIS(onStateChangeRequest(state, oic));
	notifyOperationInvoked(oic, outIS);

	// Only signal changed value of metric, when invocation successful.
	if (outIS == InvocationState::Fin) {
		updateState(state);
    }
}

MDM::SetAlertStateResponse SDCProvider::SetAlertStateAsync(const MDM::SetAlertState & request) {
	const OperationInvocationContext oic(request.OperationHandleRef(), incrementAndGetTransactionId());
	const CDM::AbstractAlertState * incomingCDMState = &(request.ProposedAlertState());

	auto genResponse = [this, &oic](InvocationState v) {
		notifyOperationInvoked(oic, v);
		// TODO: 0 = replace with real sequence ID
		MDM::SetAlertStateResponse sasr(MDM::InvocationInfo(oic.transactionId,MDM::InvocationState(ConvertToCDM::convert(v))),xml_schema::Uri("0"));
		sasr.MdibVersion(getMdibVersion());

		return sasr;
	};

	const std::string targetHandle(getMdDescription().getOperationTargetForOperationHandle(oic.operationHandle));
	if (targetHandle.empty()) {
		return genResponse(InvocationState::Fail);
	}
	if ((incomingCDMState->DescriptorHandle() != targetHandle)) {
		return genResponse(InvocationState::Fail);
	}

	// Cast and convert, then call into user code
	if (dynamic_cast<const CDM::AlertConditionState *>(incomingCDMState)) {
		enqueueInvokeNotification(request, oic);
		return genResponse(InvocationState::Wait);
	} else if (dynamic_cast<const CDM::AlertSignalState *>(incomingCDMState)) {
		enqueueInvokeNotification(request, oic);
		return genResponse(InvocationState::Wait);
	} else if (dynamic_cast<const CDM::AlertSystemState *>(incomingCDMState)) {
		enqueueInvokeNotification(request, oic);
		return genResponse(InvocationState::Wait);
	} else if (dynamic_cast<const CDM::LimitAlertConditionState *>(incomingCDMState)) {
		enqueueInvokeNotification(request, oic);
		return genResponse(InvocationState::Wait);
	} else {
		// cast failed.
		return genResponse(InvocationState::Fail);
	}
}

void SDCProvider::SetAlertState(const MDM::SetAlertState & request, const OperationInvocationContext & oic) {

	const CDM::AbstractAlertState * incomingCDMState = &(request.ProposedAlertState());

	const std::string targetHandle(getMdDescription().getOperationTargetForOperationHandle(oic.operationHandle));
	if (targetHandle.empty()) {
		return notifyOperationInvoked(oic, InvocationState::Fail);
	}
	if ((incomingCDMState->DescriptorHandle() != targetHandle)) {
		return notifyOperationInvoked(oic, InvocationState::Fail);
	}

	// Cast and convert, then call into user code
	if (const CDM::AlertConditionState * state = dynamic_cast<const CDM::AlertConditionState *>(incomingCDMState)) {
		SetAlertStateImpl(ConvertFromCDM::convert(*state), oic);
	} else if (const CDM::AlertSignalState * state = dynamic_cast<const CDM::AlertSignalState *>(incomingCDMState)) {
		SetAlertStateImpl(ConvertFromCDM::convert(*state), oic);
	} else if (const CDM::AlertSystemState * state = dynamic_cast<const CDM::AlertSystemState *>(incomingCDMState)) {
		SetAlertStateImpl(ConvertFromCDM::convert(*state), oic);
	} else if (const CDM::LimitAlertConditionState * state = dynamic_cast<const CDM::LimitAlertConditionState *>(incomingCDMState)) {
		SetAlertStateImpl(ConvertFromCDM::convert(*state), oic);
	} else {
		// cast failed.
		notifyOperationInvoked(oic, InvocationState::Fail);
		return;
	}
}

MDM::GetMdStateResponse SDCProvider::GetMdState(const MDM::GetMdState & request) {
    auto sdmMdStates(ConvertToCDM::convert(getMdState()));

    if (request.HandleRef().empty()) {
    	//TODO: use real SequenceID, not 0
        return MDM::GetMdStateResponse(xml_schema::Uri("0"), std::move(sdmMdStates));
    } else {
    	CDM::MdState tmpmds;
    	const std::set<std::string> reqHandles(request.HandleRef().begin(), request.HandleRef().end());
    	for (const auto & nextState : sdmMdStates->State()) {
			if (reqHandles.find(nextState.DescriptorHandle()) != reqHandles.end()) {
				tmpmds.State().push_back(nextState);
			}
    	}
    	//TODO: use real SequenceID, not 0
    	return MDM::GetMdStateResponse(xml_schema::Uri("0"), tmpmds);
    }
}

MDM::GetContextStatesResponse SDCProvider::GetContextStates(const MDM::GetContextStates & request) {
	const auto states(ConvertToCDM::convert(getMdState()));

	//TODO: use real SequenceID, not 0
	MDM::GetContextStatesResponse contextStates(xml_schema::Uri("0"));
	for (const auto & state : states->State()) {
		if (const CDM::EnsembleContextState * s = dynamic_cast<const CDM::EnsembleContextState *>(&state)) {
			contextStates.ContextState().push_back(*s);
		} else if (const CDM::LocationContextState * s = dynamic_cast<const CDM::LocationContextState *>(&state)) {
			contextStates.ContextState().push_back(*s);
		} else if (const CDM::OperatorContextState * s = dynamic_cast<const CDM::OperatorContextState *>(&state)) {
			contextStates.ContextState().push_back(*s);
		} else if (const CDM::PatientContextState * s = dynamic_cast<const CDM::PatientContextState *>(&state)) {
			contextStates.ContextState().push_back(*s);
		} else if (const CDM::WorkflowContextState * s = dynamic_cast<const CDM::WorkflowContextState *>(&state)) {
			contextStates.ContextState().push_back(*s);
		}
	}

	if (request.HandleRef().empty()) {
		// TODO: 0 = replace with real sequence ID
		MDM::GetContextStatesResponse result(xml_schema::Uri("0"));
		result.MdibVersion(getMdibVersion());
		for (const auto & state : contextStates.ContextState()) {
			result.ContextState().push_back(state);
		}
		return result;
	} else {
		// TODO: 0 = replace with real sequence ID
		MDM::GetContextStatesResponse result(xml_schema::Uri("0"));
		result.MdibVersion(getMdibVersion());
		const std::set<std::string> reqHandles(request.HandleRef().begin(), request.HandleRef().end());
		for (const auto & state : contextStates.ContextState()) {
			if (reqHandles.find(state.DescriptorHandle()) != reqHandles.end()) {
				result.ContextState().push_back(state);
			}
		}
		return result;
	}
	// TODO: 0 = replace with real sequence ID
	MDM::GetContextStatesResponse result(xml_schema::Uri("0"));
	log_warning([&] { return "The requested states handle or descriptor handle does not fit any of the provider's states"; });
	result.MdibVersion(getMdibVersion());
	return result;
}

MDM::SetContextStateResponse SDCProvider::SetContextStateAsync(const MDM::SetContextState & request) {
	const OperationInvocationContext oic(request.OperationHandleRef(), incrementAndGetTransactionId());

	auto genResponse = [this, &oic](InvocationState v) {
		notifyOperationInvoked(oic, v);
		// TODO: 0 = replace with real sequence ID
		MDM::SetContextStateResponse scsr(MDM::InvocationInfo(oic.transactionId, ConvertToCDM::convert(v)),xml_schema::Uri("0"));
		scsr.MdibVersion(getMdibVersion());
		return scsr;
	};

	const std::string targetHandle(getMdDescription().getOperationTargetForOperationHandle(oic.operationHandle));
	if (targetHandle.empty()) {
		return genResponse(InvocationState::Fail);
	}

	enqueueInvokeNotification(request, oic);
	return genResponse(InvocationState::Wait);
}



template<class TState>
void SDCProvider::SetContextStateImpl(const TState & state,  const OperationInvocationContext & oic) {
	// FIXME: check if writing to context state is allowed! = SCO exists
	const InvocationState outState(onStateChangeRequest(state, oic));
	notifyOperationInvoked(oic, outState);
	if (outState == InvocationState::Fin) {
		// Success
		updateState(state);
	}
}

void SDCProvider::SetContextState(const MDM::SetContextState & request, const OperationInvocationContext & oic) {
	for (const auto & nextState : request.ProposedContextState()) {
		if (const CDM::PatientContextState * state = dynamic_cast<const CDM::PatientContextState *>(&nextState)) {
			SetContextStateImpl(ConvertFromCDM::convert(*state), oic);
		}
		else if (const CDM::LocationContextState * state = dynamic_cast<const CDM::LocationContextState *>(&nextState)) {
			SetContextStateImpl(ConvertFromCDM::convert(*state), oic);
		}
		else if (const CDM::EnsembleContextState * state = dynamic_cast<const CDM::EnsembleContextState *>(&nextState)) {
			SetContextStateImpl(ConvertFromCDM::convert(*state), oic);
		}
		else if (const CDM::OperatorContextState * state = dynamic_cast<const CDM::OperatorContextState *>(&nextState)) {
			SetContextStateImpl(ConvertFromCDM::convert(*state), oic);
		}
		else if (const CDM::WorkflowContextState * state = dynamic_cast<const CDM::WorkflowContextState *>(&nextState)) {
			SetContextStateImpl(ConvertFromCDM::convert(*state), oic);
		}
		else if (const CDM::MeansContextState * state = dynamic_cast<const CDM::MeansContextState *>(&nextState)) {
			SetContextStateImpl(ConvertFromCDM::convert(*state), oic);
		}
	}
}


MDM::GetMdibResponse SDCProvider::GetMdib(const MDM::GetMdib & ) {
	// TODO: 0 = replace with real sequence ID
	MDM::GetMdibResponse mdib(xml_schema::Uri("0"),ConvertToCDM::convert(getMdib()));
	mdib.MdibVersion(getMdibVersion());
    return mdib;
}

MDM::GetMdDescriptionResponse SDCProvider::GetMdDescription(const MDM::GetMdDescription & request) {
	auto cdmMdd(ConvertToCDM::convert(getMdDescription()));

	if (request.HandleRef().empty()) {
		// TODO: 0 = replace with real sequence ID
		MDM::GetMdDescriptionResponse mddr(xml_schema::Uri("0"),std::move(cdmMdd));
		mddr.MdibVersion(getMdibVersion());
		return mddr;
	} else {
		CDM::MdDescription filteredDescription;
		for (const auto & mds : cdmMdd->Mds()) {
			if (std::find(request.HandleRef().begin(), request.HandleRef().end(), mds.Handle()) != request.HandleRef().end()) {
				filteredDescription.Mds().push_back(mds);
			}
		}
		MDM::GetMdDescriptionResponse gmddr(xml_schema::Uri("0"),filteredDescription);
		gmddr.MdibVersion(getMdibVersion());

		return gmddr;
	}
}

void SDCProvider::updateState(const AlertSystemState & object) {
	notifyAlertEventImpl(object);
}

void SDCProvider::updateState(const AlertSignalState & object) {
	notifyAlertEventImpl(object);
}

void SDCProvider::updateState(const AlertConditionState & object) {
	notifyAlertEventImpl(object);
}

void SDCProvider::updateState(const EnumStringMetricState & object) {
	evaluateAlertConditions(object.getDescriptorHandle());
	notifyEpisodicMetricImpl(object);
}

void SDCProvider::updateState(const LimitAlertConditionState & object) {
	notifyAlertEventImpl(object);
}

void SDCProvider::updateState(const NumericMetricState & object) {
	evaluateAlertConditions(object.getDescriptorHandle());
	notifyEpisodicMetricImpl(object);
}


void SDCProvider::updateState(const StringMetricState & object) {
	evaluateAlertConditions(object.getDescriptorHandle());
	notifyEpisodicMetricImpl(object);
}


// regarding to the given standard the DSAMS is implemented as an TCP transported Metric
void SDCProvider::updateState(const DistributionSampleArrayMetricState & object) {
	evaluateAlertConditions(object.getDescriptorHandle());
	notifyEpisodicMetricImpl(object);
}

// UDP metrices
void SDCProvider::updateState(const RealTimeSampleArrayMetricState & object) {
	evaluateAlertConditions(object.getDescriptorHandle());
	notifyStreamMetricImpl(object);
}



// context states
void SDCProvider::updateState(const EnsembleContextState & object) {
	notifyContextEventImpl(object);
}

void SDCProvider::updateState(const MeansContextState & object) {
	notifyContextEventImpl(object);
}

void SDCProvider::updateState(const LocationContextState & object) {
	notifyContextEventImpl(object);
}

void SDCProvider::updateState(const WorkflowContextState & object) {
	notifyContextEventImpl(object);
}

void SDCProvider::updateState(const PatientContextState & object) {
	notifyContextEventImpl(object);
}

void SDCProvider::updateState(const OperatorContextState & object) {
	notifyContextEventImpl(object);
}

template<class T> void SDCProvider::notifyAlertEventImpl(const T & object) {
	if (object.getDescriptorHandle().empty()) {
		log_error([&] { return "State's descriptor handle is empty, event will not be fired!"; });
		return;
	}

	incrementMDIBVersion();
	replaceState(object);
	
	// TODO: replace sequence id
	MDM::EpisodicAlertReport report(xml_schema::Uri("0"));

	MDM::ReportPart3 reportPart;
	reportPart.AlertState().push_back(ConvertToCDM::convert(object));
	report.ReportPart().push_back(reportPart);

	_adapter->notifyEvent(report);
}

template<class T> void SDCProvider::notifyContextEventImpl(const T & object) {
	if (object.getDescriptorHandle().empty()) {
		log_error([&] { return "State's descriptor handle is empty, event will not be fired!"; });
		return;
	}

	incrementMDIBVersion();
	replaceState(object);

	MDM::ReportPart reportPart;

	reportPart.ContextState().push_back(ConvertToCDM::convert(object));

	MDM::EpisodicContextReport report(xml_schema::Uri("0"));
	report.ReportPart().push_back(reportPart);

	_adapter->notifyEvent(report);
}

template<class T>
void SDCProvider::notifyEpisodicMetricImpl(const T & object) {
	if (object.getDescriptorHandle().empty()) {
		log_error([&] { return "State's descriptor handle is empty, event will not be fired!"; });
		return;
	}

	incrementMDIBVersion();
	replaceState(object);

	MDM::ReportPart1 reportPart;
	reportPart.MetricState().push_back(ConvertToCDM::convert(object));

	// TODO: replace sequence id
	MDM::EpisodicMetricReport report(xml_schema::Uri("0"));
	report.ReportPart().push_back(reportPart);

	_adapter->notifyEvent(report);
}

template<class T>
void SDCProvider::notifyStreamMetricImpl(const T & object) {
	if (object.getDescriptorHandle().empty()) {
		log_error([&] { return "State's descriptor handle is empty, event will not be fired!"; });
		return;
	}

	incrementMDIBVersion();
	replaceState(object);

	// TODO: replace sequence id
	MDM::WaveformStream waveformStream(xml_schema::Uri("0"));

	//waveformStream.State().at(0).
	waveformStream.State().push_back(ConvertToCDM::convert(object));

	_adapter->notifyEvent(waveformStream);

}



void SDCProvider::firePeriodicReportImpl(const std::vector<std::string> & handles) {
	if (handles.empty()) {
		log_debug([&] { return "List of handles is empty, event will not be fired!"; });
		return;
	}

	Poco::Mutex::ScopedLock lock(mutex);
	const auto mdstate(ConvertToCDM::convert(getMdState()));

	MDM::ReportPart3 periodicAlertReportPart;
	MDM::ReportPart periodicContextChangedReportPart;
	MDM::ReportPart1 periodicMetricReportPart;

	for(const auto & state: mdstate->State()) {
		if (std::find(handles.begin(), handles.end(), state.DescriptorHandle()) != handles.end()) {
			if (auto castedState = dynamic_cast<const CDM::AbstractAlertState *>(&state)) {
				periodicAlertReportPart.AlertState().push_back(*castedState);
			}
			if (auto castedState = dynamic_cast<const CDM::AbstractContextState *>(&state)) {
				periodicContextChangedReportPart.ContextState().push_back(*castedState);
			}
			if (auto castedState = dynamic_cast<const CDM::AbstractMetricState *>(&state)) {
				periodicMetricReportPart.MetricState().push_back(*castedState);
			}
		}
	}

	// TODO: replace sequence id s
	MDM::PeriodicAlertReport periodicAlertReport(xml_schema::Uri("0"));
	periodicAlertReport.MdibVersion(getMdibVersion());
	periodicAlertReport.ReportPart().push_back(periodicAlertReportPart);
	_adapter->notifyEvent(periodicAlertReport);

	MDM::PeriodicContextReport periodicContextReport(xml_schema::Uri("0"));
	periodicContextReport.MdibVersion(getMdibVersion());
	periodicContextReport.ReportPart().push_back(periodicContextChangedReportPart);
	_adapter->notifyEvent(periodicContextReport);

	MDM::PeriodicMetricReport periodicMetricReport(xml_schema::Uri("0"));
	periodicMetricReport.ReportPart().push_back(periodicMetricReportPart);
	_adapter->notifyEvent(periodicMetricReport);
}

void SDCProvider::setAlertConditionPresence(const std::string & alertConditionHandle, bool conditionPresence, const OperationInvocationContext & oic) {
	const MdState mdsc(getMdState());

	// Modify alert condition's presence
	AlertConditionState condState;
	LimitAlertConditionState limitCondState;
	if (mdsc.findState(alertConditionHandle, condState)) {
		condState.setPresence(conditionPresence);
		const InvocationState is(onStateChangeRequest(condState, oic));
		if (is == InvocationState::Fin) {
			// Success
			updateState(condState);
		}
	} else if (mdsc.findState(alertConditionHandle, limitCondState)) {
		limitCondState.setPresence(conditionPresence);
		const InvocationState is(onStateChangeRequest(limitCondState, oic));
		if (is == InvocationState::Fin) {
			// Success
			updateState(limitCondState);
		}
	} else {
		return;
	}

	// Search all alert signal descriptors with reference to the alert condition descriptor handle
	const std::vector<AlertSignalDescriptor> asds(getMdDescription().collectAllAlertSignalDescriptors());
	std::map<std::string, AlertSignalDescriptor> matchingDescriptors;
	std::list<AlertSignalDescriptor> listDescriptors;

	for (const auto & descriptor : asds) {
		if (descriptor.getConditionSignaled() == alertConditionHandle) {
			matchingDescriptors.insert( std::map<std::string, AlertSignalDescriptor>::value_type ( descriptor.getHandle(), descriptor) );
		}
	}

	// Check and modify all found alert signal states, call into state handler, if needed
	auto informUser = [this, &oic](AlertSignalState & state, const AlertSignalPresence & presence) {
		state.setPresence(presence);
		const InvocationState is(onStateChangeRequest(state, oic));
		if (is == InvocationState::Fin) {
			updateState(state);
		}
	};

	// Search all alert signal states
	std::vector<AlertSignalState> ass(mdsc.findAlertSignalStates());
	for (auto & nextState : ass) {
		auto descriptor = matchingDescriptors.find(nextState.getDescriptorHandle());
		if (descriptor == matchingDescriptors.end()) {
			continue;
		}

		if (conditionPresence) {
			informUser(nextState, AlertSignalPresence::On);
		} else {
			if (!nextState.hasPresence()) {
				informUser(nextState, AlertSignalPresence::Off);
			}
			if (descriptor->second.getLatching()) {
				switch (nextState.getPresence()) {
					case AlertSignalPresence::On: informUser(nextState, AlertSignalPresence::Latch); break;
					case AlertSignalPresence::Off: break;
					case AlertSignalPresence::Ack: break;
					case AlertSignalPresence::Latch: break;
				}
			} else {
				informUser(nextState, AlertSignalPresence::Off);
			}
		}
	}
}

void SDCProvider::evaluateAlertConditions(const std::string & source) {
	Poco::Mutex::ScopedLock lock(mutex);
	const MdDescription description(getMdDescription());

	std::vector<std::string> relevantDescriptors;
	for (const auto & alertCondition : description.collectAllAlertConditionDescriptors()) {
		const auto sources(alertCondition.getSourceList());
		if (std::find(sources.begin(), sources.end(), source) != sources.end()) {
			relevantDescriptors.push_back(alertCondition.getHandle());
		}
	}
	for (const auto & limitAlertCondition : description.collectAllLimitAlertConditionDescriptors()) {
		const auto sources(limitAlertCondition.getSourceList());
		if (std::find(sources.begin(), sources.end(), source) != sources.end()) {
			relevantDescriptors.push_back(limitAlertCondition.getHandle());
		}
	}

	if (relevantDescriptors.empty()) {
		return;
	}

	for (const auto & handler : stateHandlers) {
		if (SDCProviderAlertConditionStateHandler<AlertConditionState> * h = dynamic_cast<SDCProviderAlertConditionStateHandler<AlertConditionState> *>(handler.second)) {
			if (std::find(relevantDescriptors.begin(), relevantDescriptors.end(), h->getDescriptorHandle()) != relevantDescriptors.end()) {
				h->sourceHasChanged(source);
			}
		} else if (SDCProviderAlertConditionStateHandler<LimitAlertConditionState> * h = dynamic_cast<SDCProviderAlertConditionStateHandler<LimitAlertConditionState> *>(handler.second)) {
			if (std::find(relevantDescriptors.begin(), relevantDescriptors.end(), h->getDescriptorHandle()) != relevantDescriptors.end()) {
				h->sourceHasChanged(source);
			}
		}
	}
}

MdibContainer SDCProvider::getMdib() {
    MdibContainer container;
	Poco::Mutex::ScopedLock lock(mutex);
    container.setMdDescription(getMdDescription());
	container.setMdState(getMdState());
	container.setMdibVersion(getMdibVersion());
    return container;
}

MdDescription SDCProvider::getMdDescription() const {
	return *m_mdDescription;
}

MdState SDCProvider::getMdState() {
	Poco::Mutex::ScopedLock lock(mutex);
	return mdibStates;
}


void SDCProvider::startup() {
	try {
		log_notice([&] { return "Starting adapter on interface: " + configuration.getBindAddress().toString() + " and port: " + std::to_string(configuration.getPort()) + ". ";});
		_adapter->start(configuration);
	} catch (const Poco::Net::NetException & e) {
		log_error([&] { return "Net Exception: " + std::string(e.what()) + " Socket unable to be opened. Provider startup aborted.";});
		return;
	} catch (std::runtime_error & ex_re) {
		log_error([&] { return ex_re.what(); });
		return;
	}

    // Grab all states (start with all operation states and add states from user handlers)
    Poco::Mutex::ScopedLock lock(mutex);
    mdibStates = MdState(operationStates);
    for (const auto & handler : stateHandlers) {
		if (SDCProviderAlertConditionStateHandler<AlertConditionState> * h = dynamic_cast<SDCProviderAlertConditionStateHandler<AlertConditionState> *>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else if (SDCProviderMDStateHandler<AlertSignalState> * h = dynamic_cast<SDCProviderMDStateHandler<AlertSignalState> *>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else if (SDCProviderMDStateHandler<EnumStringMetricState> * h = dynamic_cast<SDCProviderMDStateHandler<EnumStringMetricState> *>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else if (SDCProviderMDStateHandler<AlertSystemState> * h = dynamic_cast<SDCProviderMDStateHandler<AlertSystemState> *>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else if (SDCProviderAlertConditionStateHandler<LimitAlertConditionState> * h = dynamic_cast<SDCProviderAlertConditionStateHandler<LimitAlertConditionState> *>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else if (SDCProviderMDStateHandler<NumericMetricState> * h = dynamic_cast<SDCProviderMDStateHandler<NumericMetricState> *>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else if (SDCProviderMDStateHandler<RealTimeSampleArrayMetricState> * h = dynamic_cast<SDCProviderMDStateHandler<RealTimeSampleArrayMetricState> *>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else if (SDCProviderMDStateHandler<DistributionSampleArrayMetricState> * h = dynamic_cast<SDCProviderMDStateHandler<DistributionSampleArrayMetricState> *>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else if (SDCProviderMDStateHandler<StringMetricState> * h = dynamic_cast<SDCProviderMDStateHandler<StringMetricState> *>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else if (SDCProviderComponentStateHandler<ClockState> * h = dynamic_cast<SDCProviderComponentStateHandler<ClockState> *>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else if (SDCProviderComponentStateHandler<MdsState> * h = dynamic_cast<SDCProviderComponentStateHandler<MdsState>*>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else if (SDCProviderComponentStateHandler<VmdState> * h = dynamic_cast<SDCProviderComponentStateHandler<VmdState> *>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else if (SDCProviderComponentStateHandler<ChannelState>  * h = dynamic_cast<SDCProviderComponentStateHandler<ChannelState>*>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else if (dynamic_cast<SDCProviderActivateOperationHandler *>(handler.second)) {
			// NOOP
			// well I gess not...
		} else if (SDCProviderMDStateHandler<LocationContextState>  * h = dynamic_cast<SDCProviderMDStateHandler<LocationContextState>*>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else if (SDCProviderMDStateHandler<PatientContextState>  * h = dynamic_cast<SDCProviderMDStateHandler<PatientContextState>*>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else if (SDCProviderMDStateHandler<MeansContextState>  * h = dynamic_cast<SDCProviderMDStateHandler<MeansContextState>*>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else if (SDCProviderMDStateHandler<WorkflowContextState>  * h = dynamic_cast<SDCProviderMDStateHandler<WorkflowContextState>*>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else if (SDCProviderMDStateHandler<OperatorContextState>  * h = dynamic_cast<SDCProviderMDStateHandler<OperatorContextState>*>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else if (SDCProviderMDStateHandler<EnsembleContextState>  * h = dynamic_cast<SDCProviderMDStateHandler<EnsembleContextState>*>(handler.second)) {
			mdibStates.addState(h->getInitialState());
		} else {
    		log_fatal([&] { return "Unknown handler type! This is an implementation error in the OSCLib!"; });
    		exit(1);
   		}
	}
    providerInvoker.reset(new AsyncProviderInvoker(*this, invokeQueue));
    providerInvoker->start();
    // Validation
    {
		const xml_schema::Flags xercesFlags(xml_schema::Flags::dont_validate | xml_schema::Flags::no_xml_declaration | xml_schema::Flags::dont_initialize);
		std::ostringstream xml;
		xml_schema::NamespaceInfomap map;
		CDM::MdibContainer(xml, *ConvertToCDM::convert(getMdib()), map, OSELib::XML_ENCODING, xercesFlags);

		OSELib::SDC::DefaultOSCPSchemaGrammarProvider grammarProvider;
		auto rawMessage = OSELib::Helper::Message::create(xml.str());
		auto xercesDocument = OSELib::Helper::XercesDocumentWrapper::create(*rawMessage, grammarProvider);
		std::unique_ptr<CDM::Mdib> result(CDM::MdibContainer(xercesDocument->getDocument()));
		if (result == nullptr) {
			log_fatal([&] { return "Fatal error, can't create MDIB - schema validation error! Offending MDIB: \n" + xml.str(); });
			std::exit(1);
		}
    }
    runningState = true;
}

void SDCProvider::shutdown() {
    if (providerInvoker) {
    	providerInvoker->interrupt();
    	providerInvoker.reset();
    }
	if (_adapter) {
		_adapter->stop();
		_adapter.reset();
	}
	runningState = false;
	Poco::Mutex::ScopedLock unlock(mutex);
}

bool SDCProvider::isRunning() {
	return runningState;
}


template<class T> void SDCProvider::replaceState(const T & object) {
    Poco::Mutex::ScopedLock lock(mutex);
    // Check for existing state
    std::unique_ptr<CDM::MdState> cachedStates(ConvertToCDM::convert(mdibStates));
    CDM::MdState target;
    for (const auto & state : cachedStates->State()) {
        if (state.DescriptorHandle() == object.getDescriptorHandle()) {
            // Found, add parameter instead of current
            target.State().push_back(*ConvertToCDM::convert(object));
            continue;
        }
        // Copy
        target.State().push_back(state);
    }
    mdibStates = ConvertFromCDM::convert(target);
}


void SDCProvider::addMdStateHandler(SDCProviderStateHandler * handler) {
    handler->parentProvider = this;


    // TODO: Multistates implementation.. regarding to the SDC standard it should be possible to define states with the same descriptor handle!
    // this is only possible for context states. maybe do a type check.
    // but before implementing -> check the whole frameworks mechanics
    if (stateHandlers.find(handler->getDescriptorHandle()) != stateHandlers.end()) {
    	log_error([&] { return "A SDCProvider handler for handle " + handler->getDescriptorHandle() + " already exists. It will be overridden."; });
    }



    // ToDo: Behold! check with simpleOSCP if this really works°!
    if (auto activate_handler = dynamic_cast<SDCProviderActivateOperationHandler *>(handler)) {
    	const MdDescription mddescription(getMdDescription());
    	for (const auto & mds : mddescription.collectAllMdsDescriptors()) {
    		if (!mds.hasSco()) {
    			continue;
    		}

    		const auto sco(ConvertToCDM::convert(mds.getSco()));
    		for (const auto & operation : sco->Operation()) {
    			if (operation.Handle() == activate_handler->getDescriptorHandle()
    					&& nullptr != dynamic_cast<const CDM::ActivateOperationDescriptor *>(&operation)) {
    				stateHandlers[handler->getDescriptorHandle()] = handler;
    				return;
    			}
    		}
    	}
    	log_error([&] { return "Could not add handler because no ActivateOperationDescriptor with matching handle was found."; });
    }


    // TODO: Move streaming service to service controller
    // add DistributionArray...
    else if (auto streamHandler = dynamic_cast<SDCProviderMDStateHandler<RealTimeSampleArrayMetricState> *>(handler)) {
    	//int port = SDCLibrary::getInstance().extractFreePort();
//    	_adapter->addStreamingPort(4444);
    	// FIXME: delete after testing that streaming works on more than one address!
    	_adapter->addStreamingPort(5555);


    	stateHandlers[handler->getDescriptorHandle()] = handler;
    }
    else {
		stateHandlers[handler->getDescriptorHandle()] = handler;
	}
}


void SDCProvider::removeMDStateHandler(SDCProviderStateHandler * handler) {
    stateHandlers.erase(handler->getDescriptorHandle());
}

void SDCProvider::setEndpointReference(const std::string & epr) {
	this->endpointReference = epr;
}


void SDCProvider::setMdDescription(const MdDescription & mdDescription) {
	Poco::Mutex::ScopedLock lock(getMutex());
	m_mdDescription = std::make_shared<MdDescription>(mdDescription);
}

void SDCProvider::setMdDescription(std::string xml) {
	OSELib::SDC::DefaultOSCPSchemaGrammarProvider grammarProvider;
	auto rawMessage = OSELib::Helper::Message::create(xml);
	auto xercesDocument = OSELib::Helper::XercesDocumentWrapper::create(*rawMessage, grammarProvider);

	std::unique_ptr<CDM::Mdib> result(CDM::MdibContainer(xercesDocument->getDocument()));

	if (result != nullptr) {
		Poco::Mutex::ScopedLock lock(getMutex());
		if (result->MdDescription().present()) {
			this->m_mdDescription.reset(new MdDescription(ConvertFromCDM::convert(result->MdDescription().get())));
		}

	} else {
		log_fatal([&] { return " Fatal error, can't create MDIB - schema validation error! Offending MDIB: \n" + xml; });
		//todo: proper exeption handling
		std::exit(1);
	}

}



const std::string SDCProvider::getEndpointReference() const {
	return endpointReference;
}

template<typename T> InvocationState SDCProvider::onStateChangeRequest(const T & state, const OperationInvocationContext & oic) {
	std::map<std::string, SDCProviderStateHandler *>::iterator it(stateHandlers.find(state.getDescriptorHandle()));
    if (it != stateHandlers.end()) {
    	if (SDCProviderMDStateHandler<T> * handler = dynamic_cast<SDCProviderMDStateHandler<T> *>(it->second)) {
        	return handler->onStateChangeRequest(state, oic);
    	}
    }
    return InvocationState::Fail;
}

void SDCProvider::notifyOperationInvoked(const OperationInvocationContext & oic, Data::SDC::InvocationState is) {
	if (oic.transactionId == 0 && oic.operationHandle.empty())
		return;

	// todo: replace IstanceIdentifier by real value
	MDM::ReportPart5 oirPart(MDM::InvocationInfo(oic.transactionId, ConvertToCDM::convert(is)), CDM::InstanceIdentifier(), oic.operationHandle);
	// todo: replace sequence id
	MDM::OperationInvokedReport oir(xml_schema::Uri("0"));
	oir.MdibVersion(getMdibVersion());
	oir.ReportPart().push_back(oirPart);

	_adapter->notifyEvent(oir);
}

template<class T>
void SDCProvider::addSetOperationToSCOObjectImpl(const T & source, MdsDescriptor & ownerMDS) {
	// get sco object or create new
	std::unique_ptr<CDM::ScoDescriptor> scoDescriptor(Defaults::ScoDescriptorInit(xml_schema::Uri("")));
	if (ownerMDS.hasSco()) {
		scoDescriptor = ConvertToCDM::convert(ownerMDS.getSco());
	} else {
		// only set handle if previously created
		scoDescriptor->Handle(ownerMDS.getHandle() + "_sco");
	}

	// add operation descriptor to sco and write back to mds
	scoDescriptor->Operation().push_back(source);
	ownerMDS.setSco(ConvertFromCDM::convert(*scoDescriptor));

	Poco::Mutex::ScopedLock lock(mutex);

	// Now add a state object for the sco descriptor to the cached states.
	std::unique_ptr<CDM::MdState> cachedOperationStates(ConvertToCDM::convert(operationStates));
	bool existingOperationStateFound(false);
	for (const auto & state : cachedOperationStates->State()) {
		if (state.DescriptorHandle() == source.Handle()) {
			if (dynamic_cast<const CDM::AbstractOperationState *>(&state)) {

				existingOperationStateFound = true;
				break;
			}
		}
	}

	if (existingOperationStateFound) {
		// NOOP
		return;
	} else {
		// add new operation state
		if (dynamic_cast<const CDM::SetValueOperationDescriptor *>(&source)) {
			CDM::SetValueOperationState operationState(
					source.Handle(),
					CDM::OperatingMode::En);
			cachedOperationStates->State().push_back(operationState);
		}
		else if (dynamic_cast<const CDM::SetStringOperationDescriptor *>(&source)) {
			CDM::SetStringOperationState operationState(
					source.Handle(),
					CDM::OperatingMode::En);
			cachedOperationStates->State().push_back(operationState);
		} else if (dynamic_cast<const CDM::SetAlertStateOperationDescriptor *>(&source)) {
			CDM::SetAlertStateOperationState operationState(
					source.Handle(),
					CDM::OperatingMode::En);
			cachedOperationStates->State().push_back(operationState);
		} else if (dynamic_cast<const CDM::SetComponentStateOperationDescriptor*>(&source)) {
			CDM::SetComponentStateOperationState operationState(
					source.Handle(),
					CDM::OperatingMode::En);
			cachedOperationStates->State().push_back(operationState);
		} else if (dynamic_cast<const CDM::SetContextStateOperationDescriptor *>(&source)) {
			CDM::SetContextStateOperationState operationState(
					source.Handle(),
					CDM::OperatingMode::En);
			cachedOperationStates->State().push_back(operationState);
		} else if (dynamic_cast<const CDM::SetMetricStateOperationDescriptor *>(&source)) {
			CDM::SetMetricStateOperationState operationState(
					source.Handle(),
					CDM::OperatingMode::En);
			cachedOperationStates->State().push_back(operationState);
		}

		// replace cached states by update.
		operationStates = ConvertFromCDM::convert(*cachedOperationStates);
	}
}

void SDCProvider::addActivateOperationForDescriptor(const ActivateOperationDescriptor & descriptor, MdsDescriptor & ownerMDS) {
	addSetOperationToSCOObjectImpl(*ConvertToCDM::convert(descriptor), ownerMDS);
}

void SDCProvider::createSetOperationForDescriptor(const AlertConditionDescriptor & descriptor, MdsDescriptor & ownerMDS) {
	const CDM::SetAlertStateOperationDescriptor setOperation(descriptor.getHandle() + "_sco", descriptor.getHandle());
	addSetOperationToSCOObjectImpl(setOperation, ownerMDS);
}

void SDCProvider::createSetOperationForDescriptor(const AlertSignalDescriptor & descriptor, MdsDescriptor & ownerMDS) {
	const CDM::SetAlertStateOperationDescriptor setOperation(descriptor.getHandle() + "_sco", descriptor.getHandle());
	addSetOperationToSCOObjectImpl(setOperation, ownerMDS);
}

void SDCProvider::createSetOperationForDescriptor(const AlertSystemDescriptor & descriptor, MdsDescriptor & ownerMDS) {
	const CDM::SetAlertStateOperationDescriptor setOperation(descriptor.getHandle() + "_sco", descriptor.getHandle());
	addSetOperationToSCOObjectImpl(setOperation, ownerMDS);
}

void SDCProvider::createSetOperationForDescriptor(const EnumStringMetricDescriptor & descriptor, MdsDescriptor & ownerMDS) {
	const CDM::SetStringOperationDescriptor setOperation(descriptor.getHandle() + "_sco", descriptor.getHandle());
	addSetOperationToSCOObjectImpl(setOperation, ownerMDS);
}

void SDCProvider::createSetOperationForDescriptor(const LimitAlertConditionDescriptor & descriptor, MdsDescriptor & ownerMDS) {
	const CDM::SetAlertStateOperationDescriptor setOperation(descriptor.getHandle() + "_sco", descriptor.getHandle());
	addSetOperationToSCOObjectImpl(setOperation, ownerMDS);
}

void SDCProvider::createSetOperationForDescriptor(const NumericMetricDescriptor & descriptor, MdsDescriptor & ownerMDS) {
	const CDM::SetValueOperationDescriptor setOperation(descriptor.getHandle() + "_sco", descriptor.getHandle());
	addSetOperationToSCOObjectImpl(setOperation, ownerMDS);
}

void SDCProvider::createSetOperationForDescriptor(const StringMetricDescriptor & descriptor, MdsDescriptor & ownerMDS) {
	const CDM::SetStringOperationDescriptor setOperation(descriptor.getHandle() + "_sco", descriptor.getHandle());
	addSetOperationToSCOObjectImpl(setOperation, ownerMDS);
}

template<class T>
void SDCProvider::createSetOperationForContextDescriptor(const T & descriptor, MdsDescriptor & ownerMDS) {
	const CDM::SetContextStateOperationDescriptor setOperation(descriptor.getHandle() + "_sco", descriptor.getHandle());
	addSetOperationToSCOObjectImpl(setOperation, ownerMDS);
}

void SDCProvider::createSetOperationForDescriptor(const PatientContextDescriptor & descriptor, MdsDescriptor & ownerMDS) {
	createSetOperationForContextDescriptor(descriptor, ownerMDS);
}

void SDCProvider::createSetOperationForDescriptor(const LocationContextDescriptor & descriptor, MdsDescriptor & ownerMDS) {
	createSetOperationForContextDescriptor(descriptor, ownerMDS);
}

void SDCProvider::createSetOperationForDescriptor(const EnsembleContextDescriptor & descriptor, MdsDescriptor & ownerMDS) {
	createSetOperationForContextDescriptor(descriptor, ownerMDS);
}

void SDCProvider::createSetOperationForDescriptor(const OperatorContextDescriptor & descriptor, MdsDescriptor & ownerMDS) {
	createSetOperationForContextDescriptor(descriptor, ownerMDS);
}

void SDCProvider::createSetOperationForDescriptor(const WorkflowContextDescriptor & descriptor, MdsDescriptor & ownerMDS) {
	createSetOperationForContextDescriptor(descriptor, ownerMDS);
}

const Dev::DeviceCharacteristics& SDCProvider::getDeviceCharacteristics() const {
	return devicecharacteristics;
}

void SDCProvider::setDeviceCharacteristics(const Dev::DeviceCharacteristics& deviceCharacteristics) {
	this->devicecharacteristics = deviceCharacteristics;
}

unsigned long long int SDCProvider::getMdibVersion() const {
	return mdibVersion;
}

void SDCProvider::incrementMDIBVersion() {
	mdibVersion++;
}

Poco::Mutex & SDCProvider::getMutex() {
	return mutex;
}

void SDCProvider::setPeriodicEventInterval(const int seconds, const int milliseconds) {
	const Poco::Timespan interval (seconds, milliseconds);
	if (interval > Poco::Timespan(0, 250)) {
		Poco::Mutex::ScopedLock lock(mutex);
		periodicEventInterval = interval;
	}
}

std::vector<std::string> SDCProvider::getHandlesForPeriodicUpdate() {
	Poco::Mutex::ScopedLock lock(mutex);
	return handlesForPeriodicUpdates;
}

void SDCProvider::addHandleForPeriodicEvent(const std::string & handle) {
	Poco::Mutex::ScopedLock lock(mutex);
	handlesForPeriodicUpdates.push_back(handle);
}

void SDCProvider::removeHandleForPeriodicEvent(const std::string & handle) {
	Poco::Mutex::ScopedLock lock(mutex);
	auto iterator = std::find(handlesForPeriodicUpdates.begin(), handlesForPeriodicUpdates.end(), handle);
	if (iterator != handlesForPeriodicUpdates.end()) {
		handlesForPeriodicUpdates.erase(iterator);
	}
}


void SDCProvider::setConfiguration(MDPWSTransportLayerConfiguration config) {
	configuration = config;
}

} /* namespace SDC */
} /* namespace Data */
} /* namespace SDCLib */
