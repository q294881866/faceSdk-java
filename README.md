![输入图片说明](http://www.qiansou.cn/Scripts/img/lmxt-1bi1.jpg "在这里输入图片标题")  
# Version 4.0
千搜科技第四代人脸识别引擎java接口 

## 目录结构 
java 目录存放java SDK的源码  
jni 存放java调用C++ SDK的jni源码 
JavaApi 新的不需要依赖jni代码的java API 源码

## java版本
java version "1.8.0_111"  
Java(TM) SE Runtime Environment (build 1.8.0_111-b14)  
Java HotSpot(TM) 64-Bit Server VM (build 25.111-b14, mixed mode)  

## jni是C++包装层，要区分32位还是64位。
32位SDK: x86_32\JNIWisFaceEngineWrap.dll  
64位SDK: x86_64\JNIWisFaceEngineWrap.dll  
java SDK: com.qs.face.jar    


## 注意：将C++的SDK目录加入系统Path目录 

License文件需要放在 java.exe的根目录下。  

## 注意
这个版本的java sdk 是针对的windows系统编译的  
如果您使用的是linux系统，java的代码基本不用改变，但是需要自己编译jni目录下的c++代码，将本地c++的sdk libWisFaceEngineWrapV4.so 包装成jni 供java调用


## 技术支持
QQ： 2843028512  
QQ群：567619263

## Q&A
Q：sdk是离线的还是基于云服务通过http调用的。  
A: 离线的，不需要联网，直接在本地计算 

## C++ SDK 下载连接
https://share.weiyun.com/e41b3a527ccafea4a00fdb64e505588a