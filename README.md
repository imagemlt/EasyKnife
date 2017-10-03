# EasyKnife
a simple app like CKnife

>本程序为mfc大作业所做，模仿CKnife项目。限于本人水平仅支持linux+php+mysql架构，界面采用CDHTMLDIALOG实现，基于jquery easyui。

## 依赖：
* Visual Studio 2015
* jsoncpp
* libcurl


## 功能

* 文件管理
  * 列目录
  * 上传下载
  * open_basedir绕过
* 数据库管理
* 模拟终端
* 自定义参数位置及加密

## 自定义配置用法：
  * 格式：json
  * 参数：
    * encrypt(string): 加密方法，目前支持BASE64_ENCODE BASE64_DECODE ROT13 PADDING 格式为"加密方法1 加密参数*加密方法2 加密参数"
    * place(string): 参数位置，值为GET POST COOKIE HEADER 
    * placevalue(string): 与place对应，表示参数
    * addonget(object): 附加的get参数键值对，其中如果键与密码重复<+>将被替换为执行的脚本。
    * addonpost(object): 附加的post参数键值对，与上者相同。
    
>由于时间限制，仅在上传下载处使用了多线程，因此会造成一定的卡顿，以后将逐渐修复此bug。  
项目仅为demo版本，希望大家提出意见与建议。
