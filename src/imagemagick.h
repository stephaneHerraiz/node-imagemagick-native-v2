#ifndef IMAGEMAGICK_H
#define IMAGEMAGICK_H

#include <Magick++.h>
#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include "nan.h"

using namespace v8;
using namespace node;

// Base context for calls shared on sync and async code paths
struct im_ctx_base {
    Nan::Callback * callback;
    std::string error;

    char* srcData;
    size_t length;
    int debug;
    int ignoreWarnings;
    std::string srcFormat;

    // generated blob by convert or composite
    Magick::Blob dstBlob;

    virtual ~im_ctx_base() {}
};
// Extra context for identify
struct identify_im_ctx : im_ctx_base {
    Magick::Image image;

    identify_im_ctx() {}
};
// Extra context for convert
struct convert_im_ctx : im_ctx_base {
    unsigned int maxMemory;

    unsigned int width;
    unsigned int height;
    unsigned int xoffset;
    unsigned int yoffset;
    bool strip;
    bool trim;
    bool autoOrient;
    double trimFuzz;
    std::string resizeStyle;
    std::string gravity;
    std::string format;
    std::string filter;
    std::string blur;
    std::string background;
    Magick::ColorspaceType colorspace;
    unsigned int quality;
    int rotate;
    int density;
    int flip;
    std::string draw;

    convert_im_ctx() {}
};
// Extra context for composite
struct composite_im_ctx : im_ctx_base {
    char* compositeData;
    size_t compositeLength;

    std::string gravity;

    composite_im_ctx() {}
};

Local<Value> WrapPointer(char *ptr, size_t length);
Local<Value> WrapPointer(char *ptr);

#define RETURN_BLOB_OR_ERROR(req) \
    do { \
        im_ctx_base* _context = static_cast<im_ctx_base*>(req->data); \
        if (!_context->error.empty()) { \
            Nan::ThrowError(_context->error.c_str()); \
        } else { \
            const Local<Value> _retBuffer = WrapPointer((char *)_context->dstBlob.data(), _context->dstBlob.length()); \
            info.GetReturnValue().Set(_retBuffer); \
        } \
        delete req; \
    } while(0);

void AutoOrient(Magick::Image *image);

bool ReadImageMagick(Magick::Image *image, Magick::Blob srcBlob, std::string srcFormat, im_ctx_base *context);

#endif