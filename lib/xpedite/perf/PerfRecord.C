///////////////////////////////////////////////////////////////////////////////
//
// TraceRecord - struct to decode perf trace point sample records
//
// Author: Manikandan Dhamodharan, Morgan Stanley
//
///////////////////////////////////////////////////////////////////////////////

#include <xpedite/perf/PerfRecord.H>
#include <xpedite/util/Util.H>
#include <sstream>

namespace xpedite { namespace perf {

  std::string toString(RecordType recordType_) {
    switch(recordType_) {
      case RecordType::MMAP: 
        return "Perf mmap record";
      case RecordType::LOST: 
        return "Perf lost record";
      case RecordType::COMM: 
        return "Perf comm record";
      case RecordType::EXIT: 
        return "Perf exit record";
      case RecordType::THROTTLE: 
        return "Perf throttle record";
      case RecordType::UNTHROTTLE: 
        return "Perf unthrottle record";
      case RecordType::FORK: 
        return "Perf fork record";
      case RecordType::READ: 
        return "Perf read record";
      case RecordType::SAMPLE: 
        return "Perf sample record";
      case RecordType::MMAP2: 
        return "Perf mmap2 record";
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,1,0)
      case RecordType::AUX: 
        return "Perf aux record";
      case RecordType::ITRACE_START: 
        return "Perf itrace start record";
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,2,0)
      case RecordType::LOST_SAMPLES: 
        return "Perf lost samples record";
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,3,0)
      case RecordType::SWITCH: 
        return "Perf switch record";
      case RecordType::SWITCH_CPU_WIDE: 
        return "Perf switch cpu wide record";
#endif
    }
    return "UnKnown";
  }

  std::string PerfRecord::toString() const {
    std::ostringstream os;
    os << perf::toString(type()) << " [size - " << _header.size << "]";
    return os.str();
  }

  std::string PerfSamplesRecord::toString() const {
    std::ostringstream os;
    os << perf::toString(type()) << " "
       << "[size - "               << _header.size
       << " | sampleId - "         << _sampleId
       << " | pid - "              << _pid
       << " | tid - "              << _tid
       << " | time - "             << _time
       << " | id - "               << _id
       << " | streamId - "         << _streamId
       << " | cpu - "              << _cpu
       << " | call chain depth - " << _nr
       << "]";
    auto stackTrace = util::buildStackTrace((void**)_ips, _nr);
    return os.str() + "\n" + stackTrace;
  }

}}
