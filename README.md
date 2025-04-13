### 2025HFUT工创赛智能物流搬运赛道

- [ ] ***DevelopmentEnvironment**:STM32CubeMX+VSCode/Clion+Ozone*
- [ ] ***PackageManager**:Mingw64*
- [ ] ***ToolChain**:CMake(v3.30)+ninja+arm-none-eabi-g++/gcc/gdb+OpenOCD*
- [ ] ***Language**:Cpp17*

------

**newcar_f407_zero**为主工程；

**User_File**存放用户代码；

**User_Third_Party**存放通用Cpp库；

**User_Config**存放烧录配置文件；

**Middlewares**存放CubeMX中间库(arm_math);

**Drivers/Core**为CubeMX自动生成；

**cmake-build-debug**存放Clion调用cmake进行编译后产物(elf所在文件夹，用于ozone调试);

------

其中，**User_File/1_Middleware**基本移植于科大Robowalker战队步兵，分层架构同参照其步兵框架；b站搜索：**TrojanGeneric**

基本分层思路：**Middleware**层包含HAL库二次封装提供统一风格接口，**device**层开发时注册其所需硬件资源进行基本数据处理并提供对外接口，**car**层开发时注册其所需的device组件并进行device功能的相互配合，**task**层调用相应的car组件进行顶层任务管理与总体任务注册与初始化。

***原则：***

- [ ] *尽可能使用**OOP编程**，**回调函数**思想，**事件注册**与**事件响应**思想；*
- [ ] *减少每个组件之间**耦合度**，减少**全局变量**，**全局函数**，减少混乱的**标志位**，使用回调与类属性的访问，使用**清晰**的命名；*
- [ ] *利用好**构造**与**析构**函数，减少初始化时代码阅读复杂度；*
- [ ] *分层后，每层的每个组件原则上只允许调用**下一层级**的组件，不可跨层级调用；*

> ***水平低下，代码仍有诸多不合理与混乱之处，可自行改进***
>
> 终：遗憾才是常态——2025/4/13
