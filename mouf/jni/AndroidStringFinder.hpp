#ifndef ANDROID_STRING_FINDER
#define ANDROID_STRING_FINDER

#include <android/log.h>
#include <android_native_app_glue.h>

#include <string>

#include <jni.h>

#include "myjni.hpp"
#include <map>


#include "StringFinder.hpp"

class AndroidStringFinder : public StringFinder{
  struct android_app* app;


  ///this is very crude. Most of it could be put in a constructor to
  ///keep a global ref on the necessary intermediate object and do it
  ///once and for all.
  std::string load(std::string& id) {

    JNIEnv *jni;
  
    app->activity->vm->AttachCurrentThread(&jni, NULL);
  
    jclass activityClass = jni->FindClass("android/app/NativeActivity");
    JNI_ASSERT(jni, activityClass);
  
    jmethodID getClassLoader = jni->GetMethodID(activityClass,"getClassLoader", "()Ljava/lang/ClassLoader;");
    JNI_ASSERT(jni, getClassLoader);

    jobject cls = jni->CallObjectMethod(app->activity->clazz, getClassLoader);
    JNI_ASSERT(jni, cls);
  
    jclass classLoader = jni->FindClass("java/lang/ClassLoader");
    JNI_ASSERT(jni, classLoader);
  
    jmethodID findClass = jni->GetMethodID(classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    JNI_ASSERT(jni, findClass);
  
    jstring strClassName = jni->NewStringUTF("com.godrik.MouF.R$string");
    JNI_ASSERT(jni, strClassName);
  
    jclass classIWant = (jclass)jni->CallObjectMethod(cls, findClass, strClassName);   
    JNI_ASSERT(jni, classIWant);  

    jfieldID jid = jni->GetStaticFieldID (classIWant, id.c_str(), "I");
    JNI_ASSERT(jni, jid);

    jint ji = jni->GetStaticIntField(classIWant, jid);
    JNI_ASSERT(jni, ji);

    //accessing ressource class

    jclass contextClass = jni->FindClass("android/content/Context");
    JNI_ASSERT(jni, contextClass);  
  
    jmethodID getRessources = jni->GetMethodID(contextClass,"getResources", "()Landroid/content/res/Resources;");
    JNI_ASSERT(jni, getRessources);
  
    jobject res = jni->CallObjectMethod(app->activity->clazz, getRessources);
    JNI_ASSERT(jni, res);
  
    //accessing string
  
  
    jclass resClass = jni->FindClass("android/content/res/Resources");
    JNI_ASSERT(jni, resClass);  
  
    jmethodID getString = jni->GetMethodID(resClass,"getString", "(I)Ljava/lang/String;");
    JNI_ASSERT(jni, getString);
  
    jstring js = (jstring) jni->CallObjectMethod(res, getString, ji);
    JNI_ASSERT(jni, js);
  
    const char *cMessage = jni->GetStringUTFChars(js, NULL); 
    std::string out;
    if (cMessage) { 
      std::stringstream ss;
      ss<<cMessage;
      out = ss.str();
      //    Log::log<<out<<Log::endl; 
      jni->ReleaseStringUTFChars(js, cMessage); 
    } 
  
  
    jni->DeleteLocalRef(activityClass);
    jni->DeleteLocalRef(cls);
    jni->DeleteLocalRef(classLoader);
    jni->DeleteLocalRef(strClassName);
    jni->DeleteLocalRef(classIWant);
    jni->DeleteLocalRef(contextClass);
    jni->DeleteLocalRef(res);
    jni->DeleteLocalRef(resClass);
    jni->DeleteLocalRef(js);
  
    app->activity->vm->DetachCurrentThread();
    return out;
  

  }

  std::map<std::string, std::string> cache;

public:
  AndroidStringFinder(struct android_app* a):app(a){}

  virtual std::string find(std::string& id ) {
    auto it = cache.find(id);
    if (it == cache.end()) {
      std::string rep = load(id);
      cache.insert(std::pair<std::string, std::string>(id, rep));
      return rep;
    }
    
    return it->second;
  }

  virtual void clear() {cache.clear();}

  virtual std::string find(const char* id ) {
    std::string sid = id;
    return find (sid);
  }
};

#endif
