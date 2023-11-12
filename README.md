# Win32 Native Downloader

🚀 A fast, tiny, native win32 downloader & installer for Windows platform.

[中文介绍](./README-zh.md)

## Features:

- Pure C++ (std:c++14).
- Tiny, the release version is only 414kb.
- Without any other dependencies.
- Just modify a little, very easy to use.
- Very beautiful UI framework, highly customizable.
- Download a zip file and install it to the specified directory.

## Compile and Build

Visual Studio 2022

## How to use:

You only need to modify one file and recompile it to get a customized downloader.

Edit file src/Global.cpp (Source Files/Global.cpp in VC++).

```c++
/// <configuration>
/// Set the following string and then compile

// Query zip download link
std::string CGlobal::downloadurl_ = "http://127.0.0.1:5001/download-url";
// directory name
std::string CGlobal::appDirName_ = "MyeXeAppDir";
// Exe file name such as eXeScope.exe or a relative path such as bin\\eXeScope.exe
std::string CGlobal::appname_ = "eXeScope.exe";
// shortcut file name
std::string CGlobal::shortcutname_ = "eXeScope";
// Link to open after installation
std::string CGlobal::openurl_ = "https://github.com/sinajia/Downloader";

/// </configuration>
```

You should modify the values of the above five variables.

The program will first request a json string from a specified resource. The get request url is stored in CGlobal::downloadurl_. Your server side needs to give the real download link.

Here is an example of a server side (Node.js)

```js
const express = require('express')
const app = express()

app.get('/download-url', (req, res) => {
  res.json({
    downloadUrl: 'http://xxxx.com/zipfile.zip',
  })
})

const server = require('http').createServer(app)

server.listen(5001, '0.0.0.0', function (err) {
  console.log('running')
})
```

"downloadUrl" is the real installation package link. `The package must be a zip file.`

You should also replace App.ico in src/res and six PNG images (1.png, 2.png, 3.png, 4.png, 5.png, 6.png) in src/res/png.

If you have done the above, please recompile it. Start now !

## Support

Help support the development and maintenance of the software ❤️

USDT(TRC20)

TYWsj6oBb1zqkhMvYhXBuFYJD21dVWzXFL

