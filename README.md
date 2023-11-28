## 燕山大学单片机设计二级项目(ysu_car)
燕山大学电子信息系单片机二级项目，大四上，小车
==无协实验室514==

#### 前期准备

- [ ] 将printf映射到蓝牙串口
- [ ] 设置关键的三个变量，mode，flag（三种状态），speed
- [ ] 模块准备
  - [ ] 电机控制
  - [ ] 循迹模块
  - [ ] 红外避障模块
  - [ ] 追光模块
  - [ ] 超声波模块
  - [ ] 舵机模块
  - [ ] 蓝牙
  - [ ] 蜂鸣器
  - [ ] RGB灯
  - [ ] ADC监控电量

#### 模式

将不同的模式通过手机向蓝牙发送单个字符，单片机收到命令后，需要flag的立即停止运动，蜂鸣器响三秒，等待按键按下

| mode |                  功能                  | flag |
| :--: | :------------------------------------: | :--: |
|  0   |         电机关闭，停止各种动作         |  ×   |
|  1   |         蓝牙向手机发送help信息         |  ×   |
|  2   | 手机蓝牙控制，发送小写字母控制（asdw） |  √   |
|  3   |                循迹模式                |  √   |
|  4   |            红外加超声波避障            |  √   |
|  5   |                红外遥控                |  √   |
|  6   |                追光模式                |  √   |
|  7   |             返回超声波距离             |  ×   |
|  8   |              返回电池电量              |  ×   |
|  9   |   （左三圈，右三圈，beep+RGB+舵机）    |  √   |
|  r   |                  红灯                  |  ×   |
|  g   |                  绿灯                  |  ×   |
|  b   |                  蓝灯                  |  ×   |
|  A   |                全部全速                |  ×   |
|  B   |                最高速度                |  ×   |
|  C   |                一般速度                |  ×   |
|  D   |                  低速                  |  √   |

<br/>

#### 相互调用

在项目添加`bsp.h`和`bsp.c`，封装各种功能

在`usart.c`中使用函数`Processing_received_data`改变 `mode` 和 `flag`

```
// usart.h
typedef enum{FLAG_CLEAR,FLAG_WAITSETTING,FLAG_OK}YSU_FLAG_STATUS;
typedef enum{
	MODE_STOP=0,
	MODE_HELP=1<<0,
	MODE_BLUETOOTH_HELP=1<<1,
	MODE_BLUETOOTH_CTRL=1<<2,
	MODE_TRACK = 1<<3,
	MODE_OBSTACLE_AVOIDANCE = 1<<4,
	MODE_INFRARED_REMOTE_COPNTROL=1<<5,
	MODE_PURSE_LIGHT = 1<<6,
	MODE_ULTRASONIC_DISTANCE=1<<7,
	MODE_BATTERY_POWER=1<<8,
	MODE_DANCE = 1<<9,
	
	
	
	MODE_RED_LIGHT = 1<<13,
	MODE_GREEN_LIGHT = 1<<14,
	MODE_BLUE_LIGHT = 1<<15,
	
	MODE_ALL_HIGHEST_SPEED = 1<<20,
	MODE_MAXIMUM_SPEED = 1<<21,
	MODE_GENERAL_SPEED = 1<<22,
	MODE_MINIMUM_SPEED = 1<<23,
}YSU_MODE_STATUS;
```



#### 部分代码具体实现

```
typedef enum{FLAG_CLEAR,FLAG_WAITSETTING,FLAG_OK}YSU_FLAG_STATUS;
typedef enum{MODE_STOP=0,MODE_HELP=1<<0,MODE_BLUETOOTH=1<<1 ...}YSU_MODE_STATUS;
// main.c 
while(1){
  if(flag == FLAG_WAITSETTING)
    waitting_key();
  // 在串口中断中更改mode
  while(flag == FLAG_OK && (mode| MODE_BLUETOOTH)){
    // 蓝牙控制
  }
}
// bsp.c 
// 等待按键按下
void waitting_key()
{
  // 开启外部中断,外部中断更改flag
  int count = 3000;
  while(flag == FLAG_WAITSETTING && count--){
    beep_on();
    delay_ms(1000);
  }
  // 关闭外部中断
}
```

#### 避障逻辑

设置一个范围，[upDist,minDist]

两个红外线避障模块 LeftSensorValue，RightSensorValue

1. 如果距离大于 upDist 并且两个红外值都为1，那么小车前进

2. 如果距离在范围中间，那么，左值为0则右转，右值为0则左转

3. 如果距离在范围中间，左右值都为0，那么舵机转动，

   > 舵机左边距离大左转，右边距离大右转
   >
   > 如果舵机左右距离都小于距离范围，后退

4. 如果距离在范围中间，左右都为1，继续前进

5. 如果距离小于范围，后退。再舵机控制
