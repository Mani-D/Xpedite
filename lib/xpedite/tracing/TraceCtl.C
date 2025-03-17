///////////////////////////////////////////////////////////////////////////////
//
// TraceCtl - Logic to program and track kernel tracepoints using linux perf api
//
// Author: Manikandan Dhamodharan, Morgan Stanley
//
///////////////////////////////////////////////////////////////////////////////

#include <xpedite/tracing/TraceCtl.H>
#include <xpedite/tracing/Request.H>
#include <xpedite/perf/PerfRecords.H>
#include <iostream>

namespace xpedite { namespace tracing {

  TraceCtl TraceCtl::_instance {};

  TraceCtl::TraceCtl() {
  }

  bool TraceCtl::enable(const Request& request_) {
    auto eventAttrs = perf::buildPerfTraceAttrs(request_.traces());
    PerfEventSetMap perfEventSetMap {};
    return PerfEventsCtl::enable(eventAttrs, perfEventSetMap);
  }

  void TraceCtl::disable() noexcept {
    PerfEventsCtl::disable();
  }

  void TraceCtl::poll() {
    const auto SAMPLES_BUFFER_SIZE = (1 << 4) * getpagesize();
		std::cout << "Active events count - " << activeEvents().size() << std::endl;
    for(auto& kv : activeEvents()) {
      auto& eventSet = kv.second;
      auto buffer = eventSet->buffer();
#if 0
      std::cout << "page - " << buffer << " | head - " << buffer->data_head << " | tail - "
        << buffer->data_tail << std::endl;
      auto record = (perf::PerfEventsRecord*) (reinterpret_cast<char*>(buffer) + getpagesize());
      std::cout << "record | size - " << record->_header.size << std::endl;
      std::cout << record->toString() << std::endl;
#endif
      perf::PerfRecords records {buffer, SAMPLES_BUFFER_SIZE};
      for(auto& record : records) {
        if(record.type() == perf::RecordType::SAMPLE) {
          auto& samplesRecord = static_cast<perf::PerfSamplesRecord&>(record);
          std::cout << "PerfRecord - " << samplesRecord.toString() << std::endl;
        }
        else {
          std::cout << "PerfRecord - " << record.toString() << std::endl;
        }
      }
    }
  }
}}
