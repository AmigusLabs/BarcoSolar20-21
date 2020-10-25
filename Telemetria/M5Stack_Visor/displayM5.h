int pantalla1_inicio() {
  M5.Lcd.fillScreen(TFT_BLACK); // Black screen fill
  M5.Lcd.drawXBitmap(55, 0, logo, logo_W, logo_H, TFT_MAGENTA);
  M5.Lcd.drawXBitmap(6, 4, logo_labs, logo_labs_W, logo_labs_H, TFT_BLUE);
  M5.Lcd.drawXBitmap(4, 35, logo_amigus, logo_amigus_W, logo_amigus_H, TFT_ORANGE);
  M5.Lcd.fillRect(163, 183, 154, 54, BLACK);
  M5.Lcd.fillRect(163, 183, 154, 2, 0x7bef);
  M5.Lcd.fillRect(163, 183, 2, 54, 0x7bef);
  M5.Lcd.fillRect(165, 235, 152, 2, WHITE);
  M5.Lcd.fillRect(315, 185, 2, 52, WHITE);
  M5.Lcd.setTextFont(4);
  M5.Lcd.setCursor(10, 100);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.print("Valor:   ");
}


int pantalla2_inicio() {
  M5.Lcd.fillScreen(TFT_BLACK); // Black screen fill
  M5.Lcd.drawXBitmap(55, 0, logo, logo_W, logo_H, TFT_MAGENTA);
  M5.Lcd.drawXBitmap(6, 4, logo_labs, logo_labs_W, logo_labs_H, TFT_BLUE);
  M5.Lcd.drawXBitmap(4, 35, logo_amigus, logo_amigus_W, logo_amigus_H, TFT_ORANGE);
  M5.Lcd.fillRect(165, 185, 150, 50, BLACK);
}
