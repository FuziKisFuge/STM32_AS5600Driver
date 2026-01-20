# AS5600 driver STM32-höz (HAL)

Egyszerű, lightweight AS5600 mágneses abszolút enkóder driver STM32 mikrokontrollerekhez, STM32 HAL réteg használatával.

A driver API-kat biztosít:

- szöghelyzet lekérdezésére (PWM, I2C),
- az enkóder konfigurálására,
- státusz és hibák lekérdezésére,
- valamint motorvezérléshez hasznos segédfunkciókra, például **abszolút szögpozíció számontartására**.

---

## Függőségek

- **STM32 HAL**
  - HAL_TIM (PWM input capture)
  - HAL_I2C

---

## Támogatott hardver és környezet

- AS5600 mágneses abszolút enkóder
- STM32 mikrokontrollerek (HAL alapú projektek)

---

## Funkcióáttekintés

| Funkció | Támogatott | Nem támogatott |
|------|----------|---------------|
| Szöghelyzet lekérdezés | I2C<br>PWM | Analóg |
| Pozíció | Abszolút szögpozíció számontartása | Szögsebesség számítás |
| Hibakezelés | Error callback különböző hibákra | – |
| Státusz lekérdezés | Magnet too strong<br>Magnet too weak<br>Magnet detected | – |
| Konfigurálás | Power mode<br>Hysteresis<br>Output stage<br>PWM frequency<br>Slow filter<br>Fast filter threshold<br>Watchdog | Burn (min/max szögtartomány beállítás) |

> **Megjegyzés:**  
> A *Burn* funkció az AS5600-ban permanens beállítást jelent, ezért a driver szándékosan nem támogatja.

---

## Quick start

#### Perifériák:

> 1. I2C periféria inicializálása (Standard / Normal mode)
> 2. TIM periféria inicializálása PWM input capture módban

#### Kód lépések:

> 1. Driver header include  
> 2. Enkóder handle létrehozása  
> 3. Enkóder inicializálása  
> 4. Perifériák csatolása  
> 5. Enkóder konfigurálása  

---

## Példakód

> **Megjegyzés:**  
> A példa STM32CubeMX által generált projektstruktúrát feltételez.

```c
#include "AS5600Driver.h"

AS5600Handle_Typedef *Encoder1;

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();

  /* Encoder létrehozása */
  Encoder1 = AS5600_Create(0x36, 360.0f, 0.0f);

  /* Perifériák hozzárendelése */
  AS5600_AttachPeripheral(Encoder1, &hi2c1, &htim2);

  /* Encoder konfigurálása */
  AS5600_Configure(Encoder1,
                   NOM,
                   OFF,
                   PWM,
                   _115Hz,
                   _16x,
                   SlowFilterOnly,
                   Off);

  /* Státusz frissítése */
  AS5600_UpdateStatus(Encoder1);

  while (1)
  {
    /* Szöghelyzet kiolvasása PWM alapján */
    AS5600_ReadAngle_PWM(Encoder1);

    /* Abszolút szögpozíció frissítése */
    AS5600_UpdateAbsolutePosition(Encoder1);
  }
}
```

---

## Hibakezelés

A hibakezelés AS5600Driver_Config.h fájlban található define értékével állítható.

Hibakezelés aktív:
```c
#define config_USE_ERROR_HANDLING	  1
```

Hibakezelés inaktív:
```c
#define config_USE_ERROR_HANDLING	  0
```

Amennyiben a hibakezelés aktív akkor a AS5600_ErrorHandler() függvény hívódik meg. Ennek implementálása jelenleg:
```c
__attribute__((weak)) void AS5600_ErrorHandler(AS5600Handle_Typedef *pAS, eInfo error)
{
	switch(error)
	{
	case AS5600_OK:
		while(1);
		break;

	case AS5600_NULL_POINTER:
		while(1);
		break;

	case AS5600_CALLOC_FAIL:
		while(1);
		break;

	case AS5600_I2C_COMM_ERROR:
		while(1);
		break;

	case AS5600_PWM_MODE_NOT_INITIALIZED:
		while(1);
		break;

	case AS5600_INPUT_PWM_FREQ_ERROR:
		//while(1);
		break;


	case AS5600_INPUT_PWM_DUTYCYCLE_ERROR:
		while(1);
		break;

	case AS5600_DEVIDE_BY_ZERO:
		while(1);
		break;

	case AS5600_INVALID_MAX_ANGLE:
		while(1);
		break;

	case AS5600_INVALID_MIN_ANGLE:
		while(1);
		break;

	case AS5600_INVALID_MIN_MAX_ANGLE:
		while(1);
		break;

	case AS5600_OTHER_ERROR:
		while(1);
		break;


	}
}
```

A weak attributumnak köszönhetően lehetőség van a hibakezelő függvényt egyedileg is implementálni. Erre egy példa (main.c):
```c
void AS5600_ErrorHandler(AS5600Handle_Typedef *pAS, eInfo error)
{
	switch(error)
	{
	case AS5600_I2C_COMM_ERROR:
		if (HAL_I2C_IsDeviceReady(&hi2c1, (0x36 << 1), 4, 1000) == HAL_OK)
                   {
                   uart_printf(&huart1, "Device OK");
                   }
                   else
                   {
                   uart_printf(&huart1, "Device NOT available");
                   }
		break;


	case AS5600_INPUT_PWM_FREQ_ERROR:
		uart_printf(&huart1, "Wrong PWM settings");
		break;



	}
}
```

---

## API áttekintés

Részletesen a Docs.md -ben!

---
