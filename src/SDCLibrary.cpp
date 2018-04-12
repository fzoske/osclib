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

/**
 *  @file SDCLibrary.cpp
 *  @project OSCLib
 *  @date 24.08.2011
 *  @author besting, buerger
 *  @copyright (c) SurgiTAIX AG
 *
 */

#include "Poco/ConsoleChannel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/Notification.h"
#include "Poco/PatternFormatter.h"
#include "Poco/ScopedLock.h"
#include "Poco/SimpleFileChannel.h"
#include "Poco/SingletonHolder.h"
#include "Poco/SplitterChannel.h"

#include <xercesc/util/PlatformUtils.hpp>

#include "OSCLib/SDCLibrary.h"
#include "OSCLib/Data/SDC/SDCConsumer.h"
#include "OSCLib/Data/SDC/SDCProvider.h"

#include "OSELib/DPWS/PingManager.h"

namespace SDCLib {

const std::string CURRENT_LIB_VERSION("3.0.3");

SDCLibrary::SDCLibrary() :
	WithLogger(OSELib::Log::BASE),
	initialized(false),
	m_IP4enabled(true),
	m_IP6enabled(true),
	m_discoveryTimeMilSec(5000),
	portListInUseStatus(false)
{
	Poco::AutoPtr<Poco::ConsoleChannel> consoleChannel(new Poco::ConsoleChannel);
	Poco::AutoPtr<Poco::SimpleFileChannel> fileChannel(new Poco::SimpleFileChannel);
	Poco::AutoPtr<Poco::SplitterChannel> splitterChannel(new Poco::SplitterChannel);
	fileChannel->setProperty("path", "osclib.log");
	fileChannel->setProperty("rotation", "10 M");
	splitterChannel->addChannel(consoleChannel);
	splitterChannel->addChannel(fileChannel);

	Poco::AutoPtr<Poco::PatternFormatter> patternFormatter(new Poco::PatternFormatter);
	patternFormatter->setProperty("pattern", "%q %H:%M:%S.%i %s:\n  %t");
	Poco::AutoPtr<Poco::FormattingChannel> formattingChannel(new Poco::FormattingChannel(patternFormatter, splitterChannel));

	getLogger().setChannel(formattingChannel);

	createPortLists(5000, 1000);
}

SDCLibrary::~SDCLibrary() {
    shutdown();
}

SDCLibrary & SDCLibrary::getInstance() {
	static Poco::SingletonHolder<SDCLibrary> singletonHolder;
	return *singletonHolder.get();
}

void SDCLibrary::startup(OSELib::LogLevel debugLevel) {
	if (!initialized) {
		initialized = true;
		setDebugLevel(debugLevel);
		log_notice([&]{ return "SDCLib version " + CURRENT_LIB_VERSION + " (C) 2018 SurgiTAIX AG"; });
        xercesc::XMLPlatformUtils::Initialize();
	} else {
		log_error([&]{ return "SDCLib already initialized!"; });
	}
}

void SDCLibrary::shutdown() {
	if (initialized) {
        initialized = false;
        _latestPingManager.reset();
        xercesc::XMLPlatformUtils::Terminate();
	}
}


void SDCLibrary::setPortList(std::deque<unsigned int> portList) {
	Poco::Mutex::ScopedLock lock(mutex);

	if (!isPortListInUse()) {
		reservedPorts.clear();
		reservedPorts = portList;
		availablePorts.clear();
		availablePorts = portList;
	} else {
		log_error([&] {return "Some ports are already in use (bound to sockets). Operation not possible. Please restart the whole library to change port list.";});
	}
}

std::deque<unsigned int> SDCLibrary::getAvailablePorts() {
	return availablePorts;
}

void SDCLibrary::createIncreasingPortList(unsigned int start, unsigned int range) {
	Poco::Mutex::ScopedLock lock(mutex);

	if (!isPortListInUse()) {
		const auto end(start + range);
		log_information([&]{ return "Using ports: [" + std::to_string(start) + "," + std::to_string(end) + ")"; });

		createPortLists(start, range);
	} else {
		log_error([&] {return "Some ports are already in use (bound to sockets). Operation not possible. Please restart the whole library to change port list.";});
	}
}

void SDCLibrary::createPortLists(unsigned int start, unsigned int range) {
	reservedPorts.clear();
	for (unsigned int i = start; i < start + range; i++) {
		reservedPorts.push_back(i);
	}
	availablePorts = reservedPorts;
}

unsigned int SDCLibrary::extractFreePort() {
	Poco::Mutex::ScopedLock lock(mutex);
	const unsigned int result(availablePorts.front());
	availablePorts.pop_front();
	portListInUseStatus = true;
	return result;
}

void SDCLibrary::returnPortToPool(unsigned int port) {
	Poco::Mutex::ScopedLock lock(mutex);
	if (std::find(reservedPorts.begin(), reservedPorts.end(), port) != reservedPorts.end()) {
		// no doubling ports if user tries to return a port already returned
		if (std::find(availablePorts.begin(), availablePorts.end(), port) == availablePorts.end()) {
			// pushed to back to leave some time if a socket is opened shortly after again
			availablePorts.push_back(port);
		}
	}
}

bool SDCLibrary::isPortListInUse() {
	return portListInUseStatus;
}

void SDCLibrary::dumpPingManager(std::unique_ptr<OSELib::DPWS::PingManager> pingManager) {
	Poco::Mutex::ScopedLock lock(mutex);
	_latestPingManager = std::move(pingManager);
}

bool SDCLibrary::isInitialized() {
	return initialized;
}

void SDCLibrary::setIP4enabled(bool IP4enabled) {
	m_IP4enabled = IP4enabled;
}

void SDCLibrary::setIP6enabled(bool IP6enabled) {
	m_IP6enabled = IP6enabled;
}

bool SDCLibrary::getIP4enabled() {
	return m_IP4enabled;
}

bool SDCLibrary::getIP6enabled() {
	return m_IP6enabled;
}


void SDCLibrary::setDiscoveryTime(int discoveryTimeMilSec){
	m_discoveryTimeMilSec = discoveryTimeMilSec;
}

int SDCLibrary::getDiscoveryTime(){
	return m_discoveryTimeMilSec;
}

} /* namespace SDCLib */
