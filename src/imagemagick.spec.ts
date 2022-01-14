import { ImageMagick } from './index';
import { mocked } from 'ts-jest/utils';
import * as fs from 'fs';

// here the whole foo var is mocked deeply
// const mockedImageMagick = mocked(ImageMagick, true)

async function compareImage(image1: Buffer, image2: Buffer) {
    const imCmp = new ImageMagick({
        srcData: image1,
        format: 'PNG'
    });
    return await imCmp.compareImage(image2);
}

describe('Compare test', () => {
    it('compare different images test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            threadResource: 10,
            format: 'PNG'

        }
        const im = new ImageMagick(options);
        const compareImageBuffer = await fs.promises.readFile('./test/results/testresult-rotation.png');
        const res = await im.compareImage(compareImageBuffer);
        expect(res).toBe(false);
    });
    it('compare same images test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            threadResource: 10,
            format: 'PNG'

        }
        const im = new ImageMagick(options);
        const compareImageBuffer = await fs.promises.readFile('./test/test.jpeg');
        const res = await im.compareImage(compareImageBuffer);
        expect(res).toBe(true);
    });
});
describe('Image info test', () => {
    it('Get image size test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        const size = await im.getSize();
        expect(size).toEqual({
            height: 315,
            width: 630
        });
    });
});
describe('Format JPEG image to PNG test', () => {
    it('convert JPEG to PNG test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        const bufferResult = await im.getImage();
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-convertpng.png');
        const res = await compareImage(bufferTestResult, bufferResult);
        expect(res).toBe(true);
    });
    it('90° rotation test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        await im.rotate(90);
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-rotation.png');
        const res = await im.compareImage(bufferTestResult);
        expect(res).toBe(true);
    });
    it('Wrong 90° rotation test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        await im.rotate(91);
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-rotation.png');
        const res = await im.compareImage(bufferTestResult);
        expect(res).toBe(false);
    });
    it('Resize (default:aspectfill) test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        await im.resize({
            width: 320,
            height: 240

        });
        const bufferResult = await im.getImage();
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-resize.png');
        const res = await compareImage(bufferResult, bufferTestResult);
        expect(res).toBe(true);
    });
    it('Resize aspectfit test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        await im.resize({
            width: 320,
            height: 240,
            resizeStyle: 'aspectfit'

        });
        const bufferResult = await im.getImage();
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-resize-aspectfit.png');
        const res = await compareImage(bufferResult, bufferTestResult);
        expect(res).toBe(true);
    });
    it('Resize fill test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        await im.resize({
            width: 320,
            height: 240,
            resizeStyle: 'fill'

        });
        const bufferResult = await im.getImage();
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-resize-fill.png');
        const res = await compareImage(bufferResult, bufferTestResult);
        expect(res).toBe(true);
    });
    it('Resize crop test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        await im.resize({
            width: 320,
            height: 240,
            resizeStyle: 'crop',
        });
        const bufferResult = await im.getImage();
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-resize-crop.png');
        const res = await compareImage(bufferResult, bufferTestResult);
        expect(res).toBe(true);
    });
    it('Resize crop with xoffset & yoffset test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        await im.resize({
            width: 320,
            height: 240,
            resizeStyle: 'crop',
            xoffset: 100,
            yoffset: 50

        });
        const bufferResult = await im.getImage();
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-resize-cropoffset.png');
        const res = await compareImage(bufferResult, bufferTestResult);
        expect(res).toBe(true);
    });
});

describe('Draw test', () => {
    it('Draw text without fonts test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        await im.drawText(20, 20, 'this a test');
        const bufferResult = await im.getImage();
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-text.png');
        const res = await compareImage(bufferResult, bufferTestResult);
        expect(res).toBe(true);
    });

    it('Draw text with color \'red\' test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        await im.drawText(20, 20, 'this a test', { color: 'red' });
        const bufferResult = await im.getImage();
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-textcolor.png');
        const res = await compareImage(bufferResult, bufferTestResult);
        expect(res).toBe(true);
    });
    it('Draw text with color \'#ff00ff\' test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        await im.drawText(20, 20, 'this a test', { color: '#ff00ff' });
        const bufferResult = await im.getImage();
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-texthexcolor.png');
        const res = await compareImage(bufferResult, bufferTestResult);
        expect(res).toBe(true);
    });
    it('Draw text with font DejaVu-Sans and size 20 test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        await im.drawText(20, 20, 'this a test', {
            font: {
                family: 'DejaVu-Sans',
                size: 20,
            }
        });
        const bufferResult = await im.getImage();
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-textfont.png');
        const res = await compareImage(bufferResult, bufferTestResult);
        expect(res).toBe(true);
    });

    it('Draw rectangle test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        await im.drawRectangle(20, 20, 100, 100, {});
        const bufferResult = await im.getImage();
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-rectangle.png');
        const res = await compareImage(bufferResult, bufferTestResult);
        expect(res).toBe(true);
    });
    it('Draw blue rectangle test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        await im.drawRectangle(20, 20, 100, 100, {
            color: 'blue'
        });
        const bufferResult = await im.getImage();
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-rectanglecolor.png');
        const res = await compareImage(bufferResult, bufferTestResult);
        expect(res).toBe(true);
    });
    it('Draw rectangle with red stroke test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        await im.drawRectangle(20, 20, 100, 100, {
            stroke: {
                color: 'red',
                width: 5
            }
        });
        const bufferResult = await im.getImage();
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-rectanglestroke.png');
        const res = await compareImage(bufferResult, bufferTestResult);
        expect(res).toBe(true);
    });

    it('Draw circle test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        await im.drawCircle(100, 100, 50, 50, {});
        const bufferResult = await im.getImage();
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-circle.png');
        const res = await compareImage(bufferResult, bufferTestResult);
        expect(res).toBe(true);
    });
    it('Draw blue circle test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        await im.drawCircle(100, 100, 50, 50, {
            color: 'blue'
        });
        const bufferResult = await im.getImage();
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-circlecolor.png');
        const res = await compareImage(bufferResult, bufferTestResult);
        expect(res).toBe(true);
    });
    it('Draw blue circle with red stroke test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        await im.drawCircle(100, 100, 50, 50, {
            color: 'blue',
            stroke: {
                color: 'red',
                width: 5
            }
        });
        const bufferResult = await im.getImage();
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-circlestroke.png');
        const res = await compareImage(bufferResult, bufferTestResult);
        expect(res).toBe(true);
    });
    it('Draw blue circle with red transparent stroke test', async () => {
        const srcData = await fs.promises.readFile('./test/test.jpeg');
        const options = {
            srcData: srcData,
            format: 'PNG'
        }
        const im = new ImageMagick(options);
        await im.drawCircle(100, 100, 50, 50, {
            color: 'blue',
            stroke: {
                color: 'red',
                width: 20,
                opacity: 50
            }
        });
        const bufferResult = await im.getImage();
        const bufferTestResult = await fs.promises.readFile('./test/results/testresult-circlestrokeopacity.png');
        const res = await compareImage(bufferResult, bufferTestResult);
        expect(res).toBe(true);
    });

});
describe('Retreive image test', () => {

    it.todo('Store image file test');
});