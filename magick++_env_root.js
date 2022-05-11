'use strict'

let MAGICK_ROOT = ""
if (process.env.MAGICK_ROOT) {
    MAGICK_ROOT = process.env.MAGICK_ROOT;
    MAGICK_ROOT = MAGICK_ROOT.split("\\").join("/");
}
process.stdout.write(MAGICK_ROOT)
