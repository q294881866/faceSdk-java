# face-v4-java-sdk,
version4 java sdk  

## java 目录存放java SDK的源码  
## jni 存放java调用C++ SDK的jni源码 

## java版本
java version "1.8.0_111"  
Java(TM) SE Runtime Environment (build 1.8.0_111-b14)  
Java HotSpot(TM) 64-Bit Server VM (build 25.111-b14, mixed mode)  

## jni是C++包装层，要区分32位还是64位。，
32位SDK: x86_32\JNIWisFaceEngineWrap.dll  
64位SDK: x86_64\JNIWisFaceEngineWrap.dll  
java SDK: com.qs.face.jar    


# 注意：将C++的SDK目录加入系统Path目录 ，

License文件需要放在 java.exe的根目录下。  

## 注意，
这个版本的java sdk 是针对的windows系统编译的，
如果您使用的是linux系统，java的代码基本不用改变，但是需要自己编译jni目录下的c++代码，将本地c++的sdk libWisFaceEngineWrapV4.so 包装成jni 供java调用


 