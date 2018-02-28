//修改 2018.2.19
//2018.2.28 add tdma_set_SWBA
struct ieee80211_hw;
void tdma_set_SWBA(struct ieee80211_hw *hw,int period);
void tdma_tasklet(unsigned long data);