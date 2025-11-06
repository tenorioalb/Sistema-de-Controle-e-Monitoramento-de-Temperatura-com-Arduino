# 🤖 Sistema de Controle e Monitoramento de Temperatura com Arduino

![Tinkercad](https://img.shields.io/badge/Simulado%20em-Tinkercad-blue?logo=tinkercad)

Este repositório contém os arquivos do "Projeto 2" da disciplina de Microcontroladores e Aplicações.

## 📝 Descrição

Este projeto consiste no desenvolvimento de um sistema de monitoramento e controle automático de temperatura utilizando um Arduino Uno como unidade central. O sistema é projetado para resolver o problema de superaquecimento em ambientes ou dispositivos eletrônicos, oferecendo uma solução de baixo custo, automatizada e eficiente.

O sistema mede continuamente a temperatura ambiente, exibe o status em um display LCD e ajusta a velocidade de um cooler (via PWM) de forma proporcional à temperatura.

### 🎓 Contexto Acadêmico

* **Instituição:** Universidade Federal de Alagoas (UFAL)
* **Instituto:** Instituto de Computação (IC)
* **Curso:** Engenharia de Computação
* **Disciplina:** Microcontroladores e Aplicações
* **Professor:** Prof. Erick de Andrade Barboza

## ✨ Funcionalidades

O sistema implementa os seguintes requisitos:

* Monitoramento da temperatura ambiente em tempo real.
* Exibição da temperatura e do modo de operação atual em um display LCD 16x2.
* **Modo Automático:** Ajuste automático da velocidade do cooler (via PWM) com base na temperatura medida.
* **Modo Manual:** Permite ao usuário alternar para o controle manual e selecionar presets de velocidade (Frio, Normal, Quente).
* Alerta sonoro (buzzer) para temperaturas elevadas que atingem um limite pré-definido.

## 🛠️ Componentes Utilizados

* 1x Arduino Uno R3
* 1x Display LCD 16x2
* 1x Sensor de Temperatura (TMP36)
* 1x Motor CC (para simular o cooler)
* 1x Transistor NPN (2N2222)
* 1x Diodo (1N4007)
* 1x Buzzer (Piezzo)
* 2x Botões (Pushbuttons)
* 1x Potenciômetro (10kΩ - para contraste do LCD)
* 1x Resistor (1kΩ - para a base do transistor)
* 2x Resistores (10kΩ - pull-down para os botões)
* 1x Protoboard (Placa de Ensaio)
* Jumpers (fios de conexão)

## 🚀 Como Simular

Este projeto pode ser simulado em duas plataformas: Tinkercad ou Wokwi.

### Simulação no Tinkercad

1.  Crie um novo "Circuito" no Tinkercad.
2.  Adicione os componentes da lista acima na área de trabalho.
3.  Monte o circuito elétrico conforme o diagrama de conexões (wiring).
4.  Clique na aba "Código", mude de "Blocos" para "Texto".
5.  Copie o conteúdo do arquivo `/tinkercad/sketch.ino` deste repositório e cole no editor de código do Tinkercad.
6.  Inicie a simulação.


## 🕹️ Instruções de Operação

Ao iniciar a simulação, o sistema entra em **Modo Automático** por padrão.

* **Modo Automático:** A velocidade do cooler é ajustada automaticamente com base na temperatura. (Abaixo de 20°C fica desligado, aumentando até 100% em 50°C).
* **Botão 1 (Pino D7):** Pressione para alternar entre "Modo Automático" e "Modo Manual".
* **Botão 2 (Pino D6):** (Disponível apenas no Modo Manual) Pressione para alternar entre os presets de velocidade:
    * `Manual: Normal` (Velocidade média)
    * `Manual: Frio` (Velocidade baixa)
    * `Manual: Quente` (Velocidade alta)
* **Alerta Sonoro:** O buzzer será ativado automaticamente se a temperatura ultrapassar 40°C, independentemente do modo.

## 📁 Estrutura do Repositório
