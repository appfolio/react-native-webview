#include "webview_lifecycle_impl.hpp"

namespace RNCWebView {
    std::shared_ptr<WebviewLifecycle> WebviewLifecycle::create() {
        return std::make_shared<WebviewLifecycleImpl>();
    }

    WebviewLifecycleImpl::WebviewLifecycleImpl() {

    }

    bool WebviewLifecycleImpl::onShouldStartLoadWithRequest(std::string url) {
        return true;
    }
}
