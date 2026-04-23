double ONE_BTC_IN_SATOCHI = 123073.55;

double btc_to_satochi(double btc_amount){
    double result = (btc_amount * ONE_BTC_IN_SATOCHI); //cour actuel, a intégrer api
    return result;
}

double satochi_to_btc(double satochi_amount){
    double result = satochi_amount/ONE_BTC_IN_SATOCHI;
    return result;
}