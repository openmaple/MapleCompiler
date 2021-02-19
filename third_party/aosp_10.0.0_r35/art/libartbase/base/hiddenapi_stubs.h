/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ART_LIBARTBASE_BASE_HIDDENAPI_STUBS_H_
#define ART_LIBARTBASE_BASE_HIDDENAPI_STUBS_H_

#include <set>
#include "string_view_format.h"

namespace art {
namespace hiddenapi {

const std::string kPublicApiStr = "public-api";
const std::string kSystemApiStr = "system-api";
const std::string kTestApiStr = "test-api";
const std::string kCorePlatformApiStr = "core-platform-api";

class ApiStubs {
 public:
  enum class Kind {
    kPublicApi,
    kSystemApi,
    kTestApi,
    kCorePlatformApi,
  };

  static std::string ToString(Kind api) {
    switch (api) {
      case Kind::kPublicApi:
        return kPublicApiStr;
      case Kind::kSystemApi:
        return kSystemApiStr;
      case Kind::kTestApi:
        return kTestApiStr;
      case Kind::kCorePlatformApi:
        return kCorePlatformApiStr;
    }
  }

  static bool IsStubsFlag(const std::string& api_flag_name) {
    return api_flag_name == kPublicApiStr || api_flag_name == kSystemApiStr ||
        api_flag_name == kTestApiStr || api_flag_name == kCorePlatformApiStr;
  }
};

}  // namespace hiddenapi
}  // namespace art


#endif  // ART_LIBARTBASE_BASE_HIDDENAPI_STUBS_H_
