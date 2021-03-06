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
#ifndef ZORBA_JDBC_MODULE_SETNUMERIC_H
#define ZORBA_JDBC_MODULE_SETNUMERIC_H

#include <zorba/empty_sequence.h>
#include <zorba/external_module.h>
#include <zorba/item_factory.h>
#include <zorba/zorba.h>

#include "JavaVMSingleton.h"


namespace zorba {
  namespace jdbc {

    class SetNumericFunction : public ContextualExternalFunction
    {
    private:
      const ExternalModule *theModule;
      ItemFactory *theFactory;
      XmlDataManager *theDataManager;

    public:
      SetNumericFunction(const ExternalModule *aModule) :
          theModule(aModule),
          theFactory(Zorba::getInstance(0)->getItemFactory()),
          theDataManager(Zorba::getInstance(0)->getXmlDataManager()) { }

      ~SetNumericFunction() { }

    public:
      virtual String getURI() const { return theModule->getURI(); }

      virtual String getLocalName() const { return "set-numeric"; }

      virtual ItemSequence_t
          evaluate(const ExternalFunction::Arguments_t &args,
                   const zorba::StaticContext *,
                   const zorba::DynamicContext *) const;
    };


  }
}; // namespace zorba, jdbc

#endif //ZORBA_JDBC_MODULE_SETNUMERIC_H