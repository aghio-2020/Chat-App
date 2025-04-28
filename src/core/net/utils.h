#include <boost/asio.hpp>
#include <thread>
#include <functional>
#include <future>
#include <memory>
#include <iostream>

namespace core::utils
{

    template<class SignalData>
    using Signal = std::function<void(boost::system::error_code, SignalData)>;

    template <typename Func, typename Handler>
    void asyncInvoke(boost::asio::io_context& io, Func func, Handler handler) 
    {
        // Create a shared pointer to hold the result future
        auto task = std::make_shared<std::packaged_task<typename std::invoke_result_t<Func>()>>(std::move(func));

        // Get the future
        auto future = task->get_future();

        // Run the task in a background thread
        std::thread([task]() 
        {
            (*task)();  // Run the function
        }).detach();

        // Poll the result asynchronously (non-blocking)
        boost::asio::post(io, [future = std::move(future), handler = std::move(handler), &io]() mutable 
        {
            // Instead of blocking here, use another post to poll again later if not ready
            if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) 
            {
                handler(future.get());  // Invoke the handler with the result
            }
            else 
            {
                // Try again later
                boost::asio::post(io, [future = std::move(future), handler = std::move(handler), &io]() mutable 
                {
                    asyncInvoke(io, [f = std::move(future)]() mutable { return f.get(); }, handler);
                });
            }
        });
    }

}
