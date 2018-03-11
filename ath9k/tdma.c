//修改 2018.2.19
#include <net/tdma.h>
#include "ath9k.h"

void tdma_tasklet(unsigned long data)
{
    static int slot=0;
    struct ath_softc *sc = (struct ath_softc *)data;
    struct ieee80211_hw *hw=sc->hw;
    struct ath_hw *ah=sc->sc_ah;
    //struct ieee80211_local *local = hw_to_local(hw);
    if(ah->opmode==NL80211_IFTYPE_AP){
        ++slot;
        u64 tsf = ath9k_hw_gettsf64(ah);
        if(slot==1){
            ath9k_beacon_tasklet(data);
            printk("Slot = 1, SWBA is acivate at %llu\n",tsf);
        }else{
            if(slot==2){
                printk("Slot = 2, SWBA is acivate at %llu\n",tsf);
            }
            if(slot==3){
                printk("Slot = 3, SWBA is acivate at %llu\n",tsf);
            }
            tdma_send_data(hw);
            if(slot==50)slot=0;
        }
    }else if(ah->opmode==NL80211_IFTYPE_STATION){
        static int count=0;
        ah->imask &= ~ATH9K_INT_SWBA;
        ++count;
        u64 tsf=0;
        tsf = ath9k_hw_gettsf64(ah);
        printk("(4)SWBA activate %d times at %llu\n",count,tsf);
        if(count==1000)count=0;      
    }
    
}