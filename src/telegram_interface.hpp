#pragma once
#include "cpr/cprtypes.h"
#include <string>
#include <string_view>
#include <functional>
#include <nlohmann/json.hpp>
#include <thread>
#include <vector>

class TelegramBaseInterface{
    public:
    TelegramBaseInterface(const std::string_view token);
    
    void send_message (const std::string& message,const long long chat_id,const long long reply_id=0);
    void async_send_message (const std::string& message,const long long chat_id,const long long reply_id=0);
    protected:
    const std::string m_token;
    private:
    const cpr::Url m_sendmessage_address;
    std::vector<std::jthread> m_send_threads;
};

class TelegramInterface
:public TelegramBaseInterface{
public:
    TelegramInterface(const std::string_view token, const std::function<void(const nlohmann::json&, TelegramBaseInterface*)>& poll_signal );
    void mainloop ();
private:
    auto poll () -> nlohmann::json;
    const std::function<void (const nlohmann::json&, TelegramBaseInterface*)> m_poll_signal; // let's call this when we successfully poll()'ed;
    const std::string m_update_addr;
    unsigned long m_update_id;
};
