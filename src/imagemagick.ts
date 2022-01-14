const fs = require('fs');
const im = require('./Release/imagemagick.node');
export interface Options {
    srcData: Buffer,
    format?: string,
    threadResource?: number,
    density?: number
}

export interface TextOptions {
    color?: string,
    font?: {
        family: string,
        size: number
    },
    stroke?: Stroke
}

export interface DrawOptions {
    color?: string,
    stroke?: Stroke
}

export interface Stroke {
    color: string,
    width: number,
    opacity?: number
}

export interface Size {
    width: number,
    height: number,
}

export class ImageMagick {
    private im: any;

    constructor(options: Options) {
        this.im = new im.IMObject(options);
    }

    resize(options): Promise<void> {
        return new Promise((resolve, reject) => {
            this.im.resize(options, (error) => {
                if (error) {
                    return reject(error);
                }
                return resolve();
            });
        });
    }

    rotate(angle: number): Promise<void> {
        return new Promise((resolve, reject) => {
            this.im.rotate(angle, (error) => {
                if (error) {
                    return reject(error);
                }
                return resolve();
            });
        });
    }

    drawRectangle(upperLeftX: number, upperLeftY: number
        , lowerRightX: number, lowerRightY: number, options: DrawOptions): Promise<void> {
        return new Promise((resolve, reject) => {
            this.im.drawRectangle(upperLeftX, upperLeftY, lowerRightX, lowerRightY, options, (error) => {
                if (error) {
                    return reject(error);
                }
                return resolve();
            });
        });
    }

    drawCircle(x: number, y: number, perimX: number, perimY: number, options: DrawOptions): Promise<void> {
        return new Promise((resolve, reject) => {
            this.im.drawCircle(x, y, perimX, perimY, options, (error) => {
                if (error) {
                    return reject(error);
                }
                return resolve();
            });
        });
    }

    drawText(x: number, y: number, text, options: TextOptions = {}): Promise<void> {
        return new Promise((resolve, reject) => {
            this.im.drawText(x, y, text, options, (error) => {
                if (error) {
                    return reject(error);
                }
                return resolve();
            });
        });
    }

    getImageFile(path: string): Promise<void> {
        return new Promise((resolve, reject) => {
            this.im.getImage((buf) => {
                fs.writeFile(path, buf, (err) => {
                    if (err) {
                        return reject(err);
                    } else {
                        return resolve();
                    }
                });
            });
        });
    }
    getImage(): Promise<Buffer> {
        return new Promise((resolve, reject) => {
            this.im.getImage((buf, err) => {
                if (err) {
                    return reject(err);
                } else {
                    return resolve(buf);
                }
            });
        });
    }

    getSize(): Promise<Size> {
        return new Promise((resolve, reject) => {
            const width = this.im.getWidth();
            const height = this.im.getHeight();
            resolve({
                width: width,
                height: height
            });
        });
    }

}