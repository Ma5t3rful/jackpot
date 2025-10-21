#include "telegram_interface.hpp"
#include "cpr/api.h"
#include "cpr/cprtypes.h"
#include "cpr/parameters.h"
#include "nlohmann/json_fwd.hpp"
#include <algorithm>
#include <chrono>
#include <format>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string_view>
#include <thread>


TelegramBaseInterface::TelegramBaseInterface(const std::string_view token):
m_token(token),
m_sendmessage_address(std::format("https://api.telegram.org/bot{}/sendMessage",m_token))
{}


auto TelegramBaseInterface::send_message(const std::string& message, const long long id, const long long reply_id) -> void
{
    constexpr auto retries = 5;
    for(auto i = 0 ; i < retries ; i++)
    {
        const auto response = cpr::Get(cpr::Url(m_sendmessage_address),
        cpr::Parameters{{"text",message},{"chat_id",std::to_string(id)}});
        if(response.status_code == 200 and response.text.starts_with("{\"ok\":true"))return ;
    }
}
auto TelegramBaseInterface::async_send_message (const std::string& message, const long long id, const long long reply_id) -> void
{
    // "safe" memory leak (Made a quick fix for it)
    m_send_threads.emplace_back(&TelegramInterface::send_message,this,message,id,reply_id);
    if(m_send_threads.size() > 20)
    {
        auto rem_iter = std::ranges::remove_if(m_send_threads,&std::jthread::joinable);
        m_send_threads.erase(rem_iter.begin(),rem_iter.end());
    }
}



TelegramInterface::TelegramInterface(const std::string_view token, const std::function<void(const nlohmann::json&, TelegramBaseInterface*)>& poll_signal ):
TelegramBaseInterface(token),
m_update_addr(std::format("https://api.telegram.org/bot{}/getUpdates",m_token)),
m_poll_signal(poll_signal),
m_update_id(0)
{
    const auto _ = //ignore the first poll and discard all previous masseges.
    poll();
}


nlohmann::json TelegramInterface::poll()
{
    const auto tel_server_response = cpr::Get(cpr::Url{m_update_addr},
                                              cpr::Parameters{{"allowed_updates","[\"message]\""},{"limit", "100"},{"offset",std::to_string(m_update_id+1)}});
    if(tel_server_response.status_code != 200){std::this_thread::sleep_for(std::chrono::microseconds(10));return {};}
    const auto json_response = nlohmann::json::parse(tel_server_response.text)["result"];
    if(json_response.size()>0)m_update_id = json_response.back()["update_id"];
    return json_response;
}


void TelegramInterface::mainloop ()
{
    while(true)
    {
        for(const auto& p: poll()){
            if(not p.contains("message"))continue;
            m_poll_signal(p["message"],this);
        }
    }
}

