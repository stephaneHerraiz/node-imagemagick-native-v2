#ifndef DRAWRECTANGLE_H
#define DRAWRECTANGLE_H

#include "stroke.h"
#include <Magick++.h>
#include <nan.h>
#include <v8.h>

#include <nan.h>
#include <v8.h>

struct DrawRectangleOptions {
  Magick::Color color;
  StrokeOptions *stroke;
};

class DrawRectangleWorker : public Nan::AsyncWorker {
  public:
    DrawRectangleWorker(Nan::Callback *callback, Magick::Image *image,
      double, double,double, double, DrawRectangleOptions*
    );
    ~DrawRectangleWorker();

    static DrawRectangleOptions* castOptions(Local<Object>, Isolate*);

    void Execute();

  private:
    Magick::Image *image;
    double upperLeftX_;
    double upperLeftY_;
    double lowerRightX_;
    double lowerRightY;
    DrawRectangleOptions *options_;
};
#endif