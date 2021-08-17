const fs = require('fs');
const Im = require('./build/Release/imagemagick.node');

class ImageMagick {
    
    constructor(options) {
        this.im = new Im.IMObject(options);
    }

    resize(options) {
        return new Promise((resolve, reject) => {
            this.im.resize(options, () => {
                return resolve();
            });
        });
    }

    rotation(angle) {
        return new Promise((resolve, reject) => {
            this.im.rotate(angle, () => {
                return resolve();
            });
        });
    }

    getImage(path) {
        return new Promise((resolve, reject) => {
            this.im.getImage((buf) => {
                fs.writeFile(path,buf, (err) => {
                    if (err) {
                        return reject(err);
                    } else {
                        return resolve();
                    }
                });
            });
        });
    }

}

module.exports = ImageMagick;