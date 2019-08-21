#include<eosio/eosio.hpp>
#include<eosio/asset.hpp>
#include<eosio/singleton.hpp>
#include<vector>


using namespace eosio;
using namespace std;


CONTRACT projectgame: public contract {
public:

using contract::contract;

ACTION transfer (name from, name to, asset quantity, string memo);

private:

TABLE payment{
    name username;
    vector<asset> values;

    uint64_t primary_key() const { return username.value;}

};
typedef eosio::multi_index<"payment"_n, payment> payment_index;


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


TABLE potsetting{
    uint64_t potnumber;
    vector<uint64_t> potvalue;
};
typedef eosio::singleton<"singletons"_n, potsetting> pot_setting;
typedef eosio::multi_index<"singletons"_n, potsetting> dummy_for_abi;
};

