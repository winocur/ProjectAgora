#ifndef H_RESOURCES
#define H_RESOURCES

struct Resources {
    i32 energy;
    i32 production;
    i32 exchange;
};

bool HasEnoughResources (Resources stock, Resources cost) {

    if(stock.energy     < cost.energy) return false;
    if(stock.exchange   < cost.exchange) return false;
    if(stock.production < cost.production) return false;

    return true;
}

Resources operator+ (Resources res1, Resources res2) {
    return {
        res1.energy     + res2.energy,
        res1.production + res2.production,
        res1.exchange   + res2.exchange,
    };
}

Resources operator- (Resources res1, Resources res2) {
    return {
        res1.energy     - res2.energy,
        res1.production - res2.production,
        res1.exchange   - res2.exchange,
    };
}

#endif