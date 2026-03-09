# FuraFoscan

PINOUT

Button start              -> B13, Pull up
IRReceiver                -> B10 
Led Start                 -> C13 

Button sensor tilting     -> B12, Pull up

--QRE1113 IR LED módulo Sensor de reflexión infrarroja condensador descarga placa de disyuntor DC 3,3 -5V Qre1113 cámara Ir
Traker sensor left        -> A5, ADC1 Channel 0
Led traker left           -> A15
Traker sensor right       -> A1, ADC1 Channel 1
Led traker right          -> B3

--Sharp GP2Y0E03 4-50CM
Distance left             -> A7, ADC1 Channel 2
Led distance left         -> A12
Distance right            -> A0, ADC1 Channel 3
Led distance right        -> B4
Distance lateral left     -> A6, ADC1 Channel 4
Led distance lateral left -> A11
Distance lateral right    -> A4, ADC1 Channel 5
Led distance lateral righ -> B5
Distance central          -> BO, ADC1 Channel 6
Led distance lateral righ -> A8

Motor PWM left         -> B7, TIM4 PWM Generarion Channel 1
Motor PWM right        -> B6, TIM4 PWM Generarion Channel 2

PB5
BP4
PB3
PA15
PA11
PA12


Listado pines BlackPill

PA1 :
PA2 :
PA3 :
PA4 :
PA5 : SCK (SPI1: SCK, MISO, MOSI: Es el bus SPI principal, usado para lectores de tarjetas SD, algunas pantallas y módulos de comunicación inalámbrica.
PA6 : MISO (SPI1: SCK, MISO, MOSI: Es el bus SPI principal, usado para lectores de tarjetas SD, algunas pantallas y módulos de comunicación inalámbrica.
PA7 : MOSI (SPI1: SCK, MISO, MOSI: Es el bus SPI principal, usado para lectores de tarjetas SD, algunas pantallas y módulos de comunicación inalámbrica.
PA8 :
PA9 : TX (UART1: TX, RX): Es el puerto serie principal, ideal para enviar mensajes de depuración a un monitor serie en tu PC. Es una herramienta de depuración increíblemente útil.
PA10: RX (UART1: TX, RX): Es el puerto serie principal, ideal para enviar mensajes de depuración a un monitor serie en tu PC. Es una herramienta de depuración increíblemente útil.
PA11:
PA12:
PA13: SWDIO (SWDIO y SWCLK): Son los pines del puerto de depuración en serie (SWD). Sin ellos, no puedes programar ni depurar el microcontrolador con un ST-Link. Son la línea de vida entre tu PC y el STM32. Nunca los uses para otra cosa. 
PA14: SWCLK (SWDIO y SWCLK): Son los pines del puerto de depuración en serie (SWD). Sin ellos, no puedes programar ni depurar el microcontrolador con un ST-Link. Son la línea de vida entre tu PC y el STM32. Nunca los uses para otra cosa.
PA15:

PB1 :
PB2 :
PB3 :
PB4 :
PB5 :
PB6 : En ocasiones se sustituye con PB8
PB7 : En ocasiones se sustituye con PB9
PB8 : SCL (I2C1: SCL, SDA): Es el bus I2C más común. Lo usan infinidad de sensores, pantallas OLED, etc. A veces también se usa PB6/PB7, pero PB8/PB9` es una combinación muy habitual a reservar
PB9 : SDA (I2C1: SCL, SDA): Es el bus I2C más común. Lo usan infinidad de sensores, pantallas OLED, etc. A veces también se usa PB6/PB7, pero PB8/PB9` es una combinación muy habitual a reservar
PB10:
PB11:
PB12:
PB13:
PB14:
PB15:

PC13: Este pin está conectado al LED azul integrado en la placa. Puedes usarlo, pero ten en cuenta que cualquier cosa que hagas con él (configurarlo como salida, leerlo como entrada) interactuará con el circuito del LED. No es un pin "limpio" de propósito general.
PC14:
PC15: 

BOOT0: (BOOT0, BOOT1) Estos pines determinan desde qué memoria arranca el microcontrolador (desde la memoria Flash, RAM o el bootloader del sistema). Si los usas como GPIO, puedes impedir que tu programa se ejecute al reiniciar la placa.
BOOT1: (BOOT0, BOOT1) Estos pines determinan desde qué memoria arranca el microcontrolador (desde la memoria Flash, RAM o el bootloader del sistema). Si los usas como GPIO, puedes impedir que tu programa se ejecute al reiniciar la placa.

PH0: (PH0, PH1)  (OSC_IN y OSC_OUT): Están conectados al cristal oscilador externo de alta velocidad (HSE). Si tu configuración de reloj depende de este cristal, usar estos pines impedirá que el microcontrolador funcione a la velocidad esperada o que funcione en absoluto.
PH1: (PH0, PH1)  (OSC_IN y OSC_OUT): Están conectados al cristal oscilador externo de alta velocidad (HSE). Si tu configuración de reloj depende de este cristal, usar estos pines impedirá que el microcontrolador funcione a la velocidad esperada o que funcione en absoluto.