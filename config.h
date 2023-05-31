#ifndef CONFIG_H_
#define CONFIG_H_

//#define _GL5528      (int)0x0000 //(analógico) (intensidade luminosa)
//#define _401A        (int)0x0001 //(analógico) (qualidade do ar) 
//#define _GUVA_S12D   (int)0x0002 //(analógico) (intensidade de raios utra violeta)
//#define _DHT22       (int)0x0003 //(digital) (temperatura) (umidade) 
//#define _BMP085      (int)0x0004 //(i2c) (pressão) (temperatura)

#define FIRMWARE_ID 1 //uint32_t =  4 bytes -> [0-4294967295]

//Forma redusita de tipos (S = sensor) (A = actuating)
//não utilizar espaços na sting de alto-registro
#define STATION_CONFIG "6,0,DHT22,S,2,temperature,humidity,1,LDR_GL5528,S,1,candela,2,TP_401A,S,1,pts,3,GUVA_S12D,S,1,uv,4,Bucket_Rain_Gauge,S,1,precipitation,5,BMP085,S,4,pa,temperature,altitude,atmosphere"

//#define STATION_CONFIG "6,0,DHT22,S,2,temperature,humidity,1,LDR_GL5528,S,1,candela,2,TP_401A,S,1,pts,3,GUVA_S12D,S,1,uv,4,Bucket_Rain_Gauge,S,1,precipitation,5,BMP085,S,4,pa,temperature,altitude,atmosphere,6,SIM28,S,8,flat,flon,year,month,day,hour,minute,second"

//,0,DHT22,S,2,temperature,humidity
//,1,LDR_GL5528,S,1,candela
//,2,TP_401A,S,1,pts
//,3,GUVA_S12D,S,1,uv
//,4,Bucket_Rain_Gauge,S,1,precipitation
//,5,BMP085,S,4,pa,temperature,altitude,atmosphere
//,6,SIM28,S,8,flat,flon,year,month,day,hour,minute,second

    //DHT22 = temperature,humidity
  
    //LDR_GL5528 = candela

    //TP_401A = pts / partículas totais em suspensão (PTS)

    //GUVA_S12D = uv

    //Bucket_Rain_Gauge = precipitation
    
    //BMP085 = pa,temperature,altitude,atmosphere               //[0]:atmospheric pressure (Pa), [1]:temperature (C°), [2]: altitude (m), [3]: atmosphere (atm) 

    //SIM28 = flat,flon,year,month,day,hour,minute,second



//! Máximo número de sensores
#define MAX_SENSORS 7      // sensores na estação

//! Máximo número de atuadores
#define MAX_ACTUATORS 0    // atuadores na estação


#endif /* CONFIG_H_ */
