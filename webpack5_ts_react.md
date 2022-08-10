# webpack-ts-react

## 安装依赖

```javascript
npm i webpack webpack-cli -D

npm i react react-dom -S

```

## loaders

支持最原始的 jsx 文件，让项目支持 react 的 jsx。
支持 jsx 需要额外配置 babel 去处理 jsx 文件，将 jsx 转译成为浏览器可以识别的 js。

需要用到如下几个库:

```javascript
npm i -D @babel/core @babel/preset-env babel-loader @babel/plugin-transform-runtime @babel/preset-react
```

- babel-loader
- @babel/core
- @babel/preset-env
- @babel/plugin-transform-runtime
- @babel/preset-react
  稍微梳理一下这几个 babel 的作用:

### babel-loader

首先对于我们项目中的 jsx 文件我们需要通过一个"转译器"将项目中的 jsx 文件转化成 js 文件，babel-loader 在这里充当的就是这个转译器。

### babel/core

但是 babel-loader 仅仅识别出了 jsx 文件，内部核心转译功能需要@babel/core 这个核心库，@babel/core 模块就是负责内部核心转译实现的

### babel/preset-env

@babel/prest-env 是 babel 转译过程中的一些预设，它负责将一些基础的 es 6+语法，比如 const/let...转译成为浏览器可以识别的低级别兼容性语法。

这里需要注意的是@babel/prest-ent 并不会对于一些 es6+并没有内置一些高版本语法的实现比如 Promise 等 polyfill，你可以将它理解为语法层面的转化不包含高级别模块(polyfill)的实现。

### babel/plugin-transform-runtime

@babel/plugin-transform-runtime,上边我们提到了对于一些高版本内置模块，比如 Promise/Generate 等等@babel/preset-env 并不会转化，所以@babel/plugin-transform-runtime 就是帮助我们来实现这样的效果的,他会在我们项目中如果使用到了 Promise 之类的模块之后去实现一个低版本浏览器的 polyfill。

### @babel/preset-react

目前这些插件处理的都是 js 文件，我们得让她能够识别并处理 jsx 文件。
此时就引入了我们至关重要的@babel/preset-react 这个插件. 将 html 部分转为 React.createElement 函数

## webpack 编写

```javascript
//webpack.base.js
const path = require("path");

module.exports = {
  // 入口文件，这里之后会着重强调
  entry: {
    main: path.resolve(__dirname, "../src/packages/home/index.jsx"),
  },
  module: {
    rules: [
      {
        test: /\.jsx?$/,
        use: "babel-loader",
      },
    ],
  },
};
```

上边我们讲到了 babel-loader 仅仅是一个桥梁，真正需要转译作用的其他的插件。接下来就让我们来使用它:

babel-loader 提供了两种配置方式，一种是直接在 webpack 配置文件中编写 options，另一个是官方推荐的在项目目录下建立.babelrc 文件单独配置 babel。

```javascript
// .babelrc
{
  "presets": [
    "@babel/preset-env",
    "@babel/preset-react"
  ],
  "plugins": [
    [
      "@babel/plugin-transform-runtime",
      {
        "regenerator": true
      }
    ]
  ]

}

```

pacakge.json 增加 build 脚本

```javascript
{
  "name": "pages",
  "version": "1.0.0",
  "description": "",
  "main": "index.js",
  "scripts": {
    "build": "webpack --config ./scripts/webpack.base.js"
  },
  "keywords": [],
  "author": "",
  "license": "ISC",
  "devDependencies": {
    "@babel/core": "^7.15.5",
    "@babel/plugin-transform-runtime": "^7.15.0",
    "@babel/preset-env": "^7.15.6",
    "@babel/preset-react": "^7.14.5",
    "babel-loader": "^8.2.2",
    "webpack": "^5.53.0",
    "webpack-cli": "^4.8.0"
  },
  "dependencies": {
    "react": "^17.0.2",
    "react-dom": "^17.0.2"
  }
}
```

这一步我们已经成功让 webpack 识别 jsx 代码并且支持将高版本 js 转化为低版本 javascript 代码了。
大概流程。主要就是:

```javascript
1. 创建babel配置转译jsx/js内容。
2. 创建入口文件。
3. webpack中对于jsx/js内容使用babel-loader调用babel配置好的预设和插件进行转译。
```

## 配置 TypeScript 支持

针对 TypeScript 代码的支持其实业内存在两种编译方式:

1. 直接通过 TypeScript 去编译 ts/tsx 代码。
2. 通过 babel 进行转译。

### babel 支持 Typescirpt

babel 内置了一组预设去转译 TypeScript 代码 --@babel/preset-typescript。

```javascript
npm install --save-dev @babel/preset-typescript
```

修改之前的配置:

首先我们先来修改之前.babelrc 配置文件，让 babel 支持转译的 ts 文件:

```javascript
{
  "presets": [
    "@babel/preset-env",
    "@babel/preset-react",
+   "@babel/preset-typescript"
  ],
  "plugins": [
    [
      "@babel/plugin-transform-runtime",
      {
        "regenerator": true
      }
    ]
  ]
}
```

这里我们在 presets 添加了@babel/preset-typescrpt 预设去让 babel 支持 typescript 语法。

### webpack 支持 ts/tsx 文件

```javascript
// webpack.base.jf
const path = require("path");

module.exports = {
  // 入口文件，这里之后会着重强调
  entry: {
    // 这里修改`jsx`为`tsx`
    main: path.resolve(__dirname, "../src/packages/home/index.tsx"),
  },
  module: {
    rules: [
      {
        // 同时认识ts jsx js tsx 文件
        test: /\.(t|j)sx?$/,
        use: "babel-loader",
      },
    ],
  },
};
```

这里我们将 ts,js,tsx,jsx 文件都交给 babel-loader 处理。

### 初始化 tsconfig.json

```javascript
npm i -D typescript
```

调用 tsc --init 命令初始化 tsconfig.json:

虽然说我们使用 babel 进行的编译，tsconfig.json 并不会在编译时生效。但是 tsconfig.json 中的配置非常影响我们的开发体验，接下来我们就来稍微修改一下一下它吧。

jsx 选项：他的作用是指定 jsx 的生成什么样的代码

```javascript
// "jsx": "preserve",                                /* Specify what JSX code is generated. */
```

改成

```javascript
"jsx": "react",
```

接下来我们来修改一下 ts 中的模块解析规则，将它修改为 node:

```javascript
"moduleResolution": "node",
```

[深入理解 typescript](https://jkchao.github.io/typescript-book-chinese/project/compilationContext.html#tsconfig-json)

对 react 增加 type 类型声明解决飘红

```javascript
npm i -D @types/react-dom @types/react
```

现在已经完成支持 typescript 和 react 了

## webpack 配置静态资源支持

资源模块类型(asset module type)，通过添加 4 种新的模块类型，来替换所有这些 loader：

- asset/resource  发送一个单独的文件并导出 URL。之前通过使用  file-loader  实现。
- asset/inline  导出一个资源的 data URI。之前通过使用  url-loader  实现。
- asset/source  导出资源的源代码。之前通过使用  raw-loader  实现。
- asset  在导出一个 data URI 和发送一个单独的文件之间自动选择。之前通过使用  url-loader，并且配置资源体积限制实现。

当在 webpack 5 中使用旧的 assets loader（如  file-loader/url-loader/raw-loader  等）和 asset 模块时，你可能想停止当前 asset 模块的处理，并再次启动处理，这可能会导致 asset 重复，你可以通过将 asset 模块的类型设置为  'javascript/auto'  来解决。

我们可以通过设置 Rule.parser.dataUrlCondition.maxSize 选项来修改此条件

其实 maxSize 就相当于 url-loader 中的 limit 属性，资源大小在 maxSize 之内使用行内 asset/inline 处理，超过之后就使用 resource 导出资源。当然这个配置也支持导出一个函数自定义配置实现。

```javascript
const path = require("path");

module.exports = {
  // 入口文件，这里之后会着重强调
  entry: {
    main: path.resolve(__dirname, "../src/packages/home/index.tsx"),
  },
  module: {
    rules: [
      {
        // 同时认识ts jsx js tsx 文件
        test: /\.(t|j)sx?$/,
        use: "babel-loader",
      },
      {
        test: /\.(png|jpe?g|svg|gif)$/,
        type: "asset/inline",
      },
      {
        test: /\.(eot|ttf|woff|woff2)$/,
        type: "asset/resource",
        generator: {
          filename: "fonts/[hash][ext][query]",
        },
      },
    ],
  },
};
```

#### 解决报错

- ts 文件中针对 image 的引入，ts 并不能正确的识别。

```typescript
declare module "*.svg";
declare module "*.png";
declare module "*.jpg";
declare module "*.jpeg";
declare module "*.gif";
declare module "*.bmp";
declare module "*.tiff";
//declare为声明语法，意思为声明ts全局模块，这样我们就可以正常引入对应的资源了。
```

- 我们在 index.tsx 中引入了对应的 app.tsx，当存在后缀时 ts 会进行报错提示:

接下来让我们来解决这个问题吧。其实无法就是引入文件时默认后缀名的问题:
目前 webpack 不支持默认后缀名.tsx
而 tsconfig.json 中是支持后缀名.tsx，所以显示声明会提示错误。
我们来统一这两个配置:

1. 别名统一
   修改 webpack 别名配置:

```javascript
// webpack.base.js
const path = require("path");

module.exports = {
  // 入口文件，这里之后会着重强调
  entry: {
    main: path.resolve(__dirname, "../src/packages/home/index.tsx"),
  },
  resolve: {
    alias: {
      "@src": path.resolve(__dirname, "../src"),
      "@packages": path.resolve(__dirname, "../src/packages"),
      "@containers": path.resolve(__dirname, "../src/containers"),
    },
    mainFiles: ["index", "main"],
    extensions: [".ts", ".tsx", ".scss", "json", ".js"],
  },
  module: {
    rules: [
      {
        // 同时认识ts jsx js tsx 文件
        test: /\.(t|j)sx?$/,
        use: "babel-loader",
      },
      {
        test: /\.(png|jpe?g|svg|gif)$/,
        type: "asset/inline",
      },
      {
        test: /\.(eot|ttf|woff|woff2)$/,
        type: "asset/resource",
        generator: {
          filename: "fonts/[hash][ext][query]",
        },
      },
    ],
  },
};
```

这里我们添加了 resolve 的参数，配置了别名@src,@package,@container。
以及当我们不书写文件后缀时，默认的解析规则 extensions 规则。
同时还配置了 mainFiles，解析文件夹路径～

但是基于 typescript 开发，所以 ts 文件中并不知道我们在 webpack 中配置的别名路径。
修改 tsconfig.json 别名配置

```javascript
    // tsconfig.json
    ...
     "baseUrl": "./",
    /* Specify the base directory to resolve non-relative module names. */
    "paths": {
      "@src/*": ["./src/*"],
      "@packages/*": ["./src/packages/*"],
      "@containers/*": ["./src/containers/*"],
    },
```

如果要配置 paths 那么一定是要配置 baseUrl 的，所谓 baseUrl 就是我们的 paths 是相对于那个路径开始的。

## 配置 css/sass

针对于 sass 文件，同样是 webpack 不认识的文件。咱们同样是需要 loader 去处理。
这里用到的 loader 如下:

- sass-loader
- resolve-url-loader
- postcss-loader
- css-loader
- MiniCssExtractPlugin.loader

### sass-loader

针对于 sass 文件我们首先一定是要使用 sass 编译成为 css 的，所以我们首先需要对.scss 结尾的文件进行编译成为 css 文件。

```javascript
npm i -D sass-loader sass
//sass-loader 需要预先安装 Dart Sass 或 Node Sass（可以在这两个链接中找到更多的资料）。这可以控制所有依赖的版本， 并自由的选择使用的 Sass 实现。
//sass-loader的作用就类似我们之前讲到过的babel-loader，可以将它理解成为一个桥梁，sass转译成为css的核心是由node-sass或者dart-sass去进行编译工作的。
```

### resolve-url-loader

由于 Saass 的实现没有提供 url 重写的功能，所以相关的资源都必须是相对于输出文件（ouput）而言的。

- 如果生成的 CSS 传递给了 css-loader，则所有的 url 规则都必须是相对于入口文件的（例如：main.scss）。
- 如果仅仅生成了 CSS 文件，没有将其传递给 css-loader，那么所有的 url 都是相对于网站的根目录的。

- 所以针对于 sass 编译后的 css 文件中的路径是不正确的，并不是我们想要的相对路径模式。

所以使用 resolve-url-loader 来处理文件引入路径问题。

```
npm i -D resolve-url-loader
```

### postcss-loader

针对于 postcss 其实这里并不打算深入去讲解，它是 babel 一样都是两个庞然大物。拥有自己独立的体系,在这里你需要清楚的是我们使用 postcss-loader 处理生成的 css.

```javascript
npm i -D postcss-loader postcss
npm i -D cssnano autoprefixer@latest
```

我们在项目根目录下新建一个 postcss.config.js 的文件:

```javascript
module.exports = {
  plugins: [
    require("autoprefixer"),
    require("cssnano")({
      preset: "default",
    }),
  ],
};

//autoprefixer插件的作用是为我们的css内容添加浏览器厂商前缀兼容。
//cssnano的作用是尽可能小的压缩我们的css代码。
```

### css-loader

css-loader 是解析我们 css 文件中的@import/require 语句分析的.

```javascript
npm i -D css-loader
```

### MiniCssExtractPlugin.loader

这个插件将 CSS 提取到单独的文件中。它为每个包含 CSS 的 JS 文件创建一个 CSS 文件。它支持按需加载 CSS 和 SourceMaps。
这里需要提一下他和 style-loader 的区别，这里我们使用了 MiniCssExtractPlugin 代替了 style-loader。
style-loader 会将生成的 css 添加到 html 的 header 标签内形成内敛样式，这显然不是我们想要的。所以这里我们使用 MiniCssExtractPlugin.loader 的作用就是拆分生成的 css 成为独立的 css 文件。

```javascript
npm i -D mini-css-extract-plugin
```

### 生成 sass 最终配置文件

```javascript
// webapck.base.js
const path = require("path");
const MiniCssExtractPlugin = require("mini-css-extract-plugin");

module.exports = {
  // 入口文件，这里之后会着重强调
  entry: {
    main: path.resolve(__dirname, "../src/packages/home/index.tsx"),
  },
  resolve: {
    alias: {
      "@src": path.resolve(__dirname, "../src"),
      "@packages": path.resolve(__dirname, "../src/packages"),
      "@containers": path.resolve(__dirname, "../src/containers"),
    },
    mainFiles: ["index", "main"],
    extensions: [".ts", ".tsx", ".scss", "json", ".js"],
  },
  module: {
    rules: [
      {
        test: /\.(t|j)sx?$/,
        use: "babel-loader",
      },
      {
        test: /\.(sa|sc)ss$/,
        use: [
          {
            loader: MiniCssExtractPlugin.loader,
          },
          "css-loader",
          "postcss-loader",
          {
            loader: "resolve-url-loader",
            options: {
              keepQuery: true,
            },
          },
          {
            loader: "sass-loader",
            options: {
              sourceMap: true,
            },
          },
        ],
      },
      {
        test: /\.(png|jpe?g|svg|gif)$/,
        type: "asset/inline",
      },
      {
        test: /\.(eot|ttf|woff|woff2)$/,
        type: "asset/resource",
        generator: {
          filename: "fonts/[hash][ext][query]",
        },
      },
    ],
  },
  plugins: [
    new MiniCssExtractPlugin({
      filename: "assets/[name].css",
    }),
  ],
};
```

```javascript
// postcss.config.js
module.exports = {
  plugins: [
    require("autoprefixer"),
    require("cssnano")({
      preset: "default",
    }),
  ],
};
```

## 配置 html 页面

当前我们所有涉及的都是针对单页面应用的配置，此时我们迫切需要一个 html 展示页面。
此时就引入我们的主角，我们后续的多页面应用也需要机遇这个插件生成 html 页面
html-webpack-plugin，其实看到这里我相信大家对这个插件原本就已经耳熟能详了。

简单介绍一下它的作用: 这个插件为我们生成 HTML 文件，同时可以支持自定义 html 模板。

```javascript
npm i --dev html-webpack-plugin

```

```javascript
// webpack.base.js
const path = require("path");
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const htmlWebpackPlugin = require("html-webpack-plugin");

module.exports = {
  // 入口文件，这里之后会着重强调
  entry: {
    main: path.resolve(__dirname, "../src/packages/home/index.tsx"),
  },
  resolve: {
    alias: {
      "@src": path.resolve(__dirname, "../src"),
      "@packages": path.resolve(__dirname, "../src/packages"),
      "@containers": path.resolve(__dirname, "../src/containers"),
    },
    mainFiles: ["index", "main"],
    extensions: [".ts", ".tsx", ".scss", "json", ".js"],
  },
  module: {
    rules: [
      {
        test: /\.(t|j)sx?$/,
        use: "babel-loader",
      },
      {
        test: /\.(sa|sc)ss$/,
        use: [
          {
            loader: MiniCssExtractPlugin.loader,
          },
          "css-loader",
          "postcss-loader",
          {
            loader: "resolve-url-loader",
            options: {
              keepQuery: true,
            },
          },
          {
            loader: "sass-loader",
            options: {
              sourceMap: true,
            },
          },
        ],
      },
      {
        test: /\.(png|jpe?g|svg|gif)$/,
        type: "asset/inline",
      },
      {
        test: /\.(eot|ttf|woff|woff2)$/,
        type: "asset/resource",
        generator: {
          filename: "fonts/[hash][ext][query]",
        },
      },
    ],
  },
  plugins: [
    new MiniCssExtractPlugin({
      filename: "assets/[name].css",
    }),
    // 生成html名称为index.html
    // 生成使用的模板为public/index.html
    new htmlWebpackPlugin({
      filename: "index.html",
      template: path.resolve(__dirname, "../public/index.html"),
    }),
  ],
};
```

## 配置开发环境预览

上边的长篇大论已经能满足一个 SPA 单页面应用的构建了，但是我们总不能每次修改代码都需要执行一次打包命令在预览吧。

这样的话也太过于麻烦了，别担心 webpack 为我们提供了 devServer 配置，支持我们每次更新代码热重载。

我们来使用一下这个功能吧～

首先让我们在 scripts 目录下新建一个 webpack.dev.js 文件，表示专门用于开发环境下的打包预览:

虽然 devServer 已经内置了 hot:true 达到热重载，但是我们仍然需要安装 webpack-dev-server。

```javascript
// webpack.dev.js
const { merge } = require("webpack-merge");
const baseConfig = require("./webpack.base");
const path = require("path");

const devConfig = {
  mode: "development",
  devServer: {
    // static允许我们在DevServer下访问该目录的静态资源
    // 简单理解来说 当我们启动DevServer时相当于启动了一个本地服务器
    // 这个服务器会同时以static-directory目录作为跟路径启动
    // 这样的话就可以访问到static/directory下的资源了
    static: {
      directory: path.join(__dirname, "../public"),
    },
    // 默认为true
    hot: true,
    // 是否开启代码压缩
    compress: true,
    // 启动的端口
    port: 9000,
  },
};

module.exports = merge(devConfig, baseConfig);
```

这里需要提到的是 webpack-merge 这个插件是基于 webpack 配置合并的，这里我们基于 webpack.base.js 和 webpack.dev.js 合并导出了一个配置对象。

devServer 需要使用 webpack serve 启动。

```javascript
 "scripts": {
+   "dev": "webpack serve --config ./scripts/webpack.dev.js",
    "build": "webpack --config ./scripts/webpack.base.js",
    "test": "echo \"Error: no test specified\" && exit 1"
  },

```

### 支持端口被占用启动

这里有一个小 tip 当我们的 devServer 端口被占用的时候我们再次启动项目会因为相同的端口被占用而报错

解决端口占用的问题我们需要借助一个第三方库 node-portfinder

首先先让我们来给 scripts/utils/constant.js 中添加一个常量为固定的启动端口:

```javascript
// scripts/utils/constant.js
...
// 固定端口
const BASE_PROT = 9000
...
module.exports = {
  MAIN_FILE,
  log,
  separator,
  BASE_PROT
}
```

接下来让我们来修改一下上边的 webpack.dev.js:

```javascript
const { merge } = require("webpack-merge");
const baseConfig = require("./webpack.base");
const portfinder = require("portfinder");
const path = require("path");
const { BASE_PROT } = require("./utils/constant");

portfinder.basePort = BASE_PROT;

const devConfig = {
  mode: "development",
  devServer: {
    // static允许我们在DevServer下访问该目录的静态资源
    // 简单理解来说 当我们启动DevServer时相当于启动了一个本地服务器
    // 这个服务器会同时以static-directory目录作为跟路径启动
    // 这样的话就可以访问到static/directory下的资源了
    static: {
      directory: path.join(__dirname, "../public"),
    },
    // 默认为true
    hot: true,
    // 是否开启代码压缩
    compress: true,
    // 启动的端口
    port: BASE_PROT,
  },
};

module.exports = async function () {
  try {
    // 端口被占用时候 portfinder.getPortPromise 返回一个新的端口(往上叠加)
    const port = await portfinder.getPortPromise();
    devConfig.devServer.port = port;
    return merge(devConfig, baseConfig);
  } catch (e) {
    throw new Error(e);
  }
};
```

我们把导出从对象变成了导出一个函数,webpack 中配置的导出支持一个对象的同时也支持一个函数～
然后函数中调用 portfinder.getPortPromise()判断当前端口是否占用，如果占用 portfinder 会返回一个新的端口，此时我们修改 devConfig 中的端口并且返回最新的配置进行启动就可以了！

# 最终配置 和安装项

## 安装项

```javascript
npm i -D webpack webpack-cli  @babel/core @babel/preset-env babel-loader @babel/plugin-transform-runtime @babel/preset-react  typescript @types/react-dom @types/react  sass-loader sass  resolve-url-loader  postcss-loader postcss cssnano autoprefixer@latest  css-loader mini-css-extract-plugin  html-webpack-plugin  chalk inquirer execa @babel/preset-typescript webpack-dev-server
```

```javascript
npm i react react-dom
```

## basewebpack

```javascript
// webpack.base.js
const path = require("path");
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const htmlWebpackPlugin = require("html-webpack-plugin");

module.exports = {
  // 入口文件，这里之后会着重强调
  entry: {
    main: path.resolve(__dirname, "../src/packages/home/index.tsx"),
  },
  resolve: {
    alias: {
      "@src": path.resolve(__dirname, "../src"),
      "@packages": path.resolve(__dirname, "../src/packages"),
      "@containers": path.resolve(__dirname, "../src/containers"),
    },
    mainFiles: ["index", "main"],
    extensions: [".ts", ".tsx", ".scss", "json", ".js"],
  },
  module: {
    rules: [
      {
        test: /\.(t|j)sx?$/,
        use: "babel-loader",
      },
      {
        test: /\.(sa|sc)ss$/,
        use: [
          {
            loader: MiniCssExtractPlugin.loader,
          },
          "css-loader",
          "postcss-loader",
          {
            loader: "resolve-url-loader",
            options: {
              keepQuery: true,
            },
          },
          {
            loader: "sass-loader",
            options: {
              sourceMap: true,
            },
          },
        ],
      },
      {
        test: /\.(png|jpe?g|svg|gif)$/,
        type: "asset/inline",
      },
      {
        test: /\.(eot|ttf|woff|woff2)$/,
        type: "asset/resource",
        generator: {
          filename: "fonts/[hash][ext][query]",
        },
      },
    ],
  },
  plugins: [
    new MiniCssExtractPlugin({
      filename: "assets/[name].css",
    }),
    // 生成html名称为index.html
    // 生成使用的模板为public/index.html
    new htmlWebpackPlugin({
      filename: "index.html",
      template: path.resolve(__dirname, "../public/index.html"),
    }),
  ],
};
```

## dev

```javascript
// webpack.dev.js
const { merge } = require("webpack-merge");
const baseConfig = require("./webpack.base");
const portfinder = require("portfinder");
const path = require("path");
const { BASE_PROT } = require("./utils/constant");

portfinder.basePort = BASE_PROT;

const devConfig = {
  mode: "development",
  devServer: {
    // static允许我们在DevServer下访问该目录的静态资源
    // 简单理解来说 当我们启动DevServer时相当于启动了一个本地服务器
    // 这个服务器会同时以static-directory目录作为跟路径启动
    // 这样的话就可以访问到static/directory下的资源了
    static: {
      directory: path.join(__dirname, "../public"),
    },
    // 默认为true
    hot: true,
    // 是否开启代码压缩
    compress: true,
    // 启动的端口
    port: BASE_PROT,
  },
};

module.exports = async function () {
  try {
    // 端口被占用时候 portfinder.getPortPromise 返回一个新的端口(往上叠加)
    const port = await portfinder.getPortPromise();
    devConfig.devServer.port = port;
    return merge(devConfig, baseConfig);
  } catch (e) {
    throw new Error(e);
  }
};
```

## prod

```javascript
// webpack.prod.js
const path = require("path");
const { CleanWebpackPlugin } = require("clean-webpack-plugin");
const FriendlyErrorsWebpackPlugin = require("friendly-errors-webpack-plugin");
const { merge } = require("webpack-merge");
const baseConfig = require("./webpack.base");

const prodConfig = {
  mode: "production",
  devtool: "source-map",
  output: {
    filename: "js/[name].js",
    path: path.resolve(__dirname, "../dist"),
  },
  plugins: [new CleanWebpackPlugin(), new FriendlyErrorsWebpackPlugin()],
};
module.exports = merge(prodConfig, baseConfig);
```

## .babelrc

```javascript
{
  "presets": [
    "@babel/preset-env",
    "@babel/preset-react",
+   "@babel/preset-typescript"
  ],
  "plugins": [
    [
      "@babel/plugin-transform-runtime",
      {
        "regenerator": true
      }
    ]
  ]
}

```

## tsconfig.json

```javascript
{
  "compilerOptions": {
    "target": "es2016",
    "jsx": "react",
    "module": "commonjs",
    "moduleResolution": "node",
    "esModuleInterop": true,
    "forceConsistentCasingInFileNames": true,
    "strict": true,
    "skipLibCheck": true,
    "baseUrl": "./",
    /* Specify the base directory to resolve non-relative module names. */
    "paths": {
      "@src/*": ["./src/*"],
      "@packages/*": ["./src/packages/*"],
      "@containers/*": ["./src/containers/*"],
    },
  }
}

```

## postcss.config.js

```javascript
module.exports = {
  plugins: [
    require("autoprefixer"),
    require("cssnano")({
      preset: "default",
    }),
  ],
};
```
