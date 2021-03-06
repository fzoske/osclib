/*
 * ExampleProvider.cpp
 *
 *  @Copyright (C) 2016, SurgiTAIX AG
 *  Author: buerger
 *
 *  This program sends a RealTimeSampleArrayMetricState (HANDLE_STREAM_METRIC) and a NumericMatricState (HANDLE_GET_METRIC) to the network. It changes the HANDLE_STREAM_METRIC
 *
 */

#include "OSCLib/SDCLibrary.h"
#include "OSCLib/Data/SDC/SDCProvider.h"

#include "OSCLib/Data/SDC/SDCProviderComponentStateHandler.h"
#include "OSCLib/Data/SDC/SDCProviderStateHandler.h"
#include "OSCLib/Data/SDC/SDCProviderMDStateHandler.h"
#include "OSCLib/Data/SDC/MDIB/MdsState.h"
#include "OSCLib/Data/SDC/MDIB/ChannelDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/CodedValue.h"
#include "OSCLib/Data/SDC/MDIB/SimpleTypesMapping.h"
#include "OSCLib/Data/SDC/MDIB/MdsDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/LocalizedText.h"
#include "OSCLib/Data/SDC/MDIB/MdDescription.h"
#include "OSCLib/Data/SDC/MDIB/MetricQuality.h"
#include "OSCLib/Data/SDC/MDIB/Range.h"
#include "OSCLib/Data/SDC/MDIB/RealTimeSampleArrayMetricDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/RealTimeSampleArrayMetricState.h"
#include "OSCLib/Data/SDC/MDIB/SampleArrayValue.h"
#include "OSCLib/Data/SDC/MDIB/NumericMetricState.h"
#include "OSCLib/Data/SDC/MDIB/NumericMetricValue.h"
#include "OSCLib/Data/SDC/MDIB/NumericMetricDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/StringMetricDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/StringMetricState.h"
#include "OSCLib/Data/SDC/MDIB/StringMetricValue.h"
#include "OSCLib/Data/SDC/MDIB/SystemContextDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/MetaData.h"
#include "OSCLib/Dev/DeviceCharacteristics.h"
#include "OSCLib/Data/SDC/MDIB/VmdDescriptor.h"
#include "OSCLib/Data/SDC/MDIB/custom/OperationInvocationContext.h"
#include "OSCLib/Data/SDC/MDPWSTransportLayerConfiguration.h"

#include "BICEPS_MessageModel-fwd.hxx"

#include "OSCLib/Util/DebugOut.h"
#include "OSCLib/Util/Task.h"

#include "OSELib/SDC/ServiceManager.h"

#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/ScopedLock.h"
#include "Poco/Thread.h"
#include "Poco/Net/IPAddress.h"

using namespace SDCLib;
using namespace SDCLib::Util;
using namespace SDCLib::Data::SDC;


const std::string DEVICE_EPR("UDI-1234567890");

const std::string VMD_DESCRIPTOR_HANDLE("holdingDevice_vmd");
const std::string CHANNEL_DESCRIPTOR_HANDLE("holdingDevice_channel");
const std::string MDS_DESCRIPTOR_HANDLE("holdingDevice_mds");

const std::string HANDLE_SET_METRIC("handle_set");
const std::string HANDLE_GET_METRIC("handle_get");
const std::string HANDLE_STREAM_METRIC("handle_stream");
const std::string HANDLE_STRING_METRIC("handle_string");


class NumericProviderStateHandlerGet : public SDCProviderMDStateHandler<NumericMetricState> {
public:

	NumericProviderStateHandlerGet(std::string descriptorHandle) : SDCProviderMDStateHandler(descriptorHandle) {
	}


	// Helper method
	NumericMetricState createState(double value) {
		NumericMetricState result(HANDLE_GET_METRIC);
		result
			.setMetricValue(NumericMetricValue(MetricQuality(MeasurementValidity::Vld)).setValue(value))
			.setActivationState(ComponentActivation::On);
		return result;
	}

	// define how to react on a request for a state change. This handler should not be set, thus always return Fail.
	InvocationState onStateChangeRequest(const NumericMetricState & state, const OperationInvocationContext & oic) override {
		// extract information from the incoming operation
		DebugOut(DebugOut::Default, "ExampleProvider") << "Operation invoked. Handle: " << oic.operationHandle << std::endl;
		return InvocationState::Fail;
	}


	NumericMetricState getInitialState() override {
		NumericMetricState nms = createState(42.0);
		return nms;
	}

	void setNumericValue(double value) {
		NumericMetricState nms = createState(value);
		updateState(nms);
	}
};



class NumericProviderStateHandlerSet : public SDCProviderMDStateHandler<NumericMetricState> {
public:

    NumericProviderStateHandlerSet(const std::string descriptorHandle) : SDCProviderMDStateHandler(descriptorHandle) {
    }

    InvocationState onStateChangeRequest(const NumericMetricState & state, const OperationInvocationContext & oic) override {
    	// Invocation has been fired as WAITING when entering this method
    	notifyOperationInvoked(oic, InvocationState::Start);
    	// Do stuff
        DebugOut(DebugOut::Default, "ExampleProvider") << "Provider: handle_set received state change request. State's value: " << state.getMetricValue().getValue() << std::endl;
        // extract information from the incoming operation
        DebugOut(DebugOut::Default, "ExampleProvider") << "Operation invoked. Handle: " << oic.operationHandle << std::endl;
        // if success return Finished
        return InvocationState::Fin;  // Framework will update internal MDIB with the state's value and increase MDIB version
    }

    // Helper method
    NumericMetricState createState() {
        NumericMetricState result(HANDLE_SET_METRIC);
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
        std::unique_ptr<NumericMetricState> result(getParentProvider().getMdState().findState<NumericMetricState>(HANDLE_SET_METRIC));

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


class StreamProviderStateHandler : public SDCProviderMDStateHandler<RealTimeSampleArrayMetricState> {
public:

    StreamProviderStateHandler(std::string descriptorHandle) : SDCProviderMDStateHandler(descriptorHandle) {
    }

    // Helper method
    RealTimeSampleArrayMetricState createState() {
        RealTimeSampleArrayMetricState realTimeSampleArrayState(descriptorHandle);
        realTimeSampleArrayState
            .setActivationState(ComponentActivation::On);
        return realTimeSampleArrayState;
    }


    RealTimeSampleArrayMetricState getInitialState() override {
        return createState();
    }

    // disallow set operation for this state
    InvocationState onStateChangeRequest(const RealTimeSampleArrayMetricState & state, const OperationInvocationContext & oic) override {
    	// extract information from the incoming operation
    	DebugOut(DebugOut::Default, "ExampleProvider") << "Operation invoked. Handle: " << oic.operationHandle << std::endl;
    	return InvocationState::Fail;
    }


    // convenient update method
    void updateStateValue(const SampleArrayValue & sav) {
        RealTimeSampleArrayMetricState realTimeSampleArrayState = createState();
        realTimeSampleArrayState
            .setMetricValue(sav);
        updateState(realTimeSampleArrayState);
    }
};

class StringProviderStateHandler : public SDCProviderMDStateHandler<StringMetricState> {
public:
	StringProviderStateHandler(std::string descriptorHandle) : SDCProviderMDStateHandler(descriptorHandle) {

	}

	InvocationState onStateChangeRequest(const StringMetricState & state, const OperationInvocationContext & oic) override {
		notifyOperationInvoked(oic, InvocationState::Start);
		// Do something if a state change is requested
		DebugOut(DebugOut::Default, "ExampleProvider") << "String state of provider state changed to " << state.getMetricValue().getValue() << std::endl;
		// return Finished if successful
		return InvocationState::Fin;
	}


	// Helper method
	StringMetricState createState() {
		StringMetricState result(HANDLE_STRING_METRIC);
		result
			.setMetricValue(StringMetricValue(MetricQuality(MeasurementValidity::Vld)).setValue("StringMetricValueInit"));

	    return result;
	}

	StringMetricState getInitialState() override {
		StringMetricState result = createState();
	        return result;
	    }

private:
	std::string descriptorHandle;
};


class OSCPStreamProvider : public Util::Task {
public:

    OSCPStreamProvider() :
    	sdcProvider(),
    	streamProviderStateHandler(HANDLE_STREAM_METRIC),
    	stringProviderStateHandler(HANDLE_STRING_METRIC),
    	numericProviderStateHandlerGet(HANDLE_GET_METRIC),
    	numericProviderStateHandlerSet(HANDLE_SET_METRIC),

    	streamMetricDescriptor(HANDLE_STREAM_METRIC,
    		    		CodedValue("MDCX_EXAMPLE_STREAM"),
    		    		MetricCategory::Msrmt,
    		    		MetricAvailability::Cont,
    		    		1,
    		    		xml_schema::Duration(0,0,0,0,0,0,1)),
		setMetricDescriptor(HANDLE_SET_METRIC,
						CodedValue("MDCX_EXAMPLE_SET"),
						MetricCategory::Set,
						MetricAvailability::Cont,
						1.0),
		getMetricDescriptor(HANDLE_GET_METRIC,
				CodedValue("MDCX_EXAMPLE_GET"),
				MetricCategory::Set,
				MetricAvailability::Cont,
				1),
		stringMetricDescriptor(HANDLE_STRING_METRIC,
						CodedValue("MDCX_EXAMPLE_STRING"),
						MetricCategory::Set,
						MetricAvailability::Cont)
    	{

		sdcProvider.setEndpointReference(DEVICE_EPR);
		Dev::DeviceCharacteristics devChar;
		devChar.addFriendlyName("en", "OSCLib ExampleProvider");
		sdcProvider.setDeviceCharacteristics(devChar);

		// feature: bind provider to a specific interface
		MDPWSTransportLayerConfiguration providerConfig = MDPWSTransportLayerConfiguration();
//		providerConfig.setBindAddress(Poco::Net::IPAddress("192.168.178.150"));
		providerConfig.setPort(6464);
		sdcProvider.setConfiguration(providerConfig);


        // Channel
        ChannelDescriptor holdingDeviceParameters(CHANNEL_DESCRIPTOR_HANDLE);
        holdingDeviceParameters
        	.setSafetyClassification(SafetyClassification::MedB)
			.addMetric(streamMetricDescriptor)
			.addMetric(setMetricDescriptor)
			.addMetric(getMetricDescriptor)
			.addMetric(stringMetricDescriptor);


        // VMD
        VmdDescriptor holdingDeviceModule(VMD_DESCRIPTOR_HANDLE);
        holdingDeviceModule
        	.addChannel(holdingDeviceParameters);

        // MDS
        MdsDescriptor holdingDeviceSystem(MDS_DESCRIPTOR_HANDLE);
        holdingDeviceSystem
        	.setType(CodedValue("MDC_DEV_DOCU_POSE_MDS")
        			.addConceptDescription(LocalizedText().setRef("uri/to/file.txt").setLang("en")))
        	.setMetaData(
        		MetaData().addManufacturer(LocalizedText().setRef("SurgiTAIX AG"))
        		.setModelNumber("1")
        		.addModelName(LocalizedText().setRef("EndoTAIX"))
        		.addSerialNumber("1234"))
        	.addVmd(holdingDeviceModule);



        sdcProvider.createSetOperationForDescriptor(setMetricDescriptor, holdingDeviceSystem);

        // create and add description
		MdDescription mdDescription;
		mdDescription.addMdsDescriptor(holdingDeviceSystem);

		sdcProvider.setMdDescription(mdDescription);

        // Add handler
		sdcProvider.addMdStateHandler(&numericProviderStateHandlerGet);
		sdcProvider.addMdStateHandler(&streamProviderStateHandler);

		sdcProvider.addMdStateHandler(&numericProviderStateHandlerSet);
		sdcProvider.addMdStateHandler(&stringProviderStateHandler);

    }

    void startup() {
    	sdcProvider.startup();
    }

    void shutdown() {
    	sdcProvider.shutdown();
    }

    void updateStateValue(const SampleArrayValue & sav) {
        streamProviderStateHandler.updateStateValue(sav); // updates handles and the parent provider
    }

private:

    SDCProvider sdcProvider;

    StreamProviderStateHandler streamProviderStateHandler;
    StringProviderStateHandler stringProviderStateHandler;
    NumericProviderStateHandlerGet numericProviderStateHandlerGet;
    NumericProviderStateHandlerSet numericProviderStateHandlerSet;


    RealTimeSampleArrayMetricDescriptor streamMetricDescriptor;
    NumericMetricDescriptor setMetricDescriptor;
    NumericMetricDescriptor getMetricDescriptor;
    StringMetricDescriptor  stringMetricDescriptor;


public:

    // Produce stream values
    // runImpl() gets called when starting the provider thread by the inherited function start()
    virtual void runImpl() override {

    	// RealTimeArray
		const std::size_t size(10);
		RealTimeValueType samples;
		for (std::size_t i = 0; i < size; i++) {
			samples.push_back(i);
		}
		long index(0);

		while (!isInterrupted()) {
			{
                updateStateValue(SampleArrayValue(MetricQuality(MeasurementValidity::Vld)).setSamples(samples));
                DebugOut(DebugOut::Default, "ExampleProvider") << "Produced stream chunk of size " << size << ", index " << index << std::endl;
			}


			// Update the NumericMetricState's value using the state handler's method
			numericProviderStateHandlerGet.setNumericValue(index/size);
			DebugOut(DebugOut::Default, "ExampleProvider") << "NumericMetric: value changed to " << index/size << std::endl;
			Poco::Thread::sleep(1000);
			index += size;

		}
    }
};



int main()
{
	// Startup
	DebugOut(DebugOut::Default, "ExampleProvider") << "Startup" << std::endl;
    SDCLibrary::getInstance().startup(OSELib::LogLevel::Notice);
    SDCLibrary::getInstance().setIP6enabled(false);
    SDCLibrary::getInstance().setIP4enabled(true);

	OSELib::SDC::ServiceManager oscpsm;
	OSCPStreamProvider provider;
	provider.startup();
	provider.start();

	std::string temp;
	DebugOut(DebugOut::Default, "ExampleProvider") << "Press key to exit program.";
	std::cin >> temp;

	// Shutdown
	DebugOut(DebugOut::Default, "ExampleProvider") << "Shutdown." << std::endl;
	provider.shutdown();
    SDCLibrary::getInstance().shutdown();
}
