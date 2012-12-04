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

#include "setnumeric.h"
#include "jdbc.h"

namespace zorba
{
namespace jdbc
{


ItemSequence_t
SetNumericFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
	jthrowable lException = 0;
  JNIEnv *env = JdbcModule::getJavaEnv(aStaticContext);
  Item result;
	try
  {
		// Local variables
    String lStatementUUID = JdbcModule::getStringArg(args, 0);

    InstanceMap* lInstanceMap = JdbcModule::getCreateInstanceMap(aDynamincContext, INSTANCE_MAP_PREPAREDSTATEMENTS);
    if (lInstanceMap==NULL)
    {
      JdbcModule::throwError("SQL003", "Prepared statement does not exist.");
    }
    jobject oPreparedStatement = lInstanceMap->getInstance(lStatementUUID);
    if(oPreparedStatement==NULL)
    {
      JdbcModule::throwError("SQL003", "Prepared statement does not exist.");
    }

    int index = JdbcModule::getItemArg(args, 1).getIntValue();
    Item value = JdbcModule::getItemArg(args, 2);
    int type = value.getTypeCode();

    jclass cPreparedStatement = env->FindClass("java/sql/PreparedStatement");
    CHECK_EXCEPTION(env);

    switch (type) {
      case zorba::store::SchemaTypeCode::XS_DOUBLE:
        env->CallVoidMethod(oPreparedStatement, env->GetMethodID(cPreparedStatement, "setDouble", "(ID)V"), index, value.getDoubleValue());
      break;
      case zorba::store::SchemaTypeCode::XS_FLOAT:
        env->CallVoidMethod(oPreparedStatement, env->GetMethodID(cPreparedStatement, "setFloat", "(IF)V"), index, value.getDoubleValue());
      break;
      case zorba::store::SchemaTypeCode::XS_INTEGER:
        env->CallVoidMethod(oPreparedStatement, env->GetMethodID(cPreparedStatement, "setLong", "(IJ)V"), index, value.getIntValue());
      break;
      case zorba::store::SchemaTypeCode::XS_DECIMAL:
        env->CallVoidMethod(oPreparedStatement, env->GetMethodID(cPreparedStatement, "setDouble", "(ID)V"), index, value.getDoubleValue());
      break;
      default:
        JdbcModule::throwError("SQL004", "Error setting numeric value.");
      break;
    }
    CHECK_EXCEPTION(env);
	}
  catch (zorba::jvm::VMOpenException&)
	{
    JdbcModule::throwError("VM001", "Could not start the Java VM (is the classpath set?).");
	}
	catch (JavaException&)
	{
    JdbcModule::throwJavaException(env, lException);
	}
  
	return ItemSequence_t(new EmptySequence());
}

}}; // namespace zorba, jdbc