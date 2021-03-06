/* <base/event_counter.h>

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

#pragma once

#include <cstdint>
#include <cassert>

#include <base/class_traits.h>
#include <base/fd.h>

namespace Base {

  /* TODO */
  class TEventCounter {
    NO_COPY(TEventCounter);
    public:

    /* TODO */
    TEventCounter(uint64_t initial_count = 0);

    /* TODO */
    const TFd &GetFd() const;

    /* TODO */
    uint64_t Pop();

    /* TODO */
    void Push(uint64_t count = 1);

    private:

    /* TODO */
    TFd Fd;

  };  // TEventCounter

}  // Base
