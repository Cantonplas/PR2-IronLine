from Comunicaciones import *
from robodk.robolink import *  
from robodk.robomath import *  
from time import sleep
import threading

RDK = Robolink()
lock = threading.Lock()
Hondarob2 = RDK.Item('Honda1Robot2', ITEM_TYPE_PROGRAM)
Chopperrob2 = RDK.Item('Chopper1Robot2', ITEM_TYPE_PROGRAM)
Hondarob1rep = RDK.Item('Honda2Robot1', ITEM_TYPE_PROGRAM)
Chopperrob1rep = RDK.Item('Chopper2Robot1', ITEM_TYPE_PROGRAM)
Hondarob1 = RDK.Item('Honda1Robot1', ITEM_TYPE_PROGRAM)
Chopperrob1 = RDK.Item('Chopper1Robot1', ITEM_TYPE_PROGRAM)

def Manejo_robodk(Coms:Comunicaciones,RDK):
    Repetido = False
    
    while True:
        Cambio_entre_mensaje = False
        sleep(0.5)
        if Coms.cambio == False or Coms.Emergency == True:
            continue
        lock.acquire()
        
        if Coms.Robot1.tipo_chasis == Coms.Robot2.tipo_chasis and Coms.Robot1.usado == False and Coms.Robot2.usado == False:
            RDK.setParam("iguales",1)
            Repetido = 1
        else:
            RDK.setParam("iguales",0)
            Repetido = 0
            
        if Coms.Robot2.usado == False and RDK.getParam("Robot2Operando") == 0:
            Cambio_entre_mensaje = True
            Coms.Robot2.usado = True
            RDK.setParam("Robot2Operando",1)
            RDK.setParam("ColorElegido2",Coms.Robot2.color_chasis)
            if Coms.Robot2.tipo_chasis == 1:
                if Hondarob2.Valid():
                    Hondarob2.RunProgram()
                sleep(0.5)
            elif Coms.Robot2.tipo_chasis == 2:
                if Chopperrob2.Valid():
                    Chopperrob2.RunProgram()
                sleep(0.5)
        lock.release()
        sleep(0.1)
        lock.acquire()
        
        RDK.setParam("iguales",Repetido)
        if Coms.Robot1.usado == False and RDK.getParam("Robot1Operando") == 0:
            if Coms.Robot1.tipo_chasis == Coms.Robot2.tipo_chasis and Coms.Robot1.usado == False and Coms.Robot2.usado == True:
                RDK.setParam("iguales",1)
                Repetido = 1
            Coms.Robot1.usado = True
            RDK.setParam("Robot1Operando",1)
            RDK.setParam("ColorElegido1",Coms.Robot1.color_chasis)
            if Repetido == True:
                if Coms.Robot1.tipo_chasis == 1 :
                    if Hondarob1rep.Valid():
                        Hondarob1rep.RunProgram()
                elif Coms.Robot1.tipo_chasis == 2:
                    if Chopperrob1rep.Valid():
                        Chopperrob1rep.RunProgram()   
            elif Coms.Robot1.tipo_chasis == 1 :
                if Hondarob1.Valid():
                    Hondarob1.RunProgram()
            elif Coms.Robot1.tipo_chasis == 2:
                if Chopperrob1.Valid():
                    Chopperrob1.RunProgram()
        Coms.cambio = False
        lock.release()
        continue   

def Mandar_situacion(Coms:Comunicaciones,RDK):
    while True:
        Counter = 0
        if Coms.Emergency == True:
            sleep(0.5)
            continue
        lock.acquire()
        while Counter < 5:
            if Coms.cambio == True:
                Counter +=1
                sleep(0.15)
            else:
                break
        Robot1_operando = not RDK.getParam("Robot1Operando")
        Robot2_operando = not RDK.getParam("Robot2Operando")
        print(f"Robot1 operando: {Robot1_operando}, Robot2 operando: {Robot2_operando}")
        Coms.enviar_estado_robots(Robot1_operando,Robot2_operando)
        lock.release()
        sleep(1)

def Parada_emergencia():
    if Hondarob1.Busy():
        Hondarob1.Stop()
    if Chopperrob1.Busy():
        Chopperrob1.Stop()
    if Hondarob2.Busy():
        Hondarob2.Stop()
    if Chopperrob2.Busy():
        Chopperrob2.Stop()
    if Hondarob1rep.Busy():
        Hondarob1rep.Stop()
    if Chopperrob1rep.Busy():
        Chopperrob1rep.Stop()
    
broker = "mqtt.dsic.upv.es"
topic = "giirob/pr2/ironline/python"
username = "giirob"
password = "UPV2024"

# broker = "broker.emqx.io"
# topic = "pruebas/placa_cojones/python"
# username = None
# password = None

if __name__ == "__main__":
    comunicaciones = Comunicaciones(broker,topic,username,password,lock)
    comunicaciones.conectar()
    Parada_emergencia()
    Reset = RDK.Item('Reset', ITEM_TYPE_PROGRAM)
    if Reset.Valid():
        Reset.RunProgram()
    RDK.setParam("Robot1Operando",0)
    RDK.setParam("Robot2Operando",0)
    RDK.setParam("ColorElegido1",0)
    RDK.setParam("ColorElegido2",0)
    
    comunicaciones.enviar_estado_robots(True, True) 
    thread_manejo = threading.Thread(target=Manejo_robodk, args=(comunicaciones, RDK))
    thread_mandar = threading.Thread(target=Mandar_situacion,args=(comunicaciones,RDK))
    thread_mandar.start()
    thread_manejo.start()
    Emergencia = False
    while True:
        try:
            if comunicaciones.Emergency == True:
                comunicaciones.enviar_estado_robots(False,False)
                if Emergencia == False:
                    Parada_emergencia()
                print("Parada de emergencia activada. Esperando reinicio...")
                Emergencia = True
            elif comunicaciones.Emergency == False and Emergencia == True:
                RDK.setParam("Robot1Operando",0)
                RDK.setParam("Robot2Operando",0)
                Reset.RunProgram()
                Emergencia = False
            sleep(0.5)
        except KeyboardInterrupt:
            thread_manejo.join()
            thread_mandar.join()
            print("Interrumpido por el usuario.")
            break
    
    
