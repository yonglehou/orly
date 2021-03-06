/* <orly/indy/disk/util/index_manager.test.cc>

   Unit test for <orly/indy/disk/util/index_manager.h>.

   Copyright 2010-2014 OrlyAtomics, Inc.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

#include <orly/indy/disk/util/index_manager.h>

#include <orly/indy/disk/sim/mem_engine.h>
#include <orly/indy/fiber/fiber_test_runner.h>

#include <test/kit.h>

using namespace std;
using namespace chrono;
using namespace Base;
using namespace Orly::Indy;
using namespace Orly::Indy::Disk;
using namespace Orly::Indy::Disk::Util;
using namespace Orly::Indy::Fiber;
using namespace Orly::Indy::Util;

static const size_t BlockSize = 4096UL * 16;

TBufBlock::TPool TBufBlock::Pool(BlockSize, 200);

FIXTURE(Typical) {
  TFiberTestRunner runner([](std::mutex &mut, std::condition_variable &cond, bool &fin, Fiber::TRunner::TRunnerCons &) {
    const size_t consol_thresh = 20UL;
    const size_t num_iter = 10000L;
    TScheduler scheduler(TScheduler::TPolicy(4, 10, milliseconds(10)));
    Sim::TMemEngine mem_engine(&scheduler,
                               256 /* disk space: 256 MB */,
                               256,
                               16384 /* page cache slots: 64MB */,
                               1 /* num page lru */,
                               1024 /* block cache slots: 64MB */,
                               1 /* num block lru */);

    typedef TIndexManager<size_t, 1000, 100> TMyManager;
    TMyManager manager(HERE, 0UL, consol_thresh, TVolume::TDesc::Fast, mem_engine.GetEngine(), true /* do cache */);
    for (size_t i = 0; i < num_iter; ++i) {
      manager.Emplace(num_iter - 1 - i);
    }
    size_t found = 0U;
    size_t matched = 0UL;
    for (TMyManager::TCursor csr(&manager, 50); csr; ++csr) {
      matched += (*csr == found) ? 1UL : 0UL;
      ++found;
    }
    EXPECT_EQ(found, num_iter);
    EXPECT_EQ(num_iter, found);
    std::lock_guard<std::mutex> lock(mut);
    fin = true;
    cond.notify_one();
  });
}

FIXTURE(ManySortFiles) {
  TFiberTestRunner runner([](std::mutex &mut, std::condition_variable &cond, bool &fin, Fiber::TRunner::TRunnerCons &) {
    const size_t consol_thresh = 20UL;
    const size_t num_iter = 1000000L;
    TScheduler scheduler(TScheduler::TPolicy(4, 10, milliseconds(10)));
    Sim::TMemEngine mem_engine(&scheduler,
                               256 /* disk space: 256 MB */,
                               256,
                               16384 /* page cache slots: 64MB */,
                               1 /* num page lru */,
                               1024 /* block cache slots: 64MB */,
                               1 /* num block lru */);

    typedef TIndexManager<size_t, 100, 100> TMyManager;
    TMyManager manager(HERE, 0UL, consol_thresh, TVolume::TDesc::Fast, mem_engine.GetEngine(), true /* do cache */);
    for (size_t i = 0; i < num_iter; ++i) {
      manager.Emplace(num_iter - 1 - i);
    }
    size_t found = 0U;
    size_t matched = 0UL;
    for (TMyManager::TCursor csr(&manager, 50); csr; ++csr) {
      matched += (*csr == found) ? 1UL : 0UL;
      ++found;
    }
    EXPECT_EQ(found, num_iter);
    EXPECT_EQ(num_iter, found);
    std::lock_guard<std::mutex> lock(mut);
    fin = true;
    cond.notify_one();
  });
}
