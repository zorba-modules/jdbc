/*
 * Copyright 2006-2012 The FLWOR Foundation.
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

#include "connect.h"
#include "jdbc.h"

namespace zorba
{
namespace jdbc
{


ItemSequence_t
ConnectFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
	jthrowable lException = 0;
  JNIEnv *env = JdbcModule::getJavaEnv(aStaticContext);
  Item result;
  try
  {
    jstring url, username, password;
		// read input param 0
    Item item = JdbcModule::getItemArg(args, 0);
    bool hasUsername=false;
    if (item.isJSONItem()) 
    {
      Iterator_t lKeys = item.getObjectKeys();
        
      lKeys->open();
      Item lKey;
      while (lKeys->next(lKey))
      {
        zorba::String keystring = lKey.getStringValue();
        if (keystring=="url") {
          url =  env->NewStringUTF(item.getObjectValue(keystring).getStringValue().c_str());
        } else if (keystring=="user") {
          username =  env->NewStringUTF(item.getObjectValue(keystring).getStringValue().c_str());
          hasUsername = true;
        } else if (keystring=="password") {
          password =  env->NewStringUTF(item.getObjectValue(keystring).getStringValue().c_str());
        } else if (keystring.compare("type")) {
        } else if (keystring.compare("driver")) {
        }
      }
      lKeys->close();
    }

    jmethodID mConnection;
    jobject oConnection;
    jclass cDriverManager = env->FindClass("java/sql/DriverManager");
    CHECK_EXCEPTION(env);
    if (hasUsername) {
      mConnection = env->GetStaticMethodID(cDriverManager, "getConnection", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/sql/Connection;");
      CHECK_EXCEPTION(env);
      oConnection = env->CallStaticObjectMethod(cDriverManager, mConnection, url, username, password);
      CHECK_EXCEPTION(env);
    } else {
      mConnection = env->GetStaticMethodID(cDriverManager, "getConnection", "(Ljava/lang/String;)Ljava/sql/Connection;");
      CHECK_EXCEPTION(env);
      oConnection = env->CallStaticObjectMethod(cDriverManager, mConnection, url);
      CHECK_EXCEPTION(env);
    }

    InstanceMap* lInstanceMap = JdbcModule::getCreateInstanceMap(aDynamincContext, INSTANCE_MAP_CONNECTIONS);
    String lStrUUID = JdbcModule::getUUID();
    lInstanceMap->storeInstance(lStrUUID, oConnection);

    result = theFactory->createAnyURI(lStrUUID);
	}
  catch (zorba::jvm::VMOpenException&)
	{
    JdbcModule::throwError("VM001", "Could not start the Java VM (is the classpath set?).");
	}
	catch (JavaException&)
	{
    JdbcModule::throwJavaException(env, lException);
	}
  
  return ItemSequence_t(new SingletonItemSequence(result));
}

}}; // namespace zorba, jdbc