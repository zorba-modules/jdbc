/*
 * Copyright 2006-2016 The FLWOR Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once
#ifndef ZORBA_JDBC_MODULE_INSTANCEMAP_H
#define ZORBA_JDBC_MODULE_INSTANCEMAP_H

#include <map>
#include <string>
#include <zorba/zorba.h>
#include "jni.h"

namespace zorba {
  namespace jdbc {

    class InstanceMap : public ExternalFunctionParameter
    {
    private:
      typedef std::map <String, jobject> InstanceMap_t;
      InstanceMap_t *instanceMap;

    public:
      String id;

      InstanceMap();

      bool
          storeInstance(const String &, jobject);

      jobject
          getInstance(const String &);

      bool
          deleteInstance(const String &);

      void
          destroy() throw();
    };


  }
}; // namespace zorba, jdbc

#endif //ZORBA_JDBC_MODULE_INSTANCEMAP_H
