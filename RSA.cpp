#include "RSA.h"
#include <iostream>

uint32_t publicKey;
uint32_t privateKey;
uint32_t moduluss;

using namespace std;

// Function to calculate the greatest common divisor
uint32_t gcd(uint32_t a, uint32_t b) {
    
    while (b > 0) {
        a %= b;
        uint32_t tmp = a;
        a = b;
        b = tmp;
  }
  return a;
}

uint32_t num_gen(int bits) {
    uint32_t val, random_num = 0;
    // Loop to read from A1, to generate bit and multiply by pow(2,  bit position)
    for (int i = 0; i < bits; ++i) {
        val = analogRead(A1); // Read random noise from A1
        if (val&1) {
          random_num += pow(2, i);
        }
        delay(5);
    }
    random_num += pow(2, bits);
    return random_num;
}

// Given an integer x, possibly negative, return an integer
// in the range 0..m-1 that is congruent to x (mod m)
int64_t reduce_mod(int64_t x, uint32_t m) {
    uint32_t rem = x % m;
    if (rem < 0) {
        rem += m;
    }
    return rem;
}

// Generate public key
uint32_t generate_e(uint32_t phi_n, int bits) {
    uint32_t e;
    do {
        e = num_gen(bits);
    } while(gcd(e, phi_n) != 1);
    return e;
}

// Find a d such that e·d ≡ 1 (mod φ(n)) or declares fail
uint32_t generate_d(uint32_t e, uint32_t phi_n) {
    if (gcd(e, phi_n) != 1) {
        return 1; //FAIL
    }
    else {
        // Find an integer d such that (e*d) == 1 (mod phi_n)
        uint32_t q, r[40], s[40], t[40];
        r[0] = e; r[1] = phi_n;
        s[0] = 1; s[1] = 0;
        t[0] = 0; t[1] = 1;
        
        uint32_t i = 1;
        while (r[i] > 0) {
            q = r[i-1] / r[i];
            r[i+1] = r[i-1] - q*r[i];
            s[i+1] = s[i-1] - q*s[i];
            t[i+1] = t[i-1] - q*t[i];
            ++i;
        }
        uint32_t d = s[i-1];
        
        if (d < 0 || d >= phi_n) {
            d = reduce_mod(d, phi_n);
        }
        return d;
    }
}




void setKeys(uint32_t p, uint32_t q, int bits) {
    // First part of public key:
    uint32_t n = p * q;

    uint32_t phi = (p - 1) * (q - 1);

    uint32_t e = generate_e(phi, bits);
    uint32_t d = generate_d(e, phi);

    publicKey = e;
    privateKey = d;
    moduluss = n;
}
