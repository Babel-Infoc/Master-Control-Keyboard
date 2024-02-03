//#include <SimpleRotary.h>

#include <BleKeyboard.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h> 
#include "pin_definitions.h"
#include "Matrix.h"

// Setup keyboard
BleKeyboard Kbd("Master Control Keyboard", "Babel Infocalypse", 100);

// https://github.com/OfflineRocket/Sanctuary-Keyboard-Firmware
// https://docs.arduino.cc/learn/built-in-libraries/eeprom/

// Rotary encoder setup
// SimpleRotary rotary(Rotary1,Rotary2,40);
// byte RDir, PrevRDir;                        //Keep track of Rotary Encoder Direction

//  Code to "store" devices, so that the keyboard can switch connections on the fly ////////////////////////////////////////////////////////////////////////
// Adapted from : https://github.com/Cemu0/ESP32_USBHOST_TO_BLE_KEYBOARD/blob/main/src/USBHIDBootBLEKbd.cpp
// Primarily stores the selected MAC address in EEPROM storage
const int maxdevice = 3;
uint8_t MACAddress[maxdevice][6] =
    {
        {0x35, 0xAF, 0xA4, 0x07, 0x0B, 0x66},
        {0x31, 0xAE, 0xAA, 0x47, 0x0D, 0x61},
        {0x31, 0xAE, 0xAC, 0x42, 0x0A, 0x31}

}; 

// Basically just change the selected ID and reset - MAC address can only be changed before the keyboard start, so write to store selection, until changed again
// Take in device number, and set the EEPROM to the selected - selects what address to shift to, instead of iterating to that address
/*void changeID(int DevNum)
{
  // Serial.println("changing MAC...");

  // Make sure the selection is valid
  if (DevNum < maxdevice)
  {
    // Write and commit to storage, reset ESP 32
    EEPROM.write(0, DevNum);
    EEPROM.commit();
    // esp_reset();
    esp_sleep_enable_timer_wakeup(1);
    esp_deep_sleep_start();
  }
}*/

// Top unit LED's, #LED2
extern Adafruit_NeoPixel strip2(LED2_NUM, LED2_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  // Initialize Column output pin and set default state
  for (int i = 0; i < NumCols; i++)
  {
    pinMode(Cols[i], OUTPUT);
    digitalWrite(Cols[i], LOW);
  }

  // Initialize Row pins as input, with Pulldown
  for (int i = 0; i < NumRows; i++)
  {
    pinMode(Rows[i], INPUT_PULLDOWN);
  }

  // Setup the MAC address
  EEPROM.begin(4);                                    // Begin EEPROM, allow us to store
  int deviceChose = EEPROM.read(0);                   // Read selected address from storage
  esp_base_mac_addr_set(&MACAddress[deviceChose][0]); // Set MAC address based on that stored value
  //esp_iface_mac_addr_set(&MACAddress[deviceChose][0], ESP_MAC_BASE);
  //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/misc_system_api.html?highlight=esp_base_mac_addr_set#_CPPv422esp_iface_mac_addr_setPK7uint8_t14esp_mac_type_t
  //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/misc_system_api.html?highlight=esp_base_mac_addr_set#_CPPv414esp_mac_type_t

  Kbd.begin();              // Initialise Bluetooth Keyboard

  strip2.begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
  strip2.show();             // Turn OFF all pixels ASAP
  strip2.setBrightness(50);  // Set BRIGHTNESS to about 1/5 (max = 255)

}

int RowCnt = 0; // Keep track of scanned row - needs to be outside of the void loop so it isn't reset

// Loops to iterate through all functions
void loop()
{
  // Check if the keyboard is connected, if so, scan the matrix
  if (Kbd.isConnected())
  {
    // Initialize new Row to scan
    digitalWrite(Rows[RowCnt], HIGH);

    // Check columns
    int ColCnt = 0;
    // Repeat until all columns are scanned
    while (ColCnt <= (NumCols - 1))
    {
      // Check state of current position, and make sure it was previously off
      bool isPressed = (digitalRead(Cols[ColCnt]) == HIGH && KeyIsPressed[RowCnt][ColCnt]);
      if (isPressed)
      {
        // Switch based on the key pressed, allows for unique functions other than alphanumerics
        switch (KeyMatrix[RowCnt][ColCnt])
        {
        // Change the ID of the bluetooth, so you can connect to another device
/*
        case 0:
        case 1:
        case 2:
          //changeID(KeyMatrix[RowCnt][ColCnt]);
          break;
        // Rotary encoder button, play pause not an int
        case 3:
          Kbd.press(KEY_MEDIA_PLAY_PAUSE);
          break;
        // All other buttons are pressed through the Layer array
        case 5:
          ESP.restart();
          break;
        case NULL_CON:
          break;
        case NEXT:
          Kbd.press(KEY_MEDIA_NEXT_TRACK);
          Kbd.releaseAll();
          break;
        case PREV:
          Kbd.press(KEY_MEDIA_PREVIOUS_TRACK);
          break;
*/ // the switch looks a bit superfluous now
        default:
          Kbd.press(KeyMatrix[RowCnt][ColCnt]);
        }
        // Assign the current key as ON, so it doesn't constantly press the button
        KeyIsPressed[RowCnt][ColCnt] = true;
        // Serial.print( KeyMatrix[a][ColCnt] );
      }
      // Otherwise, check if the switch was released
      else if (digitalRead(Cols[ColCnt]) == LOW && KeyIsPressed[RowCnt][ColCnt])
      {
        // Switch based on the switch released
        switch (KeyMatrix[RowCnt][ColCnt])
        {
        // Nothing for the tactile switch
/*
        case 1:
        case 2:
        case 0:
          break;
        // Release the rotary encoder button
        case 3:
          Kbd.release(KEY_MEDIA_PLAY_PAUSE);
          break;
        case FUNCTION_SW:
          KeyIsPressed[RowCnt][ColCnt] = false;
          
          Kbd.release(KEY_MEDIA_PLAY_PAUSE);
          Kbd.release(KEY_MEDIA_PREVIOUS_TRACK);
          Kbd.release(KEY_MEDIA_NEXT_TRACK);
          Kbd.releaseAll();
          break;
        case NULL_CON:
          KeyIsPressed[RowCnt][ColCnt] = false;
          Kbd.release(KEY_MEDIA_PLAY_PAUSE);
          Kbd.release(KEY_MEDIA_PREVIOUS_TRACK);
          Kbd.release(KEY_MEDIA_NEXT_TRACK);
          Kbd.releaseAll();
          break;
        case NEXT:
          Kbd.release(KEY_MEDIA_NEXT_TRACK);
          break;
        case PREV:
          Kbd.release(KEY_MEDIA_PREVIOUS_TRACK);
          break;
        // Release all other keys on the keyboard
*/ // the switch looks a bit superfluous now
        default:
          Kbd.release(KeyMatrix[RowCnt][ColCnt]);
        }
        // Let the keyboard know it's off, and to not constantly release keys that aren't released
        KeyIsPressed[RowCnt][ColCnt] = false;
      }
      // Increase column to scan
      ColCnt++;
    }

    // Reset back to original row to scan
    digitalWrite(Rows[RowCnt], LOW);
    // Increase row outputted
    RowCnt++;
    // Loop back to original row if out of the number of rows
    if (RowCnt >= (NumRows))
    {
      RowCnt = 0;
    }
    /*
    // Check Rotary Encoder once a cycle
    RDir = rotary.rotate();
    switch (RDir)
    {
    case 1:
      if (PrevRDir != RDir)
      {
        Kbd.release(KEY_MEDIA_VOLUME_UP);
      }
      Kbd.press(KEY_MEDIA_VOLUME_DOWN);
      PrevRDir = 1;
      break;
    case 2:
      if (PrevRDir != RDir)
      {
        Kbd.release(KEY_MEDIA_VOLUME_DOWN);
      }
      Kbd.press(KEY_MEDIA_VOLUME_UP);
      PrevRDir = 2;
      break;
    default:
      switch (PrevRDir)
      {
      case 1:
        Kbd.release(KEY_MEDIA_VOLUME_DOWN);
        break;
      case 2:
        Kbd.release(KEY_MEDIA_VOLUME_UP);
        break;
      }
      PrevRDir = 0;
      break;
    }
    */
    rainbow(10);      // Animate LED;s
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
extern void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip2.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip2.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}


// send_string_as_keyreps(&Kbd, "a");
/*
void send_string_as_keyreps(BleKeyboard *keyb, const char *str)
{
  uint8_t buf[keyboard_report_len]{0};
  size_t key_idx = 0;
  keyreport_t keyrep = {0};
  char c;
  uint8_t scan_code;
  uint8_t mod;
  bool duplicate;

  // fetch a character and advance the string
  c = *(str++);
  // check if we need to use the shift key
  mod = KC_SHIFT(c) ? KM_L_SHIFT : 0;
  // pick the scan code
  scan_code = KC_CHAR(c);

  while (scan_code != '\0')
  {
    keyrep.modifiers = mod;
    keyrep.keys[key_idx++] = scan_code;

    // fetch a character and advance the string
    c = *(str++);
    // check if we need to use the shift key
    mod = KC_SHIFT(c) ? KM_L_SHIFT : 0;
    // pick the scan code
    scan_code = KC_CHAR(c);

    // check for dupliate keystrokes, they need to be sent as new reports
    duplicate = false;
    for (size_t past_idx = 0; past_idx < key_idx; past_idx++)
    {
      if (scan_code == keyrep.keys[past_idx])
      {
        duplicate = true;
      }
    }
    // check if we need to split the string into a new report
    if ((keyrep.modifiers != mod) || // shift needs to be pressed or released,
        (key_idx == 6) ||            // the key report is full
        (scan_code == '\0') ||       // we've reached the end of the string
        (duplicate == true)          // we need to send a repeated keystroke
    )
    {
      // using methods from:
      // https://github.com/T-vK/ESP32-BLE-Keyboard/blob/master/BleKeyboard.h
      keyb->send_keyboard_report(
          pack_keyboard_report(
              keyrep.modifiers,
              keyrep.keys,
              buf));

      // if a key is pressed in two subsequent reports,
      // it is considered held for both reports.
      // release all keys to allow repeated keypresses
      key_idx = 0;
      memset(keyrep.keys, 0, 6);

      keyb->send_keyboard_report(
          pack_keyboard_report(
              0, // keyrep.modifiers, // send a fully open key map so we can safely stop here
              keyrep.keys,
              buf));
    }
  }
}
*/
