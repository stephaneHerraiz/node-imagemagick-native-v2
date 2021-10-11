#include "stroke.h"

StrokeOptions* castStrokeOPtions (Local<Object> options_, Isolate* isolate_) {
  if(options_->IsUndefined()) {
    return NULL;
  }
  
  Local<Context> context = isolate_->GetCurrentContext();
  StrokeOptions* options = new StrokeOptions();

  Local<Value> color = Nan::Get( options_, Nan::New<String>("color").ToLocalChecked() ).ToLocalChecked();
    if(!color->IsUndefined()) {
        v8::String::Utf8Value v8_text(isolate_, color->ToString());
        options->color = *v8_text;
    } else {
        options->color = "black";
    }

    Local<Value> lineJoinVal = Nan::Get( options_, Nan::New<String>("lineJoin").ToLocalChecked() ).ToLocalChecked();
    if(!lineJoinVal->IsUndefined()) {
        v8::String::Utf8Value str(isolate_, lineJoinVal);
        std::string lineJoin(*str);
        if(lineJoin == "UndefinedJoin") {
          options->lineJoin = Magick::UndefinedJoin;
        } else if(lineJoin == "MiterJoin") {
          options->lineJoin = Magick::MiterJoin;
        }else if(lineJoin == "RoundJoin") {
          options->lineJoin = Magick::RoundJoin;
        } else if(lineJoin == "BevelJoin") {
          options->lineJoin = Magick::BevelJoin;
        } else {
          options->lineJoin = Magick::MiterJoin;
        }
    } else {
      options->lineJoin = Magick::MiterJoin;
    }
    Local<Value> width = Nan::Get( options_, Nan::New<String>("width").ToLocalChecked() ).ToLocalChecked();
    options->width = width->NumberValue(context).FromJust();

    Local<Value> opacity = Nan::Get( options_, Nan::New<String>("opacity").ToLocalChecked() ).ToLocalChecked();
    options->opacity = opacity->NumberValue(context).FromJust();

  return options;
}