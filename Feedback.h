/* Arduino Non-Blocking Feedback Library
 *
 * v0.1 - Electric Echidna, Jul 2013
 *
 * www.electric-echidna.com
 *
 *
 * About
 * =====
 * This is a non-blocking user feedback library for Arduino based systems.
 * The library avoids the use of the 'delay' function typically used for
 * user feedback and allows code to continue running while feedback is
 * provided.
 *
 * Usage
 * =====
 * Add fb.Handle() to the main loop then call the
 * feedback functions like Beep,Flash and BeepAndFlash
 *
 * License
 * =======
 * http://creativecommons.org/licenses/by-nc-sa/3.0/
 *
 */


#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <Arduino.h>
#include <ChainableLED.h>

#define DEFAULT_DELAY 200
#define DEFAULT_BUZZER 3
#define DEFAULT_SDA A4
#define DEFAULT_SCL A5

typedef struct {
    int red;
    int green;
    int blue;
} Colour;

Colour blue = {0,0,255};
Colour red = {255,0,0};
Colour green = {0,255,0};
Colour off = {0,0,0};



class Feedback {
 
    int _buzzer;
    int _beeps;
    int _flashes;
    bool _isBeeping;
    bool _isFlashing;
    unsigned long _previous;
    unsigned long _elapsed;
    unsigned long _delay;
    ChainableLED *_led;

    Colour _flashOn;
    Colour _flashOff;

    public:

        Feedback(int buzzer) {
            _buzzer=buzzer;
            _previous = millis();
            _elapsed = 0;
            _beeps = 0;
            _flashes = 0;
            _isBeeping=false;
            _isFlashing=false;
            _delay = DEFAULT_DELAY;
            _led = new ChainableLED(DEFAULT_SCL,DEFAULT_SDA,1);
            pinMode(_buzzer,OUTPUT);
            digitalWrite(_buzzer,LOW);
            ledColour(off);
        }

        void SetBuzzerPin(int buzzer) {
            _buzzer = buzzer;
        }

        void SetLedPins(int SDA,int SCL) {

            delete _led;
            _led = new ChainableLED(SCL,SDA,1);
            ledColour(off);

        }

        void Handle() {

            unsigned long now = millis();
            _elapsed+=now-_previous;
            _previous=now;


            if (_elapsed>=_delay) {
                _elapsed=0;
                if (_isBeeping) {
                    digitalWrite(_buzzer,LOW);
                    _isBeeping = false;
                } else if (_beeps) {
                    _isBeeping = true;
                    digitalWrite(_buzzer,HIGH);
                    _beeps--;
                }
                if (_isFlashing) {
                    ledColour(_flashOff);
                    _isFlashing = false;
                } else if (_flashes) {
                    ledColour(_flashOn);
                    _isFlashing = true;
                    _flashes--;
                }
            }
        }

        void ledColour(Colour c) {

            _led->setColorRGB(0,c.red,c.green,c.blue);
        }

        void Beep(int beeps) {
            _beeps = beeps;
        }

        void Flash(int flashes,Colour on,Colour off) {
            _flashes=flashes;
            _flashOn = on;
            _flashOff = off;
        }

        void SetDelay(int delay) {

            _delay=delay;

        }

        void BeepAndFlash(int num,Colour on,Colour off) {
            Beep(num);
            Flash(num,on,off);
        }

        void BeepAndFlash(int num,Colour on,Colour off, int delay) {

            SetDelay(delay);
            BeepAndFlash(num,on,off);
        }

};

Feedback fb = Feedback(DEFAULT_BUZZER);

#endif
