///////////////////////////////////////////////////////////////////////////////
//
// Logic to encode pmu events attributes using linux perf events api
//
// PerfEventSet - A set of perf events, programmed and collected as a group
//
// The attributes set supports customizing the following properties
//  1. Two types of the perf event (PERF_TYPE_HARDWARE and PERF_TYPE_RAW)
//  2. Event select code for a chosen hardware performance counter
//  3. Flags to exclude collection in user/kernel space
//
//  The attributes disable the group leader (first element in the set) by default.
//
// Author: Manikandan Dhamodharan, Morgan Stanley
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <xpedite/pmu/EventSet.h>
#include <linux/perf_event.h>
#include <linux/version.h>
#include <vector>
#include <stdexcept>

namespace xpedite { namespace perf {

  struct PerfEventAttrSet {

    using PMUEventAttrs = std::vector<perf_event_attr>;

    // https://lwn.net/articles/373473
    const static uint16_t PERF_RAW_CPU_CLK_UNHALTED_REF {0x13c};

    PerfEventAttrSet()
      : _pmuEventAttrs {} {
    }

    void addEvent(uint32_t eventType_, uint16_t eventSelect_, bool excludeUser_, bool excludeKernel_) {
      _pmuEventAttrs.emplace_back(buildEventAttr(eventType_, eventSelect_, excludeUser_, excludeKernel_));
    }

    void addSamplingEvent(uint32_t eventType_, uint16_t eventSelect_, uint64_t sampleTypeMask_, uint64_t readFormatMask_,
        bool excludeUser_, bool excludeKernel_) {
      auto attr = buildEventAttr(eventType_, eventSelect_, excludeUser_, excludeKernel_);
      attr.sample_type = sampleTypeMask_;
      attr.read_format = readFormatMask_;
      attr.sample_period = 1;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
      attr.sample_max_stack = 127;
#endif
      _pmuEventAttrs.emplace_back(attr);
    }

    const PMUEventAttrs& values() const noexcept {
      return _pmuEventAttrs;
    }

    explicit operator bool() const noexcept {
      return !_pmuEventAttrs.empty();
    }

    int size() const noexcept {
      return static_cast<int>(_pmuEventAttrs.size());
    }

    std::string toString() const;

    private:

    PMUEventAttrs _pmuEventAttrs;

    perf_event_attr buildEventAttr(uint32_t eventType_, uint16_t eventSelect_, bool excludeUser_, bool excludeKernel_) {
      perf_event_attr attr {};
      attr.type = eventType_;
      attr.config = eventSelect_;
      attr.exclude_user = excludeUser_;
      attr.exclude_kernel = excludeKernel_;
      attr.size = sizeof(attr);
      attr.disabled = (size() == 0);
      return attr;
    }
  };

  std::string toString(perf_event_attr attr_);

  PerfEventAttrSet buildPerfEventAttrs(const EventSet& eventSet_) noexcept;

  PerfEventAttrSet buildPerfTraceAttrs(const std::vector<int>& traces_) noexcept;

}}
