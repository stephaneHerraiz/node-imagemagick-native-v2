#include "drawText.h"

DrawTextWorker::DrawTextWorker(
    Nan::Callback *callback,
    Magick::Image *image,
    double x_,
    double y_,
    std::string text_,
    DrawTextOptions *options_) : Nan::AsyncWorker(callback), image(image),
                                 x_(x_), y_(y_), text_(text_), options_(options_) {}

DrawTextWorker::~DrawTextWorker() {}

void DrawTextWorker::Execute()
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
        }
        if (options_->font->size > 0)
        {
            drawList.push_back(Magick::DrawablePointSize(options_->font->size));
        }
        drawList.push_back(Magick::DrawableFillColor(options_->color)); // Fill color
        drawList.push_back(Magick::DrawableFont(options_->font->family));
        drawList.push_back(Magick::DrawableText(x_, y_, text_));
        image->draw(drawList);
    }
    catch (Magick::Error &error)
    {
        std::string err = "DrawText - ";
        err += error.what();
        Nan::AsyncWorker::SetErrorMessage(err.c_str());
    }
    catch (std::exception &error)
    {
        std::string err = "DrawText - Caught C++ STD exception: ";
        err += error.what();
        Nan::AsyncWorker::SetErrorMessage(err.c_str());
    }
}

// void HandleErrorCallback()
// {
//     Nan::HandleScope scope;
//     v8::Local<v8::Value> argv[] = {
//         Nan::New(Nan::AsyncWorker::ErrorMessage()).ToLocalChecked(), // return error message
//         Nan::Null()};
//     Nan::Call(callback->GetFunction(), Nan::GetCurrentContext()->Global(), 2, argv);
// }

DrawTextOptions *DrawTextWorker::castOptions(Local<Object> options_, Isolate *isolate)
{
    Local<Context> context = isolate->GetCurrentContext();
    DrawTextOptions *options = new DrawTextOptions();
    options->font = new Font();

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
    Local<Object> strokeObj = Local<Object>::Cast(Nan::Get(options_, Nan::New<String>("stroke").ToLocalChecked()).ToLocalChecked());
    if (!strokeObj->IsUndefined())
    {
        options->stroke = castStrokeOPtions(strokeObj, isolate);
    }
    else
    {
        options->stroke = NULL;
    }

    // Cast options font object
    Local<Object> fontObj = Local<Object>::Cast(Nan::Get(options_, Nan::New<String>("font").ToLocalChecked()).ToLocalChecked());

    if (!fontObj->IsUndefined())
    {
        Local<Value> fontFamily = Nan::Get(fontObj, Nan::New<String>("family").ToLocalChecked()).ToLocalChecked();
        if (!fontFamily->IsUndefined())
        {
            v8::String::Utf8Value v8_text(isolate, fontFamily->ToString());
            options->font->family = *v8_text;
        }
        else
        {
            options->font->family = "DejaVu-Sans";
        }

        Local<Value> size = Nan::Get(fontObj, Nan::New<String>("size").ToLocalChecked()).ToLocalChecked();
        options->font->size = size->NumberValue(context).FromJust();
    }
    else
    {
        options->font->family = "DejaVu-Sans";
    }

    return options;
}