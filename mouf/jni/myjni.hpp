#ifndef MY_JNI_HPP
#define MY_JNI_HPP

#include "log.hpp"


// My checking routine.
#define JNI_ASSERT(jni, cond) {				\
    if (!(cond)) {					\
      std::stringstream ss;				\
      ss << __FILE__ << ":" << __LINE__;		\
      Log::log<<ss.str()<<Log::endl;			\
    }							\
    if (jni->ExceptionCheck()) {			\
      Log::log<<"Exception: "<<__FILE__<<":"<<__LINE__<<" "<<Log::endl; \
      jthrowable throwable = jni->ExceptionOccurred();			\
      jni->ExceptionDescribe(); /* optionally dump the stack trace */   \
      jni->ExceptionClear(); /* mark the exception as "handled" */      \
      jclass clazz = jni->GetObjectClass(throwable);                    \
      jmethodID getMessageMethod = jni->GetMethodID(clazz, "getMessage", "()Ljava/lang/String;"); \
      jstring message = (jstring)jni->CallObjectMethod(throwable, getMessageMethod); \
      const char *cMessage = jni->GetStringUTFChars(message, NULL); \
      if (cMessage) { \
	Log::log<<cMessage<<Log::endl;		       \
        jni->ReleaseStringUTFChars(message, cMessage); \
      } \
      jni->DeleteLocalRef(clazz); \
    } \
  } 


#endif
