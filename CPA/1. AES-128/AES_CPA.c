#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "AES_CPA.h"

#define XN 	"CTF-1-AES-trace.bin"
#define YN 	"CTF-1-AES-plain.bin"

#define startpt 0
#define endpt 7000

void cpa() {
	FILE    *RFP, *YFP, *WFP;
    float   *corr;
    u8      **PT;
    u8      *hw;
    float   *mx;
    float   **data;
    int     trNum       = 5000;
    int     trLen       = 24000;
    int     wt, loc;
	u8		RS[16];
    cr      local, global;
    
    hw = (u8*)calloc(sizeof(u8), trNum);
    mx = (float*)calloc(sizeof(float), trNum);
    
    corr = (float*)calloc(sizeof(double), trLen);

	if((RFP = fopen(XN, "rt")) == NULL) puts("MAIN :: TRACE FILE IS NOT DETECTED");

	data = (float**)calloc(sizeof(float*), trNum);
    for(int i = 0 ; i < trNum ; i++)
        data[i] = (float*)calloc(sizeof(float), trLen);

    for(int i = 0 ; i < trNum ; i++)
        fread(data[i], sizeof(float), trLen, RFP);
    fclose(RFP);

    if((YFP = fopen(YN, "rt")) == NULL) puts("MAIN :: MODEL FILE IS NOT DETECTED");
    PT = (u8**)calloc(sizeof(u8*), trNum);
    for(int i = 0 ; i < trNum ; i++)
        PT[i] = (u8*)calloc(sizeof(u8), 16);
    
    for(int i = 0 ; i < trNum ; i++)
        fread(PT[i], sizeof(u8), 16, YFP);

    fclose(YFP);

    
    for(int block = 0 ; block < 16 ; block++) {
        init(&local, 0);
        init(&global, 0);

        for(int key = 0 ; key < 256 ; key++) {
            init(&local, 0);
            for(int in = 0 ; in < trNum ; in++) {
                PT[block][in] = S[0][PT[block][in] ^ key];
                for(int i = 0 ; i < 8 ; i++)
                    hw[trNum] += (PT[block][in] >> i) & 0b1;
            }

            for(int loc = startpt ; loc < endpt ; loc++) {
                for(int i = 0 ; i < trNum ; i++)
                    mx[i] = data[i][loc];

                corr[loc] = correlation(hw, mx, trNum);

                if(fabs(corr[loc]) > local.maxcorr) {
                    local.maxcorr = fabs(corr[loc]);
                    local.maxloc  = loc;
                    local.maxkey  = key;
                    }
            }
            if(key == 255)
            printf("\r  %02dth Block | KEY[%02X] CORR[%lf]                         \n", block, local.maxkey, local.maxcorr);
            else    
            printf("\r%02dth Block : %.1lf%% CR[%lf] K[%02X]", block, ((double)key / 255) * 100, local.maxcorr, local.maxkey);

            if(local.maxcorr > global.maxcorr)
                global = local;
        }

    }



    free(corr);
    for(int i = 0 ; i < trNum ; i++) {
        free(data[i]);
        free(PT[i]);
    }
    free(data);
    free(PT);
    free(hw);
    free(mx);
}

int main()
{
	u8 	PT[16] = { 0xB6, 0xB6, 0xBE, 0xB0, 0x7E, 0xA7, 0xA7, 0x9F, 0x14, 0x72, 0xBF, 0x8D, 0x1B, 0xE3, 0xD3, 0xAE };
	u8 	CT[16] = { 0x96, 0x2D, 0x9A, 0xE2, 0x76, 0xA5, 0x34, 0x52, 0xF8, 0xC4, 0x06, 0xAC, 0xC6, 0x7D, 0x52, 0xB4 };
	u8 	*MK;
    
    cpa();

	for(int i = 0 ; i < 16 ; i++) printf("%02X ", MK[i]);
	puts(" ");
}
