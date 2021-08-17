const fs = require('fs').promises;
const Im = require('../src/index');

(async () => {
    const srcData = await fs.readFile('test/test.jpg');
    const options = {
        srcData: srcData,
        format: 'JPEG',
        threadResource: 10
    }
    const hrstart = process.hrtime();
    const im = new Im(options);
    let hrend = process.hrtime(hrstart);
    console.info('Execution time (hr): %ds %dms', hrend[0], hrend[1] / 1000000);
    await im.rotation(90);
    hrend = process.hrtime(hrstart);
    console.info('Execution time (hr): %ds %dms', hrend[0], hrend[1] / 1000000);
    await im.resize({
        width: 320,
        height: 240,
        resizeStyle: 'aspectfit'});
        hrend = process.hrtime(hrstart);
    console.info('Execution time (hr): %ds %dms', hrend[0], hrend[1] / 1000000);
    await im.getImage('./test/after5.jpg');
    hrend = process.hrtime(hrstart);
    console.info('Execution time (hr): %ds %dms', hrend[0], hrend[1] / 1000000);
    process.exit();
})();