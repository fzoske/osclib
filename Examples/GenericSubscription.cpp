
#include "OSCLib/SDCLibrary.h"
#include "OSCLib/Data/SDC/MDIB/ConvertToCDM.h"
#include "OSCLib/Data/SDC/MDIB/custom/MdibContainer.h"
#include "OSCLib/Data/SDC/MDIB/MdsDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/AlertConditionDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/AlertConditionState.h"
#include "OSCLib/Data/SDC/MDIB/AlertSignalDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/AlertSignalState.h"
#include "OSCLib/Data/SDC/MDIB/AlertSystemDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/AlertSystemState.h"
#include "OSCLib/Data/SDC/MDIB/ChannelDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/ClockDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/ClockState.h"
#include "OSCLib/Data/SDC/MDIB/EnumStringMetricDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/EnumStringMetricState.h"
#include "OSCLib/Data/SDC/MDIB/MdsDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/MdsState.h"
#include "OSCLib/Data/SDC/MDIB/LimitAlertConditionDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/LimitAlertConditionState.h"
#include "OSCLib/Data/SDC/MDIB/MdDescription.h"
#include "OSCLib/Data/SDC/MDIB/NumericMetricDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/NumericMetricState.h"
#include "OSCLib/Data/SDC/MDIB/NumericMetricValue.h"
#include "OSCLib/Data/SDC/MDIB/StringMetricDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/StringMetricState.h"
#include "OSCLib/Data/SDC/MDIB/StringMetricValue.h"
#include "OSCLib/Data/SDC/MDIB/RealTimeSampleArrayMetricDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/RealTimeSampleArrayMetricState.h"
#include "OSCLib/Data/SDC/MDIB/Real
#include "OSCLib/Data/SDC/MDIB/RTValueType.h"
#include "OSCLib/Data/SDC/MDIB/VMDDescriptor.h"
#include "OSELib/SDC/SDCConstants.h
#include "OSCLib/Data/SDC/SDCConsumer.h"
#include "OSCLib/Data/SDC/SDCConsumerAlertConditionStateHandler.h"
#include "OSCLib/Data/SDC/SDCConsumerAlertSignalStateHandler.h"
#include "OSCLib/Data/SDC/SDCConsumerEnumStringMetricStateHandler.h"
#include "OSCLib/Data/SDC/SDCConsumerLimitAlertConditionStateHandler.h"
#include "OSCLib/Data/SDC/SDCConsumerNumericMetricStateHandler.h"
#include "OSCLib/Data/SDC/SDCConsumerRealTimeSampleArrayMetricStateHandler.h"
#include "OSCLib/Data/SDC/SDCConsumerStringMetricStateHandler.h"
#include "OSCLib/Data/SDC/SDCProvider.h"
#include "OSCLib/Util/DebugOut.h"

#include "OSELib/SDC/ServiceManager.h"

#include "osdm.hxx"

#include "Poco/Thread.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"

#include "tclap/CmdLine.h"
#include "tclap/UnlabeledMultiArg.h"
#include "tclap/ValueArg.h"

using namespace SDCLib;
using namespace SDCLib::Util;
using namespace SDCLib::Data::SDC;

std::string stateTypeAsString(const AlertConditionState & ) {
	return "AlertConditionState";
}

std::string stateTypeAsString(const AlertSignalState & ) {
	return "AlertSignalState";
}

std::string stateTypeAsString(const EnumStringMetricState & ) {
	return "EnumStringMetricState";
}

std::string stateTypeAsString(const LimitAlertConditionState & ) {
	return "LimitAlertConditionState";
}

std::string stateTypeAsString(const NumericMetricState & ) {
	return "NumericMetricState";
}

std::string stateTypeAsString(const RealTimeSampleArrayMetricState & ) {
	return "RealTimeSampleArrayMetricState";
}

std::string stateTypeAsString(const StringMetricState & ) {
	return "StringMetricState";
}

template<class State>
std::string printPayload(const State & state) {
	return "Printing payload not implemented for " + stateTypeAsString(state);
}

template<>
std::string printPayload(const EnumStringMetricState & state) {
	if (!state.hasObservedValue()) {
		return "no observed value";
	}

	const StringMetricValue value(state.getObservedValue());
	if (!value.hasValue()) {
		return "no value";
	}

	return "Value: " + value.getValue();
}

template<>
std::string printPayload(const NumericMetricState & state) {
	if (!state.hasObservedValue()) {
		return "no observed value";
	}

	const NumericMetricValue value(state.getObservedValue());
	if (!value.hasValue()) {
		return "no value";
	}

	return "Value: " + std::to_string(value.getValue());
}

template<>
std::string printPayload(const StringMetricState & state) {
	if (!state.hasObservedValue()) {
		return "no observed value";
	}

	const StringMetricValue value(state.getObservedValue());
	if (!value.hasValue()) {
		return "no value";
	}

	return "Value: " + value.getValue();
}

template<>
std::string printPayload(const RealTimeSampleArrayMetricState & state) {
	if (!state.hasObservedValue()) {
		return "no observed value";
	}

	const RealTimeSampleArrayValue value(state.getObservedValue());
	if (!value.hasSamples()) {
		return "no samples";
	}

	std::string result("Values: ");
	for (const auto & v : value.getSamples().getValues()) {
		result += std::to_string(v) + " ";
	}
	return result;
}

template<class ConsumerHandlerType>
class Handler : public ConsumerHandlerType {
public:
	Handler(const std::string & handle) :
		_handle(handle)
	{
	}

    virtual ~Handler() {
    }

    void onStateChanged(const typename ConsumerHandlerType::StateType & state) override {
        DebugOut(DebugOut::Default, "GenericSubscription")
        		<< "Metric event for " << stateTypeAsString(state) << " with handle " << _handle << " and payload:" << std::endl
				<< printPayload(state);
    }

    std::string getHandle() override {
        return _handle;
    }

private:
    const std::string _handle;
};

int main (int argc, char * argv[])
{
	std::string epr;
	std::vector<std::string> handles;

	try {
		TCLAP::CmdLine cmd("Generic subscription tool for various value types.", ' ', "0.01");
		TCLAP::ValueArg<std::string> eprArg("e", "epr", "EndpointReference of target OSCP Device.", true, "UDI_EXAMPLE_1234", "string");
		cmd.add(eprArg);
		TCLAP::UnlabeledMultiArg<std::string> handlesArg("handle", "Handle of metric that should be subscribed to.", true, "string");
		cmd.add(handlesArg);

		cmd.parse( argc, argv );

		epr = eprArg.getValue();
		handles = handlesArg.getValue();

		if (epr.empty()) {
			std::cerr << "EPR must not be empty. " << std::endl;
			std::exit(0);
		}

	} catch (TCLAP::ArgException & e) {
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
		std::exit(0);
	}

	DebugOut(DebugOut::Default, "GenericSubscription") << std::endl << "Startup";
	SDCLibrary::getInstance().startup();
	SDCLibrary::getInstance().createIncreasingPortList(42000);

	OSELib::SDC::ServiceManager oscpsm;
	std::unique_ptr<SDCConsumer> consumer(oscpsm.discoverEndpointReference(epr));
	if (consumer) {
		DebugOut(DebugOut::Default, "GenericSubscription") << "Connected to device with epr: " << epr;
		const MDDescription mdd(consumer->getMdDescription());

		std::vector<std::unique_ptr<SDCConsumerOperationInvokedHandler>> handlers;

		for (const auto & handle : handles) {
			bool match(false);
			{
				AlertConditionDescriptor descriptor;
				if (mdd.findDescriptor(handle, descriptor)) {
					match = true;
					std::unique_ptr<SDCConsumerOperationInvokedHandler> handler(new Handler<SDCConsumerAlertConditionStateHandler>(handle));
					consumer->registerStateEventHandler(handler.get());
					handlers.push_back(std::move(handler));
				}
			}
			{
				AlertSignalDescriptor descriptor;
				if (mdd.findDescriptor(handle, descriptor)) {
					match = true;
					std::unique_ptr<SDCConsumerOperationInvokedHandler> handler(new Handler<SDCConsumerAlertSignalStateHandler>(handle));
					consumer->registerStateEventHandler(handler.get());
					handlers.push_back(std::move(handler));
				}
			}
			{
				EnumStringMetricDescriptor descriptor;
				if (mdd.findDescriptor(handle, descriptor)) {
					match = true;
					std::unique_ptr<SDCConsumerOperationInvokedHandler> handler(new Handler<SDCConsumerEnumStringMetricStateHandler>(handle));
					consumer->registerStateEventHandler(handler.get());
					handlers.push_back(std::move(handler));
				}
			}
			{
				LimitAlertConditionDescriptor descriptor;
				if (mdd.findDescriptor(handle, descriptor)) {
					match = true;
					std::unique_ptr<SDCConsumerOperationInvokedHandler> handler(new Handler<SDCConsumerLimitAlertConditionStateHandler>(handle));
					consumer->registerStateEventHandler(handler.get());
					handlers.push_back(std::move(handler));
				}
			}
			{
				NumericMetricDescriptor descriptor;
				if (mdd.findDescriptor(handle, descriptor)) {
					match = true;
					std::unique_ptr<SDCConsumerOperationInvokedHandler> handler(new Handler<SDCConsumerNumericMetricStateHandler>(handle));
					consumer->registerStateEventHandler(handler.get());
					handlers.push_back(std::move(handler));
				}
			}
			{
				RealTimeSampleArrayMetricDescriptor descriptor;
				if (mdd.findDescriptor(handle, descriptor)) {
					match = true;
					std::unique_ptr<SDCConsumerOperationInvokedHandler> handler(new Handler<SDCConsumerRealTimeSampleArrayMetricStateHandler>(handle));
					consumer->registerStateEventHandler(handler.get());
					handlers.push_back(std::move(handler));
				}
			}
			{
				StringMetricDescriptor descriptor;
				if (mdd.findDescriptor(handle, descriptor)) {
					match = true;
					std::unique_ptr<SDCConsumerOperationInvokedHandler> handler(new Handler<SDCConsumerStringMetricStateHandler>(handle));
					consumer->registerStateEventHandler(handler.get());
					handlers.push_back(std::move(handler));
				}
			}
			if (!match) {
				DebugOut(DebugOut::Default, "GenericSubscription") << "Could not finde descriptor for handle : " << handle << epr;
			}
		}

		std::string temp;
		std::cin >> temp;

		for (auto & handler : handlers) {
			consumer->unregisterStateEventHandler(handler.get());
		}
	} else {
		DebugOut(DebugOut::Default, "GenericSubscription") << "Connection failed for epr: " << epr;
	}

	SDCLibrary::getInstance().shutdown();
	DebugOut(DebugOut::Default, "GenericSubscription") << "Shutdown" << std::endl;
}

