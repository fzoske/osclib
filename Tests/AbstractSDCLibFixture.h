/*
 * AbstractSDCLibFixture.h
 *
 *  Created on: 23.05.2014
 *      Author: roehser
 */

#ifndef AbstractSDCLibFixture_H_
#define AbstractSDCLibFixture_H_

#include <string>

#include "OSCLib/SDCLibrary.h"

#include "OSELib/Helper/WithLogger.h"

namespace SDCLib {
namespace Tests {

struct AbstractSDCLibFixture : public OSELib::WithLogger {
public:
	AbstractSDCLibFixture(const std::string & testname, OSELib::LogLevel debuglevel, int portStart) :
		WithLogger(OSELib::Log::BASE),
		testname(testname)
	{
		log_notice([&]{ return testname + ":  Startup."; });
		SDCLibrary::getInstance().startup(debuglevel);
        SDCLibrary::getInstance().createIncreasingPortList(portStart);
	}

	virtual ~AbstractSDCLibFixture() {
		SDCLibrary::getInstance().shutdown();
		log_notice([&]{ return testname + ":  Shutdown."; });
	}

private:
	const std::string testname;
};

}
}

#endif /* AbstractSDCLibFixture_H_ */
