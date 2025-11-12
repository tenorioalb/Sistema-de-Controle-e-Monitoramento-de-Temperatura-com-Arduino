// --- Bibliotecas ---
#include <LiquidCrystal.h>
#include <math.h>

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

// --- Configurações do NTC ---
// (Altere estes valores para o seu sensor específico!)
const float R_FIXO = 1000.0;     // Valor do resistor fixo (ex: 10k Ohms)
const float R_NOMINAL = 10000.0;   // Resistência nominal do NTC (ex: 10k Ohms)
const float TEMP_NOMINAL = 25.0;   // Temperatura nominal (geralmente 25 C)
const float B_VALOR = 3950.0;      // Coeficiente Beta (B-value) do NTC

void setup() {
  lcd.begin(16, 2);
  lcd.print("Iniciando Sistema...");
  
  pinMode(pinoCooler, OUTPUT);
  pinMode(pinoBuzzer, OUTPUT);
  
  pinMode(pinoBotaoModo, INPUT);
  pinMode(pinoBotaoPreset, INPUT); // Configura o novo pino do botão
  
  delay(1500);
  Serial.begin(9600);
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
  // 1. Faz a leitura do valor analógico (0-1023)
  int leituraSensor = analogRead(pinoSensor);

  // 2. Converte a leitura para a resistência do NTC (Usando a Fórmula B)
  // Esta fórmula assume o circuito: 5V -> NTC -> (pinoSensor) -> R_Fixo -> GND
  float R_NTC = R_FIXO * ( (1024.0 - (float)leituraSensor) / (float)leituraSensor );
  
  // 3. Calcula a temperatura usando a equação do Parâmetro Beta
  float steinhart;
  steinhart = R_NTC / R_NOMINAL;                     
  steinhart = log(steinhart);                        
  steinhart /= B_VALOR;                              
  steinhart += 1.0 / (TEMP_NOMINAL + 273.15);      
  steinhart = 1.0 / steinhart;                     
  
  // 4. Converte de Kelvin para Celsius e atualiza a variável global
  temperaturaC = steinhart - 273.15;               
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