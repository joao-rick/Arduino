#ifndef SCOMMP_H
#define SCOMMP_H

#include <Arduino.h>
#include "config.h"
#include "device.h"

#include "SoftwareSerial.h"

// cabesalho da palavra 
#define SBYTE (char)0x69 // Start Byte
#define VBITE (char)0x02 // Vercao do protocolo SCOMMP

// fim da palavra
#define EBYTE (char)0xAC // End Byte

//Lista De Comandos
#define STATUS (char)0x00 //- STATUS (status de uma estação) (configuração da estação referente aos sensores e atuadores)
#define SENSE  (char)0x01 //- SENSE  (status de um sensor conectado a estação) (retorna todos os dados coletados pelo sensor)
#define ACT    (char)0x02 //- ACT    (executa um ação vinculada a um atuador) -(talvez seja necessário criar uma lista de ações básicas especificas para cada atuador, elas podem ser inseridas futuramente em novos comandos)-
#define RQVAR  (char)0x03 //- RQVAR  (comando que pede os dados necessários para que seja validada a comunicação entre o Collector e a estação)
#define RQERRO (char)0x04 //- RQERRO (comando usado para informar ao Colletor que ouve um erro na requisição dos dados necessários para a validação da comunicação com a estação)
#define EPROM  (char)0x05 //- EPROM  (comando responsável por escrever a partir da posição 00 da eprom um dado passado)
#define VALID  (char)0x06 //- VALID  (comando referente aos dados de validação Firmware_ID e Station_ID)
#define ISACT  (char)0x07 //- ISACT (Comando enviado as estações cadastradas no banco requisitando confirmação de sua ativação) (enviado quando o colector e inicializado) (faz com que as estação retente restabelecer a conexão com o Collecotor)

#define RBYTE  (char)0xEE //- RBYTE  (byte verificador de cadastro anterior da estação)


//#define _  (char)0x__ //- _  (comando para ...)

//pinos para a SoftwareSerial | debug
#define rxPin 11
#define txPin 12

class SCOMMP 
{
    public:
  
  //! SoftwareSerial apenas para debug (remover posteriomente)
  SoftwareSerial _serial =  SoftwareSerial(rxPin, txPin);
  
  //=========================================================
  
        //! vesao do protocolo
        //const static uint8_t version = VBITE;
  
    	//! Callback para a funcao que trata a saida do modulo
	void (*charProcessed) (char);
  
  	//! Sensores
	Sensor* sensors[MAX_SENSORS];
	//! n de sensores
	unsigned int nsensors;
	//! Atuadores
	Actuator* actuators[MAX_ACTUATORS];
	//! n de atuadores
	unsigned int nactuators;

	//! Comprimento da requisicao
	unsigned int reqlen;

	//! Dados brutos da requisicao
	char req[255];
        
        //tamanho da palavra a ser recebida
        int size = 0xFF;
  
        //definir o tamanho dos IDs 
        
        int id_size = 4;
     
        //! Firmware_ID
        //char* FID =  FIRMWARE_ID;// "00000000-0000-0000-0000-000000000001"; 
        uint8_t FID[4]; 

        //! Station_ID
        uint8_t SID[4]; //Station_ID //<<guardar na eprom (uint8_t SID[4])
 
       //confirma se a base esta registrada
        bool registred = false;

       // flag de verificação de ativação
        bool active = false;
  
  	/** Construtor
	*/
	SCOMMP();


        /** Inicializa a comunicao scommp
        
		@param charProcessed callback para uma funcao que trata a saida do modulo
	*/
	void begin (void (*)(char));

	/** Verifica se a comunicao foi iniciada
		
		@return true se a comunicao tiver sido iniciada, false se nao
	*/
	bool started();

	/** Reinicia a comunicao
	*/
	void reset();


	/** Caractere recebido

		E chamado quando um caractere foi recebido pela serial.
		@param c o caractere recebido
	*/
	void charReceived (char);


	/** Adiciona um sensor

		@param sensor o sensor a ser adicionado
	*/
	void addSensor (Sensor*);

	/** Adiciona um atuador

		@param actuator o atuador a ser adicionado
	*/
	void addActuator (Actuator*);



//======================================================
	/** Envia string ao cliente
	*/
	void print (char*);


	/** limpa a EPROM
	*/
        void EEPROM_Clear();
        
        
        /** retorna ao cliente todo o conteudo da EPROM
	*/
        void  EEPROM_Rand();
//====================================================== 


/** Envia os dados para confirmacao de resgistro da estacao
*/
void registrationRequest();

/** Ler a id da estacao armazenada na EPROM
*/
void sid_read();

//atualiza o id da estacao
void update_id();

//valida se a estaço ja foi anteriomente cadastrada
void isValid();

//retorna um erro caso a mensagem recebida no seja valida
void error();

//retorna o dado de um sersor
void sense();

//retorna os dadso contidos na memoria da estação 
void status();

//-------------------------

private:

/** Processa uma requisiacao do cliente quando esta terminar de ser lida
*/
void requestReady ();

//==================================================================
// Funcoes que tratam a coleta de dados e enviam ao cliente:
//==================================================================

	/** Envia uma resposta ao cliente

	*/
	void response();

//====================================================================  
  
  
};


#endif /* SCOMMP_H */
