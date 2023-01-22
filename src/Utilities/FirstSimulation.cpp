#include <cmath>
#include <random>
#include <vector>
#include <cstdlib>
#include <iostream>
#include "qcustomplot.h"
using namespace std;
#include "Utilities.hpp"
#include "../Protocols/UniswapV2Pool.hpp"
//#include <C:\Users\slshu\OneDrive\Documents\Ecole Polytechnique\Year 2\Intro to C++\Project\src\Utilities\Utilities.hpp>
//#include <C:\Users\slshu\OneDrive\Documents\Ecole Polytechnique\Year 2\Intro to C++\Project\src\Protocols\UniSwapV2Pool.hpp>


enum Players {
    Trader = 0,
    Liquidity_Provider = 1,
    Arbritageur = 2
};

enum Provision_Type {
    Withdraw = 0,
    Supply = 1
};

double action_type(double market_price_ratio, double pool_price_ratio) {
    /* This function adjust probabilities of actions based on the ratios of the market price
     *
     * @param : market_price_ratio : self-explanotary
     * @param : pool_price_ratio : self-explanotary
     *
     * @return: The next action type that will be taken (LP, Trade, Arb)
     */

    double l2t = 0.15;

    // Adjustable constant, to make change how often LP is chosen

    double random = (rand() % 1000);
    double sample = random/1000;

    // Random Sample

    double arb_inc = abs(1 - market_price_ratio/pool_price_ratio);

    double arb_prob = 1.8 * arb_inc;

    // Adjustable constant, to change how often Arbritaguer is chosen; method of calculation
    // uses the ratio and how different the ratios are of pool and true market price

    std::cout << sample << std::endl;
    std::cout << arb_prob << std::endl;

    if (sample < l2t){
        return Liquidity_Provider;
    }
    else if (sample < l2t + arb_prob){
        return Arbritageur;
    }
    else {
        return Trader;
    }
    ;
}


Token* trade_sim_tokenchoice(Token* tok1, Token* tok2 ){
    /* Returns the token that will be traded
     *
     * @param : tok1
     * @param : tok2
     *
     * @return : token that has been chosen
     */

    double random = rand()%1000;
    double sample = random/1000;

    if (sample<0.5){
        return tok1;
    }
    else {
        return tok2;
    }
}

double trade_sim_tradeval(double market_token, double trader_token){
    /* This function gives the value of the trade based on how much the trader has and how much is present in the market
     *
     * @param : market_token
     * @param : trader_token
     *
     * @return: trade value sample
     */


    double mean_trade = market_token/5;
    double var_trade = trader_token/100;

    //These can be changed to change the overall size of trades within the pool

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(mean_trade,var_trade);


    double sample = distribution(generator);

    std::cout << sample << std::endl;
    std::cout << mean_trade << std::endl;
    std::cout << var_trade << std::endl;

    return sample;
}

double arb_simulation(double market_exchange_rate, double quantity1, double quantity2){
    /* This function tells us how much of the token should be traded for a perfect arbritage oppurtunity (UniSwap V2)
     * @param: market_exchange_rate
     * @param: quantity1 : How much of token 1
     * @param: quantity2 : How much of token 2
     *
     * @return: the value to be traded
     */

    double constant = quantity1 * quantity2;
    double new_quantity_1 = sqrt(constant * market_exchange_rate);


    return quantity1 - new_quantity_1;
}
double arb_simulation_balancer(double quantity1, double quantity2, double weight_1, double weight_2, double spot_pricechange){
    /* This function tells us how much of the token should be traded for a perfect arbritage oppurtunity (Balancer Pool)
     * @return: the value to be traded
     */

    double init_spot_price=(quantity1*weight_2)/(quantity2*weight_1);
    double powr=weight_2/(weight_1+weight_2);
    double should_trade=quantity2*(pow((spot_pricechange/init_spot_price),powr)-1);


    return should_trade;
}

double provision_type(){
    /* This function just tells us whether we should provide or withdraw
     *
     * @return: Provision Type
     */

    double random = rand()%1000;
    double sample = random/1000;

    if (sample < 0.5){
        return 1;
    }
    else {
        return 0;
    }
}

std::unordered_map<Token *, double> liquidity(double market_token1, double market_token2, double lp_token1, double lp_token2, Token *token1, Token *token2){
    /* This function calculates the value and in what ratio the provision should take place
     *
     * @param: market_token1 : How much token 1 in the market
     * @param: market_token2 : How much token 2 in the market
     * @param: lp_token1 : How much token 1 in the LP wallet
     * @param: lp_token2 : How much token 2 in LP wallet
     * @param: token1
     * @param: token2
     *
     * @return: dictionary of correct values of token 1 and token 2
     */


    double mean = lp_token1/10;
    double var = lp_token1/100;

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(mean,var);

    double token1_provision_quantity = distribution(generator);
    std::cout << token1_provision_quantity << std::endl;

    //random sampling of token 1 provision quantity

    double ratio = market_token2/market_token1;

    double token2_provision_quantity = ratio * token1_provision_quantity;

    //calculation of correct token 2 provision quantity

    return {{token1, token1_provision_quantity}, {token2, token2_provision_quantity}};

}

double random_withdrawal (double pool_tokens) {
    /* This function samples the value of pool withdrawal
     *
     * @param: pool_tokens : How many pool tokens the LP has
     *
     * @return random sample of withdrawal
     */

    double mean = pool_tokens/10;
    double var = pool_tokens/100;

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(mean,var);

    return distribution(generator);
}

void print(std::vector<double> const &input)
{
    std::cout << "Hello!!!" << std::endl;
    for (int i = 0; i < input.size(); i++) {
        std::cout << input.at(i) << ' ';
    }
}

void SimulationOne(){
    Token *token1 = Token::GetToken("token1");
    Token *token2 = Token::GetToken("token2");

    UniswapV2Pool pool({{token1, 100}, {token2, 200}});

    Account *account = Account::GetAccount("LP");
    account->Deposit(token1, 100);
    account->Deposit(token2, 100);

    Account *trader = Account::GetAccount("Trader");
    trader->Deposit(token1, 1000);
    trader->Deposit(token2, 1000);

    Account *arb = Account::GetAccount("Arbritageur");
    arb->Deposit(token1, 100);
    arb->Deposit(token2, 100);

    std::vector<double> slippages = {1, 0};
    std::vector<double> spot_exchange_rates = {2, 0.5};
    std::vector<double> trader_balances_1 = {3, 1000};
    std::vector<double> trader_balances_2 = {4, 1000};
    std::vector<double> pool_quantity_1 = {5, 100};
    std::vector<double> pool_quantity_2 = {6, 200};
    std::vector<double> test = {7};


    for (int i = 0; i < 10; i++){

        double swap_quantity = pool.SimulateSwap(token1, token2, 1);
        double slip = pool.ComputeSlippage(token1, token2, 1);
        slippages.push_back(slip);

        pool.ExecuteSwap(trader, token1, token2, 1, swap_quantity);

        double exchange_rate = pool.ComputeSpotExchangeRate(token1, token2);
        spot_exchange_rates.push_back(exchange_rate);

        double quantity_1 = pool.GetQuantity(token1);
        double quantity_2 = pool.GetQuantity(token2);
        pool_quantity_1.push_back(quantity_1);
        pool_quantity_2.push_back(quantity_2);

        double trader_1 = trader->GetQuantity(token1);
        double trader_2 = trader->GetQuantity(token2);
        trader_balances_1.push_back(trader_1);
        trader_balances_2.push_back(trader_2);

    }
    print(slippages);
    print(spot_exchange_rates);
    print(trader_balances_1);
    print(trader_balances_2);
    print(pool_quantity_1);
    print(pool_quantity_2);
    print(test);

    QCustomPlot customPlot;
        customPlot.setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        customPlot.xAxis->setLabel("x-axis label");
        customPlot.yAxis->setLabel("y-axis label");
        customPlot.legend->setVisible(true);

        // Add data to the chart
        QCPGraph *slippagesGraph = customPlot.addGraph();
        slippagesGraph->setData(slippages);
        slippagesGraph->setPen(QPen(Qt::red));
        slippagesGraph->setName("Slippages");

        QCPGraph *spotExchangeRatesGraph = customPlot.addGraph();
        spotExchangeRatesGraph->setData(spot_exchange_rates);
        spotExchangeRatesGraph->setPen(QPen(Qt::blue));
        spotExchangeRatesGraph->setName("Spot Exchange Rates");

        QCPGraph *traderBalancesGraph1 = customPlot.addGraph();
        traderBalancesGraph1->setData(trader_balances_1);
        traderBalancesGraph1->setPen(QPen(Qt::green));
        traderBalancesGraph1->setName("Trader Balance 1");

        QCPGraph *traderBalancesGraph2 = customPlot.addGraph();
        traderBalancesGraph2->setData(trader_balances_2);
        traderBalancesGraph2->setPen(QPen(Qt::magenta));
        traderBalancesGraph2->setName("Trader Balance 2");

        QCPGraph *poolQuantityGraph1 = customPlot.addGraph();
        poolQuantityGraph1->setData(pool_quantity_1);
        poolQuantityGraph1->setPen(QPen(Qt::cyan));
        poolQuantityGraph1->setName("Pool Quantity 1");

        QCPGraph *poolQuantityGraph2 = customPlot.addGraph();
        poolQuantityGraph2->setData(pool_quantity_2);
        poolQuantityGraph2->setPen(QPen(Qt::yellow));
        poolQuantityGraph2->setName("Pool Quantity 2");

        // Set axis ranges and show the chart
        customPlot.rescaleAxes();
        customPlot.replot();
        customPlot.show();

}

void SimulationTwo(){
    Token *token1 = Token::GetToken("token1");
    Token *token2 = Token::GetToken("token2");

    //Define tokens

    UniswapV2Pool pool({{token1, 0}, {token2, 0}});

    //Initialise pool with 0 tokens

    Account *account = Account::GetAccount("LP");
    account->Deposit(token1, 1000);
    account->Deposit(token2, 1000);

    //Define account for Liquidity Provider

    pool.Provide(account, {{token1, 100}, {token2, 200}});

    //Deposit tokens into Pool

    Account *trader = Account::GetAccount("Trader");
    trader->Deposit(token1, 1000);
    trader->Deposit(token2, 1000);

    //Define account for Trader

    Account *arb = Account::GetAccount("Arbritageur");
    arb->Deposit(token1, 100);
    arb->Deposit(token2, 100);


    //Define account for Arbritageur

    std::vector<double> slippages = {0};
    std::vector<double> spot_exchange_rates = {0.5};
    std::vector<double> trader_balances_tok1 = {1000};
    std::vector<double> trader_balances_tok2 = {1000};
    std::vector<double> lp_balances_tok1 = {1000};
    std::vector<double> lp_balances_tok2 = {1000};
    std::vector<double> lp_balances_pooltok = {1000};
    std::vector<double> arb_balances_tok1 = {1000};
    std::vector<double> arb_balances_tok2 = {1000};
    std::vector<double> pool_quantity_1 = {10};
    std::vector<double> pool_quantity_2 = {20};
    
    /*
    QCustomPlot customPlot;
        customPlot.setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        customPlot.xAxis->setLabel("x-axis label");
        customPlot.yAxis->setLabel("y-axis label");
        customPlot.legend->setVisible(true);

        // Add data to the chart
        QCPGraph *slippagesGraph = customPlot.addGraph();
        slippagesGraph->setData(slippages);
        slippagesGraph->setPen(QPen(Qt::red));
        slippagesGraph->setName("Slippages");

        QCPGraph *spotExchangeRatesGraph = customPlot.addGraph();
        spotExchangeRatesGraph->setData(spot_exchange_rates);
        spotExchangeRatesGraph->setPen(QPen(Qt::blue));
        spotExchangeRatesGraph->setName("Spot Exchange Rates");

        QCPGraph *traderBalancesGraph1 = customPlot.addGraph();
        traderBalancesGraph1->setData(trader_balances_tok1);
        traderBalancesGraph1->setPen(QPen(Qt::green));
        traderBalancesGraph1->setName("Trader Balance of Token 1");

        QCPGraph *traderBalancesGraph2 = customPlot.addGraph();
        traderBalancesGraph2->setData(trader_balances_tok2);
        traderBalancesGraph2->setPen(QPen(Qt::magenta));
        traderBalancesGraph2->setName("Trader Balance of Token 2");
    
        QCPGraph *poolQuantityGraph1 = customPlot.addGraph();
        poolQuantityGraph1->setData(lp_balances_tok1);
        poolQuantityGraph1->setPen(QPen(Qt::brown));
        poolQuantityGraph1->setName("Liquidity Pool Balance of Token 1");
    
        QCPGraph *poolQuantityGraph1 = customPlot.addGraph();
        poolQuantityGraph1->setData(lp_balances_tok2);
        poolQuantityGraph1->setPen(QPen(Qt::orange));
        poolQuantityGraph1->setName("Liquidity Pool Balance of Token 2");
    
        QCPGraph *poolQuantityGraph1 = customPlot.addGraph();
        poolQuantityGraph1->setData(lp_balances_pooltok);
        poolQuantityGraph1->setPen(QPen(Qt::purple));
        poolQuantityGraph1->setName("Liquidity Pool Total Balance");

        QCPGraph *poolQuantityGraph1 = customPlot.addGraph();
        poolQuantityGraph1->setData(arb_balances_tok1);
        poolQuantityGraph1->setPen(QPen(Qt::pink));
        poolQuantityGraph1->setName("Arbritageur Balance for Token 1");
    
        QCPGraph *poolQuantityGraph1 = customPlot.addGraph();
        poolQuantityGraph1->setData(arb_balances_tok2);
        poolQuantityGraph1->setPen(QPen(Qt::lightgreen));
        poolQuantityGraph1->setName("Arbritageur Balance for Token 2");
    
        QCPGraph *poolQuantityGraph1 = customPlot.addGraph();
        poolQuantityGraph1->setData(pool_quantity_1);
        poolQuantityGraph1->setPen(QPen(Qt::cyan));
        poolQuantityGraph1->setName("Pool Quantity 1");

        QCPGraph *poolQuantityGraph2 = customPlot.addGraph();
        poolQuantityGraph2->setData(pool_quantity_2);
        poolQuantityGraph2->setPen(QPen(Qt::yellow));
        poolQuantityGraph2->setName("Pool Quantity 2");

        // Set axis ranges and show the chart
        customPlot.rescaleAxes();
        customPlot.replot();
        customPlot.show();
    */

    //Initialize vector lists for all variables to be monitered

    double market_exchange_rate = 0.5;

    //Specify constant Market Exchange Rate

    for (int i = 0; i < 15; i++){
        std::cout << "---" << std::endl;
        double exchange_rate = pool.ComputeSpotExchangeRate(token1, token2);
        double action = action_type(market_exchange_rate, exchange_rate);

        // Compute the spot exchange rate and compare that with the market exchange rate to
        // randomly determine what action should be taken

        if (action == Liquidity_Provider){
            std::cout << "LP" << std::endl;
            double type_provision = provision_type();

            // Determine whether we will supply or withdraw Tokens

            if (type_provision == Supply){
                std::cout << "Supply" << std::endl;
                std::unordered_map<Token *, double> liquid = liquidity(pool.GetQuantity(token1), pool.GetQuantity(token2), account->GetQuantity(token1), account->GetQuantity(token2), token1, token2);
                pool.Provide(account, liquid);

                //Calculate how much liquidity should be provided and provide the pool with calculated amount

            }

            else if (type_provision == Withdraw){
                std::cout << "Withdraw" << std::endl;
                double pool_tok = account->GetQuantity(pool.pool_token_);
                double withdrawal = random_withdrawal(pool_tok);

                std::unordered_map<Token *, double> tokens = pool.SimulateWithdrawal(withdrawal);
                pool.ExecuteWithdrawal(account, withdrawal, tokens);

                //Calculate how much liquidity should be withdrawn and withdraw from pool with calculated amount
            }

        }
        else if (action == Trader){
        std::cout << "Trade" << std::endl;
        Token* demanded = trade_sim_tokenchoice(token1, token2);
        Token* supplied;

        //Determine which Token to supply and demand

        if (demanded == token1){
            supplied = token2;
        }
        else {
            supplied = token1;
        }

        double trade_val = trade_sim_tradeval(pool.GetQuantity(demanded), trader->GetQuantity(demanded));
        double swap_req = pool.ComputeInputRequirement(supplied, demanded, trade_val);
        pool.ExecuteSwap(trader, supplied, demanded, swap_req, trade_val);
        }

        //Execute Swap

        else if (action == Arbritageur){
            std::cout << "Arb" << std::endl;
            double change_token1 = arb_simulation(market_exchange_rate, pool.GetQuantity(token1), pool.GetQuantity(token2));

            if (change_token1 < 0){
                double output = pool.ComputeSwappedQuantity(token1, token2, abs(change_token1));
                pool.ExecuteSwap(arb, token1, token2, abs(change_token1), output);
            }
            else if (change_token1 > 0){
                double input = pool.ComputeInputRequirement(token2, token1, change_token1);
                pool.ExecuteSwap(arb, token2, token1, input, change_token1);
            }

            //Implement perfect arbritage function



        }

    }

}

