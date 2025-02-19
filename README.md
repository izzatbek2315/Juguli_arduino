# ESP8266 Wi-Fi Car Remote Control

Bu loyiha ESP8266 mikrocontrolleri asosida ishlaydigan Wi-Fi orqali boshqariladigan avtomobil loyihasidir. Qurilma Wi-Fi tarmog‘iga ulanib, mobil ilova:(ESP8266 wifi robot car) brauzer yoki bot orqali boshqariladi.

## Xususiyatlari
- Wi-Fi orqali boshqarish
- Oldinga, orqaga, chapga va o‘ngga harakatlanish
- Signal va faralarni yoqish/o‘chirish
- Avariyaviy chiroqlar (hazard lights) rejimi
- ESP8266 OTA (Over-the-Air) yangilanishi

## Qanday ishlaydi?

ESP8266 Wi-Fi-ga ulanganidan so‘ng, **"Car_IP:[IP]"** SSID sifatida 2 daqiqa davomida ko‘rinadi. 2 daqiqadan keyin avtomatik ravishda AP (Access Point) rejimi o‘chadi, ammo "Pult" SSID har doim qoladi.

### 1. Qurilmani yoqish
- ESP8266-ni yoqing va Wi-Fi-ga ulang.
- **SSID**: `Pult` **Parol**: `12341234`
- Yoki agar tarmoqga ulanganda SSID "Car_IP:[IP]" ko‘rinsa, uni tanlab, brauzerda IP-ni kiriting.

### 2. Mashinani boshqarish
Mashina quyidagi buyruqlar bilan harakat qiladi:

| Buyruq | Tavsif |
|--------|--------|
| F | Oldinga yurish |
| B | Orqaga yurish |
| R | O‘ngga burilish |
| L | Chapga burilish |
| G | Oldinga-chapga burilish |
| H | Orqaga-chapga burilish |
| I | Oldinga-o‘ngga burilish |
| J | Orqaga-o‘ngga burilish |
| S | To‘xtash |
| V | Signal chalish |
| W | Old chiroqni yoqish |
| w | Old chiroqni o‘chirish |
| 0 | Orqa chiroqni yoqish |
| 1 | Orqa chiroqni o‘chirish |
| 9 | Avariyaviy chiroqlarni yoqish |
| q | Avariyaviy chiroqlarni o‘chirish |
| 8 | Chap va o‘ng burilish chiroqlarini yoqish |

### 3. Buyruqlarni yuborish
Brauzerda **`http://[ESP_IP]/?State=[BUYRUQ]`** yozib yuboring.

Misol:
```
http://192.168.1.100/?State=F
```
Bu mashinani oldinga harakatlantiradi.

### 4. OTA (Over-the-Air) Yangilash
Agar kodni yangilash kerak bo‘lsa:
- Arduino IDE-da "Tools" bo‘limidan **ESP8266 OTA Update**-ni tanlang.
- Qurilmani Wi-Fi orqali yangilang.

## Loyiha uchun zarur kutubxonalar
- **ESP8266WiFi** - ESP8266 uchun Wi-Fi boshqaruvi
- **ESP8266WebServer** - Web server orqali boshqarish
- **WiFiManager** - Wi-Fi ulanishini avtomatik sozlash
- **ArduinoOTA** - OTA orqali kodni yangilash

## Foydalanish huquqlari
Bu loyiha ochiq kodli bo‘lib, istalgan foydalanuvchi undan o‘z loyihalarida foydalanishi mumkin.

