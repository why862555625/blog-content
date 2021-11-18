@[TOC]
# 学习体会
要想看源码首先需要明白几点
1、为什么这个东西要出来、有什么作用和好处
2、这个东西怎么用
3、接下来才到他的原理解读
真希望在重复这个过程的时候会得到启发，从而做出真正属于自己的东西。


## 1、koa为什么会出来
一、如果直接使用node来进行服务端处理会显得非常麻烦、所以才有了express和koa等框架。所谓的框架无非就是对原有的api进行了进一步的封装，使得功能更加好用和完善。其次，在设计的时候还需要保证框架可以很好的扩展生态，也就是可以附加额外的功能，在koa中体现就是中间件。
二、koa相比express更加轻量，几乎所有的额外功能都需要中间件来扩展、需要什么引入什么就好了，所以体积更小。

## 2、koa原理
koa其实很简单、主要做了两件事情，1、将res和req封装到ctx中，2、实现中间件。
在源码中共有四个文件，application、context、request和response。application是主文件、暴露koa类。

### 1、ctx的实现
#### 一、ctx的实现
koa实现了两个类扩充了原生req和res，request和response。

拿一个 取请求头数据的简单例子 


首先通过application中的createContext方法将req和res方法挂载到request和response上

```javascript
    createContext(req, res) {
        const context = Object.create(this.context) //对于每次请求  都要保证ctx相互独立 以下类似
        const request = context.request = Object.create(this.request)
        const response = context.response = Object.create(this.response)
        context.app = request.app = response.app = this
        context.req = request.req = response.req = req  //挂载req
        context.res = request.res = response.res = res//挂载res
        request.ctx = response.ctx = context
        request.response = response
        response.request = request
        context.originalUrl = request.originalUrl = req.url
        context.state = {}
        return context
    }
```
随后通过挂载在上面的req对其进行封装，从req.headers=>request.headers
问题？为什么要创建一个request，而不直接加在ctx上呢？
我认为主要原因是降低代码的耦合性，从而可以更好的维护。
```javascript
module.exports = {
    get header() {
        return this.req.headers
    },
    set header(val) {
        this.req.headers = val
    },
    }
```
随后通过context文件将request上的属性和方法挂载到ctx上
request.headers=>ctx.headers

```javascript
delegate(proto, 'request')
    .method('acceptsLanguages')
    .method('acceptsEncodings')
    .method('acceptsCharsets')
    .method('accepts')
    .method('get')
    .method('is')
    .access('querystring')
    .access('idempotent')
    .access('socket')
    .access('search')
    .access('method')
    .access('query')
    .access('path')
    .access('url')
    .access('accept')
    .getter('origin')
    .getter('href')
    .getter('subdomains')
    .getter('protocol')
    .getter('host')
    .getter('hostname')
    .getter('URL')
    .getter('header')
    .getter('headers')
    .getter('secure')
    .getter('stale')
    .getter('fresh')
    .getter('ips')
    .getter('ip')
```
delegate方法就是一个数据劫持方法，当你访问ctx上的header方法时会给你映射到request方法上
[源码地址](https://github.com/tj/node-delegates/blob/master/index.js)

不过dlegate是使用__defineGetter__   和__defineSetter__ 对对象进行劫持的，但是方法已经废弃，现在使用Object.defineProperty进行劫持比较好。
大概就是下面这样的代码。

```javascript
Object.defineProperty(proto, "header", {
    get() {
        return request  ["header"]
    }
})
```

大概ctx的封装就是这个样子。
#### 二、实现中间件功能
在我们使用koa 的时候需要用到use方法，而中间的箭头函数就是一个中间件。箭头函数有两个规定参数，ctx和next。而这种有规定好参数的函数一般都是框架先对其保存，随后在内部在进行调用。

```javascript
app.use((ctx,next) => {
    ctx.body = 12
})
```
application中use方法的实现   
很简单，就是将传入的中间件放入到middleware中保存。
```javascript
    use(fn) {
        // 将中间件保存
        if (typeof fn !== 'function') throw new TypeError('middleware must be a function!')
        this.middleware.push(fn)
        return this
    }
```

那是怎么调用的呢?
首先listen方法将callback提出
```javascript
    listen(...args) {
        // 开启监听端口
        // 将callback抽离
        const server = http.createServer(this.callback())
        return server.listen(...args)
    }
```
在callback中使用compose解析中间件，随后将中间件放入到handleRequest执行。
那么关键就是compose函数了。
```javascript
    callback() {
        // 洋葱模型的实现
        // fn=dispatch(0)  fn等于中间件递归循环的开始
        const fn = this.compose(this.middleware)
        const handleRequest = (req, res) => {
            const ctx = this.createContext(req, res)
            return this.handleRequest(ctx, fn)
        }
        return handleRequest
    }
```
逻辑直接看注释就好了

主要思想就是先返回第一个中间件，并将next设成第二个中间件，当在第一个中间件中调用next方法时候就相当与调用第二个方法，一直到最后一个或者没有调用next为止（递归终止条件，也就是代码中的if），当触发终止条件时就会回溯，依次执行next后面的内容，也就形成了洋葱模型。

```javascript
    compose(middleware) {
        // 错误检测
        if (!Array.isArray(middleware)) throw new TypeError('Middleware stack must be an array!')
        for (const fn of middleware) {
            if (typeof fn !== 'function') throw new TypeError('Middleware must be composed of functions!')
        }

        return function(context, next) {
        		//检测是否多次调用next
            let index = -1
                // 从0（也就是第一个）开始递归调用
            return dispatch(0)

            function dispatch(i) {
                // next 只能调用一次   （）
                if (i <= index) return Promise.reject(new Error('next() called multiple times'))
                index = i
                let fn = middleware[i]
                    // 全部调用完毕  promise循环调用结束 开始回溯
                if (i === middleware.length) fn = next
                if (!fn) return Promise.resolve()
                try {
                    // 如果没有调用完毕  继续返回promise  next=dispatch.bind(null, i + 1) 就是下一个中间件
                    return Promise.resolve(fn(context, dispatch.bind(null, i + 1)))
                } catch (err) {
                    return Promise.reject(err)
                }
            }
        }
    }
```


简单示例:
假设有两个中间件one和two

 1. 首先第一个中间件one进行调用，
2. 先执行 console.log("one ")，输出one 
 3. 然后走到next，next是dispatch.bind(null, i + 1)也就是第二个中间件，
4. 开始执行 console.log("two ") ，输出two
5. 开始执行next ，此时达到递归终止条件，返回Promise.resolve()
6. 开始执行   console.log('i am two') 输出 i am two
7. 第一个中间件的next执行完毕，
8. 开始执行console.log('i am one') 输出 i am one

最终输出结果
one
two
 i am two
 i am one
```javascript
app.use(async(ctx, next) => {
    console.log("one ")
    await next()
    console.log('i am one')
})
app.use(async(ctx, next) => {
    console.log("two ")
    await next()
    console.log('i am two')
})
```


koa剩下的一些就是错误处理和起始状态的设置，就不在这里赘述了，有兴趣的可以去看一下源码。[我的GitHub](https://github.com/why862555625/cli-koa-component)
看koa的过程中发现了 async和await的原理我还不明白，下一步学习的源码就是它了。冲冲冲
