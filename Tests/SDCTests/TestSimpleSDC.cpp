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
 * TestSimpleSDC.cpp
 *
 *  @Copyright (C) 2017, SurgiTAIX AG
 *  Author: besting, buerger, roehser
 *
 *	This unit test covers the basic functionality of the SDC library
 *
 *	1. 3 different kinds of Metrices (numeric, string, enum) are tested regarding
 *	- eventing
 *	- requesting
 *	- settability
 *	2. An alert system is implemented. Automatic alerting is evaluated:
 *	- the referenced alert state () is changed (attribute presence is changed)
 *	- one metric is increased over time (handle_cur). The value is evaluated by a state handler (LimitAlertConditionStateHandler) referencing the handle_alert_condition.
 *		If the value is out of bound an alert is triggered. The logic is implemented in the user code! Only the defined limits are taken from the descriptor.
 *
 */





#include "OSCLib/SDCLibrary.h"
#include "OSCLib/Data/SDC/SDCConsumer.h"
#include "OSCLib/Data/SDC/FutureInvocationState.h"
#include "OSCLib/Data/SDC/SDCProvider.h"
#include "OSCLib/Data/SDC/SDCProviderMDStateHandler.h"
#include "OSCLib/Data/SDC/SDCProviderAlertConditionStateHandler.h"
#include "OSCLib/Data/SDC/SDCProviderActivateOperationHandler.h"
#include "OSCLib/Data/SDC/SDCProviderComponentStateHandler.h"
#include "OSCLib/Data/SDC/SDCConsumerMDStateHandler.h"
#include "OSCLib/Data/SDC/MDIB/ActivateOperationDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/AllowedValue.h"
#include "OSCLib/Data/SDC/MDIB/LimitAlertConditionDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/LimitAlertConditionState.h"
#include "OSCLib/Data/SDC/MDIB/AlertSignalDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/AlertSignalState.h"
#include "OSCLib/Data/SDC/MDIB/AlertSystemDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/AlertSystemState.h"
#include "OSCLib/Data/SDC/MDIB/ChannelDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/ChannelState.h"
#include "OSCLib/Data/SDC/MDIB/CodedValue.h"
#include "OSCLib/Data/SDC/MDIB/EnumStringMetricDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/EnumStringMetricState.h"
#include "OSCLib/Data/SDC/MDIB/MdsDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/MdsState.h"
#include "OSCLib/Data/SDC/MDIB/InstanceIdentifier.h"
#include "OSCLib/Data/SDC/MDIB/LocalizedText.h"
#include "OSCLib/Data/SDC/MDIB/LocationContextDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/LocationContextState.h"
#include "OSCLib/Data/SDC/MDIB/MdDescription.h"
#include "OSCLib/Data/SDC/MDIB/MetricQuality.h"
#include "OSCLib/Data/SDC/MDIB/NumericMetricDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/NumericMetricState.h"
#include "OSCLib/Data/SDC/MDIB/NumericMetricValue.h"
#include "OSCLib/Data/SDC/MDIB/OperatorContextDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/PatientContextDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/PatientContextState.h"
#include "OSCLib/Data/SDC/MDIB/PatientDemographicsCoreData.h"
#include "OSCLib/Data/SDC/MDIB/Range.h"
#include "OSCLib/Data/SDC/MDIB/StringMetricDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/StringMetricState.h"
#include "OSCLib/Data/SDC/MDIB/StringMetricValue.h"
#include "OSCLib/Data/SDC/MDIB/SystemContextDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/MetaData.h"
#include "OSCLib/Data/SDC/MDIB/VmdDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/VmdState.h"
#include "OSCLib/Data/SDC/MDIB/Udi.h"
#include "OSCLib/Data/SDC/MDIB/custom/OperationInvocationContext.h"
#include "OSCLib/Util/DebugOut.h"
#include "OSCLib/Util/Task.h"
#include "../AbstractSDCLibFixture.h"
#include "../UnitTest++/src/UnitTest++.h"

#include <memory>

#include "Poco/Event.h"
#include "Poco/Mutex.h"
#include "Poco/ScopedLock.h"

#include "OSELib/SDC/ServiceManager.h"

using namespace SDCLib;
using namespace SDCLib::Util;
using namespace SDCLib::Data::SDC;

namespace SDCLib {
namespace Tests {
namespace SimpleSDC {

//
// define handles and IDs
//

const std::string DEVICE_ENDPOINT_REFERENCE("EPR_1234");
const std::string DEVICE_UDI("UDI_1234");
const int DEFAULT_TIMEOUT(5000);

// Device's taxonomic description
const std::string MDS_HANDLE("sampleMDS");
const std::string VMD_DESCRIPTOR_HANDLE("vmd_descriptor");
const std::string CHANNEL_DESCRIPTOR_HANDLE("first_channel");

// Metrices
//
// all metrices are tested to be succesfully requested by the consumer from the provider
//
// 1. a provider side changing numeric metric representing a measured value
const std::string NUMERIC_METRIC_CURRENT_HANDLE("handle_cur");
// 2. a numeric metric representing a a limit for this changing value
// this value is set writable, thus the consumer is able to set it
const std::string NUMERIC_METRIC_MAX_HANDLE("handle_max");
// 3. a string metric which is also set by the consumer
const std::string STRING_METRIC_HANDLE("handle_str");
// 4. an enum metric is also tried to be set by the consumer with legal an illegal values
const std::string ENUM_METRIC_HANDLE("handle_enum");

// AlertSystem
//
// 1. a limit alert condition that consists of the bounds which must be kept by the handle_cur numeric metric state
// 		a LimitAlertConditionStateHandler with the same name checks compliance
const std::string ALERT_CONDITION_HANDLE("handle_alert_condition_limit");
// 2. an alert signal whose attribute presents is changed
const std::string ALERT_SIGNAL_HANDLE("handle_alert_signal");
// 3. an latching alert signal which latched state is checked
const std::string ALERT_SIGNAL_LATCHING_HANDLE("handle_alert_signal_latching");


// component state handler
const std::string ALERT_SYSTEM_HANDLE("handle_alert_system");
const std::string CMD_HANDLE("cmd_handle");


// context states
const std::string LOCATION_CONTEXT_HANDLE("location_context");
const std::string PATIENT_CONTEXT_HANDLE("patient_context");

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Consumer event handlers
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


class ExampleConsumerNumericHandler : public SDCConsumerMDStateHandler<NumericMetricState> {
public:
	ExampleConsumerNumericHandler(const std::string & descriptorHandle) : SDCConsumerMDStateHandler(descriptorHandle),
    	weight(0)
	{
	}

    virtual ~ExampleConsumerNumericHandler() {

    }

    void onStateChanged(const NumericMetricState & state) override {
        double val = state.getMetricValue().getValue();
        DebugOut(DebugOut::Default, "SimpleSDC") << "Consumer: Received value changed of " << descriptorHandle << ": " << val << std::endl;
    	Poco::Mutex::ScopedLock lock(mutex);
        weight = (float)val;
        eventEMR.set();
    }

    void onOperationInvoked(const OperationInvocationContext & oic, InvocationState is) override {
        DebugOut(DebugOut::Default, "SimpleSDC") << "Consumer: Received operation invoked (ID, STATE) of " << descriptorHandle << ": " << oic.transactionId << ", " << EnumToString::convert(is) << std::endl;
    }

    float getWeight() {
    	Poco::Mutex::ScopedLock lock(mutex);
    	float result(weight);
        return result;
    }

	Poco::Event & getEventEMR() {
		return eventEMR;
	}

private:
    Poco::Mutex mutex;
    float weight;
    Poco::Event eventEMR;
};

class ExampleConsumerEnumStringMetricHandler : public SDCConsumerMDStateHandler<EnumStringMetricState> {
public:
	ExampleConsumerEnumStringMetricHandler(const std::string & descriptorHandle) : SDCConsumerMDStateHandler(descriptorHandle)
	{
	}

    virtual ~ExampleConsumerEnumStringMetricHandler() {

    }

    void onStateChanged(const EnumStringMetricState & state) override {
    	const std::string val(state.getMetricValue().getValue());
        DebugOut(DebugOut::Default, "SimpleSDC") << "Consumer: Received value changed of " << descriptorHandle << ": " << val << std::endl;
        eventEMR.set();
    }

    void onOperationInvoked(const OperationInvocationContext & oic, InvocationState is) override {
        DebugOut(DebugOut::Default, "SimpleSDC") << "Consumer: Received operation invoked (ID, STATE) of " << descriptorHandle << ": " << oic.transactionId << ", " << EnumToString::convert(is) << std::endl;
    }

	Poco::Event & getEventEMR() {
		return eventEMR;
	}

private:
    Poco::Event eventEMR;
};

class ExampleConsumerStringMetricHandler : public SDCConsumerMDStateHandler<StringMetricState> {
public:
	ExampleConsumerStringMetricHandler(const std::string & descriptorHandle) : SDCConsumerMDStateHandler(descriptorHandle)
	{
	}

    virtual ~ExampleConsumerStringMetricHandler() {

    }

    void onStateChanged(const StringMetricState & state) override {
    	const std::string val(state.getMetricValue().getValue());
        DebugOut(DebugOut::Default, "SimpleSDC") << "Consumer: Received value changed of " << handle << ": " << val << std::endl;
        eventEMR.set();
    }

    void onOperationInvoked(const OperationInvocationContext & oic, InvocationState is) override {
        DebugOut(DebugOut::Default, "SimpleSDC") << "Consumer: Received operation invoked (ID, STATE) of " << handle << ": " << oic.transactionId << ", " << EnumToString::convert(is) << std::endl;
    }

	Poco::Event & getEventEMR() {
		return eventEMR;
	}

private:
    const std::string handle;
    Poco::Event eventEMR;
};

class ExampleConsumerAlertSignalHandler : public SDCConsumerMDStateHandler<AlertSignalState> {
public:
	ExampleConsumerAlertSignalHandler(const std::string & descriptorHandle) : SDCConsumerMDStateHandler(descriptorHandle)
	{
	}

    virtual ~ExampleConsumerAlertSignalHandler() {

    }

    void onStateChanged(const AlertSignalState & state) override {
        DebugOut(DebugOut::Default, "SimpleSDC") << "Consumer: Received alert signal changed of " << descriptorHandle << ", presence = " << EnumToString::convert(state.getPresence()) << std::endl;
        if (state.getPresence() == AlertSignalPresence::Off) {
        	eventEAROff.set();
        }
        if (state.getPresence() == AlertSignalPresence::On) {
        	eventEAROn.set();
        }
        if (state.getPresence() == AlertSignalPresence::Latch) {
        	eventEARLatch.set();
        }
    }

    void onOperationInvoked(const OperationInvocationContext & oic, InvocationState is) override {
        DebugOut(DebugOut::Default, "SimpleSDC") << "Consumer: Received operation invoked (ID, STATE) of " << descriptorHandle << ": " << oic.transactionId << ", " << EnumToString::convert(is) << std::endl;
    }

	Poco::Event & getEventEAROff() {
		return eventEAROff;
	}

	Poco::Event & getEventEAROn() {
		return eventEAROn;
	}

	Poco::Event & getEventEARLatch() {
		return eventEARLatch;
	}

private:
    Poco::Event eventEAROff;
    Poco::Event eventEAROn;
    Poco::Event eventEARLatch;
};

// context state handlers

class ExampleLocationContextEventHandler : public SDCConsumerMDStateHandler<LocationContextState> {
public:

	ExampleLocationContextEventHandler(std::string descriptorHandle) : SDCConsumerMDStateHandler(descriptorHandle) {
	}

	virtual void onStateChanged(const LocationContextState & state) override {
        DebugOut(DebugOut::Default, "SimpleSDC") << "Consumer: Received location context values changed for handle" << state.getHandle() << std::endl;
  		eventEMR.set();
	}

	Poco::Event & getEventEMR() {
		return eventEMR;
	}

private:
	Poco::Event eventEMR;
};


class ExamplePatientContextEventHandler : public SDCConsumerMDStateHandler<PatientContextState> {
public:

	ExamplePatientContextEventHandler(std::string descriptorHandle) : SDCConsumerMDStateHandler(descriptorHandle) {
	}

	virtual void onStateChanged(const PatientContextState & state) override {
        DebugOut(DebugOut::Default, "SimpleSDC") << "Consumer: Received patient context values changed for handle" << state.getHandle() << std::endl;
  		eventEMR.set();
	}

	Poco::Event & getEventEMR() {
		return eventEMR;
	}

private:
	Poco::Event eventEMR;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Provider handlers
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Context states are handled like metrices or alerts

// Note:
// quasi-multi states can be implemented like single states as well. Since they have the same descriptor handle
// each time a multi state (which has the same descritptor handle but differs in the handle attribute) changes,
// the state hander is called. The user has to destinguish!

class LocationContextStateHandler : public SDCProviderMDStateHandler<LocationContextState> {
public:
	LocationContextStateHandler(std::string descriptorHandle) : SDCProviderMDStateHandler(descriptorHandle) {}
	virtual InvocationState onStateChangeRequest(const LocationContextState & state,  const OperationInvocationContext & oic) override {
		if (state.getDescriptorHandle() !=  LOCATION_CONTEXT_HANDLE)
			return InvocationState::Fail;

		DebugOut(DebugOut::Default, "SimpleSDC") << "Provider: ContextHandler received state change request" << std::endl;
		return InvocationState::Fin;
	}

	virtual LocationContextState getInitialState() override {
		LocationContextState locationContextState(descriptorHandle, "multistate1");
		return locationContextState;
	}

};

class PatientContextStateHandler : public SDCProviderMDStateHandler<PatientContextState> {
public:
	PatientContextStateHandler(std::string descriptorHandle) : SDCProviderMDStateHandler(descriptorHandle) {}
	virtual InvocationState onStateChangeRequest(const PatientContextState & state,  const OperationInvocationContext & oic) override {
		if (state.getDescriptorHandle() !=  PATIENT_CONTEXT_HANDLE)
			return InvocationState::Fail;

		DebugOut(DebugOut::Default, "SimpleSDC") << "Provider: ContextHandler received state change request" << std::endl;
		return InvocationState::Fin;
	}

	virtual PatientContextState getInitialState() override {
		PatientContextState patientContextState(descriptorHandle, "multistate1");
		return patientContextState;
	}

};


class MaxValueStateHandler : public SDCProviderMDStateHandler<NumericMetricState> {
public:

    MaxValueStateHandler(std::string descriptorHandle) : SDCProviderMDStateHandler(descriptorHandle) {
    }

    InvocationState onStateChangeRequest(const NumericMetricState & state, const OperationInvocationContext & oic) override {
        // Invocation has been fired as WAITING when entering this method
    	DebugOut(DebugOut::Default, "SimpleSDC") << "Provider: MaxValueStateHandler received state change request" << std::endl;

    	notifyOperationInvoked(oic, InvocationState::Start);

        // we can update here, but if we return Fin, the framework will also update
        updateState(state);

        // Usually, update the real device's state here.

        return InvocationState::Fin;  // Framework will update internal MDIB with the state's value and increase MDIB version
    }

    // Helper method
    NumericMetricState createState() {
        NumericMetricState result(descriptorHandle);
        result
            .setMetricValue(NumericMetricValue(MetricQuality(MeasurementValidity::Vld)).setValue(2.0))
            .setActivationState(ComponentActivation::On);
        return result;
    }

    NumericMetricState getInitialState() override {
        NumericMetricState result = createState();
        return result;
    }

    // Convenience value getter
    float getMaxWeight() {
    	std::unique_ptr<NumericMetricState> result(getParentProvider().getMdState().findState<NumericMetricState>(NUMERIC_METRIC_MAX_HANDLE));
        // check if result is valid
        if (result != nullptr) {
        	// In real applications, check if state has an observed value and if the observed value has a value!
        	return (float)result->getMetricValue().getValue();
        } else {
        	DebugOut(DebugOut::Default, "ExampleProvider") << "Maximum weight metric not found." << std::endl;
        	return 0;
        }
    }
};





class CurValueStateHandler : public SDCProviderMDStateHandler<NumericMetricState> {
public:

    CurValueStateHandler(std::string descriptorHandle) : SDCProviderMDStateHandler(descriptorHandle) {
    }

	// define how to react on a request for a state change. This handler should not be set, thus always return Fail.
	InvocationState onStateChangeRequest(const NumericMetricState & state, const OperationInvocationContext & oic) override {
		return InvocationState::Fail;
	}
    // Helper method
    NumericMetricState createState(float value) {
        NumericMetricState result(descriptorHandle);
        result
            .setMetricValue(NumericMetricValue(MetricQuality(MeasurementValidity::Vld)).setValue(value))
            .setActivationState(ComponentActivation::On)
            .setDescriptorHandle(NUMERIC_METRIC_CURRENT_HANDLE)
            .setLifeTimePeriod(xml_schema::Duration(0,0,0,0,0,0,1));
        return result;
    }

    NumericMetricState getInitialState() override {
        return createState(0);
    }

    void setNumericValue(float value) {
        NumericMetricState currentWeightState = createState(value);
        // Call update function (this will update internal MDIB and increase MDIB version number)
        updateState(currentWeightState);
    }

};

class EnumStringMetricStateHandler : public SDCProviderMDStateHandler<EnumStringMetricState> {
public:

	EnumStringMetricStateHandler(std::string descriptorHandle) : SDCProviderMDStateHandler(descriptorHandle) {
    }

    InvocationState onStateChangeRequest(const EnumStringMetricState & state, const OperationInvocationContext & oic) override {
        // Invocation has been fired as WAITING when entering this method
    	DebugOut(DebugOut::Default, "SimpleSDC") << "Provider: EnumStringMetricStateHandler received state change request" << std::endl;
    	notifyOperationInvoked(oic, InvocationState::Start);

        // we can update here, but if we return Fin, the framework will also notify
        //updateState(currentStringState);

        // Usually, update the real device's state here.

        return InvocationState::Fin;  // Framework will update internal MDIB with the state's value and increase MDIB version
    }

    // Helper method
    EnumStringMetricState createState(const std::string & value) {
        EnumStringMetricState result(descriptorHandle);
        result
            .setMetricValue(StringMetricValue(MetricQuality(MeasurementValidity::Vld)).setValue(value))
            .setActivationState(ComponentActivation::On);
        return result;
    }

    EnumStringMetricState getInitialState() override {
        return createState("hello");
    }

};

class StrValueStateHandler : public SDCProviderMDStateHandler<StringMetricState> {
public:

    StrValueStateHandler(std::string descriptorHandle) : SDCProviderMDStateHandler(descriptorHandle) {
    }

    InvocationState onStateChangeRequest(const StringMetricState & state, const OperationInvocationContext & oic) override {
        // Invocation has been fired as WAITING when entering this method
    	DebugOut(DebugOut::Default, "SimpleSDC") << "Provider: StrValueStateHandler received state change request" << std::endl;
    	notifyOperationInvoked(oic, InvocationState::Start);

        // we can update here, but if we return Fin, the framework will also notify
        //updateState(currentStringState);

        // Usually, update the real device's state here.

        return InvocationState::Fin;  // Framework will update internal MDIB with the state's value and increase MDIB version
    }

    // Helper method
    StringMetricState createState(const std::string & value) {
        StringMetricState result(STRING_METRIC_HANDLE);
        result
            .setMetricValue(StringMetricValue(MetricQuality(MeasurementValidity::Vld)).setValue(value))
            .setActivationState(ComponentActivation::On);
        return result;
    }

    StringMetricState getInitialState() override {
        return createState("Test");
    }

    // Convenience setter
    void setStringValue(const std::string & value) {
        StringMetricState currentState = createState(value);
        updateState(currentState);
    }

};

class AlertSignalStateHandler : public SDCProviderMDStateHandler<AlertSignalState> {
public:

	AlertSignalStateHandler(std::string descriptorHandle) : SDCProviderMDStateHandler(descriptorHandle) {
    }

    InvocationState onStateChangeRequest(const AlertSignalState & state, const OperationInvocationContext & oic) override {
        // Invocation has been fired as WAITING when entering this method
    	DebugOut(DebugOut::Default, "SimpleSDC") << "Provider: AlertSignalStateHandler received state change: " << EnumToString::convert(state.getPresence()) << std::endl;

    	notifyOperationInvoked(oic, InvocationState::Start);

    	// Update the real device's state here (update device alarms)! Check state's presence and alertSignalState's presence values!

    	// we can update here, but if we return Fin, the framework will also notify
    	//updateState(state);

        return InvocationState::Fin;  // Framework will update internal MDIB with the state's value and increase MDIB version
    }

    // Helper method
    AlertSignalState createState() {
        AlertSignalState result(ALERT_SIGNAL_HANDLE, AlertActivation::On); // Reference alert signal descriptor's handle // Component is working
        result
            .setPresence(AlertSignalPresence::Off);  // No alarm signal
        return result;
    }

    AlertSignalState getInitialState() override {
        return createState();
    }

};

class LatchingAlertSignalStateHandler : public SDCProviderMDStateHandler<AlertSignalState> {
public:

	LatchingAlertSignalStateHandler(std::string descriptorHandle) : SDCProviderMDStateHandler(descriptorHandle) {
    }

    InvocationState onStateChangeRequest(const AlertSignalState & state, const OperationInvocationContext & oic) override {
        // Invocation has been fired as WAITING when entering this method
    	DebugOut(DebugOut::Default, "SimpleSDC") << "Provider: LatchingAlertSignalStateHandler received state change: " << EnumToString::convert(state.getPresence()) << std::endl;

    	notifyOperationInvoked(oic, InvocationState::Start);

    	// Update the real device's state here (update device alarms)! Check state's presence and alertSignalState's presence values!

    	// we can update here, but if we return Fin, the framework will also notify
    	//updateState(alertSignalState);

        return InvocationState::Fin;  // Framework will update internal MDIB with the state's value and increase MDIB version
    }

    // Helper method
    AlertSignalState createState() {
        AlertSignalState result(ALERT_SIGNAL_LATCHING_HANDLE, AlertActivation::On); // Reference alert signal descriptor's handle // Component is working
        result
            .setPresence(AlertSignalPresence::Off);  // No alarm signal
        return result;
    }

    AlertSignalState getInitialState() override {
        return createState();
    }

};

class LimitAlertConditionStateHandler : public SDCProviderAlertConditionStateHandler<LimitAlertConditionState> {
public:

	LimitAlertConditionStateHandler(std::string descriptorHandle) : SDCProviderAlertConditionStateHandler(descriptorHandle)
	{
	}

    InvocationState onStateChangeRequest(const LimitAlertConditionState & state, const OperationInvocationContext & ) override {
        // Invocation has been fired as WAITING when entering this method
    	std::unique_ptr<LimitAlertConditionState> pCurrentState(getParentProvider().getMdState().findState<LimitAlertConditionState>(state.getDescriptorHandle()));

    	DebugOut(DebugOut::Default, "SimpleSDC") << "Provider: LimitAlertConditionStateHandler received state change, presence = " << state.getPresence() << std::endl;
        if (state.getPresence() != pCurrentState->getPresence()) {
    		DebugOut(DebugOut::Default, "SimpleSDC") << "Provider: LimitAlertConditionStateHandler detected presence change to: " << state.getPresence() << std::endl;
    		// do something...
    	}

    	//we can update here, but if we return Fin, the framework will also notify
    	//updateState(*pCurrentState);

    	return InvocationState::Fin;  // Framework will update internal MDIB with the state's value and increase MDIB version
    }

    // this function monitors the observered metric which is defined by the Source attribute of the corresponding LimitAlertConditionDescriptor
    void sourceHasChanged(const std::string & sourceHandle) override {
    	DebugOut(DebugOut::Default, "SimpleSDC") << "Provider: LimitAlertConditionStateHandler monitored source state changed." << std::endl;

    	// Check limit and trigger alarm condition, if needed (this method will then take care of handling all signal states)
        std::unique_ptr<NumericMetricState> pSourceState(getParentProvider().getMdState().findState<NumericMetricState>(sourceHandle));

        std::unique_ptr<LimitAlertConditionState> pLimitAlertConditionState(getParentProvider().getMdState().findState<LimitAlertConditionState>(ALERT_CONDITION_HANDLE));
        if (pSourceState->getDescriptorHandle() != sourceHandle) {
    		return;
    	}
    	if (!pSourceState->hasMetricValue()) {
    		return;
    	}
    	const auto sourceValue(pSourceState->getMetricValue());
    	if (!sourceValue.hasValue()) {
    		return;
    	}
    	const auto limits(pLimitAlertConditionState->getLimits());
    	if (!limits.hasUpper()) {
    		return;
    	}
    	if (!limits.hasLower()) {
    		return;
    	}

    	const bool triggerAlarm(sourceValue.getValue() > limits.getUpper() || sourceValue.getValue() < limits.getLower());
   		setAlertConditionPresence(pLimitAlertConditionState->getDescriptorHandle(), triggerAlarm, OperationInvocationContext::none());
    }

	// Can be used to switch condition on and off (e.g. propagate input from real device). Currently not used in this test.
	void setPresence(bool presence) {
		// We do not need to update our state here, this will be done in onStateChangeRequest (will be called by the method we invoke next)
		setAlertConditionPresence(getDescriptorHandle(), presence, OperationInvocationContext::none());
	}

    // Helper method
    LimitAlertConditionState createState() {
        LimitAlertConditionState result(ALERT_CONDITION_HANDLE, AlertActivation::On, Range().setLower(0.0).setUpper(2.0), AlertConditionMonitoredLimits::All); // Reference alert signal descriptor's handle
        result
            .setPresence(false);
        return result;
    }

    LimitAlertConditionState getInitialState() override {
        return createState();
    }

};


class AlertSystemStateHandler : public SDCProviderMDStateHandler<AlertSystemState> {
public:

	AlertSystemStateHandler(std::string descriptorHandle) : SDCProviderMDStateHandler(descriptorHandle){
    }

	AlertSystemState getInitialState() override {
        AlertSystemState alertSystemState(descriptorHandle, AlertActivation::On);  // reference alert system descriptor's handle // Alert is activated
        return alertSystemState;
    }

	// define how to react on a request for a state change. This handler should not be set, thus always return Fail.
	InvocationState onStateChangeRequest(const AlertSystemState & state, const OperationInvocationContext & oic) override {
		return InvocationState::Fail;
	}
};


// use this state handler for remote function calls
class CommandHandler : public SDCProviderActivateOperationHandler {
public:

	CommandHandler(std::string descriptorHandle) : SDCProviderActivateOperationHandler(descriptorHandle) {
    }

	InvocationState onActivateRequest(const OperationInvocationContext & oic) override {
		DebugOut(DebugOut::Default, "SimpleSDC") << "Provider: Received command!" << std::endl;
		return InvocationState::Fin;
	}
};

class AlwaysOnChannelStateHandler : public SDCProviderComponentStateHandler<ChannelState> {
public:
	AlwaysOnChannelStateHandler(const std::string & descriptorHandle) : SDCProviderComponentStateHandler(descriptorHandle){
    }

    // Helper method
    ChannelState createState() {
    	ChannelState result(descriptorHandle);
        result
            .setActivationState(ComponentActivation::On);
        return result;
    }

    virtual ChannelState getInitialState() override {
    	ChannelState state = createState();
        return state;
    }
};


class AlwaysOnVmdStateHandler : public SDCProviderComponentStateHandler<VmdState> {
public:
	AlwaysOnVmdStateHandler(const std::string & descriptorHandle) : SDCProviderComponentStateHandler(descriptorHandle){
    }

    // Helper method
    VmdState createState() {
    	VmdState result(descriptorHandle);
        result
            .setActivationState(ComponentActivation::On);
        return result;
    }

    virtual VmdState getInitialState() override {
    	VmdState state = createState();
        return state;
    }
};



class AlwaysOnMdsStateHandler : public SDCProviderComponentStateHandler<MdsState> {
public:
    AlwaysOnMdsStateHandler(const std::string & descriptorHandle) : SDCProviderComponentStateHandler(descriptorHandle){
    }

    // Helper method
    MdsState createState() {
        MdsState result(descriptorHandle);
        result
            .setActivationState(ComponentActivation::On);
        return result;
    }

    virtual MdsState getInitialState() override {
        MdsState state = createState();
        return state;
    }
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Provider
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

class OSCPHoldingDeviceProvider : public Util::Task {
public:
    OSCPHoldingDeviceProvider() :
    	currentWeight(0),
    	sdcProvider(),
    	locationContextStateHandler(LOCATION_CONTEXT_HANDLE),
    	patientContextStateHandler(PATIENT_CONTEXT_HANDLE),
    	curValueState(NUMERIC_METRIC_CURRENT_HANDLE),
    	maxValueState(NUMERIC_METRIC_MAX_HANDLE),
    	enumState(ENUM_METRIC_HANDLE),
    	strValueState(STRING_METRIC_HANDLE),
    	alertSigHandler(ALERT_SIGNAL_HANDLE),
    	latchingAlertSigHandler(ALERT_SIGNAL_LATCHING_HANDLE),
    	limitAlertConditionHandler(ALERT_CONDITION_HANDLE),
    	alertSysHandler(ALERT_SYSTEM_HANDLE),
    	cmdHandler(CMD_HANDLE),
    	channelStateHandler(CHANNEL_DESCRIPTOR_HANDLE),
    	mdsStateHandler(MDS_HANDLE),
    	vmdStateHandler(VMD_DESCRIPTOR_HANDLE)
	{
    	sdcProvider.setEndpointReference(DEVICE_ENDPOINT_REFERENCE);

        // Define semantic meaning of weight unit "kg", which will be used for defining the
        // current weight and the max weight below.
        CodedValue unit(CodeIdentifier("MDCX_CODE_ID_KG"));
        unit	.setCodingSystem("OR.NET.Codings")
				.addConceptDescription(LocalizedText().setRef("uri/to/file.txt").setLang("en"));


    	//
        // Setup metric descriptors
        //

        // define properties of current weight metric
        NumericMetricDescriptor currentWeightMetric(NUMERIC_METRIC_CURRENT_HANDLE,
        		unit,
        		MetricCategory::Msrmt,
        		MetricAvailability::Cont,
        		2);

        // add additional information: averaging period of the measurement
        currentWeightMetric.setAveragingPeriod(xml_schema::Duration(0,0,0,0,0,1,0));

        //  define properties of enum metric
        EnumStringMetricDescriptor testEnumMetric(ENUM_METRIC_HANDLE,
        		CodedValue("MDCX_CODE_ID_ENUM")
        			.setCodingSystem("OR.NET.Codings")
        			.addConceptDescription(LocalizedText().setRef("uri/to/file.txt").setLang("en")),
        		MetricCategory::Set,
        		MetricAvailability::Cont);

        testEnumMetric
			.addAllowedValue(AllowedValue("hello"))
			.addAllowedValue(AllowedValue("hallo"))
			.addAllowedValue(AllowedValue("bon jour"));


        // define properties of max weight metric
        NumericMetricDescriptor maxWeightMetric(NUMERIC_METRIC_MAX_HANDLE,
        		unit,
        		MetricCategory::Set,
        		MetricAvailability::Cont,
        		1);

        // define properties of test string metric
        StringMetricDescriptor testStringMetric(STRING_METRIC_HANDLE,
        		CodedValue("MDCX_CODE_ID_STRING"),
        		MetricCategory::Set,
        		MetricAvailability::Cont);


        //
        // Contexts
        //

        // Location context
        LocationContextDescriptor location("location_context");
        PatientContextDescriptor patient("patient_context");


        //
		// Setup alert system
		//

        // alert condition that is monitoring the handle_cur
        LimitAlertConditionDescriptor limitAlertCondition(ALERT_CONDITION_HANDLE,
        		AlertConditionKind::Tec,
        		AlertConditionPriority::Me,
        		Range()
        			.setLower(0)
        			.setUpper(2.0)
        		);

        limitAlertCondition.setType(CodedValue("MDCX_CODE_ID_ALERT_WEIGHT_CONDITION").setCodingSystem("OR.NET.Codings")).addSource(NUMERIC_METRIC_CURRENT_HANDLE);

        // create signal for condition
        AlertSignalDescriptor alertSignal(ALERT_SIGNAL_HANDLE, AlertSignalManifestation::Vis, false);
        alertSignal
        	.setConditionSignaled(ALERT_CONDITION_HANDLE);

        AlertSignalDescriptor latchingAlertSignal(ALERT_SIGNAL_LATCHING_HANDLE, AlertSignalManifestation::Vis, true);
        latchingAlertSignal
			.setConditionSignaled(ALERT_CONDITION_HANDLE);

        // Alerts
        AlertSystemDescriptor alertSystem(ALERT_SYSTEM_HANDLE);
        alertSystem
			.addAlertSignal(alertSignal)
			.addAlertSignal(latchingAlertSignal)
			.addLimitAlertCondition(limitAlertCondition);


        //
        // assemble everything following the taxonomic structure of the device
        //

        // Channel
        ChannelDescriptor holdingDeviceChannel(CHANNEL_DESCRIPTOR_HANDLE);
        holdingDeviceChannel
			.addMetric(currentWeightMetric)
			.addMetric(testEnumMetric)
        	.addMetric(maxWeightMetric)
        	.addMetric(testStringMetric)
			.setSafetyClassification(SafetyClassification::MedA);

        // VMD
        VmdDescriptor holdingDeviceModule(VMD_DESCRIPTOR_HANDLE);
        holdingDeviceModule
        	.setAlertSystem(alertSystem)
			.setHandle(VMD_DESCRIPTOR_HANDLE)
			.addChannel(holdingDeviceChannel);

        // MDS
        MdsDescriptor holdingDeviceSystem(MDS_HANDLE);
        holdingDeviceSystem
        	.setMetaData(
        		MetaData().addManufacturer(LocalizedText().setRef("SurgiTAIX AG"))
        		.setModelNumber("1")
        		.addModelName(LocalizedText().setRef("EndoTAIX"))
        		.addSerialNumber("1234-5678"))
			.setSystemContext(
				SystemContextDescriptor("MDC_SYS_CON")
			    .setPatientContext(
			    		patient)
				.setLocationContext(
						location)
				)
			.addVmd(holdingDeviceModule)
			.setType(
                CodedValue("MDCX_CODE_ID_MDS")
                .setCodingSystem("OR.NET.Codings"));

        sdcProvider.createSetOperationForDescriptor(alertSignal, holdingDeviceSystem);
        sdcProvider.createSetOperationForDescriptor(maxWeightMetric, holdingDeviceSystem);
        sdcProvider.createSetOperationForDescriptor(testEnumMetric, holdingDeviceSystem);
        sdcProvider.createSetOperationForDescriptor(testStringMetric, holdingDeviceSystem);
        sdcProvider.createSetOperationForDescriptor(location, holdingDeviceSystem);
        sdcProvider.createSetOperationForDescriptor(patient, holdingDeviceSystem);

        ActivateOperationDescriptor aod(CMD_HANDLE, NUMERIC_METRIC_MAX_HANDLE);

		sdcProvider.addActivateOperationForDescriptor(aod, holdingDeviceSystem);

		// create and add description
		MdDescription mdDescription;
		mdDescription.addMdsDescriptor(holdingDeviceSystem);

		sdcProvider.setMdDescription(mdDescription);

        // State handlers

		sdcProvider.addMdStateHandler(&locationContextStateHandler);
		sdcProvider.addMdStateHandler(&patientContextStateHandler);
		sdcProvider.addMdStateHandler(&curValueState);
		sdcProvider.addMdStateHandler(&enumState);
		sdcProvider.addMdStateHandler(&maxValueState);
		sdcProvider.addMdStateHandler(&strValueState);
		sdcProvider.addMdStateHandler(&limitAlertConditionHandler);
		sdcProvider.addMdStateHandler(&alertSigHandler);
		sdcProvider.addMdStateHandler(&latchingAlertSigHandler);
		sdcProvider.addMdStateHandler(&alertSysHandler);
		sdcProvider.addMdStateHandler(&cmdHandler);
		sdcProvider.addMdStateHandler(&channelStateHandler);
		sdcProvider.addMdStateHandler(&mdsStateHandler);
		sdcProvider.addMdStateHandler(&vmdStateHandler);
	}

    MdDescription getMdDescription() {
    	return sdcProvider.getMdDescription();
    }

    void startup() {
    	sdcProvider.startup();
    }

    void shutdown() {
    	sdcProvider.shutdown();
    }

    // Update weight periodically
    virtual void runImpl() override {
    	float nextWeight = currentWeight + 0.2f;
    	if (nextWeight > 2.5f) {
    		nextWeight = 0.1;
    	}
		setCurrentWeight(nextWeight);
		Poco::Thread::sleep(1000);
    }

    void setCurrentWeight(float value) {
        Poco::Mutex::ScopedLock lock(sdcProvider.getMutex());
        currentWeight = value;
        curValueState.setNumericValue(value);
        DebugOut(DebugOut::Default, "SimpleSDC") << "Changed value: " << currentWeight << std::endl;
        strValueState.setStringValue("Test " + std::to_string(value));
    }

private:
    float currentWeight;

    // Provider object
    SDCProvider sdcProvider;

    // State (handlers)
    LocationContextStateHandler locationContextStateHandler;
    PatientContextStateHandler patientContextStateHandler;

    CurValueStateHandler curValueState;
    MaxValueStateHandler maxValueState;
    EnumStringMetricStateHandler enumState;
    StrValueStateHandler strValueState;

    AlertSignalStateHandler alertSigHandler;
    LatchingAlertSignalStateHandler latchingAlertSigHandler;
    LimitAlertConditionStateHandler limitAlertConditionHandler;
    AlertSystemStateHandler alertSysHandler;
    CommandHandler cmdHandler;
    AlwaysOnChannelStateHandler channelStateHandler;
    AlwaysOnMdsStateHandler mdsStateHandler;
    AlwaysOnVmdStateHandler vmdStateHandler;
};

}
}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct FixtureSimpleSDC : Tests::AbstractSDCLibFixture {
	FixtureSimpleSDC() : AbstractSDCLibFixture("FixtureSimpleSDC", OSELib::LogLevel::Error, 9000) {}
};

SUITE(OSCP) {
TEST_FIXTURE(FixtureSimpleSDC, simpleoscp)
{
	DebugOut::openLogFile("Test.log.txt", true);
	try
	{
        // Provider
        Tests::SimpleSDC::OSCPHoldingDeviceProvider provider;
        provider.startup();
        provider.start();

        // MdDescription test
        MdDescription mdDescription =  provider.getMdDescription();
        // add and remove a test MDS
        MdsDescriptor mds_test("MDC_MDS_HANDLE");
        mdDescription.addMdsDescriptor(mds_test);

        CHECK_EQUAL(true, mdDescription.removeMdsDescriptor(mds_test));

        Poco::Thread::sleep(2000);
        // Consumer
        OSELib::SDC::ServiceManager oscpsm;
        std::shared_ptr<SDCConsumer> c(oscpsm.discoverEndpointReference(Tests::SimpleSDC::DEVICE_ENDPOINT_REFERENCE));

        // create state handlers
        Tests::SimpleSDC::ExampleConsumerNumericHandler eces1(Tests::SimpleSDC::NUMERIC_METRIC_CURRENT_HANDLE);
        Tests::SimpleSDC::ExampleConsumerNumericHandler eces2(Tests::SimpleSDC::NUMERIC_METRIC_MAX_HANDLE);
        Tests::SimpleSDC::ExampleConsumerStringMetricHandler eces3(Tests::SimpleSDC::STRING_METRIC_HANDLE);
        Tests::SimpleSDC::ExampleConsumerEnumStringMetricHandler eces4(Tests::SimpleSDC::ENUM_METRIC_HANDLE);
        Tests::SimpleSDC::ExampleConsumerAlertSignalHandler alertSignalsink(Tests::SimpleSDC::ALERT_SIGNAL_HANDLE);
        Tests::SimpleSDC::ExampleConsumerAlertSignalHandler latchingAlertSignalsink(Tests::SimpleSDC::ALERT_SIGNAL_LATCHING_HANDLE);
        Tests::SimpleSDC::ExampleLocationContextEventHandler locationEventHandler(Tests::SimpleSDC::LOCATION_CONTEXT_HANDLE);
        Tests::SimpleSDC::ExamplePatientContextEventHandler  patientEventHandler(Tests::SimpleSDC::PATIENT_CONTEXT_HANDLE);


        // Discovery test
        CHECK_EQUAL(true, c != nullptr);

		if (c != nullptr) {
			SDCConsumer & consumer = *c;
            // MDIB test
            MdibContainer mdib(consumer.getMdib());

            { // test access to system metadata of mds implemented by provider above
            	std::unique_ptr<MdsDescriptor> pMdsDescriptor(mdib.getMdDescription().findDescriptor<MdsDescriptor>(Tests::SimpleSDC::MDS_HANDLE));
            	if (pMdsDescriptor != nullptr) {
            		if (pMdsDescriptor->hasMetaData()) {
            			const MetaData metadata(pMdsDescriptor->getMetaData());

            			if (!metadata.getUdiList().empty()) {
							const std::string serialNumber(metadata.getSerialNumberList().at(0));
							CHECK_EQUAL("1234-5678",serialNumber);
            			}

            		}
            	}
            }

            { // test presence of system context descriptors
            	std::unique_ptr<MdsDescriptor> pMdsDescriptor(mdib.getMdDescription().findDescriptor<MdsDescriptor>(Tests::SimpleSDC::MDS_HANDLE));
            	if (pMdsDescriptor != nullptr) {
            		SystemContextDescriptor sc(pMdsDescriptor->getSystemContext());
            		CHECK_EQUAL(true, sc.hasPatientContext());
            		CHECK_EQUAL(true, sc.getOperatorContextList().empty());
            	}
            }
            {	// lookup descriptors that should exist for the provider implemented above
            	std::unique_ptr<NumericMetricDescriptor> pNumericMetricDescriptor(mdib.getMdDescription().findDescriptor<NumericMetricDescriptor>(Tests::SimpleSDC::NUMERIC_METRIC_CURRENT_HANDLE));
            	if (pNumericMetricDescriptor != nullptr) {
				CHECK_EQUAL("en", pNumericMetricDescriptor->getUnit().getConceptDescriptionList()[0].getLang());
				}
				std::unique_ptr<StringMetricDescriptor> pStringMetricDescriptor(mdib.getMdDescription().findDescriptor<StringMetricDescriptor>(Tests::SimpleSDC::STRING_METRIC_HANDLE));
				CHECK_EQUAL(true, pStringMetricDescriptor != nullptr);
				CHECK_EQUAL(Tests::SimpleSDC::STRING_METRIC_HANDLE, pStringMetricDescriptor->getHandle());
            }

            // Register for consumer events
            CHECK_EQUAL(true, consumer.registerStateEventHandler(&eces1));
            CHECK_EQUAL(true, consumer.registerStateEventHandler(&eces2));
            CHECK_EQUAL(true, consumer.registerStateEventHandler(&eces3));
            CHECK_EQUAL(true, consumer.registerStateEventHandler(&eces4));
            // Register for alert signal events
            CHECK_EQUAL(true, consumer.registerStateEventHandler(&alertSignalsink));
            CHECK_EQUAL(true, consumer.registerStateEventHandler(&latchingAlertSignalsink));
            // Register for context changed events
            CHECK_EQUAL(true, consumer.registerStateEventHandler(&locationEventHandler));
            CHECK_EQUAL(true, consumer.registerStateEventHandler(&patientEventHandler));

            Poco::Thread::sleep(2000);

            {	// Ensure that requests for wrong handles fail.
            	DebugOut(DebugOut::Default, "SimpleSDC") << "Numeric test..." << std::endl;
				DebugOut(DebugOut::Default, "SimpleSDC") << "SHOULD FAIL: " << std::endl;
				NumericMetricState tempState(" ");
				std::unique_ptr<NumericMetricState> pTempNMS(consumer.requestState<NumericMetricState>("unknown"));
            	CHECK_EQUAL(false, pTempNMS != nullptr);
            }
            {	// Request state of current weight
				std::unique_ptr<NumericMetricState> pTempNMS(consumer.requestState<NumericMetricState>(Tests::SimpleSDC::NUMERIC_METRIC_CURRENT_HANDLE));
				CHECK_EQUAL(true, pTempNMS != nullptr);
				CHECK_EQUAL(true, pTempNMS->hasMetricValue());
				if (pTempNMS->hasMetricValue()) {
					const double curWeight(pTempNMS->getMetricValue().getValue());
					CHECK_EQUAL(true, curWeight > 0.1);
				}
            }
            {	// Ensure that (read-only) metrics without matching SetOperation cannot be set.
            	DebugOut(DebugOut::Default, "SimpleSDC") << "SHOULD FAIL: " << std::endl;
            	std::unique_ptr<NumericMetricState> pTempNMS(consumer.requestState<NumericMetricState>(Tests::SimpleSDC::NUMERIC_METRIC_CURRENT_HANDLE));
            	CHECK_EQUAL(true, pTempNMS != nullptr);
            	CHECK_EQUAL(true, InvocationState::Fail == consumer.commitState(*pTempNMS));
            }
            {	// Get state of maximum weight
            	std::unique_ptr<NumericMetricState> pTempNMS(consumer.requestState<NumericMetricState>(Tests::SimpleSDC::NUMERIC_METRIC_MAX_HANDLE));
				CHECK_EQUAL(true, pTempNMS != nullptr);
				double maxWeight = pTempNMS->getMetricValue().getValue();
				CHECK_EQUAL(2.0, maxWeight);
            }
            {	// Get state of test enum
            	std::unique_ptr<EnumStringMetricState> pTempESMS(consumer.requestState<EnumStringMetricState>(Tests::SimpleSDC::ENUM_METRIC_HANDLE));
				CHECK_EQUAL(true, pTempESMS != nullptr);
				const std::string enumValue(pTempESMS->getMetricValue().getValue());
				CHECK_EQUAL("hello", enumValue);
            }
            {	// Set state of test enum with allowed enum value
            	std::unique_ptr<EnumStringMetricState> pTempESMS(consumer.requestState<EnumStringMetricState>(Tests::SimpleSDC::ENUM_METRIC_HANDLE));
            	CHECK_EQUAL(true, pTempESMS != nullptr);

            	pTempESMS->setMetricValue(pTempESMS->getMetricValue().setValue("bon jour"));
				FutureInvocationState fis;
				CHECK_EQUAL(true, InvocationState::Wait == consumer.commitState(*pTempESMS, fis));
				CHECK_EQUAL(true, fis.waitReceived(InvocationState::Fin, Tests::SimpleSDC::DEFAULT_TIMEOUT));
            }
            {	// Set state of test enum with illegal enum value
            	std::unique_ptr<EnumStringMetricState> pTempESMS(consumer.requestState<EnumStringMetricState>(Tests::SimpleSDC::ENUM_METRIC_HANDLE));
            	CHECK_EQUAL(true, pTempESMS != nullptr);

				const std::string enumValue(pTempESMS->getMetricValue().getValue());
				CHECK_EQUAL("bon jour", enumValue);

				pTempESMS->setMetricValue(pTempESMS->getMetricValue().setValue("bye"));
				FutureInvocationState fis;
				consumer.commitState(*pTempESMS, fis);
				CHECK_EQUAL(true, fis.waitReceived(InvocationState::Fail, Tests::SimpleSDC::DEFAULT_TIMEOUT));
            }

            // Wait here and let the current value exceed max value. This will trigger alert condition presence which in turn
            // will trigger an alert signal presence (Off -> On -> Latch)!
            Poco::Thread::sleep(8000);

			{	// Set state test for a numeric metric state (must succeed, use state handle instead of descriptor handle)
            	std::unique_ptr<NumericMetricState> pTempNMS(consumer.requestState<NumericMetricState>(Tests::SimpleSDC::NUMERIC_METRIC_MAX_HANDLE));
				CHECK_EQUAL(true, pTempNMS != nullptr);

				// Here, we increase max weight to switch condition presence => results in alert signal presence
				pTempNMS->setMetricValue(pTempNMS->getMetricValue().setValue(10));
				FutureInvocationState fis;
				CHECK_EQUAL(true, InvocationState::Wait == consumer.commitState(*pTempNMS, fis));
				CHECK_EQUAL(true, fis.waitReceived(InvocationState::Fin, Tests::SimpleSDC::DEFAULT_TIMEOUT));
			}

            {	// Set state test for a string metric state (must succeed)
                DebugOut(DebugOut::Default, "SimpleSDC") << "String test...";
            	std::unique_ptr<StringMetricState> pTempNMS(consumer.requestState<StringMetricState>(Tests::SimpleSDC::STRING_METRIC_HANDLE));
				CHECK_EQUAL(true, pTempNMS != nullptr);

				pTempNMS->setMetricValue(pTempNMS->getMetricValue().setValue("Test2"));
				FutureInvocationState fis;
				CHECK_EQUAL(true, InvocationState::Wait == consumer.commitState(*pTempNMS, fis));
				CHECK_EQUAL(true, fis.waitReceived(InvocationState::Fin, Tests::SimpleSDC::DEFAULT_TIMEOUT));
            }

            {	// Activate test
                DebugOut(DebugOut::Default, "SimpleSDC") << "Activate test...";
                FutureInvocationState fis;
				CHECK_EQUAL(true, InvocationState::Wait == c->activate(Tests::SimpleSDC::CMD_HANDLE, fis));
				CHECK_EQUAL(true, fis.waitReceived(InvocationState::Fin, Tests::SimpleSDC::DEFAULT_TIMEOUT));
            }

            {	// Location context test
                DebugOut(DebugOut::Default, "SimpleSDC") << "Location context test...";
                // todo: check here if working!
                LocationContextState lcs(Tests::SimpleSDC::LOCATION_CONTEXT_HANDLE, "location_context_state");
                lcs.setContextAssociation(ContextAssociation::Assoc);
                lcs.addIdentification(InstanceIdentifier().setRoot("hello").setExtension("world"));
                FutureInvocationState fis;
                locationEventHandler.getEventEMR().reset();
                CHECK_EQUAL(true, InvocationState::Wait == consumer.commitState(lcs, fis));
				CHECK_EQUAL(true, locationEventHandler.getEventEMR().tryWait(3000));
				CHECK_EQUAL(true, fis.waitReceived(InvocationState::Fin, Tests::SimpleSDC::DEFAULT_TIMEOUT));
				DebugOut(DebugOut::Default, "SimpleSDC") << "Location context test done...";
            }
            {	// Patient context test
            	DebugOut(DebugOut::Default, "SimpleSDC") << "Patient context test...";
				PatientContextState pcs(Tests::SimpleSDC::PATIENT_CONTEXT_HANDLE, "patient_context_state");
				pcs.setContextAssociation(ContextAssociation::Assoc);
				pcs.addIdentification(InstanceIdentifier().setRoot("hello").setExtension("world"));
				pcs.setCoreData(PatientDemographicsCoreData()
						.setGivenname("Max")
						.setBirthname("")
						.setFamilyname("Mustermann"));
						// FIXME: DateOfBirth does not work yet -> schema validation fail because union is not rightly implemented
						//.setDateOfBirth("08.05.1945"));
				FutureInvocationState fis;
				patientEventHandler.getEventEMR().reset();
				CHECK_EQUAL(true, InvocationState::Wait == consumer.commitState(pcs, fis));

				CHECK_EQUAL(true, patientEventHandler.getEventEMR().tryWait(3000));

				CHECK_EQUAL(true, fis.waitReceived(InvocationState::Fin, Tests::SimpleSDC::DEFAULT_TIMEOUT));
				DebugOut(DebugOut::Default, "SimpleSDC") << "Patient context test done...";
			}
            // Run for some time to receive and display incoming metric events.
			Poco::Thread::sleep(5000);

			// Stop dummy events created by provider
			provider.interrupt();

			{	// Switch alert signal state off
            	std::unique_ptr<AlertSignalState> pTempASS(consumer.requestState<AlertSignalState>(Tests::SimpleSDC::ALERT_SIGNAL_HANDLE));
				CHECK_EQUAL(true, pTempASS != nullptr);


				pTempASS->setPresence(AlertSignalPresence::Off);
				FutureInvocationState fis;
				CHECK_EQUAL(true, InvocationState::Wait == consumer.commitState(*pTempASS, fis));
				CHECK_EQUAL(true, fis.waitReceived(InvocationState::Fin, Tests::SimpleSDC::DEFAULT_TIMEOUT));
			}

            Poco::Thread::sleep(5000);

            CHECK_EQUAL(true, eces1.getWeight() > 0);

            CHECK_EQUAL(true, eces1.getEventEMR().tryWait(Tests::SimpleSDC::DEFAULT_TIMEOUT));
            CHECK_EQUAL(true, eces2.getEventEMR().tryWait(Tests::SimpleSDC::DEFAULT_TIMEOUT));
            CHECK_EQUAL(true, eces3.getEventEMR().tryWait(Tests::SimpleSDC::DEFAULT_TIMEOUT));
            CHECK_EQUAL(true, eces4.getEventEMR().tryWait(Tests::SimpleSDC::DEFAULT_TIMEOUT));

            CHECK_EQUAL(true, alertSignalsink.getEventEAROff().tryWait(Tests::SimpleSDC::DEFAULT_TIMEOUT));
            CHECK_EQUAL(true, alertSignalsink.getEventEAROn().tryWait(Tests::SimpleSDC::DEFAULT_TIMEOUT));
            CHECK_EQUAL(true, latchingAlertSignalsink.getEventEARLatch().tryWait(Tests::SimpleSDC::DEFAULT_TIMEOUT));



            CHECK_EQUAL(true, consumer.unregisterStateEventHandler(&eces1));
            CHECK_EQUAL(true, consumer.unregisterStateEventHandler(&eces2));
            CHECK_EQUAL(true, consumer.unregisterStateEventHandler(&eces3));
            CHECK_EQUAL(true, consumer.unregisterStateEventHandler(&eces4));
            CHECK_EQUAL(true, consumer.unregisterStateEventHandler(&alertSignalsink));
            CHECK_EQUAL(true, consumer.unregisterStateEventHandler(&latchingAlertSignalsink));
            
            consumer.setContextStateChangedHandler(nullptr);

            DebugOut(DebugOut::Default, "SimpleSDC") << "Finished...";
            
            consumer.disconnect();
		}

        Poco::Thread::sleep(2000);
        provider.shutdown();
    } catch (char const* exc) {
		DebugOut(DebugOut::Default, std::cerr, "simpleoscp") << exc;
	} catch (...) {
		DebugOut(DebugOut::Default, std::cerr, "simpleoscp") << "Unknown exception occurred!";
	}
	DebugOut::closeLogFile();
}
}
