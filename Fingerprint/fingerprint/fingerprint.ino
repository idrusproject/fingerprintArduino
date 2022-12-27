/*************************************************** 
 *  Jangan lupa subscribe channel youtube saya : youtube.com/mohamadidrus
 *  Untuk mendapatkan project arduino yang lainnya 
 *  Hormat saya Mohamad Idrus
 *  
 *  Konfigurasi pengkabelan Fingerprint module ke Arduino 
 *   Kabel Merah 3.3v 
 *   Kabel Hitam GND
 *   Kabel Hijau(TX) D2
 *   Kabel Putih(RX) D3
 ****************************************************/


#include <Adafruit_Fingerprint.h>
#define lock 12

SoftwareSerial mySerial(2, 3); //RX = D2, TX = D3

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  pinMode (lock,OUTPUT);
  digitalWrite(lock,HIGH);
  Serial.begin(9600);
  while (!Serial);  
  delay(100);
  Serial.println("Tunggu Sebentar ");

  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Fingerprint siap diginakan");
  } else {
    Serial.println("Fingerprint eror :( ");
    Serial.println("Coba cek pengkabelan ");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Terdapat "); Serial.print(finger.templateCount); Serial.println(" sidik jari yang terdaftar");
  Serial.println("Silahkan tempelkan jarimu");
}

void loop()                     
{
  getFingerprintIDez();
  delay(50);            
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 
  return finger.fingerID;
}

int getFingerprintIDez() {
  
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Terdeteksi jari no : "); Serial.print(finger.fingerID); 
  Serial.print(" Dengan keakuratan : "); Serial.print(finger.confidence-8); Serial.println("%"); 
  if (finger.confidence>90){
    Serial.println("Pintu terbuka");
    digitalWrite(lock,LOW);
    delay(2000);
    digitalWrite(lock,HIGH);
  }
  return finger.fingerID; 
}
