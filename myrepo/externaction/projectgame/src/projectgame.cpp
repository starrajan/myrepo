#include "../include/projectgame.hpp"


ACTION projectgame::transfer (name from, name to, asset quantity, string memo){

if (to == _self){

payment_index payindex(_self, from.value);
auto itr = payindex.find(from.value);
if(itr == payindex.end()){
 payindex.emplace(_self, [&](auto& row){
        row.username = from;
        row.values = {quantity};
    });
}
else
{
    vector<asset> balances = itr->values;
    balances.push_back(quantity);
    payindex.modify(itr, _self, [&](auto& row){
    row.values = balances;
    });
}

index_account getaccount(_self, _self.value);
auto index = getaccount.get_index<"username"_n>();
auto itrr = index.lower_bound(from.value);
if (itrr == index.end()){
    getaccount.emplace(_self, [&](auto& row){
      row.userid = getaccount.available_primary_key();
      row.username = from;
      row.total_balance = quantity;
    });
    inlinefun(from, quantity);
}
else{
    index.modify(itrr,_self,[&](auto& row){
        row.total_balance += quantity;
    });
    inlinefun(from, itrr->total_balance);
}

pot_setting potter(_self, _self.value);
potsetting newsettings;

auto payamount = quantity*70/100;

uint64_t potbalance = (payamount.amount)/10000;

if(potbalance >500 && potbalance<1000)
{
    newsettings.potnumber = 4;
    newsettings.potvalue = {potbalance*70/100, potbalance*15/100, potbalance*10/100, potbalance*5/100};
}
else if(potbalance > 1000){
    newsettings.potnumber = 5;
    newsettings.potvalue = {potbalance*60/100, potbalance*20/100, potbalance*15/100, potbalance*10/100, potbalance*5/100};
}
else {
    newsettings.potnumber = 3;
    newsettings.potvalue = {potbalance*70/100, potbalance*20/100, potbalance*10/100};
}

potter.set(newsettings, _self);
auto itrrr = potter.get();
}
else{
    return;
}

}

ACTION projectgame::inlinefun(name user, asset balance){
    print(user);
    action( 
            permission_level{get_self(),"active"_n},
            "playeracc"_n,
            "count"_n,
            make_tuple(user, balance)
        ).send();
}



extern "C"
{
     void apply(uint64_t receiver, uint64_t code, uint64_t action) {
        if (action == "transfer"_n.value && code != receiver && code == "eosio.token"_n.value)
        {
            eosio::execute_action(eosio::name(receiver), eosio::name(code), &projectgame::transfer);
        }
        if (code == receiver)
        {
            switch (action)
            {
                EOSIO_DISPATCH_HELPER(projectgame, (transfer)(inlinefun))
            }
        }
    }
}