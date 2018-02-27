//修改 2018.2.19
#include <net/tdma.h>
#include "ath9k.h"
void tdma_tasklet(unsigned long data)
{
    static int slot=0;
    struct ath_softc *sc = (struct ath_softc *)data;
    ++slot;
    if(slot==1){
        ath9k_beacon_tasklet(data);
    }else{
        //printk("++++++++++++++++++++++++++\n");
        tdma_send_data(sc->hw);
        if(slot==50)slot=0;
        //printk("-----------------------------\n");
    }
}