#ifndef DRAWCIRCLE_H
#define DRAWCIRCLE_H

#include "stroke.h"
#include <Magick++.h>
#include <nan.h>
#include <v8.h>

#include <nan.h>
#include <v8.h>

struct DrawCircleOptions {
  Magick::Color color;
  StrokeOptions *stroke;
};

class DrawCircleWorker : public Nan::AsyncWorker {
  public:
    DrawCircleWorker(Nan::Callback *callback, Magick::Image *image,
      double, double,double, double, DrawCircleOptions*
    );
    ~DrawCircleWorker();

    static DrawCircleOptions* castOptions(Local<Object>, Isolate*);

    void Execute();

  private:
    Magick::Image *image;
    double x_;
    double y_;
    double perimX_;
    double perimY_;
    DrawCircleOptions *options_;
};
#endif