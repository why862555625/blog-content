@[TOC]

最近在看vue-cli源码时发现好多class了events类，最近在工作中也经常遇到，发现只是知道他是一个类似于观察者的一个东西，但是还没有系统看过这个内置模块。所以今天搞一下。


# 事件驱动

> Node.js 是一个基于 Chrome V8 引擎的 JavaScript 运行环境。Node.js 使用了一个事件驱动、非阻塞式 I/O的模型，使其轻量又高效。Allows you to build scalable network applications usingJavaScript on the server-side.


这是官方的引用，nodejs是基于事件驱动和非阻塞I/O的方式来设计运行的，在node中大部分的模块的实现都继承了Events类。 比如，文件操作中的fs事件流，网络编程所用到的tcp，http模块等，回想自己写的程序后，会发现很多操作都基于事件驱动，Events类。

## 那么问题是什么是事件驱动呢？
简单来说，就是通过监听事件的状态变化来做出相应的操作。比如读取一个文件，文件读取完毕，或者文件读取错误，那么就触发对应的状态，然后调用对应的回掉函数来进行处理。
例如文件：

```csharp
    const fs = require('fs');
    let rs = fs.createReadStream('1.txt');
    // 监听文件打开操作
    rs.on('open', function() {
      console.log('open');
    });
    // 监听数据流读取
    rs.on('data', function(data) {
      console.log(data);
    });
    // 监听错误
    rs.on('error', function() {
      console.log('error');
    });
    // 监听读取结束操作
    rs.on('end', function() {
      console.log('end');
    });
    // 监听文件关闭操作
    rs.on('close', function() {
      console.log('close');
    });
```
# Events类用法
## 主要的几个核心API
- on(eventName, listener)和emitter.addListener(eventName, listener)：对指定事件绑定事件处理函数
- once(eventName, listener)：对指定事件指定只执行一次的事件处理函数
- emit(eventName[, ...args])： 触发指定事件
- removeListener(eventName, listener)：对指定事件解除事件处理函数
- removeAllListeners([event])：对指定的事件接触所有的事件处理函数
- setMaxListeners 设置最大队列的长度


# 实现
嗯....   好像和我想的差不多，大概就是观察者模式。
既然这样，那自己实现一下好了，加深一下印象。

```csharp
class Events {
    constructor() {
      this.events = {};
    }
    /**
     * 事件监听
     * @param {*} type        监听的事件类型
     * @param {*} listener    回调函数
     */
    on(type, listener) {
      if (this.events[type]) {
        this.events[type].push(listener);
      } else {
        this.events[type] = [listener];
      }
    }
  
    /**
     * 事件监听，但是只执行一次
     * @param {*} type        监听的事件类型
     * @param {*} listener    回调函数
     */
    once(type, listener) {
      const wraper = (...rest) => {
        listener.apply(this, rest);
        this.removeListener(type, wraper);
      };
      this.on(type, wrapper);
    }
  
    /**
     * 事件触发
     * @param {*} type        要触发的事件类型
     * @param  {...any} rest  接收到的若干个参数，这个参数会作为参数被传递到对应事件的回调函数中
     */
    emit(type, ...rest) {
      if (this.events[type]) {
        this.events[type].forEach(listener => {
          listener.apply(this, rest);
        });
      }
    }
  
    /**
     * 删除指定事件中的监听函数
     * @param {*} type      对应的事件
     * @param {*} listener  要删除的监听函数
     */
    removeListener(type, listener) {
      if (this.events[type]) {
        this.events[type].filter(l => l !== listener);
      }
    }
  }
  
  module.exports = Events;
```
