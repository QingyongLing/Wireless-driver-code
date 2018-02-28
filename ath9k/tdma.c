//修改 2018.2.19
//2018.2.28 add tdma_set_SWBA
#include <net/tdma.h>
#include "ath9k.h"
void tdma_set_SWBA(struct ieee80211_local *local,int period){
    struct ieee80211_hw *hw=&(local->hw);
    struct ath_softc *sc = hw->priv;
    struct ath_hw *ah=sc->sc_ah;
    REG_WRITE(ah, AR_SWBA_PERIOD, period);
}

void tdma_tasklet(unsigned long data)
{
    static int slot=0;
    struct ath_softc *sc = (struct ath_softc *)data;
    struct ieee80211_hw *hw=sc->hw;
    struct ath_hw *ah=sc->sc_ah;
    //struct ieee80211_local *local = hw_to_local(hw);
    if(ah->opmode==NL80211_IFTYPE_AP){
        ++slot;
        if(slot==1){
            ath9k_beacon_tasklet(data);
        }else{
            //printk("++++++++++++++++++++++++++\n");
            tdma_send_data(hw);
            if(slot==50)slot=0;
            //printk("-----------------------------\n");
        }
    }else if(ah->opmode==NL80211_IFTYPE_STATION){
        printk("SWBA activate--------------------\n");
    }
    
}