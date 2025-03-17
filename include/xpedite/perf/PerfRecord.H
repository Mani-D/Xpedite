///////////////////////////////////////////////////////////////////////////////
//
// TraceRecord - struct to decode perf trace point sample records
//
// Author: Manikandan Dhamodharan, Morgan Stanley
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <cstdint>
#include <string>
#include <linux/perf_event.h>
#include <linux/version.h>

namespace xpedite { namespace perf {

  enum class RecordType
  {
    MMAP            = PERF_RECORD_MMAP,
    LOST            = PERF_RECORD_LOST,
    COMM            = PERF_RECORD_COMM,
    EXIT            = PERF_RECORD_EXIT,
    THROTTLE        = PERF_RECORD_THROTTLE,
    UNTHROTTLE      = PERF_RECORD_UNTHROTTLE,
    FORK            = PERF_RECORD_FORK,
    READ            = PERF_RECORD_READ,
    SAMPLE          = PERF_RECORD_SAMPLE,
    MMAP2           = PERF_RECORD_MMAP2,

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,1,0)
    AUX             = PERF_RECORD_AUX,
    ITRACE_START    = PERF_RECORD_ITRACE_START,
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,2,0)
    LOST_SAMPLES    = PERF_RECORD_LOST_SAMPLES,
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,3,0)
    SWITCH          = PERF_RECORD_SWITCH,
    SWITCH_CPU_WIDE = PERF_RECORD_SWITCH_CPU_WIDE
#endif
  };

  std::string toString(RecordType recordType_);

  struct Value {
    uint64_t _value;            /* The value of the event */
    uint64_t _id;               /* if PERF_FORMAT_ID */
  };

  struct ReadFormat {
    uint64_t _nr;               /* The number of events */
    uint64_t _timeEnabled;      /* if PERF_FORMAT_TOTAL_TIME_ENABLED */
    uint64_t _timeRunning;      /* if PERF_FORMAT_TOTAL_TIME_RUNNING */
    Value _values[0];

    static uint64_t readFormatMask() noexcept {
      return (
        PERF_FORMAT_TOTAL_TIME_ENABLED |
        PERF_FORMAT_TOTAL_TIME_RUNNING |
        PERF_FORMAT_ID                 |
        PERF_FORMAT_GROUP
      );
    }
  };

  struct PerfRecord {
    perf_event_header _header;

    inline RecordType type() const {
      return static_cast<RecordType>(_header.type);
    }

    std::string toString() const;
  };

  struct PerfSamplesRecord : public PerfRecord {
    uint64_t    _sampleId;      /* if PERF_SAMPLE_IDENTIFIER */
    uint32_t    _pid, _tid;     /* if PERF_SAMPLE_TID */
    uint64_t    _time;          /* if PERF_SAMPLE_TIME */
    uint64_t    _id;            /* if PERF_SAMPLE_ID */
    uint64_t    _streamId;      /* if PERF_SAMPLE_STREAM_ID */
    uint32_t    _cpu, _res;     /* if PERF_SAMPLE_CPU */
    //ReadFormat _v;              /* if PERF_SAMPLE_READ */
    uint64_t    _nr;            /* if PERF_SAMPLE_CALLCHAIN */
    uint64_t    _ips[0];        /* if PERF_SAMPLE_CALLCHAIN */

    static uint64_t sampleTypeMask() noexcept {
      return (
        PERF_SAMPLE_IDENTIFIER  |
        PERF_SAMPLE_TID         |
        PERF_SAMPLE_TIME        |
        PERF_SAMPLE_ID          |
        PERF_SAMPLE_STREAM_ID   |
        PERF_SAMPLE_CPU         |
        //PERF_SAMPLE_READ        |
        PERF_SAMPLE_CALLCHAIN
      );
    }

    std::string toString() const;
  };

}}
