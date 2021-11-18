@[TOC]
了解async||await的原理之前需要先理解promise原理。
[项目地址](https://github.com/why862555625/Source-code/tree/master/promise)
# promise 为什么会出现
## 为什么js是单线程
这主要和js的用途有关，js是作为浏览器的脚本语言，主要是实现用户与浏览器的交互，以及操作dom；这决定了它只能是单线程，否则会带来很复杂的同步问题。
举个例子：如果js被设计了多线程，如果有一个线程要修改一个dom元素，另一个线程要删除这个dom元素，此时浏览器就会一脸茫然，不知所措。所以，为了避免复杂性，从一诞生，JavaScript就是单线程.
## 单线程的问题
单线程就好像是一个流水线，只能顺序执行执行栈中的任务，但是有的任务是很耗时的例如网络请求和定时等，如果处理这种耗时的任务，后边的任务就会搁置，导致运行时间过长。所以最初为了解决这个问题，js使用回调函数进行异步（上一篇中的辅助线程）避免堵塞。


就拿网络请求举例子把

```javascript
//成功回调
function success(text) {
    var textarea = document.getElementById('test-response-text');
    textarea.value = text;
}
//失败回调
function fail(code) {
    var textarea = document.getElementById('test-response-text');
    textarea.value = 'Error code: ' + code;
}

var request = new XMLHttpRequest(); // 新建XMLHttpRequest对象

request.onreadystatechange = function () { // 状态发生变化时，函数被回调
    if (request.readyState === 4) { // 成功完成
        // 判断响应结果:
        if (request.status === 200) {
            // 成功，通过responseText拿到响应的文本:
            return success(request.responseText);
        } else {
            // 失败，根据响应码判断失败原因:
            return fail(request.status);
        }
    } else {
        // HTTP请求还在继续...
    }
}

// 发送请求:
request.open('GET', '/api/categories');
request.send();

alert('请求已发送，请等待响应...');
```
但是如果我在成功的回调中还需要用请求过来的数据在做一次请求呢？在极端一点，在请求的请求的请求函数中还需要进行请求呢？
也就是

```javascript
function success(text) {
    // ajax请求过程。。。。
    success1(text1) {
        function success2(text2) {
            ...
        }
    }
}
```
这会使得代码非常难看，而且显得很臃肿并且不好维护，为了解决这个问题promise应运而生。

## 宏任务和微任务
在异步模式下，创建异步任务主要分为宏任务与微任务两种。ES6 规范中，宏任务（Macrotask） 称为 Task， 微任务（Microtask） 称为 Jobs。宏任务是由宿主（浏览器、Node）发起的，而微任务由 JS 自身发起。
![请添加图片描述](https://img-blog.csdnimg.cn/b48f50e9ebca40df8cd27529cf59a7ed.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAcGVyd2h5X3dhbmc=,size_16,color_FFFFFF,t_70,g_se,x_16)
### 执行顺序


![请添加图片描述](https://img-blog.csdnimg.cn/86d0d64adbcf4c5280d7f42ad55bf2a3.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAcGVyd2h5X3dhbmc=,size_13,color_FFFFFF,t_70,g_se,x_16)

1. 判断宏任务队列是否为空
 - 不空 --> 执行最早进入队列的任务 --> 执行下一步

 - 空 --> 执行下一步
2. 判断微任务队列是否为空

- 不空 --> 执行最早进入队列的任务 --> 继续检查微任务队列空不空
- 空 --> 执行下一步

因为首次执行宏队列中会有 script（整体代码块）任务，所以实际上就是 Js 解析完成后，在异步任务中，会先执行完所有的微任务，这里也是很多面试题喜欢考察的。需要注意的是，新创建的微任务会立即进入微任务队列排队执行，不需要等待下一次轮回。
# promise 怎么用 ？
常用原型方法有 then , catch , all  静态方法有resolve,reject等
```javascript
let a = new Promise((resolve, reject) => {
    resolve(111)
})
let b = new Promise((resolve, reject) => {
    reject(123)
})
let c = Promise.resolve(789)
a.then((result) => {
    console.log("🚀 ~ file: 1.js ~ line 9 ~ a.then ~ result", result) //🚀 ~ file: 1.js ~ line 9 ~ a.then ~ result 111
    return 456
}).then((result) => {
    console.log("🚀 ~ file: 1.js ~ line 12 ~ a.then ~ result", result) //🚀 ~ file: 1.js ~ line 12 ~ a.then ~ result 456
})
b.then((result) => {
    console.log("🚀 ~ file: 1.js ~ line 16 ~ b.then ~ result", result) // 没有执行
}).catch(err => {
    console.log("🚀 ~ file: 1.js ~ line 18 ~ b.then ~ err", err) //🚀 ~ file: 1.js ~ line 18 ~ b.then ~ err 123
})

let d = Promise.all([a, b])
d.then((result) => {
    console.log("🚀 ~ file: 1.js ~ line 23 ~ d.then ~ result", result)
}).catch(err => {
    console.log("🚀 ~ file: 1.js ~ line 25 ~ d.then ~ err", err) //🚀 ~ file: 1.js ~ line 25 ~ d.then ~ err 123
})
let e = Promise.all([a, c])
e.then((result) => {
    console.log("🚀 ~ file: 1.js ~ line 31 ~ e.then ~ result", result) //🚀 ~ file: 1.js ~ line 31 ~ e.then ~ result [ 111, 789 ]
}).catch(err => {
    console.log("🚀 ~ file: 1.js ~ line 33 ~ e.then ~ err", err)
})
```
# promise原理
还没查阅资料之前我的对实现原理有了一个大致的想法，then和koa的中间件实现原理的方法应该是类似的（都是提前定义好了参数的输入格式，这样以便在函数内部在合适的时机调用），在状态为改变之前（异步为完成之前）先将then中的函数收集到一个数组中，等到状态发生改时变根据改变的类型进行相应的处理。

## then实现
首先要确定promise肯定是一个类，内部维护三个状态
也就是还未处理完、失败、成功。
当状态成功时，调用then中的方法。失败时调用catch中的方法。所以需要维护两个队列，一个是then 的另一个是catch的。并且then可以链式调用，说明then也会返回一个promise，return的结果就是返回了一个promise.resolve。



根据以上推断
**那么开始写promise**

```javascript
// 先定义三个常量表示状态
const PENDING = 'pending';
const FULFILLED = 'fulfilled';
const REJECTED = 'rejected';

// 新建 MyPromise 类
class MyPromise {
    constructor(executor) {
        // executor 是一个执行器，进入会立即执行
        // 并传入resolve和reject方法
        try {
            executor(this.resolve, this.reject)
        } catch (error) {
            this.reject(error)
        }
    }

    // 储存状态的变量，初始值是 pending
    status = PENDING;

    // resolve和reject为什么要用箭头函数？
    // 如果直接调用的话，普通函数this指向的是window或者undefined
    // 用箭头函数就可以让this指向当前实例对象
    // 成功之后的值
    value = null;
    // 失败之后的原因
    reason = null;

    // 存储成功回调函数
    onFulfilledCallbacks = [];
    // 存储失败回调函数
    onRejectedCallbacks = [];

    // 更改成功后的状态
    resolve = (value) => {
        // 只有状态是等待，才执行状态修改
        if (this.status === PENDING) {
            // 状态修改为成功
            this.status = FULFILLED;
            // 保存成功之后的值
            this.value = value;
            // resolve里面将所有成功的回调拿出来执行
            while (this.onFulfilledCallbacks.length) {
                // Array.shift() 取出数组第一个元素，然后（）调用，shift不是纯函数，取出后，数组将失去该元素，直到数组为空
                this.onFulfilledCallbacks.shift()(value)
            }
        }
    }

    // 更改失败后的状态
    reject = (reason) => {
        // 只有状态是等待，才执行状态修改
        if (this.status === PENDING) {
            // 状态成功为失败
            this.status = REJECTED;
            // 保存失败后的原因
            this.reason = reason;
            // resolve里面将所有失败的回调拿出来执行
            while (this.onRejectedCallbacks.length) {
                this.onRejectedCallbacks.shift()(reason)
            }
        }
    }

    then(onFulfilled, onRejected) {
        // then 方法会传入两个函数   执行成功后的处理函数和执行失败后的执行函数
        // 如果不传 就将上一个then的值直接放到下一个 需要判断是否是函数
        const realOnFulfilled = typeof onFulfilled === 'function' ? onFulfilled : value => value;
        const realOnRejected = typeof onRejected === 'function' ? onRejected : reason => { throw reason };

        // 为了链式调用这里直接创建一个 MyPromise，并在后面 return 出去
        const promise2 = new MyPromise((resolve, reject) => {
            // 上一个promise 在这里执行
            const fulfilledMicrotask = () => {
                // 创建一个微任务等待 promise2 完成初始化
                queueMicrotask(() => {
                    try {
                        // then 函数中  return结果   
                        const x = realOnFulfilled(this.value);
                        // 传入 resolvePromise 集中处理
                        resolvePromise(promise2, x, resolve, reject);
                    } catch (error) {
                        reject(error)
                    }
                })
            }

            const rejectedMicrotask = () => {
                    // 创建一个微任务等待 promise2 完成初始化
                    queueMicrotask(() => {
                        try {
                            // 调用失败回调，并且把原因返回
                            const x = realOnRejected(this.reason);
                            // 传入 resolvePromise 集中处理
                            resolvePromise(promise2, x, resolve, reject);
                        } catch (error) {
                            reject(error)
                        }
                    })
                }
                // 判断状态
            if (this.status === FULFILLED) {
                fulfilledMicrotask()
            } else if (this.status === REJECTED) {
                rejectedMicrotask()
            } else if (this.status === PENDING) {
                // 等待
                // 因为不知道后面状态的变化情况，所以将成功回调和失败回调存储起来
                // 等到执行成功失败函数的时候再传递
                this.onFulfilledCallbacks.push(fulfilledMicrotask);
                this.onRejectedCallbacks.push(rejectedMicrotask);
            }
        })

        return promise2;
    }

    catch (onRejected) {
        // 只需要进行错误处理
        this.then(undefined, onRejected);
    } finally(fn) {
        return this.then((value) => {
            return MyPromise.resolve(fn()).then(() => {
                return value;
            });
        }, (error) => {
            return MyPromise.resolve(fn()).then(() => {
                throw error
            });
        });
    }
  }
  function resolvePromise(promise, x, resolve, reject) {
    // 如果 promise 和 x 指向同一对象，以 TypeError 为据因拒绝执行 promise
    // 这是为了防止死循环
    if (promise === x) {
        return reject(new TypeError('The promise and the return value are the same'));
    }

    if (typeof x === 'object' || typeof x === 'function') {
        // 这个坑是跑测试的时候发现的，如果x是null，应该直接resolve
        if (x === null) {
            return resolve(x);
        }

        let then;
        try {
            // 把 x.then 赋值给 then 
            then = x.then;
        } catch (error) {
            // 如果取 x.then 的值时抛出错误 e ，则以 e 为据因拒绝 promise
            return reject(error);
        }

        // 如果 then 是函数
        if (typeof then === 'function') {
            let called = false;
            // 将 x 作为函数的作用域 this 调用之
            // 传递两个回调函数作为参数，第一个参数叫做 resolvePromise ，第二个参数叫做 rejectPromise
            // 名字重名了，我直接用匿名函数了
            try {
                then.call(
                    x,
                    // 如果 resolvePromise 以值 y 为参数被调用，则运行 [[Resolve]](promise, y)
                    y => {
                        // 如果 resolvePromise 和 rejectPromise 均被调用，
                        // 或者被同一参数调用了多次，则优先采用首次调用并忽略剩下的调用
                        // 实现这条需要前面加一个变量called
                        if (called) return;
                        called = true;
                        resolvePromise(promise, y, resolve, reject);
                    },
                    // 如果 rejectPromise 以据因 r 为参数被调用，则以据因 r 拒绝 promise
                    r => {
                        if (called) return;
                        called = true;
                        reject(r);
                    });
            } catch (error) {
                // 如果调用 then 方法抛出了异常 e：
                // 如果 resolvePromise 或 rejectPromise 已经被调用，则忽略之
                if (called) return;

                // 否则以 e 为据因拒绝 promise
                reject(error);
            }
        } else {
            // 如果 then 不是函数，以 x 为参数执行 promise
            resolve(x);
        }
    } else {
        // 如果 x 不为对象或者函数，以 x 为参数执行 promise
        resolve(x);
    }
}
```
写的时候catch方法想了很久，发现其实then方法中就包含了catch，所以直接传好参数调用就好了。resolvePromise函数主要是对then中的函数返回做处理的。


## 静态方法resovle和reject实现
其实静态方法只不过是在方法内部进行了new而已，很简单。
```javascript
    // resolve 静态方法
    static resolve(parameter) {
        // 如果传入 MyPromise 就直接返回
        if (parameter instanceof MyPromise) {
            return parameter;
        }

        // 转成常规方式
        return new MyPromise(resolve => {
            resolve(parameter);
        });
    }

    // reject 静态方法
    static reject(reason) {
        return new MyPromise((resolve, reject) => {
            reject(reason);
        });
    }
```
## all、race方法实现
all方法是如果全部返回为resolve（count === length）就将结果通过promise.resolve返回
如果遇见reject则直接返回reject
race返回最快执行的一个 有返回结果就直接返回
```javascript
    static all(promiseList) {
        return new MyPromise((resolve, reject) => {
            const result = [];
            const length = promiseList.length;
            let count = 0;
            if (length === 0) {
                return resolve(result);
            }
            promiseList.forEach((promise, index) => {
                MyPromise.resolve(promise).then((value) => {
                    count++;
                    result[index] = value;
                    if (count === length) {
                        resolve(result);
                    }
                }, (reason) => {
                    reject(reason);
                });
            });
        });
    }
        static race(promiseList) {
        return new MyPromise((resolve, reject) => {
            const length = promiseList.length;
            if (length === 0) {
                return resolve();
            } else {
                for (let i = 0; i < length; i++) {
                    MyPromise.resolve(promiseList[i]).then((value) => {
                        return resolve(value);
                    }, (reason) => {
                        return reject(reason);
                    });
                }
            }
        });
    }
```
