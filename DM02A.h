/*
  DM02A.h - Library for use with DM02A - MSS ELETRONICA
  MIT License
*/
 
#ifndef DM02A_h
#define DM02A_h
 
#include <Arduino.h>

class DM02A{
    
	public:
        DM02A(uint8_t pinSIG, uint8_t pinCH, uint8_t pinEN);
		DM02A(uint8_t pinSIG, uint8_t pinCH);
        void EnviaNivel(uint8_t _nivel, uint8_t _canal);
        uint8_t feedback(uint8_t _canal);
 
    private:
        uint8_t _pinSIG;
		uint8_t _pinCH;
		uint8_t _pinEN;
		uint8_t _EN;
		uint32_t _pulso;
		uint8_t _cont;
		
};

#endif