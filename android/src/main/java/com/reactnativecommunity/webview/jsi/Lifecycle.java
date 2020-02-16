package com.reactnativecommunity.webview.jsi;

public class Lifecycle {
  public native void installBinding(long javaScriptContextHolder);
  public native boolean onShouldStartLoadWithRequest(long javaScriptContextHolder, int viewId, String url);

  private String stringField = "Private field value";

  static {
    System.loadLibrary("reactnativecommunity-webview");
  }

  public static String runTest() {
    return "Static field value";
  }
}