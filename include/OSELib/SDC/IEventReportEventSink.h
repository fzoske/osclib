/*
 * IEventReportEventSink.h
 *
 *  Created on: 10.12.2015
 *      Author: matthias
 */

#ifndef DATA_OSCP_IEVENTREPORTEVENTSINK_H_
#define DATA_OSCP_IEVENTREPORTEVENTSINK_H_

#include "OSELib/fwd.h"
#include "OSELib/DPWS/IEventSink.h"
#include "OSELib/SDC/ReportTraits.h"

namespace OSELib {
namespace SDC {

class IEventReportEventSink : public DPWS::IEventSink {
public:
	virtual ~IEventReportEventSink() = default;

	virtual void dispatch(const EpisodicAlertReportTraits::ReportType & request) = 0;
	virtual void dispatch(const EpisodicMetricReportTraits::ReportType & request) = 0;
	virtual void dispatch(const PeriodicAlertReportTraits::ReportType & request) = 0;
	virtual void dispatch(const PeriodicMetricReportTraits::ReportType & request) = 0;
	virtual void dispatch(const OperationInvokedReportTraits::ReportType & request) = 0;
	virtual void dispatch(const WaveformStreamTraits::ReportType & request) = 0;
};

} /* namespace SDC */
} /* namespace OSELib */

#endif /* DATA_OSCP_IEVENTREPORTEVENTSINK_H_ */
