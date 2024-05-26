#include <ArduinoSTL.h>

extern uint32_t publicKey;
extern uint32_t privateKey;
extern uint32_t moduluss;

uint32_t num_gen(int bits);
void setKeys(uint32_t p, uint32_t q, int bits);