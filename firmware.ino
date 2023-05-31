/**
  @file Firmware.ino
  @author Saulo Daniel Ferreira Pontes
  @version 1.3 07/2015

  Arquivo principal do firmware do arduino
*/

#include <Arduino.h>
#include <string.h>

#include "EEPROM.h"
#include "TaskScheduler.h"

#include "config.h"
#include "scommp.h"
#include "device.h"

//Gerador de dados aleatorios
#include "random_driver.h"

//Dispositivos da cabine
#include "DHT22_driver.h"
#include <DS1307RTC.h>
//#include "SIM28_driver.h"

//! Tempo em que o ultimo caractere for recebido
long lastcharTime = 0;

//! Modulo de comunicacao SCOMMP
SCOMMP scommp;

//dispositivos
random_driver random_1;

DHT22_Sensor DHT22_1;
DS1307RTC_Sensor DS1307RTC_1;
//SIM28_Sensor SIM28_1;

//! temporizadores =============
Scheduler runner;

//task de requisição de auto registro
Task t0(10000, -1, &registrationRequest);

//task de upadate do random_1
//Task tr(10000, -1, &trCallback);
//
//void trCallback() {
//  random_1.update();
//}

//task de upadate do DHT22_1
Task t1(50000, -1, &t1Callback);

void t1Callback() {
  DHT22_1.update();
}

//task de upadate do DS1307RTC
Task t2(10000, -1, &t2Callback);

void t2Callback() {
  DS1307RTC.update();
}

//Task t7(10000, -1, &t7Callback);
//
//void t7Callback() {
//  SIM28_1.update();
//}


void registrationRequest()
{
  //! Assim que o microcontrolaodor e reiniciado tenta validar 
  //! o registro da estacao  atravez do Station_ID armazenado na EPROM

  //ser nao ouver dados na serial e a estacao nao confirmou o cadastro
  if( scommp.registred==false)
  {
   scommp.registrationRequest();
  }
  else
 {
   //desabilita a requisição de cadastro
   t0.disable();
   
   //abilita a atualização de dados dos dispositivos
   //tr.enable();

   t1.enable();
   t2.enable();
//   t7.enable();

 }
}

//==============================

void setup() {

  //! inicializando a Serial
  Serial.begin(9600);

  //! inicializando o SCOMMP
  scommp.begin (serialWrite);

  //! Adiciona sensores ao módulo de comunicação

  //Driver de Teste random_1
  //scommp.addSensor(&random_1);

  //Sensor DHT22_1
  DHT22_1.pin(4);
  scommp.addSensor (&DHT22_1);

  //Sensor DS1307RTC
  DS1307RTC_1.pin(A0);
  scommp.addSensor (&DS1307RTC_1);


//  //Sensor SIM28_1
//  SIM28_1.pin(2, 3);
//  scommp.addSensor (&SIM28_1);

  //! Adiciona atuadores ao módulo de comunicação
  //scommp.addActuator (new led_driver());

  //===============================
  // configuração do tempo de update

    //iniciando a task de requisição de auto registro
    //! A cada 10 segundos tenta confirmar o cadastro da estacao
    t0.enable();
    runner.addTask(t0);

    //iniciando a task de upadate do random_1
    //runner.addTask(tr);

    //iniciando a task de upadate do DHT22_1
    runner.addTask(t1);
  
    //iniciando a task de upadate do LDR_GL5528_1
    runner.addTask(t2);

    //iniciando a task de upadate do TP_401A_1
    runner.addTask(t3);

    //iniciando a task de upadate do GUVA_S12D_1
    runner.addTask(t4);

    //iniciando a task de upadate do Bucket_Rain_Gauge_1
    runner.addTask(t5);

    //iniciando a task de upadate do BMP085_1
    runner.addTask(t6);

//    //iniciando a task de upadate do SIM28_1
//    runner.addTask(t7);
}

void loop()
{ 

// se recebeu o comando par verificar a ativação
if(scommp.active == true)
{
  scommp.active = false;
  t0.enable();
}
  
//atualiza os eventos das tasks 
runner.execute();

//SIM28_1.update_serial();

}

//! Coleta os dados recebidos pela serial e envia para o tratamenteo do scommp
void serialEvent () 
{
  if (Serial.available()) {
     char c = (char)Serial.read();
     scommp.charReceived (c);
     
     lastcharTime = millis();
    }

  if (millis()-lastcharTime >= 1000 || millis()-lastcharTime < 0)
    {
      if (scommp.started())
      {
        scommp.reset();
      }
    }
}

//! Coleta a saída do módulo de comunicação e ecoa para a serial
void serialWrite (char c)
{
  //! Escreve o caractere na serial
  Serial.write (c);
}

