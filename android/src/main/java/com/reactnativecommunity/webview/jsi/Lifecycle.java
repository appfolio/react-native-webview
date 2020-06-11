package com.reactnativecommunity.webview.jsi;

public class Lifecycle {
  public native boolean onShouldStartLoadWithRequest(long javaScriptContextHolder, int viewId, String url, boolean loading, String title, boolean canGoBack, boolean canGoForward);

  static {
    System.loadLibrary("reactnativecommunity-webview");
  }
}
