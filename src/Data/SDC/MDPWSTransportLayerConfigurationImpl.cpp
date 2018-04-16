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
 *  @date 04.04.2018
 *  @author buerger
 *  @copyright (c) SurgiTAIX AG
 *
 */

#include "OSCLib/SDCLibrary.h"
#include "OSCLib/Data/SDC/MDPWSTransportLayerConfigurationImpl.h"

namespace SDCLib {
namespace Data {
namespace SDC {

// default configuration
MDPWSTransportLayerConfigurationImpl::MDPWSTransportLayerConfigurationImpl() :
		m_httpBindAddress(Poco::Net::IPAddress::Family::IPv4),
		customPortFlag(false) {
	m_port = SDCLib::SDCLibrary::getInstance().extractFreePort();
}

MDPWSTransportLayerConfigurationImpl::MDPWSTransportLayerConfigurationImpl(const MDPWSTransportLayerConfigurationImpl & obj) :
	m_httpBindAddress(obj.getBindAddress()),
	customPortFlag(obj.hasCustomPort()) {
	if (!customPortFlag) {
		m_port = SDCLib::SDCLibrary::getInstance().extractFreePort();
	} else {
		m_port = obj.getPort();
	}
}

MDPWSTransportLayerConfigurationImpl::~MDPWSTransportLayerConfigurationImpl() {
	if (!customPortFlag) {
		SDCLib::SDCLibrary::getInstance().returnPortToPool(m_port);
	}
}

void MDPWSTransportLayerConfigurationImpl::setBindAddress(const Poco::Net::IPAddress & bindAddress) {
	m_httpBindAddress = bindAddress;
}

Poco::Net::IPAddress MDPWSTransportLayerConfigurationImpl::getBindAddress() const{
	return m_httpBindAddress;
}

unsigned int MDPWSTransportLayerConfigurationImpl::getPort() const{
	return m_port;
}

void MDPWSTransportLayerConfigurationImpl::setPort(const unsigned int port) {
	// return the port to the pool
	if (!customPortFlag) {
		SDCLibrary::getInstance().returnPortToPool(m_port);
		customPortFlag = true;
	}
	m_port = port;
}

bool MDPWSTransportLayerConfigurationImpl::hasCustomPort() const {
	return customPortFlag;
}
} /* namespace SDC */
} /* namespace Data */
} /* namespace SDCLib */
