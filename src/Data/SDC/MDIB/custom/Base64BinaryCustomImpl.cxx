
#include "OSCLib/Data/SDC/MDIB/Base64Binary.h"
#include "OSCLib/Data/SDC/MDIB/custom/ConvertFromCDM.h"
#include "OSCLib/Data/SDC/MDIB/custom/ConvertToCDM.h"

#include <memory>
#include <vector>

#include "osdm.hxx"

namespace SDCLib {
namespace Data {
namespace SDC {

std::vector<char> Base64Binary::getData() const {
	std::vector<char> result;
	if (data->size() == 0){
		return result;
	}
	result.reserve(data->size());
	for (std::size_t i = 0; i < data->size(); ++i) {
		result.push_back(data->data()[i]);
	}
	return result;
}

Base64Binary & Base64Binary::setData(const std::vector<char> & source) {
	if (source.empty()) {
		return *this;
	}

	std::unique_ptr<char []> buffer(new char [source.size()]);
	for (std::size_t i = 0; i < source.size(); ++i) {
		buffer[i] = source.at(i);
	}
	data.reset(new CDM::Base64Binary(buffer.get(), source.size()));
	return *this;
}

}
}
}
