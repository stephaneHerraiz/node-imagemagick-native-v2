#ifndef STROKE_H
#define STROKE_H

#include <v8.h>
#include <Magick++.h>
#include <nan.h>

using namespace v8;
using namespace node;

struct StrokeOptions {
  double width;
  Magick::Color color;
  double opacity;
  Magick::LineJoin lineJoin;
};

StrokeOptions* castStrokeOPtions (Local<Object>, Isolate*);

#endif