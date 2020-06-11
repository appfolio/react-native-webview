package com.reactnativecommunity.webview.jsi;

public class Lifecycle {
  public native boolean onShouldStartLoadWithRequest(long javaScriptContextHolder, int viewId, String url);

  static {
    System.loadLibrary("reactnativecommunity-webview");
  }
}
