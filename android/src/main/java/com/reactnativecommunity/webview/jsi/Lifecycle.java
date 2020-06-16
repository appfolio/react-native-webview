package com.reactnativecommunity.webview.jsi;

public class Lifecycle {
  static {
    System.loadLibrary("reactnativecommunity-webview");
  }

  public native boolean onShouldStartLoadWithRequest(long javaScriptContextHolder, String key, String url, boolean loading, String title, boolean canGoBack, boolean canGoForward);
}
