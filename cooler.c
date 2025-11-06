// --- Bibliotecas ---
#include <LiquidCrystal.h>

// --- Mapeamento de Pinos (Hardware) ---

// 1. Display LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// 2. Sensor de Temperatura
const int pinoSensor = A0;

// 3. Atuadores
const int pinoCooler = 9;
const int pinoBuzzer = 8;

// 4. Entradas (Inputs)
const int pinoBotaoModo = 7;     // Botão 1: Alterna Auto / Manual
const int pinoBotaoPreset = 6; // Botão 2: Alterna os presets manuais

// --- Variáveis Globais de Controle ---
float temperaturaC = 0.0;
bool modoManual = false;
bool estadoBotaoAnterior = LOW;
bool estadoBotaoPresetAnterior = LOW;
int presetManual = 0; // 0 = Normal, 1 = Frio, 2 = Quente

// --- Configurações do Projeto ---
const float LIMITE_ALERTA = 40.0;
const float TEMP_MIN_AUTO = 20.0;
const float TEMP_MAX_AUTO = 50.0;

// Definição dos presets manuais (0-255)
const int VELOCIDADE_FRIA = 85;   // Baixa rotação
const int VELOCIDADE_NORMAL = 170; // Rotação normal
const int VELOCIDADE_QUENTE = 255; // Alta rotação


void setup() {
  lcd.begin(16, 2);
  lcd.print("Iniciando Sistema...");
  
  pinMode(pinoCooler, OUTPUT);
  pinMode(pinoBuzzer, OUTPUT);
  
  pinMode(pinoBotaoModo, INPUT);
  pinMode(pinoBotaoPreset, INPUT); // Configura o novo pino do botão
  
  delay(1500);
}

void loop() {
  // 1. LEITURA: Verificar o Botão de Modo (Auto/Manual)
  verificarBotaoModo();

  // 2. LEITURA: Ler o Sensor de Temperatura
  lerTemperatura();

  // 3. LÓGICA: Decidir e Atuar
  if (modoManual) {
    // Se está em modo manual, verifica o botão de preset
    verificarBotaoPreset();
    controlarModoManual();
  } else {
    // Se está em modo auto, reseta o preset para o padrão
    presetManual = 0; 
    controlarModoAutomatico();
  }

  // 4. LÓGICA: Verificar Alerta Sonoro
  verificarAlerta();

  // 5. SAÍDA: Atualizar o Display LCD
  atualizarDisplay();
  
  delay(500); 
}

// --- Funções Auxiliares ---

void verificarBotaoModo() {
  bool estadoBotaoAtual = digitalRead(pinoBotaoModo);
  if (estadoBotaoAtual == HIGH && estadoBotaoAnterior == LOW) {
    modoManual = !modoManual;
    delay(50); 
  }
  estadoBotaoAnterior = estadoBotaoAtual;
}

void verificarBotaoPreset() {
  // Esta função só é chamada se estivermos no modo manual
  bool estadoBotaoAtual = digitalRead(pinoBotaoPreset);
  if (estadoBotaoAtual == HIGH && estadoBotaoPresetAnterior == LOW) {
    // Avança para o próximo preset
    presetManual++;
    if (presetManual > 2) {
      presetManual = 0; // Volta para o início (0->1->2->0)
    }
    delay(50); // Debounce
  }
  estadoBotaoPresetAnterior = estadoBotaoAtual;
}

void lerTemperatura() {
  int leituraSensor = analogRead(pinoSensor);
  float tensao = leituraSensor * (5000.0 / 1024.0);
  temperaturaC = (tensao - 500.0) / 10.0;
}

void controlarModoManual() {
  // Controla o cooler com base no preset manual selecionado
  switch (presetManual) {
    case 0: // Normal
      analogWrite(pinoCooler, VELOCIDADE_NORMAL);
      break;
    case 1: // Frio
      analogWrite(pinoCooler, VELOCIDADE_FRIA);
      break;
    case 2: // Quente
      analogWrite(pinoCooler, VELOCIDADE_QUENTE);
      break;
  }
}

void controlarModoAutomatico() {
  int velocidadePWM = map(temperaturaC, TEMP_MIN_AUTO, TEMP_MAX_AUTO, 0, 255);
  velocidadePWM = constrain(velocidadePWM, 0, 255);
  analogWrite(pinoCooler, velocidadePWM);
}

void verificarAlerta() {
  if (temperaturaC > LIMITE_ALERTA) {
    tone(pinoBuzzer, 1000);
  } else {
    noTone(pinoBuzzer);
  }
}

void atualizarDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0); // Linha 1
  lcd.print("Temp: ");
  lcd.print(temperaturaC);
  lcd.print(" C");
  
  lcd.setCursor(0, 1); // Linha 2
  if (modoManual) {
    lcd.print("Manual: ");
    switch (presetManual) {
      case 0:
        lcd.print("Normal");
        break;
      case 1:
        lcd.print("Frio");
        break;
      case 2:
        lcd.print("Quente");
        break;
    }
  } else {
    lcd.print("Modo: Auto");
  }
}