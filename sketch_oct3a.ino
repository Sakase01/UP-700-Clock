// ========================================
// DISPLAY SHARP UP-700
// Reloj + mensajes - 4 dígitos
// ========================================
#include "RTC.h"
// ----------------------------------------
// CONFIGURACIÓN DE PINES
// ----------------------------------------

const int digitPins[4] = {0, 1, 2, 3};  // Dígitos, de izquierda a derecha

const int segmentPins[8] = {4, 5, 6, 7, 8, 9, 10, 11}; // Segmentos: A B C D E F G DP

const int tiempoDigito = 2; // Tiempo que permanece activo cada dígito (ms)

// ========================================
// PATRONES DE NÚMEROS
// ========================================

// Orden: A B C D E F G
// 1 = segmento encendido
// 0 = segmento apagado
const bool numeros[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};


// ========================================
// PATRONES PERSONALIZADOS
// ========================================

// Orden: A B C D E F G
const bool abecedario[26][7] = {

  {1, 1, 1, 0, 1, 1, 1}, // A 0
  {0, 0, 1, 1, 1, 1, 1}, // B 1
  {1, 0, 0, 1, 1, 1, 0}, // C 2
  {0, 1, 1, 1, 1, 0, 1}, // D 3
  {1, 0, 0, 1, 1, 1, 1}, // E 4
  {1, 0, 0, 0, 1, 1, 1}, // F 5
  {1, 0, 1, 1, 1, 1, 0}, // G 6
  {0, 1, 1, 0, 1, 1, 1}, // H 7
  {0, 0, 0, 0, 1, 1, 0}, // I 8
  {0, 1, 1, 1, 1, 0, 0}, // J 9
  {1, 0, 1, 0, 1, 1, 1}, // K 10
  {0, 0, 0, 1, 1, 1, 0}, // L 11
  {1, 1, 1, 0, 1, 1, 0}, // M 12
  {0, 0, 1, 0, 1, 0, 1}, // N 13
  {1, 1, 1, 1, 1, 1, 0}, // O 14
  {1, 1, 0, 0, 1, 1, 1}, // P 15
  {1, 1, 1, 1, 0, 1, 1}, // Q 16
  {0, 0, 0, 0, 1, 0, 1}, // R 17
  {1, 0, 1, 1, 0, 1, 1}, // S 18
  {0, 0, 0, 1, 1, 1, 1}, // T 19
  {0, 1, 1, 1, 1, 1, 0}, // U 20
  {0, 1, 1, 1, 1, 1, 0}, // V 21
  {0, 1, 1, 1, 1, 1, 0}, // W 22
  {0, 1, 1, 0, 1, 1, 1}, // X 23
  {0, 1, 1, 1, 0, 1, 1}, // Y 24
  {1, 1, 0, 1, 1, 0, 1}  // Z 25

};


// ========================================
// SETUP
// ========================================
void setup() {
  Serial.begin(9600);
  RTC.begin(); //Inicai el reloj interno de la placa
 RTCTime startTime(
    4,
    Month::OCTOBER,
    2026,
    12,
    49,
    0,
    DayOfWeek::SUNDAY,
    SaveLight::SAVING_TIME_ACTIVE
  );
  RTC.setTime(startTime);

  // Configurar dígitos
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], LOW);

  }

  // Configurar segmentos
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], HIGH);
  }
}

// ========================================
// APAGAR TODO
// ========================================
void apagarTodo() {
  // Apagar todos los dígitos
  for (int i = 0; i < 4; i++) {
    digitalWrite(digitPins[i], LOW);
  }
  // Apagar todos los segmentos
  for (int i = 0; i < 8; i++) {
    digitalWrite(segmentPins[i], HIGH);
  }
}
// ========================================
// MOSTRAR UN PATRÓN
// ========================================
void mostrarPatron(int posicion, const bool patron[7], bool punto = false) {
  apagarTodo(); // Primero apagamos todo para evitar segmentos fantasma

  // Activar los segmentos necesarios
  for (int i = 0; i < 7; i++) {
    if (patron[i]) {
      digitalWrite(segmentPins[i], LOW);
    }
  }

  // Activar el punto decimal si se solicita
  if (punto) {
    digitalWrite(segmentPins[7], LOW);
  }

  // Activar el dígito seleccionado
  digitalWrite(digitPins[posicion], HIGH);

  // Mantenerlo encendido durante un instante
  delay(tiempoDigito);
}
// ========================================
// MOSTRAR MENSAJE
// ========================================
void mostrarMensaje(const char mensaje[4]) {
  unsigned long inicio = millis();
  while (millis() - inicio < 4000) {
    mostrarPatron(0, abecedario[mensaje[0] - 'A']);
    mostrarPatron(1, abecedario[mensaje[1] - 'A']);
    mostrarPatron(2, abecedario[mensaje[2] - 'A']);
    mostrarPatron(3, abecedario[mensaje[3] - 'A']);
  }
}
// ========================================
// MOSTRAR HORA
// ========================================
void mostrarHoraRTC() {
  RTCTime horaActual;

  // Get current time from RTC
  RTC.getTime(horaActual);

  int hora = horaActual.getHour();
  int minutos = horaActual.getMinutes();

  unsigned long inicio = millis();

  while (millis() - inicio < 1000) {
    mostrarPatron(0, numeros[hora / 10]);// Primer dígito: decenas de la hora
    mostrarPatron(1, numeros[hora % 10], true);// Segundo dígito: unidades de la hora + punto
    mostrarPatron(2, numeros[minutos / 10]);// Tercer dígito: decenas de los minutos
    mostrarPatron(3, numeros[minutos % 10]);// Cuarto dígito: unidades de los minutos
  }
}

// ========================================
// LOOP
// ========================================

void loop() {
  mostrarHoraRTC();

}