# parking-solution
Solução para Estacionamento de Veículos Autônomos: uma implementação usando a plataforma Arduino

**Manual de uso**\
Para verificar o funcionamento deste sistema, deve-se primeiramente ter montado o estacionamento, assim como o veículo com os devidos componentes (ver Tabela 1 do artigo). Com estes procedimentos realizados, deve-se então:

1. Primeiramente clonar o repositório atual em um diretório qualquer:
    * git clone https://github.com/minimiz3d/parking-solution.git
2. Depois, deve-se certificar que as dependências estão devidamente instaladas (Java e IDE do Arduino, disponível em https://www.arduino.cc/en/Main/Software);
3. Então deve-se navegar até a pasta "src" e executar o código do veículo (parking-solution.ino) para iniciar o sistema;
4. Depois, abrindo um terminal deve-se compilar e então executar o código da central que gerencia o estacionamento com os comandos:
    * Compilar: javac Central.java
    * Executar: java -cp utilities.jar;. Central (WINDOWS)
    * Executar: java Central (LINUX)
    * Executar (forma alternativa, e mais simples): Utilizando alguma IDE de Java, deve-se compilar e rodar o mesmo código.
5. Dessa forma o sistema iniciará.

**Documentação**\
Detalhes a respeito do código utilizado para esta aplicação estão localizados no diretório "doc". Além disso, por ter sido utilizada uma biblioteca para interface com o módulo WiFi tém-se o detalhamento desta no diretório "ITEADLIB_Arduino_WeeESP8266-master/doc".

**Vídeo**\
O manual de instalação e funcionalidades do sistema está disponível no seguinte endereço:

TODO: Upar vídeo no youtube.
