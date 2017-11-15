
### 使用
* 用GenCode.exe获取license
* 安装好程序，调用java接口即可使用
* 例子要使用不同的jdk运行版本。下面是详细


#### faceEngine
> 检测人脸，比较两个人脸的相似度。目前提供32、64位版本

* 将dll目录文件加到当前工程根目录，或者其它能加载到dll文件的目录
* license文件保存在 JAVA\_HOME\bin （32bit）目录下，即可执行文件java（.exe）同一目录
* 将dll目录下java\_32\_bin目录文件，放到执行本示例的32位java.exe对应目录(./bin)
* demo Test.main(String[])

> 注意

* dll目录文件,在x86\_32或者x86\_64目录
* 64位版本同32位操作不过放到相应的64位执行环境下，注意切换不同的jdk
* 这里建议直接将x86\_32或者x86\_64目录下所有文件包括license放到java/bin目录下。