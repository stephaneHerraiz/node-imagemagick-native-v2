#ifndef DRAWTEXT_H
#define DRAWTEXT_H

#include "stroke.h"
#include <Magick++.h>
#include <nan.h>
#include <v8.h>

using namespace v8;
using namespace node;

struct Font
{
  std::string family;
  Magick::StyleType style;
  Magick::StretchType strech;
  double size;
};

struct DrawTextOptions
{
  Font *font;
  Magick::GravityType gravity;
  Magick::Color color;
  StrokeOptions *stroke;
};

class DrawTextWorker : public Nan::AsyncWorker
{
public:
  DrawTextWorker(Nan::Callback *, Magick::Image *,
                 double, double, std::string, DrawTextOptions *);
  ~DrawTextWorker();

  void Execute();

  static DrawTextOptions *castOptions(Local<Object>, Isolate *);

private:
  Magick::Image *image;
  double x_;
  double y_;
  std::string text_;
  DrawTextOptions *options_;
};
#endif