///////////////////////////////////////////////////////////////////////////////
//
// TraceRecord - struct to decode perf trace point sample records
//
// Author: Manikandan Dhamodharan, Morgan Stanley
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <xpedite/perf/PerfRecord.H>
#include <unistd.h>
#include <iterator>
#include <iostream>

namespace xpedite { namespace perf {

  class PerfRecords {

    perf_event_mmap_page* const _page;
    const int _size;

    public:

    class Iterator : public std::iterator<std::input_iterator_tag, PerfRecord>
    {
      perf_event_mmap_page* const _page;
      char* const _record;
      const int _size;

      reference record() const {
        std::cout << "page - " << _page << " | tail - " << _page->data_tail << " | size - " << _size << std::endl;
        return *reinterpret_cast<PerfRecord*>(_record + (_page->data_tail % _size));
      }

      public:

      Iterator(perf_event_mmap_page* page_, int size_) noexcept
        : _page {page_}, _record {reinterpret_cast<char*>(page_) + getpagesize()}, _size {size_} {
      }


      Iterator& operator++() {
        auto recordSize = record()._header.size;
        std::cout << "operator++ Head - " << _page->data_head << " | Tail - " << _page->data_tail << " | record size - " << recordSize << std::endl;
        if(_page->data_tail + recordSize <= _page->data_head) {
          _page->data_tail += recordSize;
        }
        return *this;
      }

      Iterator operator++(int) = delete;

      bool operator==(Iterator other_) const {
        if(_page && other_._page) {
          return _page == other_._page;
        }
        auto page = _page ? _page : other_._page;
        return page && page->data_head == page->data_tail;
      }

      bool operator!=(Iterator other_) const {
        return !(*this == other_);
      }
      reference operator*() const {
        return record();
      }
    };

    PerfRecords(perf_event_mmap_page* page_, int size_) noexcept
      : _page {page_}, _size {size_} {
    }

    Iterator begin() const { return Iterator {_page, _size}; }
    Iterator end()   const { return Iterator {nullptr, 0};   }
  };

}}
