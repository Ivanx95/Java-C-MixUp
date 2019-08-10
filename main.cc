#include <jni.h>

#include <cassert>
#include <iostream>

int main(int argc, char** argv) {
  const int kNumOptions = 3;
  JavaVMOption options[kNumOptions] = {
    { const_cast<char*>("-Xmx128m"), NULL },
    { const_cast<char*>("-verbose:gc"), NULL },
    { const_cast<char*>("-Djava.class.path=/home/jaisen/Documents/Java/Embbeded/tests"), NULL }
  };

  JavaVMInitArgs vm_args;
  vm_args.version = JNI_VERSION_1_6;
  vm_args.options = options;
  vm_args.nOptions = sizeof(options) / sizeof(JavaVMOption);
  assert(vm_args.nOptions == kNumOptions);

  JNIEnv* env = NULL;
  JavaVM* jvm = NULL;
  int res = JNI_CreateJavaVM(&jvm, reinterpret_cast<void**>(&env), &vm_args);
  if (res != JNI_OK) {
    std::cerr << "FAILED: JNI_CreateJavaVM " << res << std::endl;
    return -1;
  }

  const char* kClassName = "org/casa/embbed/Main";
  jclass cls = env->FindClass(kClassName);
  if (cls == NULL) {
    std::cerr << "FAILED: FindClass" << std::endl;
    return -1;
  }
  
  const char* kMethodName = "main";
  jmethodID mid =
      env->GetStaticMethodID(cls, kMethodName, "([Ljava/lang/String;)V");
  if (mid == NULL) {
    std::cerr << "FAILED: GetStaticMethodID" << std::endl;
    return -1;
  }

  const jsize kNumArgs = 1;
  jclass string_cls = env->FindClass("java/lang/String");
  jobject initial_element = NULL;
  jobjectArray method_args = env->NewObjectArray(kNumArgs, string_cls, initial_element);

  jstring method_args_0 = env->NewStringUTF("Hello, Java!");
  env->SetObjectArrayElement(method_args, 0, method_args_0);

  env->CallStaticVoidMethod(cls, mid, method_args);
  jvm->DestroyJavaVM();

  return 0;
}