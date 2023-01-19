#include "Simulation.hpp"
#include <QThread>

Simulation::Simulation() {
    market_ = new Market();
    handler = new SignalsHandler();
    arb_ = new Arbitrager("Market Arb", 10000);
    LP_ = new Provider("Market LP", 100000);
}

std::unordered_set<PoolInterface *> Simulation::GetPools() const {
    return market_->GetMarketPools();
}
std::unordered_set<Token *> Simulation::GetTokens() const {
    return market_->GetMarketTokens();
}

Market *Simulation::GetMarket() const {
    return market_;
}

Account *Simulation::GetArb() const {
    return arb_;
}
Account *Simulation::GetLP() const {
    return LP_;
}




void Simulation::runEpoch() {
//    for (auto pool : GetPools()) {
//        arb_->StrategicTrade(pool);
//        LP_->StrategicProvide(pool);
//    }
//    market_->runEpoch();
//    handler->respondSignals();

//    for (auto pool : GetPools())
//        pool->endEpoch();
}
