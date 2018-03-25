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
    //周期
    //Beacon period is 100000
    //SWBA period is 2000
    u64 next_swba=100000;
    u64 tsf = ath9k_hw_gettsf64(ah);
    u64 temptsf=tsf-next_swba;
    u64 slot= do_div(temptsf,100000);
    do_div(slot,2000);
    if(ah->opmode==NL80211_IFTYPE_AP){
        static int send_beacon=0;
        if(slot<4&&send_beacon==0){
            send_beacon=1;
            ath9k_beacon_tasklet(data);
            //printk("Slot = %llu, beacon_tasklet is acivate at %llu\n",slot,tsf);
        }
        if(slot>3){
            send_beacon=0;
            int tempslot=slot;
            if(tempslot%2==0){
                set_tdma_slot(0);
                return;
            }
            //printk("Slot = %llu, send data is acivate at %llu\n",slot,tsf);
            set_tdma_slot(1);
            tdma_send_data(hw);
        }
    }else if(ah->opmode==NL80211_IFTYPE_STATION){
        int tempslot=slot;
        if(tempslot>3&&tempslot%2==0){
            set_tdma_slot(1);
            tdma_send_data(hw);
            //printk("Slot = %llu, beacon_tasklet is acivate at %llu\n",slot,tsf);
        }else{
            set_tdma_slot(0);
        }    
    }
    
}