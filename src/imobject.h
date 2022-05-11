
#ifndef IMOBJECT_H
#define IMOBJECT_H

#include <nan.h>
#include "imagemagick.h"
#include <Magick++.h>

#ifdef _WIN32
  #include <stdexcept>
#endif

struct image_info
{
  unsigned int maxMemory;
  char *srcData;
  size_t length;
  std::string srcFormat;

  image_info() {}
};

struct Resize_info
{
  unsigned int width;
  unsigned int height;
  unsigned int xoffset;
  unsigned int yoffset;
  std::string resizeStyle;
  std::string gravity;

  Resize_info() {}
};

class IMObject : public Nan::ObjectWrap
{
public:
  static void Init(v8::Local<v8::Object> exports);
  Magick::Image image;

private:
  explicit IMObject();
  ~IMObject();

  convert_im_ctx *context;

  static void New(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static bool readImage(Magick::Image *image, Magick::Blob srcBlob, std::string srcFormat);
  static void Getlen(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void rotate(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void resize(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void getImage(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void compare(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void baseColumns(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void getWidth(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void getHeight(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void drawRectangle(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void drawCircle(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void drawText(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static Nan::Persistent<v8::Function> constructor;
};

#endif