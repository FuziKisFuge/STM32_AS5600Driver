# AS5600 driver — részletes függvénydokumentáció

---

## AS5600_Create()

```c
AS5600Handle_Typedef *AS5600_Create(uint8_t i2cAddr, float MaxAngle, float MinAngle)
```

Enkóder példány létrehozása.

> **NOTE**
> - Dinamikus memóriafoglalás. NULL visszatérés hibás memóriakezelés esetén.
> - I2C címnél elegndő csak a címet megadni, nem szükséges a shiftelés.
> - A teljes szögtartomány célszerű 0.0f - 360.0f közé állítani



**Példa:**

```c
Encoder1 = AS5600_Create(0x36,
                        360.0f,
                         0.0f);

if (Encoder1 == NULL)
{
	while(1);
}
```

**Paraméterek:**

| Paraméter | Adattípus | Leírás |
|---|---:|---|
| i2cAddr -- [in] | uint8_t | AS5600 I2C cím |
| MaxAngle -- [in] | float | Maximális szög fokban|
| MinAngle -- [in]| float | Minimális szög fokban |

**Visszatérési érték:**

| Jelentés |
|---|
| Siker: pointer a handle-re|
|Sikertelen: NULL |

---

## AS5600_AttachPeripheral()

```c
eInfo AS5600_AttachPeripheral (AS5600Handle_Typedef *pAS,
                                I2C_HandleTypeDef *hi2c,
                                TIM_HandleTypeDef *htim)
```

Perifériák (I2C és opcionálisan TIM) hozzárendelése az enkóder handle-hez. Kötelező meghívni, mielőtt I2C/PWM olvasást végzel.

> **NOTE**
> - A hi2c paraméter kötelező (I2C kommunikációhoz).
> - A htim opcionális: csak PWM alapú olvasáshoz szükséges.
> - Az I2C periféria inicializálása szükséges átadás előtt
> - A TIM periféria inicalizálást a kód végzi
> - Olyan TIM periféria szükséges amely képes bemeneti PWM jel mérésére (TIM1, TIM2 általában)
> - Célszerű TIM2-t megadni perifériának

**Példa:**

```c
AS5600_AttachPeripheral(Encoder1, &hi2c1, &htim2);
```

**Paraméterek:**

| Paraméter | Adattípus | Leírás |
|---|---:|---|
| pAS -- [in] | AS5600Handle_Typedef* | Enkóder handle |
| hi2c -- [in] | I2C_HandleTypeDef* | HAL I2C periféria (kötelező) |
| htim -- [in] | TIM_HandleTypeDef* | (opcionális) TIM handle PWM bemenethez |

**Visszatérési érték:**

| Típus | Jelentés |
|---|---|
| eInfo | Nincs visszatérési érték. Siker/helytelen állapot a handle státuszában jelezve. |

---

## AS5600_Configure

---

## AS5600_ReadRawAngle_I2C

---

## AS5600_ReadAngle_PWM

---

## AS5600_UpdateAbsolutePosition

---

## AS5600_UpdateStatus

---

## Általános megjegyzések

---