///////////////////////////////////////////////////////////////////////////////
//
// TraceCtl - Logic to program and track kernel tracepoints using linux perf api
//
// Author: Manikandan Dhamodharan, Morgan Stanley
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vector>

namespace xpedite { namespace tracing {

  class Request
  {
    const std::vector<int> _traces;

    public:

    Request(std::vector<int> traces_)
      : _traces {traces_} {
    }

    const std::vector<int>& traces() const noexcept {
      return _traces;
    }

    std::string toString() const;
  };

}}
