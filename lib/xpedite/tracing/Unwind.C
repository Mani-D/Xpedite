///////////////////////////////////////////////////////////////////////////////
//
// TraceCtl - Logic to program and track kernel tracepoints using linux perf api
//
// Author: Manikandan Dhamodharan, Morgan Stanley
//
///////////////////////////////////////////////////////////////////////////////

#include <libunwind.h>
#include <iostream>

namespace xpedite { namespace tracing {

  void trace() {
    unw_cursor_t* cursor {};
    unw_addr_space_t as;
    void* arg;
    unw_init_remote(cursor, as, arg);
  }

}}
