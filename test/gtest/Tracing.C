///////////////////////////////////////////////////////////////////////////////
//
// Tracing - Tests to exercise programming and collection of kernel tracepoints
//
// Author: Manikandan Dhamodharan, Morgan Stanley
//
///////////////////////////////////////////////////////////////////////////////

#include <xpedite/framework/Framework.H>
#include <xpedite/tracing/TraceCtl.H>
#include <xpedite/tracing/Request.H>
#include <gtest/gtest.h>
#include <limits>
#include <thread>
#include <chrono>
#include <iostream>

namespace xpedite { namespace pmu { namespace test {

  struct TracingTest : ::testing::Test
  {
  };

  TEST_F(TracingTest, tracingUsage) {
    using namespace std::chrono_literals;
    framework::initializeThread();
    std::cout << "tracing test ..." << std::endl;
    //std::vector<int> traces {654, 655};
    std::vector<int> traces {649, 650};
    tracing::Request request {traces};
    auto rc = tracing::traceCtl().enable(request);
    ASSERT_TRUE(rc) << "failed to enable perf sampling events";
    char choice;
    for(int i=0; i<10; ++i) {
      std::cin >> choice;
      tracing::traceCtl().poll();
      //std::this_thread::sleep_for(1s);
    }
  }

}}}
