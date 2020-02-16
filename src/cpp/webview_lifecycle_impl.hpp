#pragma once

#include <string>

#include "webview_lifecycle.hpp"
//#include "webview_event.hpp"

namespace RNCWebView {

    class WebviewLifecycleImpl : public RNCWebView::WebviewLifecycle {

    public:
        WebviewLifecycleImpl();

        // Our method that returns a string
        bool onShouldStartLoadWithRequest(std::string url);
    };

}
