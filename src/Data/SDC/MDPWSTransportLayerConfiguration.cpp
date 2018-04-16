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
#include "OSCLib/Data/SDC/MDPWSTransportLayerConfiguration.h"

namespace SDCLib {
namespace Data {
namespace SDC {

// default configuration
MDPWSTransportLayerConfiguration::MDPWSTransportLayerConfiguration() :
		data(new MDPWSTransportLayerConfigurationImpl()) {
}

MDPWSTransportLayerConfiguration::MDPWSTransportLayerConfiguration(const MDPWSTransportLayerConfiguration & obj) :
		data(obj.data) {
}

// do nothing. port is returned in data's destructor
MDPWSTransportLayerConfiguration::~MDPWSTransportLayerConfiguration() {
}

MDPWSTransportLayerConfiguration & MDPWSTransportLayerConfiguration::operator=(const MDPWSTransportLayerConfiguration & obj) {
	data = obj.data;
	return *this;
}

void MDPWSTransportLayerConfiguration::setBindAddress(const Poco::Net::IPAddress & bindAddress) {
	data->setBindAddress(bindAddress);
}

Poco::Net::IPAddress MDPWSTransportLayerConfiguration::getBindAddress() const{
	return data->getBindAddress();
}

unsigned int MDPWSTransportLayerConfiguration::getPort() const{
	return data->getPort();
}

void MDPWSTransportLayerConfiguration::setPort(const unsigned int port) {
	data->setPort(port);
}

bool MDPWSTransportLayerConfiguration::hasCustomPort() const {
	return data->hasCustomPort();
}
} /* namespace SDC */
} /* namespace Data */
} /* namespace SDCLib */
