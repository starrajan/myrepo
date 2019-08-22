#include<eosio/eosio.hpp>
#include<eosio/asset.hpp>


using namespace eosio;


    CONTRACT playeracc : public contract{

    public:

        using contract::contract;

        ACTION count(name user, asset balance);

        using inlinefun = action_wrapper<"count"_n, &playeracc::count>;
    

    private:

        TABLE profile{
            name username;
            asset account_balance;

            uint64_t primary_key() const{ return username.value;}
        };
        typedef eosio::multi_index<"profiles"_n, profile> count_index;


        TABLE account {
            name username;
            uint64_t userid;
            asset total_balance;

            uint64_t primary_key() const { return userid;}
            uint64_t by_username()  const { return username.value;}
        };
        typedef eosio::multi_index<"account"_n, account,
        eosio::indexed_by<"username"_n, eosio::const_mem_fun<account, uint64_t, &account::by_username>>>
        index_account;
    };