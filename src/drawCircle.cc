#include "drawCircle.h"

DrawCircleWorker::DrawCircleWorker(Nan::Callback *callback, Magick::Image *image,
                                   double x_, double y_,
                                   double perimX_, double perimY_, DrawCircleOptions *options_) : Nan::AsyncWorker(callback), image(image),
                                                                                                  x_(x_), y_(y_),
                                                                                                  perimX_(perimX_), perimY_(perimY_), options_(options_) {}

DrawCircleWorker::~DrawCircleWorker() {}

void DrawCircleWorker::Execute()
{
    // this executes outside node main thread.
    // Cannot call any v8 or nan objects and functions here,
    // but we can run long C functions that would otherwise hold our main Thread
    try
    {
        std::list<Magick::Drawable> drawList;
        if (options_->stroke != NULL)
        {
            drawList.push_back(Magick::DrawableStrokeColor(options_->stroke->color)); // Outline color
            drawList.push_back(Magick::DrawableStrokeOpacity(options_->stroke->opacity));
            drawList.push_back(Magick::DrawableStrokeWidth(options_->stroke->width)); // Stroke width
            drawList.push_back(Magick::DrawableStrokeLineJoin(options_->stroke->lineJoin));
        }
        drawList.push_back(Magick::DrawableFillColor(options_->color));
        drawList.push_back(Magick::DrawableCircle(x_, y_, perimX_, perimY_));
        image->draw(drawList);
    }
    catch (Magick::Error &error)
    {
        std::string err = "DrawCircle - ";
        err += error.what();
        Nan::AsyncWorker::SetErrorMessage(err.c_str());
    }
    catch (std::exception &error)
    {
        std::string err = "DrawCircle - Caught C++ STD exception: ";
        err += error.what();
        Nan::AsyncWorker::SetErrorMessage(err.c_str());
    }
}

DrawCircleOptions *DrawCircleWorker::castOptions(Local<Object> options_, Isolate *isolate)
{
    DrawCircleOptions *options = new DrawCircleOptions();
    if (options_->IsUndefined())
    {
        options->color = "black";
        return options;
    }

    Local<Object> strokeObj = Local<Object>::Cast(Nan::Get(options_, Nan::New<String>("stroke").ToLocalChecked()).ToLocalChecked());
    options->stroke = castStrokeOPtions(strokeObj, isolate);

    Local<Value> color = Nan::Get(options_, Nan::New<String>("color").ToLocalChecked()).ToLocalChecked();
    if (!color->IsUndefined())
    {
        v8::String::Utf8Value v8_text(isolate, color->ToString());
        options->color = *v8_text;
    }
    else
    {
        options->color = "black";
    }

    return options;
}
