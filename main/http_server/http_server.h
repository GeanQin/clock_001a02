#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

#include "esp_http_server.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define BIND_WEB \
"<!DOCTYPE html> \
<html lang=\"en\"> \
<head> \
  <meta charset=\"UTF-8\"> \
  <title>设备绑定</title> \
  <style> \
    body { \
      background-color: black; \
      color: white; \
      text-align: center; \
    } \
    #headline { \
      margin-top: 32px; \
      margin-bottom: 32px; \
      width: 100%; \
      height: 64px; \
      font-size: 48px; \
    } \
    #apply { \
      display: inline-block; \
      width: 248px; \
      margin-top: 4px; \
    } \
  </style> \
</head> \
<body> \
  <div id=\"headline\">设备绑定</div> \
  <form action=\"/bind\" method=\"get\"> \
    <div> \
      <span>网络名称</span> \
      <input type=\"text\" name=\"ssid\"/> \
    </div> \
    <div> \
      <span>网络密码</span> \
      <input type=\"password\" name=\"psk\"/> \
    </div> \
    <div> \
      <span>用户邮箱</span> \
      <input type=\"text\" name=\"email\"/> \
    </div> \
    <div> \
      <span>用户密码</span> \
      <input type=\"password\" name=\"password\"/> \
    </div> \
    <div> \
      <span>设备名称</span> \
      <input type=\"text\" name=\"name\"/> \
    </div> \
    <div> \
      <span>摆放位置</span> \
      <input type=\"text\" name=\"position\"/> \
    </div> \
    <input id=\"apply\" type=\"submit\" value=\"提交\"/> \
  </form> \
</body> \
</html>"

httpd_handle_t http_server_start();

#ifdef __cplusplus
}
#endif

#endif

