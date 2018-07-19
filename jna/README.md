
### 使用
> 检测人脸，比较两个人脸的相似度。目前提供32、64位版本
* 用GenCode.exe获取license
* 安装好程序，调用java接口即可使用
* 将[32](./x86_32)或者[64](./x86_64)目录下所有文件拷贝到当前工程根目录，或者其它能加载到dll文件的目录
* 32/64bit 例子要使用不同的jdk运行版本。下面是详细


#### faceEngine
> 本例子用32位版本，64位版本相同

* license文件保存在 JAVA\_HOME\bin （32bit）目录下，即可执行文件java（.exe）同一目录
* 将[dll](./x86_32)目录下所有文件，放到执行本示例的32位java.exe对应目录(./bin)
* [haarcascades](./haarcascades) 文件夹是检测眼镜的模型文件
* [jar](./jar) 文件夹提供了直接可以导入的jar，
   *  QsFace4Java-1.0.1-jar-with-dependencies.jar 包含了所有依赖的jar
   *  QsFace4Java-1.0.1.jar 只包含源码
* 源码需要手动引入opencv-310.jar
* demo Test

> 注意

1. dll目录文件,在x86\_32或者x86\_64目录
2. 64位版本同32位操作不过放到相应的64位执行环境下，注意切换不同的jdk
3. 建议直接将将[32](./x86_32)或者[64](./x86_64)目录下所有文件 以及license放到java/bin目录下。
4. try{}catch(Throwable a){} 异常处理需要是Throwable（灰色图片）
5. 生产发布须知
   *  二次打包确认依赖引入，对一下引入的class目录：OpenCV、qiansou
   *  相关的[dll](./x86_32)目录下所有文件和[haarcascades](./haarcascades) 文件夹也需要加入
   *  总之要跟开发环境一样，此版本仅支持windows平台

