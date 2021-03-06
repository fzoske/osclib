/*
 * OperationInvocationContext.h
 *
 *  Created on: 05.05.2015
 *      Author: roehser
 */

#ifndef DATA_OSCP_MDIB_OPERATIONINVOCATIONCONTEXT_H_
#define DATA_OSCP_MDIB_OPERATIONINVOCATIONCONTEXT_H_

#include <string>

namespace SDCLib {
namespace Data {
namespace SDC {

class OperationInvocationContext {
public:

	static OperationInvocationContext none() {
		return OperationInvocationContext("", 0);
	}

	OperationInvocationContext(const std::string & operationHandle, const unsigned int transactionId);
	virtual ~OperationInvocationContext();

	const std::string operationHandle;
	const unsigned int transactionId;
};

} /* namespace SDC */
} /* namespace Data */
} /* namespace SDCLib */

#endif /* DATA_OSCP_MDIB_OPERATIONINVOCATIONCONTEXT_H_ */
