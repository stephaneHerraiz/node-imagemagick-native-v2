#include "imobject.h"
#include "drawRectangle.h"
#include "drawCircle.h"
#include "drawText.h"

class ResizeWorker : public Nan::AsyncWorker
{
public:
    ResizeWorker(Nan::Callback *callback, Magick::Image *image, Resize_info *resize_info)
        : Nan::AsyncWorker(callback), image(image), resize_info(resize_info) {}
    ~ResizeWorker() {}

    void Execute()
    {
        // this executes outside node main thread.
        // Cannot call any v8 or nan objects and functions here,
        // but we can run long C functions that would otherwise hold our main Thread
        std::string error = "";
        if (resize_info->width || resize_info->height)
        {
            if (!resize_info->width)
            {
                resize_info->width = image->columns();
            }
            if (!resize_info->height)
            {
                resize_info->height = image->rows();
            }

            // do resize
            if (strcmp(resize_info->resizeStyle.c_str(), "aspectfill") == 0)
            {
                // ^ : Fill Area Flag ('^' flag)
                // is not implemented in Magick++
                // and gravity: center, extent doesnt look like working as exptected
                // so we do it ourselves

                // keep aspect ratio, get the exact provided size, crop top/bottom or left/right if necessary
                double aspectratioExpected = (double)resize_info->height / (double)resize_info->width;
                double aspectratioOriginal = (double)image->rows() / (double)image->columns();
                // unsigned int xoffset = 0;
                // unsigned int yoffset = 0;
                unsigned int resizewidth;
                unsigned int resizeheight;

                if (aspectratioExpected > aspectratioOriginal)
                {
                    // expected is taller
                    resizewidth = (unsigned int)((double)resize_info->height / (double)image->rows() * (double)image->columns() + 1.);
                    resizeheight = resize_info->height;
                    // if ( strstr(resize_info->gravity.c_str(), "West") != NULL ) {
                    //     xoffset = 0;
                    // }
                    // else if ( strstr(resize_info->gravity.c_str(), "East") != NULL ) {
                    //     xoffset = (unsigned int)( resizewidth - resize_info->width );
                    // }
                    // else {
                    //     xoffset = (unsigned int)( (resizewidth - resize_info->width) / 2. );
                    // }
                    // yoffset = 0;
                }
                else
                {
                    // expected is wider
                    resizewidth = resize_info->width;
                    resizeheight = (unsigned int)((double)resize_info->width / (double)image->columns() * (double)image->rows() + 1.);
                    // xoffset = 0;
                    // if ( strstr(resize_info->gravity.c_str(), "North") != NULL ) {
                    //     yoffset = 0;
                    // }
                    // else if ( strstr(resize_info->gravity.c_str(), "South") != NULL ) {
                    //     yoffset = (unsigned int)( resizeheight - resize_info->height );
                    // }
                    // else {
                    //     yoffset = (unsigned int)( (resizeheight - resize_info->height) / 2. );
                    // }
                }

                // if (debug) printf( "resize to: %d, %d\n", resizewidth, resizeheight );
                Magick::Geometry resizeGeometry(resizewidth, resizeheight, 0, 0, 0, 0);
                try
                {
                    image->zoom(resizeGeometry);
                }
                catch (std::exception &err)
                {
                    // std::string message = "image.resize failed with error: ";
                    // message            += err.what();
                    // error = message;
                    return;
                }
                catch (...)
                {
                    error = std::string("unhandled error");
                    return;
                }

                if (strcmp(resize_info->gravity.c_str(), "None") != 0)
                {
                    // limit canvas size to cropGeometry
                    // if (debug) printf( "crop to: %d, %d, %d, %d\n", resize_info->width, resize_info->height, resize_info->xoffset, resize_info->yoffset );
                    Magick::Geometry cropGeometry(resize_info->width, resize_info->height, resize_info->xoffset, resize_info->yoffset, 0, 0);

                    Magick::Color transparent("transparent");
                    // if ( strcmp( obj->context->format.c_str(), "PNG" ) == 0 ) {
                    //     // make background transparent for PNG
                    //     // JPEG background becomes black if set transparent here
                    //     transparent.alpha( 1. );
                    // }

#if MagickLibVersion > 0x654
                    image->extent(cropGeometry, transparent);
#else
                    image->extent(cropGeometry);
#endif
                }
            }
            else if (strcmp(resize_info->resizeStyle.c_str(), "aspectfit") == 0)
            {
                // keep aspect ratio, get the maximum image which fits inside specified size
                char geometryString[32];
                sprintf(geometryString, "%dx%d", resize_info->width, resize_info->height);
                // if (debug) printf( "resize to: %s\n", geometryString );

                try
                {
                    image->zoom(geometryString);
                }
                catch (std::exception &err)
                {
                    // std::string message = "image.resize failed with error: ";
                    // message            += err.what();
                    // context->error = message;
                    return;
                }
                catch (...)
                {
                    // context->error = std::string("unhandled error");
                    return;
                }
            }
            else if (strcmp(resize_info->resizeStyle.c_str(), "fill") == 0)
            {
                // change aspect ratio and fill specified size
                char geometryString[32];
                sprintf(geometryString, "%dx%d!", resize_info->width, resize_info->height);
                // if (debug) printf( "resize to: %s\n", geometryString );

                try
                {
                    image->zoom(geometryString);
                }
                catch (std::exception &err)
                {
                    // std::string message = "image.resize failed with error: ";
                    // message            += err.what();
                    // context->error = message;
                    // return;
                }
                catch (...)
                {
                    // context->error = std::string("unhandled error");
                    return;
                }
            }
            else if (strcmp(resize_info->resizeStyle.c_str(), "crop") == 0)
            {
                unsigned int xoffset = resize_info->xoffset;
                unsigned int yoffset = resize_info->yoffset;

                if (!xoffset)
                {
                    xoffset = 0;
                }
                if (!yoffset)
                {
                    yoffset = 0;
                }

                // limit canvas size to cropGeometry
                //  if (debug) printf( "crop to: %d, %d, %d, %d\n", resize_info->width, resize_info->height, xoffset, yoffset );
                Magick::Geometry cropGeometry(resize_info->width, resize_info->height, xoffset, yoffset, 0, 0);

                Magick::Color transparent("transparent");
                //  if ( strcmp( obj->context->format.c_str(), "PNG" ) == 0 ) {
                //      // make background transparent for PNG
                //      // JPEG background becomes black if set transparent here
                //      transparent.alpha( 1. );
                //  }

#if MagickLibVersion > 0x654
                image->extent(cropGeometry, transparent);
#else
                image->extent(cropGeometry);
#endif
            }
            else
            {
                printf("resizeStyle not supported");
                return;
            }
            // if (debug) printf( "resized to: %d, %d\n", (int)obj->image.columns(), (int)obj->image.rows() );
        }
    }

    // its not necessay to implement this function,
    // unless you want a diferent callback function to run,
    // or pass some response to js
    //virtual void HandleOKCallback()
    //{
    //callback->Call(0, NULL);
    //}

private:
    Magick::Image *image;
    Resize_info *resize_info;
};

class RotationWorker : public Nan::AsyncWorker
{
public:
    RotationWorker(Nan::Callback *callback, Magick::Image *image, int angle)
        : Nan::AsyncWorker(callback), image(image), angle(angle) {}
    ~RotationWorker() {}

    void Execute()
    {
        // this executes outside node main thread.
        // Cannot call any v8 or nan objects and functions here,
        // but we can run long C functions that would otherwise hold our main Thread
        std::string error = "";
        image->rotate(angle);
    }

    // its not necessay to implement this function,
    // unless you want a diferent callback function to run,
    // or pass some response to js
    //virtual void HandleOKCallback()
    //{
    //callback->Call(0, NULL);
    //}

private:
    Magick::Image *image;
    int angle;
};

Nan::Persistent<v8::Function> IMObject::constructor;

IMObject::IMObject() {}

IMObject::~IMObject() {}

void IMObject::Init(v8::Local<v8::Object> exports)
{
    v8::Local<v8::Context> context = exports->CreationContext();
    Nan::HandleScope scope;

    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("IMObject").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(2);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "resize", resize);
    Nan::SetPrototypeMethod(tpl, "length", Getlen);
    Nan::SetPrototypeMethod(tpl, "rotate", rotate);
    Nan::SetPrototypeMethod(tpl, "getImage", getImage);
    Nan::SetPrototypeMethod(tpl, "baseColumns", baseColumns);
    Nan::SetPrototypeMethod(tpl, "getHeight", getHeight);
    Nan::SetPrototypeMethod(tpl, "getWidth", getWidth);
    Nan::SetPrototypeMethod(tpl, "drawRectangle", drawRectangle);
    Nan::SetPrototypeMethod(tpl, "drawCircle", drawCircle);
    Nan::SetPrototypeMethod(tpl, "drawText", drawText);

    constructor.Reset(tpl->GetFunction(context).ToLocalChecked());
    exports->Set(context,
                 Nan::New("IMObject").ToLocalChecked(),
                 tpl->GetFunction(context).ToLocalChecked());
}

void IMObject::New(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
    if (info.IsConstructCall())
    {
        // Invoked as constructor: `new IMObject(...)`
        IMObject *obj = new IMObject();
        obj->context = new convert_im_ctx();

        Local<Object> data = Local<Object>::Cast(info[0]);
        Local<Object> srcData = Local<Object>::Cast(Nan::Get(data, Nan::New<String>("srcData").ToLocalChecked()).ToLocalChecked());
        if (srcData->IsUndefined() || !Buffer::HasInstance(srcData))
        {
            return Nan::ThrowError("convert()'s 1st argument should have \"srcData\" key with a Buffer instance");
        }

        Local<Value> srcFormatValue = Nan::Get(data, Nan::New<String>("srcFormat").ToLocalChecked()).ToLocalChecked();
        obj->context->srcFormat = !srcFormatValue->IsUndefined() ? *Nan::Utf8String(srcFormatValue) : "";

        Local<Value> formatValue = Nan::Get(data, Nan::New<String>("format").ToLocalChecked()).ToLocalChecked();
        obj->context->format = !formatValue->IsUndefined() ? *Nan::Utf8String(formatValue) : "";

        obj->context->srcData = Buffer::Data(srcData);
        obj->context->length = Buffer::Length(srcData);

        Local<Value> threadResValue = Nan::Get(data, Nan::New<String>("threadResource").ToLocalChecked()).ToLocalChecked();
        unsigned int threadResource = !threadResValue->IsUndefined() ? Nan::To<Uint32>(threadResValue).ToLocalChecked()->Value() : 1;
        MagickCore::SetMagickResourceLimit(MagickCore::ThreadResource, threadResource);

        Local<Value> densityValue = Nan::Get(data, Nan::New<String>("density").ToLocalChecked()).ToLocalChecked();
        obj->context->density = !densityValue->IsUndefined() ? Nan::To<Uint32>(densityValue).ToLocalChecked()->Value() : -1;

        Magick::Blob srcBlob(obj->context->srcData, obj->context->length);

        if (!ReadImageMagick(&obj->image, srcBlob, obj->context->srcFormat, obj->context))
        {
            printf(obj->context->error.c_str());
            return;
        }
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    }
    else
    {
        // Invoked as plain function `IMObject(...)`, turn into construct call.
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = {info[0]};
        v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
        info.GetReturnValue().Set(
            cons->NewInstance(context, argc, argv).ToLocalChecked());
    }
}

void IMObject::Getlen(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    IMObject *obj = ObjectWrap::Unwrap<IMObject>(info.Holder());
    int len = obj->context->length;
    info.GetReturnValue().Set(Nan::New(len));
}

void IMObject::rotate(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
    IMObject *obj = ObjectWrap::Unwrap<IMObject>(info.Holder());
    double angle =
        info[0]->IsUndefined() ? 0 : info[0]->NumberValue(context).FromJust();
    Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());
    Nan::AsyncQueueWorker(new RotationWorker(callback, &(obj->image), angle));
}

void IMObject::resize(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    IMObject *obj = ObjectWrap::Unwrap<IMObject>(info.Holder());

    Resize_info *resize_info = new Resize_info();

    Local<Object> data = Local<Object>::Cast(info[0]);
    resize_info->width = Nan::To<Uint32>(Nan::Get(data, Nan::New<String>("width").ToLocalChecked()).ToLocalChecked()).ToLocalChecked()->Value();
    resize_info->height = Nan::To<Uint32>(Nan::Get(data, Nan::New<String>("height").ToLocalChecked()).ToLocalChecked()).ToLocalChecked()->Value();
    resize_info->height = Nan::To<Uint32>(Nan::Get(data, Nan::New<String>("height").ToLocalChecked()).ToLocalChecked()).ToLocalChecked()->Value();
    resize_info->xoffset = Nan::To<Uint32>(Nan::Get(data, Nan::New<String>("xoffset").ToLocalChecked()).ToLocalChecked()).ToLocalChecked()->Value();
    resize_info->yoffset = Nan::To<Uint32>(Nan::Get(data, Nan::New<String>("yoffset").ToLocalChecked()).ToLocalChecked()).ToLocalChecked()->Value();

    Local<Value> gravityValue = Nan::Get(data, Nan::New<String>("gravity").ToLocalChecked()).ToLocalChecked();
    resize_info->gravity = !gravityValue->IsUndefined() ? *Nan::Utf8String(gravityValue) : "Center";

    Local<Value> resizeStyleValue = Nan::Get(data, Nan::New<String>("resizeStyle").ToLocalChecked()).ToLocalChecked();
    resize_info->resizeStyle = !resizeStyleValue->IsUndefined() ? *Nan::Utf8String(resizeStyleValue) : "aspectfill";

    Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());
    Nan::AsyncQueueWorker(new ResizeWorker(callback, &(obj->image), resize_info));
    // Isolate* isolate = info.GetIsolate();
    // Local<Function> cb = Local<Function>::Cast(info[1]);
    // Local<Value> argv[1] = Nan::New<String>(error.c_str()).ToLocalChecked();
    // cb->Call(context, Null(isolate), 1, argv).ToLocalChecked();
}

void IMObject::baseColumns(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    IMObject *obj = ObjectWrap::Unwrap<IMObject>(info.Holder());
    int col = static_cast<int>(obj->image.baseColumns());
    info.GetReturnValue().Set(Nan::New(col));
}

void IMObject::getWidth(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    IMObject *obj = ObjectWrap::Unwrap<IMObject>(info.Holder());
    int col = static_cast<int>(obj->image.columns());
    info.GetReturnValue().Set(Nan::New(col));
}

void IMObject::getHeight(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    IMObject *obj = ObjectWrap::Unwrap<IMObject>(info.Holder());
    int rows = static_cast<int>(obj->image.rows());
    info.GetReturnValue().Set(Nan::New(rows));
}

void IMObject::getImage(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
    IMObject *obj = ObjectWrap::Unwrap<IMObject>(info.Holder());
    Magick::Blob dstBlob;
    try
    {
        if (obj->context->density > 0)
        {
            obj->image.density(Magick::Geometry(obj->context->density, obj->context->density));
        }
    }
    catch (std::exception &err)
    {
        printf("image.density failed with error");
        return;
    }

    try
    {
        obj->image.write(&dstBlob, obj->context->format);
    }
    catch (std::exception &err)
    {
        printf("image.write failed with error");
        return;
    }
    catch (...)
    {
        printf("unhandled error");
        return;
    }
    Local<Value> argv[1] = {WrapPointer((char *)dstBlob.data(), dstBlob.length())};
    Isolate *isolate = info.GetIsolate();
    Local<Function> cb = Local<Function>::Cast(info[0]);
    cb->Call(context, Null(isolate), 1, argv).ToLocalChecked();
    // info.GetReturnValue().Set(WrapPointer((char *)dstBlob.data(), dstBlob.length()) );
}

void IMObject::drawRectangle(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
    IMObject *obj = ObjectWrap::Unwrap<IMObject>(info.Holder());

    double upperLeftX = info[0]->NumberValue(context).FromJust();
    double upperLeftY = info[1]->NumberValue(context).FromJust();
    double lowerRightX = info[2]->NumberValue(context).FromJust();
    double lowerRightY = info[3]->NumberValue(context).FromJust();

    DrawRectangleOptions *options = DrawRectangleWorker::castOptions(Local<Object>::Cast(info[4]), isolate);

    Nan::Callback *callback = new Nan::Callback(info[5].As<v8::Function>());
    Nan::AsyncQueueWorker(new DrawRectangleWorker(callback, &(obj->image), upperLeftX, upperLeftY, lowerRightX, lowerRightY, options));
}

void IMObject::drawCircle(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
    IMObject *obj = ObjectWrap::Unwrap<IMObject>(info.Holder());

    double x = info[0]->NumberValue(context).FromJust();
    double y = info[1]->NumberValue(context).FromJust();
    double perimX = info[2]->NumberValue(context).FromJust();
    double perimY = info[3]->NumberValue(context).FromJust();

    DrawCircleOptions *options = DrawCircleWorker::castOptions(Local<Object>::Cast(info[4]), isolate);

    Nan::Callback *callback = new Nan::Callback(info[5].As<v8::Function>());
    Nan::AsyncQueueWorker(new DrawCircleWorker(callback, &(obj->image), x, y, perimX, perimY, options));
}

void IMObject::drawText(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
    v8::Isolate *isolate = info.GetIsolate();

    IMObject *obj = ObjectWrap::Unwrap<IMObject>(info.Holder());

    double x = info[0]->NumberValue(context).FromJust();
    double y = info[1]->NumberValue(context).FromJust();

    v8::String::Utf8Value v8_text(isolate, info[2]->ToString());
    std::string text(*v8_text);

    DrawTextOptions *options = DrawTextWorker::castOptions(Local<Object>::Cast(info[3]), isolate);

    Nan::Callback *callback = new Nan::Callback(info[4].As<v8::Function>());
    Nan::AsyncQueueWorker(new DrawTextWorker(callback, &(obj->image), x, y, text, options));
}

void init(Local<Object> exports)
{
    IMObject::Init(exports); // QuantumDepth is already defined
}

NODE_MODULE(imagemagick, init)