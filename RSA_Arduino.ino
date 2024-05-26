#include "Miller_Rabin.h"
#include "RSA.h"
#include <dht.h>

dht DHT;

#define DHT11_PIN 7

uint32_t p;
uint32_t q;
int p_bits=2;
int q_bits=2;
int e_bits=2;
int counter = 0;


uint32_t prime_gen(uint32_t bits) {
    uint32_t p;
    do {
        p = num_gen(bits);
    } while(!MillerRabinFct(p));
    return p;
}

// bool primality(uint32_t number) {
//     if (number == 0 || number == 1) {
//         return false;
//     }
//     else {
//         for (int i = 2, sqrtNum = sqrt(number); i < sqrtNum; ++i) {
//             if (number % i == 0) {
//                 return false;
//             }
//         }
//         return true;
//     }
// }

uint32_t multMod(uint32_t a, uint32_t b, uint32_t m) {
    uint32_t result = 0;
    uint32_t dblVal = a%m;
    uint32_t newB = b;

    while (newB > 0) {
        if (newB & 1) {
            result = (result + dblVal) % m;
        }
        dblVal = (dblVal << 1) % m;
        newB = (newB >> 1);
    }

    return result;
}


uint32_t powMod(uint32_t a, uint32_t b, uint32_t m) {
    uint32_t result = 1 % m;
    uint32_t sqrVal = a % m; 
    uint32_t newB = b;

    while (newB > 0) {
        if (newB & 1) {  
            result = multMod(result, sqrVal, m);
        }
        sqrVal = multMod(sqrVal, sqrVal, m);
        newB = (newB >> 1);
    }

    return result;
}

uint32_t encrypt(char c, uint32_t e, uint32_t m) {
    return powMod(c, e, m);
}

char decrypt(uint32_t x, uint32_t d, uint32_t n) {
    return (char) powMod(x, d, n);
}

void setup() {
  Serial.flush();
  Serial.begin(19200);
  p_bits=2;
  q_bits=2;
  e_bits=2;
  counter = 0;
}

std::vector<char> captureData() {
  std::vector<char> data;
  int chk = DHT.read11(DHT11_PIN);
  data.push_back('T');
  data.push_back('e');
  data.push_back('m');
  data.push_back('p');
  data.push_back('e');
  data.push_back('r');
  data.push_back('a');
  data.push_back('t');
  data.push_back('u');
  data.push_back('r');
  data.push_back('e');
  data.push_back('=');
  uint16_t temperature = DHT.temperature;
  if (temperature < 10) {
    data.push_back('0');
  }
  data.push_back((temperature / 10) + '0');
  data.push_back((temperature % 10) + '0');
  data.push_back('C');
  data.push_back(' ');
  data.push_back('H');
  data.push_back('u');
  data.push_back('m');
  data.push_back('i');
  data.push_back('d');
  data.push_back('i');
  data.push_back('t');
  data.push_back('y');
  data.push_back('=');
  uint16_t humidity = DHT.humidity;
  if (humidity < 10) {
    data.push_back('0');
  }
  data.push_back((humidity / 10) + '0');
  data.push_back((humidity % 10) + '0');
  data.push_back('%');
  data.push_back('\n');
  return data;
}

uint32_t vectorToNumber(const std::vector<char>& data) {
  uint32_t number = 0;
  for (char c : data) {
    number = (number << 8) | c;
  }
  return number;
}

std::vector<char> numberToVector(uint32_t number) {
  std::vector<char> data;
  while (number > 0) {
    char c = number & 0xFF;
    data.push_back(c);
    number = number >> 8;
  }
  std::reverse(data.begin(), data.end());
  return data;
}

void findSmallest(int& a, int& b) {
  int smallest = a;
  if (b < smallest) {
    smallest = b;
  }
  if (smallest == a) {
    a += 1;
  } else if (smallest == b) {
    b += 1;
  } 
}

void loop() {
  if(counter == 5) {
    findSmallest(p_bits, q_bits);
    counter = 0;
    e_bits=p_bits;
  }
  else
    counter++;
  
  p = prime_gen(p_bits);
  q = prime_gen(q_bits);
  setKeys(p,q,e_bits);
  std::vector<char> original_data = captureData();
  uint32_t number = vectorToNumber(original_data);
  uint32_t encrypted = encrypt(number, publicKey, moduluss);
  uint32_t decrypted = decrypt(encrypted, privateKey, moduluss);
  std::vector<char> decrypted_data = numberToVector(decrypted);
  
  Serial.print(p);
  Serial.print(',');
  Serial.print(q);
  Serial.print(',');
  Serial.print(moduluss);
  Serial.print(",");
  Serial.print(publicKey);
  Serial.print(",");
  Serial.println(privateKey);
}
