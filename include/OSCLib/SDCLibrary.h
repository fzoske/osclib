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
 *  @file SDCLibrary.h
 *  @project OSCLib
 *  @date 24.08.2011
 *  @author besting
 *  @copyright (c) SurgiTAIX AG
 *
 */

#ifndef OSCLIB_H_
#define OSCLIB_H_

#include <atomic>
#include <list>
#include <memory>
#include <set>
#include <string>

#include "Poco/Mutex.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/NotificationQueue.h"
#include "Poco/Net/IPAddress.h"

#include "OSELib/fwd.h"
#include "OSELib/Helper/WithLogger.h"

namespace SDCLib {



class SDCLibrary final : public OSELib::WithLogger {
public:

	SDCLibrary();
	~SDCLibrary();

	static SDCLibrary & getInstance();

	/**
	 * Startup framework.
	 *
	 * @param debugLevel The debug output level.
	 */
	void startup(OSELib::LogLevel debugLevel = OSELib::LogLevel::Error);

	/**
	 * Shutdown framework.
	 *
	 */
	void shutdown();

	bool isInitialized();

	/**
	 * Define which specific ports are free to use for the system.
	 *
	 * @param portList The list of free ports
	 */
	void setPortList(std::deque<unsigned int> portList);

	/**
	 * Define which specific ports are free to use for the system.
	 *
	 * @param portList The list of free ports
	 */
	std::deque<unsigned int> getAvailablePorts();

	/**
	 * Autocreates a list of free port given by the starting number and the range.
	 * The port number are just increased by one for portRange times.
	 *
	 * @param portStart The next free port number to use
	 */
	void createIncreasingPortList(unsigned int portStart, unsigned int portRange = 1000);

	/**
	 * Get the next global free port number used for bindings out of a list of free ports.
	 * 	 *
	 * @return The next free port number to use
	 */
	unsigned int extractFreePort();
	/**
	 * Returns the port to the list of free ports if the port was in the initialized list before.
	 *
	 *  @params port The port to return
	 */
	void returnPortToPool(unsigned int port);

	/**
	 * Check if some ports are bound to sockets.
	 *
	 * @return The status if some ports are bound.
	 */
	bool isPortListInUse();

	void dumpPingManager(std::unique_ptr<OSELib::DPWS::PingManager> pingManager);

	void setIP4enabled(bool IP4enabled);
	void setIP6enabled(bool IP6enabled);
	bool getIP4enabled();
	bool getIP6enabled();

    /**
    * @brief Set the time the service manager waits for the device discovery
    *
    * @param discoveryTimeSec The time in milliseconds to wait while discovery
    */
	void setDiscoveryTime(int discoveryTimeMilSec);

    /**
    * @brief Get the time the service manager waits for the device discovery
    *
    * @return The time in milliseconds to wait for discovery
    */
	int getDiscoveryTime();
private:

	bool m_IP4enabled;
	bool m_IP6enabled;
	bool initialized;
	int m_discoveryTimeMilSec;
	bool portListInUseStatus;

	void createPortLists(unsigned int portStart, unsigned int portRange = 1000);
	std::deque<unsigned int> reservedPorts;
	std::deque<unsigned int> availablePorts;

	Poco::Mutex mutex;
	std::unique_ptr<OSELib::DPWS::PingManager> _latestPingManager;

	// declaring copy-methods prevents from initializing multiple copies of singleton
	SDCLibrary(SDCLibrary const&);
	void operator=(SDCLibrary const&);
};

} /* namespace SDCLib */
#endif /* OSCLIB_H_ */
