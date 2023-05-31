
#include "scommp.h"
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include "config.h"

#include <EEPROM.h>


//==================================
// funcoes que tratam do dado float e converte em int16

short ftofix16(float num) {

  short i, f;

  //  if (fabs(num) > 2047.999f) {
  //    printf("Error: number out of range (num=%f)\n", num);
  //  }

  i = (short)num;
  f = (short)(fabs(num * 16)) & 15;
  return (i << 4) | f;
}

float fix16tof(int n)
{
  float s = 1.0f;
  if (n < 0) {
    s = -1.0f;
    n = -n;
  }
  return s * ((float)(n >> 4) + ((n & 15) / 16.0f));
}



//==================================
void PrintHex8(uint8_t *data, uint8_t length) // prints 8-bit data in hex with leading zeroes
{
     char tmp[16];
       for (int i=0; i<length; i++) { 
         sprintf(tmp, "0x%.2X",data[i]); 
         Serial.print(tmp); Serial.print(" ");
       }
}

void PrintHex16(uint16_t *data, uint8_t length) // prints 16-bit data in hex with leading zeroes
{
       char tmp[16];
       for (int i=0; i<length; i++)
       { 
         sprintf(tmp, "0x%.4X",data[i]); 
         Serial.print(tmp); Serial.print(" ");
       }
}


//=============================================


//========================


//u_int32_t value = 323321;
//u_int8_t  result[4];

//result[0] = (value & 0x000000ff);
//result[1] = (value & 0x0000ff00) >> 8;
//result[2] = (value & 0x00ff0000) >> 16;
//result[3] = (value & 0xff000000) >> 24;

// //--------------------------

//u_int32_t v = 323321;
//u_int8_t  a[4];

//u_int8_t *vp = (u_int8_t *)&v;
//a[0] = vp[0];
//a[1] = vp[1];
//a[2] = vp[2];
//a[3] = vp[3];

////memcpy(a, &v, sizeof(v));

////*(UINT32 *)a = v;

//cout<<v<<"\n";

//printf("0x%.2X ", (int)a[0]);
//printf("0x%.2X ", (int)a[1]);
//printf("0x%.2X ", (int)a[2]);
//printf("0x%.2X ", (int)a[3]);

//u_int32_t v2 = *(u_int32_t *)a;
//cout << v2;

//cout<<"\n\n";

//=============================



SCOMMP::SCOMMP ()
{
  //inicializando o Firmware_ID
  uint32_t fid = FIRMWARE_ID;
  memcpy(FID, &fid, sizeof(fid));
  
  //! Coloca NULL em todos os ponteiros
  charProcessed = NULL;

  for (int i = 0; i < MAX_SENSORS; i++)
    sensors[i] = NULL;
  for (int i = 0; i < MAX_ACTUATORS; i++)
    actuators[i] = NULL;

  //! Zera todos os inteiros
  nsensors = 0;
  nactuators = 0;
  reqlen = 0;

  registred = false;

//  //trata se exitem caracters não imprimiveis na Station_ID e os subistituem
//  //a fim de evitar erros na tranmição via HTTP
//  for ( int index = 0 ; index < id_size ; index++ )
//  {
//    if (EEPROM[index] < 32 || EEPROM[index] > 126)
//    {
//      EEPROM[index] = '!';
//    }
//  }
}

bool SCOMMP::started () {
  return (bool)reqlen;
}

void SCOMMP::reset () {
  reqlen = 0;
  size = 0;
}

void SCOMMP::begin (void (*charProcessed) (char))
{
  this->charProcessed = charProcessed;

  //lendo a Station_ID armazenada na EPROM
  sid_read();

  //inicializando a software serial de debug
  _serial.begin(9600);//<<============
}

void SCOMMP::charReceived (char c)
{
  req[reqlen] = c;

  //_serial.print(c);
  //_serial.print((char)req[reqlen]);

  //! Verifica se o primeiro byte é o SBYTE, se não for,
  //! recomeça a contagem de bytes
  if (reqlen == 0 && req[0] != SBYTE)
  {
    reqlen = 0;
    return;
  }

  //confere a vercao do protocolo
  if (reqlen == 1 && req[1] != VBITE)
  {
    reqlen = 0;
    return;
  }

  //escreve o caractar coletado apos a validação
  //_serial.print((char)req[reqlen]);


  if (reqlen == 2)
  {

    switch (req[2])
    {
      case VALID:
        size = (4 + id_size*2);
        //_serial.print("_VALID_");
        break;

      case RQVAR:
        size = (4 + id_size * 3);
        //_serial.print("_RQVAR_");
        break;

      case SENSE:
        size = (5 + id_size * 2);
        //_serial.print("_SENSE_");
      break;

      case STATUS:
        size = (4 + id_size * 2);
        //_serial.print("_STATUS_");
      break;

      case ISACT:
        size = (4 + id_size*2);
        //_serial.print("_ISACT_");
      break;

      default:
        reqlen = 0;
        break;
    }

  }

  // _serial.print(c);
  // _serial.print((char)reqlen);
  //_serial.print((char)req[reqlen]);

  //! Verifica se a contagem já ultrapassou o tamanho de uma
  //! requisição ou se lemos o EBYTE
  if (reqlen == size - 1 && req[reqlen] == EBYTE) { //

    //! Chama requestReady se lemos o EBYTE
    requestReady();

    //! Reseta contagem
    reqlen = 0;
  }
  else
    if (reqlen > size-1 )
    {
        //! Reseta contagem
        size = 0;
        reqlen = 0;
    }
        else
        {
            reqlen++;
        }


  if (reqlen >= 255)
  {
    reqlen = 0;
  }
}

void SCOMMP::requestReady ()
{

  switch (req[2])
  {
    case VALID:
      //_serial.print("isValid_");
      isValid();
      break;

    case RQVAR:
       //_serial.print("update_id_");
       update_id();
      break;

    case SENSE:
      //_serial.print("sense_");
      sense();
      break;

    case STATUS:
      //_serial.print("status_");
      status();
    break;

    case ISACT:
      //_serial.print("_ISACT_");
       registred = false;
       active = true;
     
    break;

    default:
      //error();
      return;
      break;
  }

}


//======================

//retorna os dadso contidos na memoria da estação 
void SCOMMP::status()
{
  while(Serial.available());//<<===============================================
  
  // compara se a id da estaço recebida e equivalente a id que esta na memoria eprom
  for ( int index = 0; index < id_size; index++)
  {
    if ( SID[index] != (uint8_t)(req[index + 3]))
    {
      return;
    }
  }
  
 // _serial.print("_st_");

  //envia resposta contendo os dados do sensor
  Serial.write(SBYTE);
  Serial.write(VBITE);
  Serial.write(STATUS);

  for ( int index = 0; index < id_size; index++)
  {
    Serial.write(SID[index]);
  }

  for ( int index = 0; index < id_size; index++)
  {
    Serial.write(FID[index]);
  }

      // quantidade de informaçoes a serem eviadas
      int SDL = 0;

      for ( int ID = 0; ID < nsensors; ID++)
      {
       SDL += sensors[ID]->getDegree();
      }
      
      //inteiro 16b its
      uint8_t high = ((SDL >> 7) & 0x7f) | 0x80;
      uint8_t low  = (SDL & 0x7f);

      //_serial.print("SDL: ");
      //_serial.print(((int)((high & 0x7f) << 7) | low));


     //esta dando erro ao envia-los !!!!!!!!!!!! <<--------------------------------
     
     Serial.write((uint8_t)high);
     Serial.write((uint8_t)low);

     //Serial.write('A');
     //Serial.write('B');

     //Serial.write(high);
    // Serial.write(low);

     //Serial.write((int8_t)high);
     //Serial.write((int8_t)low);

    //char cc[2] = {high,low};
    //char cc[2] = {'A',0x09};
    //Serial.write(cc,2);
     

  //dados dos sensores
  for ( int ID = 0; ID < nsensors; ID++)
  {
    //quantidade de sensores conectados a estação
    int DL = sensors[ID]->getDegree();

   //_serial.print("DL: ");
   //_serial.print(((int)DL));
    
    float data[DL];
    
    sensors[ID]->sense (data);

      //D* //valor e folat sinalizado convertidos em 16 bits
      for (int index = 0; index < DL; index++)
      {
        // no Collector a operação deve ser revertida pra colotar o valor coreto
        //float sinalizado 16 bits
        uint8_t high = ((ftofix16(data[index]) >> 7) & 0x7f) | 0x80;
        uint8_t low  = (ftofix16(data[index]) & 0x7f);

        // debug da variavel
        //_serial.print("_Valor: ");
        //_serial.print(data[index]);
        //_serial.print( fix16tof(((high & 0x7f) << 7) | low) );
          
        Serial.write((uint8_t)high);
        Serial.write((uint8_t)low);
      }
    }


      Serial.write(EBYTE);
  
}


void SCOMMP::sense()
{
    while(Serial.available());//<<===============================================
 
  // compara se a id da estaço recebida e equivalente a id que esta na memoria eprom
  for ( int index = 0; index < id_size; index++)
  {
    if ( SID[index] != (uint8_t)(req[index + 3]))
    {
      return;
    }
  }

  //id do sensor no arduino
  uint8_t ID = (char)(req[3 + id_size * 2]);

   // _serial.print("ID: ");
   // _serial.print(((int)ID));

  if (ID < MAX_SENSORS && sensors[ID] != NULL)
  {
    
    //quantidade de dados fornecidos pelo sensor
    int DL = sensors[ID]->getDegree();

   //_serial.print("DL: ");
   //_serial.print(((int)DL));


    float data[DL];
    
    if (sensors[ID]->sense (data))
    {
      
      //envia resposta contendo os dados do sensor
      Serial.write(SBYTE);
      Serial.write(VBITE);
      Serial.write(SENSE);

  for ( int index = 0; index < id_size; index++)
  {
    Serial.write(SID[index]);
  }

  for ( int index = 0; index < id_size; index++)
  {
    Serial.write(FID[index]);
  }
  
      Serial.write(ID);

      //inteiro 16b its
      uint8_t high = ((DL >> 7) & 0x7f) | 0x80;
      uint8_t low  = (DL & 0x7f);

      //_serial.print("DL: ");
      //_serial.print(((int)((high & 0x7f) << 7) | low));

      Serial.write((uint8_t)high);
      Serial.write((uint8_t)low);
      
      //D* //valor e folat sinalizado convertidos em 16 bits
      for (int index = 0; index < DL; index++)
      {
        // no Collector a operação deve ser revertida pra colotar o valor coreto
        //float sinalizado 16 bits
        uint8_t high = ((ftofix16(data[index]) >> 7) & 0x7f) | 0x80;
        uint8_t low  = (ftofix16(data[index]) & 0x7f);

        // debug da variavel
       // _serial.print("Valor:");
       // _serial.print(data[index]);
        //_serial.print( fix16tof(((high & 0x7f) << 7) | low) );
          
        Serial.write(high);
        Serial.write(low);
      }

      Serial.write(EBYTE);
    }

  }
}




//! confirma com o Collector se estacao foi cadastrada com sucesso
void SCOMMP::isValid()
{

  // compara se a id da estaço recebida e equivalente a id que esta na memoria eprom
  for ( int index = 0; index < id_size; index++)
  {
    if ( SID[index] != (uint8_t)(req[index + 3]))
    {
      registred = false;
      return;
    }
  }

  registred = true;

  //   SBYTE | VBYTE | ACTBYTE | SID | FID  | EBYTE
  //
  //   SBYTE   = 0x69
  //   VBITE   = 0X02
  //   ACTBYTE = 0x06   (VALID)
  //   SID    = -        (Estação_ID 36 bytes)
  //   FID     = -       (Firmware_ID 36 bytes)
  //   EBYTE   = 0xAC

  //  Serial.write(SBYTE);
  //  Serial.write(VBITE);
  //  Serial.write(VALID);
  //
  //  Serial.write(SID);
  //  Serial.write(FID);
  //
  //  Serial.write(EBYTE);
}


//! Ler a id da estacao armazenada na EPROM
void SCOMMP::sid_read()
{
  for ( int index = 0 ; index < id_size ; index++ )
  {
    //char c;
    //EEPROM.get( index, c );
    //SID[index] = c;

    SID[index] = EEPROM[index];

    //_serial.print(SID[index]);
  }
}


//! Ao ser inicializado o arduino envia esta mengasem a
//! fim de confirmar se ja esta cadastrado.
void SCOMMP::registrationRequest()
{
  //   SBYTE | VBYTE | ACTBYTE | SID | FID | RBYTE | DL | D* | EBYTE
  //
  //   SBYTE   = 0x69
  //   VBITE   = 0X02
  //   ACTBYTE = 0x03    (RQVAR)
  //   SID     = -       (Estação_ID) 36 bytes)
  //   FID     = -       (Firmware_ID) 36 bytes
  //   RBYTE   = -       () (8 bits)
  //   DL      =         (Tamanho do vetor) (D* 16 bits)
  //   EBYTE   = 0xAC


  while(Serial.available());//<<===============================================

  if (registred == false)
  {
    Serial.write(SBYTE);
    Serial.write(VBITE);
    Serial.write(RQVAR);

  for ( int index = 0; index < id_size; index++)
  {
    Serial.write(SID[index]);
  }

  for ( int index = 0; index < id_size; index++)
  {
    Serial.write(FID[index]);
  }

    Serial.write(EEPROM[id_size]); //RBYTE

    //separando o tamanho da mensagem
    //const unsigned char high = ((STATION_CONFIG_LENGTH >> 7) & 0x7f) | 0x80;
    //const unsigned char low  = (STATION_CONFIG_LENGTH & 0x7f);

    String s = STATION_CONFIG;

    const unsigned char high = ((s.length() >> 7) & 0x7f) | 0x80;
    const unsigned char low  = (s.length() & 0x7f);

    Serial.write(high);
    Serial.write(low);

    //conferindo se o inteiro esta coreto
    //const int value = ((high & 0x7f) << 7) | low;
    //Serial.println(value);
    
    Serial.write(STATION_CONFIG);

//  for ( int index = 0; index < s.length(); index++)
//  {
//    Serial.write(s[index]);
//  }


    Serial.write(EBYTE);

  }
}

//int a = 600; //Just an example
//char high = (char)highByte(a);
//char low = (char)lowByte(a);
//Serial.print(high,BIN);
//Serial.println(low,BIN);

//===================================================

//const int a = 600;
//const unsigned char high = ((a >> 7) & 0x7f) | 0x80;
//const unsigned char low  = (a & 0x7f);

//Serial.println(high);
//Serial.println(low);

//const int value = ((high & 0x7f) << 7) | low;

//Serial.println(value);


void SCOMMP::update_id()
{
  
  //! compara se a id da estaço recebida e equivalente a id que esta na memoria eprom
  for ( int index = 0; index < id_size; index++)
  {

    if ( SID[index] != (uint8_t)(req[index + 3]))
    {
      registred = false;
      return;
    }
  }

  //ativa o RBYTE (0xEE)
  EEPROM[id_size] = RBYTE;

  //! atualiza a SID e a EPROM (NSID)
  for ( int index = 0; index < id_size; index++)
  {
    SID[index] = (uint8_t)(req[index + (id_size * 2) + 3]);
    EEPROM.write(index, (uint8_t)SID[index]);
    
    //EEPROM.write(index,(char)(req[index+(id_size*2)+3]));
  }



  //A Estação responde ao Collector autorizando o controle.

  //   SBYTE | VBYTE | ACTBYTE | SID | FID  | EBYTE
  //
  //   SBYTE   = 0x69
  //   VBITE   = 0X02
  //   ACTBYTE = 0x06   (VALID)
  //   SID    = -        (Estação_ID 36 bytes)
  //   FID     = -       (Firmware_ID 36 bytes)
  //   EBYTE   = 0xAC

//  Serial.write(SBYTE);
//  Serial.write(VBITE);
//  Serial.write(VALID);
//
//  for ( int index = 0; index < id_size; index++)
//  {
//    Serial.write(SID[index]);
//  }
//
//  for ( int index = 0; index < id_size; index++)
//  {
//    Serial.write(FID[index]);
//  }
//
//  Serial.write(EBYTE);
}

//! Mensagem de erro
void SCOMMP::error()
{

  while(Serial.available());//<<===============================================

  //  SBYTE | VBYTE | CID | ACTBYTE | EBYTE
  //
  //   SBYTE   = 0x69
  //   VBITE   = 0X02
  //   ACTBYTE = 0x04      (RQERRO)
  //   EBYTE   = 0xAC

  Serial.write(SBYTE);
  Serial.write(VBITE);
  Serial.write(RQERRO);//retorno para o collector
  Serial.write(EBYTE);
}

//==================================================================


void SCOMMP::addSensor (Sensor *sensor) {
  //! Verifica caso de overflow
  if (nsensors < MAX_SENSORS)
  	sensors[nsensors++] = sensor; //!< Insere sensor
}

void SCOMMP::addActuator (Actuator *actuator) {
  //! Verifica caso de overflow
  //if (nactuators < MAX_ACTUATORS)
  //	actuators[nactuators++] = actuator; //!< Insere atuador
}


void SCOMMP::response ()
{
  // a implementar

}





//=====================================================================

//! retorna uma string para o cliente (usando o padrao de palavra do scommp)
//! SBYTE | (tamanho da string) | DL | D* | EBYTE
void SCOMMP::print (char*) {

}

//! Subistitui todo o conteudo da EPROM por 0x00
void SCOMMP::EEPROM_Clear()
{
  // - Arduino Uno: 1kb EEPROM storage.

  for ( int index = 0 ; index < EEPROM.length() ; index++ )
  {
    EEPROM.write(index, '!');
  }
}

//! retorna tudo que esta gravado na EPROM pela Serial
//! atualmente sem tratamento (modificar posteriomente o
//! metodo para que seja enviado um datamho requisitado
//! a partir de um treço da memoria, usando o padrao de palavra do scommp)
void  EEPROM_Rand()
{
  // - Arduino Uno: 1kb EEPROM storage.

  for ( int index = 0 ; index < EEPROM.length() ; index++ )
  {
    Serial.write(EEPROM[index]);
  }
}

//=====================================================================

