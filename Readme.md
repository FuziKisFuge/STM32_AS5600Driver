# AS5600 driver STM32-hez (HAL)

Egyszerű, light-weight AS5600 mágneses abszolút enkóder driver STM32 mikrokontrollerekhez, STM32 HAL I2C réteg használatával.

---

## Függőségek

- STM HAL

---

## Támogatott hardver és környezet

- AS5600 encoder

### Tesztelt környezet

- STM32 + HAL I2C
- Standard 7 bites I2C címzés

---

## Támogatott funkciók

- Szöghelyzet lekérdezése
  -I2C-vel
  -PWM jel alapján
  -Analóg bemenet alapján
- Abszolút szögelmozdulás számontartása
- Hibakezelési callback
- Státusz lekérdezés
- AS5600 konfigurálása
  - Power mode
  - Hysteresis
  - Output stage
  - PWM Frequency
  - Slow filter
  - FF Treshold
  - Watchdog

---

## NEM támogatott funkciók

- Minimum és maximum szögtartomány állítása (BURN)

---

## Példakódok

```c
#include "AS5600Driver.h"

AS5600Handle_Typedef *Encoder1;

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  //Encoder létrehozása
  Encoder1 = AS5600_Create(0x36,
                            360.0f,
                            0.0f);
  //Megfelelő perifériák (I2C1 és TIM2) hozzárendelése az enkóderhez
  AS5600_AttachPeripheral(Encoder1,
                            &hi2c1,
                            &htim2);
  if (Encoder1 == NULL)
  {
    while(1);
  }

  //Enkóder konfigurálása
  AS5600_Configure(Encoder1,
                    NOM,
                    OFF,
                    PWM,
                    _115Hz,
                    _16x,
                    SlowFilterOnly,
                    Off);

  //Szöghelyzet kiolvasás I2C-n keresztül
  AS5600_ReadRawAngle_I2C(Encoder1);
  //Enkóder státusz lekérdezése
  AS5600_UpdateStatus(Encoder1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      //Szöghelyzet meghatározása (bemeneti PWM jel alapján)
      AS5600_ReadAngle_PWM(Encoder1);
      //Abszolút szögpocizió frissítése
      AS5600_UpdateAbsolutePosition(Encoder1);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
```

---

## API áttekintés

Részletesen a Docs.md -ben!

---

## Ismert limitációk és hibák

---
