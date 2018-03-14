//修改 2018.2.19
#include <net/tdma.h>
#include "ath9k.h"
#include <asm/div64.h>
/*
mod = do_div(x,y);
result = x; 
*/

void tdma_tasklet(unsigned long data)
{
    struct ath_softc *sc = (struct ath_softc *)data;
    struct ieee80211_hw *hw=sc->hw;
    struct ath_hw *ah=sc->sc_ah;
    //struct ieee80211_local *local = hw_to_local(hw);
    //u32 tdma_tbtt_next=106144;  next_swba=100000us
    //Beacon period is 200000
    //SWBA period is 4000
    if(ah->opmode==NL80211_IFTYPE_AP){
        static int send_beacon=0;
        u64 tsf = ath9k_hw_gettsf64(ah);
        u64 temptsf=tsf;
        u64 slot= do_div(temptsf,200000);
        do_div(slot,4000);
        if(send_beacon==0&&slot<3){
            ath9k_beacon_tasklet(data);
            send_beacon=1;
            printk("Slot = %llu, beacon_tasklet is acivate at %llu\n",tsf);
        }
        if(slot>2){
            if(send_beacon==0){
                ath9k_beacon_tasklet(data);
                send_beacon=1;
                printk("------------Slot = %llu, beacon_tasklet is acivate at %llu\n",tsf);
            }else{
                printk("Slot = %llu, send data is acivate at %llu\n",tsf);
                tdma_send_data(hw);
            }
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