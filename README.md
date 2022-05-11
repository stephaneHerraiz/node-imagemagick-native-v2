# node-imagemagick-native-v2

This a fork of [node-imagemagick-native](https://github.com/elad/node-imagemagick-native). This [Node](http://nodejs.org/) library provide a javascript object to run [ImageMagick](http://www.imagemagick.org/) multiple conversion functions using [Magick++](http://www.imagemagick.org/Magick++/).

![TypeScript](https://img.shields.io/badge/typescript-%23007ACC.svg?style=for-the-badge&logo=typescript&logoColor=white)
![Jest](https://img.shields.io/badge/jest.svg?style=for-the-badge&logo=typescript&logoColor=white)

<a name='license'></a>

## Install
### Install ImageMagick library on Linux
``` bash
sudo apt-get update
sudo apt-get -y install \
    imagemagick \
    libmagick++-dev \
    ghostscript
```
### Install ImageMagick library on Windows
Install ImageMagick library:

x64 : [ImageMagick-6.9.12-47-Q16-HDRI-x64-dll.exe](https://download.imagemagick.org/ImageMagick/download/binaries/ImageMagick-6.9.12-47-Q16-HDRI-x64-dll.exe)

x86 : [ImageMagick-6.9.12-47-Q16-HDRI-x86-dll.exe](https://download.imagemagick.org/ImageMagick/download/binaries/ImageMagick-6.9.12-47-Q16-HDRI-x86-dll.exe)

Set environment variables:
``` bash
setx MAGICK_ROOT "[ImageMagick library path]"
setx MAGICK_CODER_MODULE_PATH "[ImageMagick library path]\modules\coders"
```
### Install package
``` bash
npm i imagemagick-native-v2
```

###

## Exemple
``` typescript
const { promises as fs } = require('fs');
const { ImageMagick } = require('imagemagick-native-v2');

const srcData = await fs.readFile('image.png');
try {
    const im = new ImageMagick({
        srcData: srcData,
        format: 'PNG'
    });
} catch (error) {
        console.error('Cound not read source image: '+ error);
        return;
    }
try {
    await im.resize({
        width: 320,
        height: 240,
        resizeStyle: 'aspectfit'
    });

    await im.drawText(10, 10, 'My fisrt text', {
        color: 'red',
        font: {
            family: "DejaVu-Sans",
            size: 10,
        },
    });
    await im.getImageFile('result.png');
} catch (error) {
    console.error(`${error}`);
}
```
## License (MIT)

Copyright (c) Stéphane Herraiz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the 'Software'), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
