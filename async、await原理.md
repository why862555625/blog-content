@[TOC]
# 1、async||await为什么会出现
promise虽然解决了回调的那种头疼写法，但是代码还是需要不停的then，不够优雅简洁

# 2、async||await用法
用async修饰函数
用await修饰函数中的异步任务
并且这个函数的返回值是一个promise

```javascript
async function a() {
    await setTimeout(_ => {
        console.log(111); //111
    })
    return 123
}
let b = a()
b.then(result => {
    console.log("🚀 ~ file: 1.js ~ line 9 ~ result", result) //🚀 ~ file: 1.js ~ line 9 ~ result 123
})
```
# 3、原理
async||await是promise和协程在 ES6 的实现的[generator](https://www.ruanyifeng.com/blog/2015/04/generator.html)的语法糖。

> 一个线程（或函数）执行到一半，可以暂停执行，将执行权交给另一个线程（或函数），等到稍后收回执行权的时候，再恢复执行。这种可以并行执行、交换执行权的线程（或函数），就称为协程。

Generator 是一个异步操作的容器。它的自动执行需要一种机制，当异步操作有了结果，能够自动交回执行权。两种方法可以做到这一点：

 - 回调函数。将异步操作包装成 Thunk 函数，在回调函数里面交回执行权。

 - Promise 对象。将异步操作包装成 Promise 对象，用then方法交回执行权。


一个基于 Promise 对象的简单自动执行器：

```javascript
function run(gen){
  var g = gen();

  function next(data){
    var result = g.next(data);
    if (result.done) return result.value;
    result.value.then(function(data){
      next(data);
    });
  }
  next();
}

function* foo() {
    let response1 = yield fetch('https://xxx') //返回promise对象
    console.log('response1')
    console.log(response1)
    let response2 = yield fetch('https://xxx') //返回promise对象
    console.log('response2')
    console.log(response2)
}
run(foo);
```
上面代码中，只要 Generator 函数还没执行到最后一步，next函数就调用自身，以此实现自动执行。通过使用生成器配合执行器，就能实现使用同步的方式写出异步代码了，这样也大大加强了代码的可读性。


前文中的代码，用async实现是这样：

```javascript
const foo = async () => {
    let response1 = await fetch('https://xxx') 
    console.log('response1')
    console.log(response1)
    let response2 = await fetch('https://xxx') 
    console.log('response2')
    console.log(response2)
}

```
一比较就会发现，async函数就是将 Generator 函数的星号（*）替换成async，将yield替换成await，仅此而已。
async函数对 Generator 函数的改进，体现在以下四点：

 1. 内置执行器。Generator 函数的执行必须依靠执行器，而 async 函数自带执行器，无需手动执行 next() 方法。
 2. 更好的语义。async和await，比起星号和yield，语义更清楚了。async表示函数里有异步操作，await表示紧跟在后面的表达式需要等待结果。
 3. 更广的适用性。co模块约定，yield命令后面只能是 Thunk 函数或 Promise
    对象，而async函数的await命令后面，可以是 Promise 对象和原始类型的值（数值、字符串和布尔值，但这时会自动转成resolved 的 Promise 对象）。
 4. 返回值是 Promise。async 函数返回值是 Promise 对象，比 Generator 函数返回的 Iterator
    对象方便，可以直接使用 then() 方法进行调用。
这里的重点是自带了执行器，相当于把我们要额外做的(写执行器/依赖co模块)都封装了在内部。比如：

```javascript
async function fn(args) {
  // ...
}
```
等同于：

```javascript
function fn(args) {
    return spawn(function* () {
        // ...
    });
}

function spawn(genF) { //spawn函数就是自动执行器，跟简单版的思路是一样的，多了Promise和容错处理
    return new Promise(function (resolve, reject) {
        // 初始化generator
        const gen = genF();
        // 开始遍历generator
        step(function () { return gen.next(undefined); });

        function step(nextF) {
            let next;
            try {
                next = nextF();
            } catch (e) {
                return reject(e);
            }
            if (next.done) {
                return resolve(next.value);
            }
            // 避免不是promise报错 全部转为promise
            Promise.resolve(next.value).then(function (v) {
                // 拿到异步结果之后才会继续往下执行
                step(function () { return gen.next(v); });
            }, function (e) {
                step(function () { return gen.throw(e); });
            });
        }
    });
}
```

