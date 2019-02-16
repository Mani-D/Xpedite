///////////////////////////////////////////////////////////////////////////////
//
// TraceCtl - Logic to program and track kernel tracepoints using linux perf api
//
// Author: Manikandan Dhamodharan, Morgan Stanley
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <xpedite/perf/PerfEventsCtl.H>

namespace xpedite { namespace tracing {

  class Request;

  class TraceCtl : public perf::PerfEventsCtl
  {
    static TraceCtl _instance;

    public:

    TraceCtl();

    bool enable(const Request& request_);

    void disable() noexcept;

    void poll();

    static TraceCtl& get() {
      return _instance;
    }
  };

  inline TraceCtl& traceCtl() {
    return TraceCtl::get();
  }

}}
