##源码结构
* 2d
这里是2d渲染的元素,重点是元素基类Entity
* base
这里存放一些非常基础的类。
+ interface 接口
+ element 包括Point(代表一个2d或3d的点),Size,Rect,颜色（Color）等
+ lang 是最基础的类，定义的自己的数据类型等
+ config 配置类
+ console 
+ modifier 
* core
核心系统
+ audio 声音系统
+ input 用户输入系统
+ opengl 图形gpu相关类
+ resource 资源管理系统
* math
数学相关类，包涵向量、矩阵、四元数等
* platform
平台相关类，目前只包涵android
* support
第三方支持，libjpeg，libpng等
* tool
工具类
