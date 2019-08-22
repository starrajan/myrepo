#include "playeracc.hpp"


    ACTION playeracc::count(name user, asset balance){
        //require_auth(name("projectgame"));
        index_account getaccount("projectgame"_n, "projectgame"_n.value);
        auto index = getaccount.get_index<"username"_n>();
        auto itr = index.lower_bound(user.value);
        print(user);
        count_index counts(_self, _self.value);
        auto itrr = counts.find(user.value);
        if(itrr == counts.end() ){
            counts.emplace("projectgame"_n,[&](auto& row){
            row.username=user;
            row.account_balance=balance;
             });
        }
        else{
            counts.modify(itrr,"projectgame"_n,[&](auto& row){
            row.account_balance += balance;
        });
        }      
    }
