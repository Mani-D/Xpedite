///////////////////////////////////////////////////////////////////////////////
//
// Logic to program pmu events using linux perf events api
//
// PerfEvent - Abstraction for reading h/w pmc using perf events api
//
// A Perf event owns and manages scope/lifetime, of the file descriptor and 
// mapped memory provided by the linux perf api
//
// Author: Manikandan Dhamodharan, Morgan Stanley
//
///////////////////////////////////////////////////////////////////////////////

#include <xpedite/perf/PerfEvent.H>
#include <xpedite/perf/PerfEventAttrSet.H>
#include <xpedite/perf/PerfEventsApi.H>
#include <xpedite/log/Log.H>
#include <xpedite/util/Errno.H>
#include <sys/mman.h>

namespace xpedite { namespace perf {

  const int PerfEvent::INVALID_FD {-1};

  perf_event_mmap_page* PerfEvent::INVALID_ADDR {reinterpret_cast<perf_event_mmap_page*>(MAP_FAILED)};

  const int SAMPLES_BUFFER_SIZE {(1 + (1 << 4)) * getpagesize()};

  PerfEvent::PerfEvent(perf_event_attr attr_, pid_t tid_, int gid_) noexcept 
    : _fd {INVALID_FD}, _handle {INVALID_ADDR}, _tid {tid_} {

    bool isLeader {gid_ == -1};
    bool isTracePoint {attr_.type == PERF_TYPE_TRACEPOINT};
    unsigned long flags {isTracePoint && isLeader ? PERF_FLAG_FD_OUTPUT : 0};

    _fd = perfEventsApi()->open(&attr_, tid_, -1, gid_, flags);
    if (_fd == INVALID_FD) {
      xpedite::util::Errno err;
      XpediteLogCritical << "failed to open pmu event (" << toString(attr_) << ") - " << err.asString() << XpediteLogEnd;
      return;
    }

    auto bufferSize = (isLeader && isTracePoint ? SAMPLES_BUFFER_SIZE : getpagesize());
    _handle = perfEventsApi()->map(_fd, bufferSize);
    if(_handle == INVALID_ADDR) {
      xpedite::util::Errno err;
      XpediteLogCritical << "failed to map pmu event (" << attr_.config << ") - " << err.asString() << XpediteLogEnd;
      return;
    }
  }

  PerfEvent::~PerfEvent() noexcept {
    if(_handle != INVALID_ADDR) {
      perfEventsApi()->unmap(_handle, getpagesize());
    }
    if(_fd != INVALID_FD) {
      perfEventsApi()->close(_fd);
    }
  }

}}
