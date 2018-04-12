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
 * TestSocketBinding.cpp
 *
 *  @Copyright (C) 2018, SurgiTAIX AG
 *  Author: buerger
 *
 *	This unit Tests is for testing the socket binding of each fundamental API class, the SDCProvider and the SDCConsumer.
 *	1. Default binding.
 *	2. Binding to a specific interface defined by its IP. This test might fail, if POCO is selecting the wrong interface.
 *	3. Binding to the a wrong interface (42.42.42.42). This test should fail.
 *	4. Binding to the same port. This should fail as well.
 *
 */
#include "OSCLib/SDCLibrary.h"
#include "OSCLib/Data/SDC/SDCConsumer.h"
#include "OSCLib/Data/SDC/SDCProvider.h"
#include "OSCLib/Data/SDC/MDPWSTransportLayerConfiguration.h"

#include "OSCLib/Util/DebugOut.h"
#include "../AbstractSDCLibFixture.h"
#include "../UnitTest++/src/UnitTest++.h"

#include "OSELib/SDC/ServiceManager.h"

#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/ScopedLock.h"
#include "Poco/Thread.h"

#include "Poco/Net/NetworkInterface.h"

using namespace SDCLib;
using namespace SDCLib::Util;
using namespace SDCLib::Data::SDC;

namespace SDCLib {
namespace Tests {
namespace SocketBindingSDC {

const std::string deviceEPR("UDI_SOCKETTEST");

class TestProvider {
public:
	TestProvider(std::string epr) : sdcProvider() {
		sdcProvider.setEndpointReference(epr);
	}

	void startup() {
		sdcProvider.startup();
	}

	void shutdown() {
		sdcProvider.shutdown();
	}

	void bindToInterface(Poco::Net::IPAddress ip) {
		MDPWSTransportLayerConfiguration config;
		config.setBindAddress(ip);
		sdcProvider.setConfiguration(config);
	}

	void bindToPort(unsigned int port) {
		MDPWSTransportLayerConfiguration config;
		config.setPort(port);
		sdcProvider.setConfiguration(config);
	}

	bool isRunning() {
		return sdcProvider.isRunning();
	}

private:
	SDCProvider sdcProvider;
};

void outputAvailablePorts()
{
	std::deque<unsigned int> portList = SDCLibrary::getInstance().getAvailablePorts();
	std::string portsAsString = "";
	for (auto port : portList) {
		portsAsString = portsAsString + ", " + std::to_string(port);

	}
	DebugOut(DebugOut::Default, "TestSocketBinding") << "Available Ports: " << portsAsString;
	DebugOut(DebugOut::Default, "TestSocketBinding") << "Total lenght: " + std::to_string(portList.size());
}

}
}
}

struct FixtureSocketBindingSDC : Tests::AbstractSDCLibFixture {
	FixtureSocketBindingSDC() : AbstractSDCLibFixture("FixturesSocketBindingSDC", OSELib::LogLevel::Debug, 10000) {}
};




SUITE(OSCP) {
TEST_FIXTURE(FixtureSocketBindingSDC, socketbindingsdc)
{
	DebugOut::openLogFile("TestSocketBinding.log.txt", true);
	// Define open ports
	{
		std::deque<unsigned int> portList;
		for (unsigned int i = 2000; i < 2010; i++) {
			portList.push_back(i);
		}
		SDCLibrary::getInstance().setPortList(portList);
	}
	SDCLib::Tests::SocketBindingSDC::outputAvailablePorts();
	try
	{
		OSELib::SDC::ServiceManager oscpsm;
		//
		// 1. Default binding
		//
		{
        // Provider
			std::unique_ptr<SDCLib::Tests::SocketBindingSDC::TestProvider> testProvider(new SDCLib::Tests::SocketBindingSDC::TestProvider(SDCLib::Tests::SocketBindingSDC::deviceEPR));

			DebugOut(DebugOut::Default, "TestSocketBinding") << "1. Test: Default binding" << std::endl;
			DebugOut(DebugOut::Default, "TestSocketBinding") << "Provider init.." << std::endl;
			testProvider->startup();
			CHECK_EQUAL(true, testProvider->isRunning());

			// Consumer
			DebugOut(DebugOut::Default, "TestSocketBinding") << "Consumer discovery..." << std::endl;
			std::shared_ptr<SDCConsumer> c(oscpsm.discoverEndpointReference(SDCLib::Tests::SocketBindingSDC::deviceEPR));

			SDCLib::Tests::SocketBindingSDC::outputAvailablePorts();
			// Discovery test
			CHECK_EQUAL(true, c != nullptr);
			if (c) {
				c->disconnect();
				c.reset();
			}
			SDCLib::Tests::SocketBindingSDC::outputAvailablePorts();

			DebugOut(DebugOut::Default, "TestSocketBinding") << "Provider shutdown." << std::endl;
			testProvider->shutdown();
			testProvider.reset();
			SDCLib::Tests::SocketBindingSDC::outputAvailablePorts();
		}

        //
		// 2. Binding to a specific valid interface defined by its IP. This test might fail, if POCO is selecting the wrong interface.
		//
		{
			DebugOut(DebugOut::Default, "TestSocketBinding") << "2. Test: Binding to a specific valid interface defined by its IP. " << std::endl;

			// Provider, reinit
			std::unique_ptr<SDCLib::Tests::SocketBindingSDC::TestProvider> testProvider = std::unique_ptr<SDCLib::Tests::SocketBindingSDC::TestProvider>(new SDCLib::Tests::SocketBindingSDC::TestProvider(SDCLib::Tests::SocketBindingSDC::deviceEPR));

			// binding to localhost is not possible, thus connect to real interface's IP Address
			Poco::Net::NetworkInterface netInterface;
			Poco::Net::NetworkInterface::List interfaceList = netInterface.list(true,true);
			Poco::Net::IPAddress addr;
			for (auto interface : interfaceList){
				if (!interface.isLoopback()){
					addr = interface.address(0);
					break;
				}
			}
			DebugOut(DebugOut::Default, "TestSocketBinding") << "Trying to bind to interface: " << addr.toString() << std::endl;
			DebugOut(DebugOut::Default, "TestSocketBinding") << "Provider init.." << std::endl;
			testProvider->bindToInterface(addr);
			testProvider->startup();
			CHECK_EQUAL(true, testProvider->isRunning());

			// Consumer
			DebugOut(DebugOut::Default, "TestSocketBinding") << "Consumer discovery..." << std::endl;
			MDPWSTransportLayerConfiguration config;
			config.setBindAddress(addr);
			std::shared_ptr<SDCConsumer> c = std::shared_ptr<SDCConsumer>(oscpsm.discoverEndpointReference(SDCLib::Tests::SocketBindingSDC::deviceEPR, config));

			// Discovery test
			CHECK_EQUAL(true, c != nullptr);
			if (c) {
				c->disconnect();
				c.reset();
			}

			DebugOut(DebugOut::Default, "TestSocketBinding") << "Provider shutdown." << std::endl;
			testProvider->shutdown();
			testProvider.reset();
		}

		//
		// 3. Binding to the a wrong interface (42.42.42.42). This test should fail.
		//
		{
			DebugOut(DebugOut::Default, "TestSocketBinding") << "3. Test: Binding to the a wrong interface (42.42.42.42). " << std::endl;

			// Provider, reinit
			std::unique_ptr<SDCLib::Tests::SocketBindingSDC::TestProvider> testProvider = std::unique_ptr<SDCLib::Tests::SocketBindingSDC::TestProvider>(new SDCLib::Tests::SocketBindingSDC::TestProvider(SDCLib::Tests::SocketBindingSDC::deviceEPR));

			// binding to localhost is not possible, thus connect to real interface's IP Address
			const Poco::Net::IPAddress addr = Poco::Net::IPAddress("42.42.42.42");
			DebugOut(DebugOut::Default, "TestSocketBinding") << "Trying to bind to interface: " << addr.toString() << std::endl;
			DebugOut(DebugOut::Default, "TestSocketBinding") << "Provider init.." << std::endl;
			testProvider->bindToInterface(addr);
			testProvider->startup();
			CHECK_EQUAL(false, testProvider->isRunning());


			// Consumer
			DebugOut(DebugOut::Default, "TestSocketBinding") << "Consumer discovery..." << std::endl;
			MDPWSTransportLayerConfiguration config;
			config.setBindAddress(addr);
			std::shared_ptr<SDCConsumer> c = std::shared_ptr<SDCConsumer>(oscpsm.discoverEndpointReference(SDCLib::Tests::SocketBindingSDC::deviceEPR, config));

			// Check if discovery failed
			CHECK_EQUAL(true, c == nullptr);
			if (c) {
				c->disconnect();
				c.reset();
			}

			DebugOut(DebugOut::Default, "TestSocketBinding") << "Provider shutdown." << std::endl;
			testProvider->shutdown();
			testProvider.reset();
		}

		//
		// 4. Binding to the same port. This should fail as well.
		//
		{
			DebugOut(DebugOut::Default, "TestSocketBinding") << "4. Test: Binding to the same port (6464, sdc hosted reserved port). This should fail as well. " << std::endl;

			// Provider, reinit
			std::unique_ptr<SDCLib::Tests::SocketBindingSDC::TestProvider> testProvider = std::unique_ptr<SDCLib::Tests::SocketBindingSDC::TestProvider>(new SDCLib::Tests::SocketBindingSDC::TestProvider(SDCLib::Tests::SocketBindingSDC::deviceEPR));

			// binding to localhost is not possible, thus connect to real interface's IP Address
			DebugOut(DebugOut::Default, "TestSocketBinding") << "Trying to bind to port: " << "6464" << std::endl;
			DebugOut(DebugOut::Default, "TestSocketBinding") << "Provider init.." << std::endl;
			// use default interface but custom port
			testProvider->bindToPort(6464);
			testProvider->startup();
			CHECK_EQUAL(true, testProvider->isRunning());


			// Consumer
			DebugOut(DebugOut::Default, "TestSocketBinding") << "Consumer discovery..." << std::endl;
			// use default interface but custom port
			MDPWSTransportLayerConfiguration config;
			config.setPort(6464);
			std::shared_ptr<SDCConsumer> c = std::shared_ptr<SDCConsumer>(oscpsm.discoverEndpointReference(SDCLib::Tests::SocketBindingSDC::deviceEPR, config));

			// Check if discovery failed
			CHECK_EQUAL(true, c == nullptr);
			if (c) {
				c->disconnect();
				c.reset();
			}

			DebugOut(DebugOut::Default, "TestSocketBinding") << "Provider shutdown." << std::endl;
			testProvider->shutdown();
			testProvider.reset();
		}

	}
	catch (char const* exc)
	{
		DebugOut(DebugOut::Default, std::cerr, "TestSocketBinding") << exc;
	}    
	catch (...)
	{
		DebugOut(DebugOut::Default, std::cerr, "TestSocketBinding") << "Unknown exception occurred!";
	}
	DebugOut::closeLogFile();
}
}
