#include <ESP8266.h>
#include <stdio.h>
#include "Motor.h"
#include "Follow.h"
#include "Ultrassonic.h"

/* Sensores */
int arrivalThr = 995;
int defaultThr = 850;
int speed = 120;
int parkingSpeed = 180;
Motor *motor = new Motor(6, 7, 9, 8, 10, 11);
Ultrassonic *ultrassonic = new Ultrassonic(A0, A1);
Follow *followmiddle = new Follow(A2);
Follow *followleft = new Follow(A3);
Follow *followright = new Follow(A5);

/* Conexão WiFi */
ESP8266 wifi(Serial1);
#define SSID "dlink-4"
#define PASSWORD "abcd1234"
#define PORT 8070
uint8_t mux_id;
uint8_t buffer[128] = {0};
char *msg = "arduino\n";
bool flagCheguei = false;
bool ack = false;
bool parking = false;
bool ok = false;
int distCounter = 0;
int vaga = 0;

void setup(void)
{
    Serial.begin(9600);
    Serial.print("Iniciando Setup. ");

    /* Setup dos sensores e motores */
    motor->setup();
    motor->setSpeed(speed);
    followright->setup();
    followleft->setup();
    followmiddle->setup();
    ultrassonic->setup();

    /* Setup do ESP8266 */
    Serial.print("Versao de Firmware ESP8266: ");
    Serial.println(wifi.getVersion().c_str());
    Serial.flush();

    /* Módulo para operar em modo Station (conecta em WiFi) e modo AP (é um ponto de WiFi tambem) */
    if (wifi.setOprToStationSoftAP())
    {
        Serial.println("Station e AP OK.");
        Serial.flush();
    }
    else
    {
        Serial.println("Erro em setar Station e AP.");
        Serial.flush();
    }

    /* Conectar no ponto de WiFi informado no inicio do codigo, e ver se corre tudo certo */
    if (wifi.joinAP(SSID, PASSWORD))
    {
        Serial.println("Conectado com sucesso.");
        Serial.println("IP: ");
        Serial.println(wifi.getLocalIP().c_str());
    }
    else
    {
        Serial.println("Falha na conexao AP.");
    }
    Serial.flush();

    /* Habilitando multiplas conexoes */
    if (wifi.enableMUX())
    {
        Serial.println("Multiplas conexoes OK.");
    }
    else
    {
        Serial.println("Erro ao setar multiplas conexoes.");
    }
    Serial.flush();

    /* Inicia servidor TCP */
    if (wifi.startTCPServer(PORT))
    {
        Serial.print("Servidor iniciado com sucesso - ");
        Serial.print("Porta: ");
        Serial.println(PORT);
    }
    else
    {
        Serial.println("Erro ao iniciar servidor.");
    }
    Serial.flush();

    Serial.print("Parking Speed = ");
    Serial.println(parkingSpeed);

    Serial.println("Setup finalizado! Iniciando delay...");
    Serial.flush();
    delay(3000);
    Serial.println("ok.");
}

void loop(void)
{
    if (!checkParkingStatus())
    {
        motor->stop();
    }
    else
        /* Seguidor de linha (FUNCIONAL) */
        followLine();

    /* Reset do buffer */
    buffer[128] = {0};

    /* Comunicação (inicia quando o carro estiver chegado no estacionamento) */
    if (flagCheguei)
    {
        /* Recebendo da central */
        uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 128);
        if (len > 0)
        {
            /* Mensagem recebida */
            Serial.print("< Recebendo: ");
            for (uint32_t i = 0; i < len; i++)
            {
                Serial.print((char)buffer[i]);
                Serial.flush();
            }

            /* Processamento */
            delay(500);
            processMsg();
            Serial.print("\n");
            Serial.flush();

            /* Enviando para central - com redundancia */
            delay(100);
            sendMsg();
            delay(500);
            sendMsg();
            delay(1000);
            sendMsg();
        }
    }
}

void followLine()
{
    /* Sensor lateral */
    if (followmiddle->alert(arrivalThr, 2))
    {
        if (!ok)
            motor->stop();

        /* Chegando no estacionamento */
        if (!flagCheguei)
        {
            Serial.println("flagCheguei on");
            Serial.flush();
            flagCheguei = true;
            msg = "cheguei\n";

            delay(2000);

            /* Enviando para central */
            sendMsg();
        }

        /* Verificando vaga correta */
        else
        {
            // 2
            if (parking && ok)
            {
                motor->stop();
                msg = "estacionando\n";
                sendMsg();

                /* TODO: Melhorar esta lógica */
                vaga--;
                if (vaga == 0)
                {
                    /* Essa curva fará o carro ir até a vaga esperada */
                    Serial.print("vagacorreta\n");
                    parking = false;
                }
                Serial.print("Etapa2done\n");

                delay(500);
                motor->setSpeed(parkingSpeed);
            }
        }
    }

    /* Sensores frontais */
    if (followleft->alert(defaultThr, 0) && followright->alert(defaultThr, 1))
    {
        motor->stop();
    }
    else if (followright->alert(defaultThr, 1))
    {
        motor->left();
    }
    else if (followleft->alert(defaultThr, 0))
    {
        motor->right();
    }

    // Ja chegou no inicio
    if (flagCheguei)
    {
        // Depois de ter obtido o numero da vaga, verifica se esta em movimento observando se nao esta sobre uma tarja preta
        if (followmiddle->checkLL(900))
        {
            parking = true;
        }
    }

    delay(15);
}

void processMsg() {
    if ((char)buffer[2] == 'V')
    {
        msg = "parking\n";

        /* Obtendo a vaga - funcional */
        vaga = ((char)buffer[4]) - '0';
        Serial.print(vaga);

        /*  PASSO A PASSO
            A partir daqui é necessário estacionar de fato.
            Portanto, agora sabemos em qual vaga devemos parar. 
            Então os seguintes passos são tomados:
            1. Reativa-se o "followLine" (ligando motores e velocidade setada novamente).
            2. Agora a cada detecção de uma vaga, verifica-se se esta é a vaga esperada da seguinte forma:
                2.1. vagas--
                2.2. vagas == 0?
                    2.2.1. sim: manter motor->right()
                    2.2.2. não: não setar a curva à direita (NOT motor->right()), seguindo em frente e repetindo o processo de verificação.
            3.  Estaciona-se (ultrassom on -> detecta fim do processo)
        */

        // 1
        Serial.print("\nEtapa1done\n");
        ok = true;
    }
}

bool sendMsg()
{
    /* Anuncia para a central */
    if (wifi.send(mux_id, (const uint8_t *)msg, sizeof(buffer)))
    {
        Serial.print("> Enviando: " + (String)msg + "\r\n");
        ack = false;
    }
    else
    {
        Serial.print("Erro enviando.\r\n");
    }
    Serial.flush();
}

bool checkParkingStatus()
{
    int dist = ultrassonic->getDistance();
    if ((dist <= 12) && (dist != 0) && (flagCheguei))
    {
        // Amostra quando obter distância esperada
        distCounter++;

        // DEBUG
        Serial.print("PROX: ");
        Serial.println(dist);
        Serial.print("counter: ");
        Serial.println(distCounter);
    }
    else
    {
        /* Faz o veiculo andar */
        motor->forward();
        motor->func();
    }

    // Caso esteja próximo à parede
    if (distCounter >= 15)
    {
        msg = "done\n";
        sendMsg();

        return false;
    }
    return true;
}