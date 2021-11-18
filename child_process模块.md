@[TOC]
因为我的第三个项目是做一个自动化部署的网站，那么操作shell脚本肯定是必不可少的，所以学习一下 child_process模块的用法。
# 1、出现的原因
 node是单进程的，必然存在一个问题，就是无法充分利用cpu等资源。node提供了child_process模块来实现子进程，从而实现一个广义上的多进程的模式。通过child_process模块，可以实现1个主进程，多个子进程的模式，主进程称为master进程，子进程又称工作进程。在子进程中不仅可以调用其他node程序，也可以执行非node程序以及shell命令等等，执行完子进程后，以流或者回调的形式返回。

# 2、Node.js 流的概念
流不是 Node.js 特有的概念。 它们是几十年前在 Unix 操作系统中引入的，程序可以通过管道运算符（|）对流进行相互交互。
Node.js 的 stream 模块 提供了构建所有流 API 的基础。 所有的流都是 EventEmitter 的实例。
## 流的优势
相对于使用其他的数据处理方法，流基本上提供了两个主要优点：

 - 内存效率: 无需加载大量的数据到内存中即可进行处理。
 - 时间效率: 当获得数据之后即可立即开始处理数据，这样所需的时间更少，而不必等到整个数据有效负载可用才开始。
## pipe()
从一个流注入到另一个流

```javascript
src.pipe(dest1).pipe(dest2)

src.pipe(dest1)
dest1.pipe(dest2)
```
## 流驱动的 Node.js API
 - process.stdin 返回连接到 stdin 的流。
 - process.stdout 返回连接到 stdout 的流。
 - process.stderr 返回连接到 stderr 的流。
 - fs.createReadStream() 创建文件的可读流。
 - fs.createWriteStream() 创建到文件的可写流。
 - net.connect() 启动基于流的连接。
 - http.request() 返回 http.ClientRequest 类的实例，该实例是可写流。
 - zlib.createGzip() 使用 gzip（压缩算法）将数据压缩到流中。
 - zlib.createGunzip() 解压缩 gzip 流。
 - zlib.createDeflate() 使用 deflate（压缩算法）将数据压缩到流中。
 - zlib.createInflate() 解压缩 deflate 流。

# child_process用法

```javascript
child_process.spawn(command[, args][, options])
```

```javascript
const spawn = require('child_process').spawn;
let cat=cp.spawn('cat',['input.txt']);
cat.stdout.on('data', (data) => {
  console.log(`stdout: ${data}`);
});
```

