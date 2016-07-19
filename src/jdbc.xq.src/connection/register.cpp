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

#include "register.h"
#include "jdbc.h"

namespace zorba {
  namespace jdbc {


    ItemSequence_t
    RegisterFunction::evaluate(
        const ExternalFunction::Arguments_t &args,
        const zorba::StaticContext *aStaticContext,
        const zorba::DynamicContext *aDynamincContext) const {
      JdbcModule::initGlobals(aStaticContext);
      Item result;
      JDBC_MODULE_TRY;
      jstring driverName(NULL);
      Item item = JdbcModule::getItemArg(args, 0);
      bool hasUsername = false;
      if (item.isJSONItem()) {
        Iterator_t lKeys = item.getObjectKeys();
        lKeys->open();
        Item lKey;
        while (lKeys->next(lKey)) {
          zorba::String keystring = lKey.getStringValue();
          zorba::String value = item.getObjectValue(keystring).getStringValue();
          if (keystring == "driver") {
            driverName = env->NewStringUTF(value.c_str());
            CHECK_EXCEPTION;
          }
        }
        lKeys->close();
      }

      jobject oRegister;
      if (driverName) {
        oRegister = env->CallStaticObjectMethod(
            jClass.classID,
            jClass.forName,
            driverName);
        CHECK_EXCEPTION;
      }

      JDBC_MODULE_CATCH;
      return ItemSequence_t(new EmptySequence());
    }


  }
}; // namespace zorba, jdbc
