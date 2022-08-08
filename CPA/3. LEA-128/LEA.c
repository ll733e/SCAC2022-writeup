#include <stdio.h>
#include <stdlib.h>


unsigned int ByteToWordLE(unsigned char *p) {
    return p[3] << 24 | p[2] << 16 | p[1] << 8 | p[0];
}
unsigned int ByteToWordBE(unsigned char *p) {
    return p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
}

unsigned int ROR(int rotate, unsigned int P) {
    unsigned int T0;
    unsigned int T1;
    T0 = P << rotate;
    T1 = P >> (32 - rotate);

    return T1 | T0;
}

unsigned int ROL(int rotate, unsigned int P) {
    unsigned int T0;
    unsigned int T1;
    T0 = P >> rotate;
    T1 = P << (32 - rotate);

    return T1 | T0;
}

unsigned int ADD(unsigned int A, unsigned int B) {
    return (A + B) % 0x100000000;
}

void KeySchedule(unsigned char *K) {
    unsigned int    δ[8];
    δ[0] = 0xc3efe9db;
    δ[1] = 0x44626b02; 
    δ[2] = 0x79e27c8a;  
    δ[3] = 0x78df30ec; 
    δ[4] = 0x715ea49e; 
    δ[6] = 0xe04ef22a;
    δ[5] = 0xc785da0a; 
    δ[7] = 0xe5c40957;

    unsigned int    T[4];
    unsigned int    RK[144];
    
    T[0] = ByteToWordLE(K);
    T[1] = ByteToWordLE(K + 4);
    T[2] = ByteToWordLE(K + 8);
    T[3] = ByteToWordLE(K + 12);

    for(int i = 0 ; i < 24 ; i++) {
        T[0] = ROL(1,  (ADD(T[0], (ROL(i    , δ[i % 4])))));
        T[1] = ROL(3,  (ADD(T[1], (ROL(i + 1, δ[i % 4])))));
        T[2] = ROL(5,  (ADD(T[2], (ROL(i + 2, δ[i % 4])))));
        T[3] = ROL(11, (ADD(T[3], (ROL(i + 3, δ[i % 4])))));
        RK[i * 6    ] = T[0];
        RK[i * 6 + 1] = T[1];
        RK[i * 6 + 2] = T[2];
        RK[i * 6 + 3] = T[1];
        RK[i * 6 + 4] = T[3];
        RK[i * 6 + 5] = T[1];
    }
    for(int i = 0 ; i < 6 ; i++) {
        printf("%08x ", RK[i]);
        if(i % 6 == 5) puts("");
    }
}

void encrypt() {
    unsigned char   K[16] = { 0x0f, 0x1e, 0x2d, 0x3c, 0x4b, 0x5a, 0x69, 0x78, 0x87, 0x96, 0xa5, 0xb4, 0xc3, 0xd2, 0xe1, 0xf0 };
    unsigned char   P[16] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
    unsigned char   C[16];

    unsigned int    X[4];
    unsigned int    RK[6];

    X[0] = ByteToWordLE(P);
    X[1] = ByteToWordLE(P + 4);
    X[2] = ByteToWordLE(P + 8);
    X[3] = ByteToWordLE(P + 12);
}

void rotate() {
    unsigned A = 0x11223344;
    puts("");
    for(int i = 0 ; i < 32 ; i++)
        printf("%d", (A >> i) & 0b1);
        
        A = ROL(2, A);
        puts("");
    for(int i = 0 ; i < 32 ; i++)
        printf("%d", (A >> i) & 0b1);
    printf("\n%08x\n", A);
}
int main() {
    unsigned char   K[16] = { 0x0f, 0x1e, 0x2d, 0x3c, 0x4b, 0x5a, 0x69, 0x78, 0x87, 0x96, 0xa5, 0xb4, 0xc3, 0xd2, 0xe1, 0xf0 };
    unsigned int    δ = 0xc3efe9db;
    unsigned int    T[4];
    //KeySchedule(K);
    T[0] = ByteToWordLE(K);
    T[1] = ByteToWordLE(K + 4);
    T[2] = ByteToWordLE(K + 8);
    T[3] = ByteToWordLE(K + 12);

    unsigned int    RK;
    RK = (T[0] + δ); //% 0x10000000;

    for(int i = 0 ; i < 32 ; i++)
        printf("%d", (RK >> i) & 0b1);
        puts("");

    RK = ROL(1, RK);
    for(int i = 0 ; i < 32 ; i++)
        printf("%d", (RK >> i) & 0b1);
        puts("");
    printf("%08x", RK);
}